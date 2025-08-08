// snapkey_evdev_cfg.cpp
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <libevdev/libevdev.h>
#include <iostream>
#include <fstream>
#include <map>
#include <cstring>
#include <cctype>

std::map<std::string, int> keyMap = {
    {"KEY_SYSRQ", KEY_SYSRQ}, {"KEY_LEFTALT", KEY_LEFTALT},
    {"KEY_A", KEY_A}, {"KEY_B", KEY_B}, {"KEY_C", KEY_C},
    {"KEY_LEFTCTRL", KEY_LEFTCTRL}, {"KEY_ESC", KEY_ESC},
    {"KEY_ENTER", KEY_ENTER}, {"KEY_TAB", KEY_TAB},
    // Add more keys as needed
};

int parseKey(const std::string& name) {
    auto it = keyMap.find(name);
    return (it != keyMap.end()) ? it->second : -1;
}

bool loadConfig(int &trigger, int &simulate) {
    std::ifstream cfg("snapkey.cfg");
    if (!cfg) {
        std::cerr << "Config file 'snapkey.cfg' not found.\n";
        return false;
    }

    std::string line;
    while (std::getline(cfg, line)) {
        size_t eq = line.find('=');
        if (eq == std::string::npos) continue;

        std::string key = line.substr(0, eq);
        std::string value = line.substr(eq + 1);
        if (key == "trigger") trigger = parseKey(value);
        else if (key == "simulate") simulate = parseKey(value);
    }

    if (trigger < 0 || simulate < 0) {
        std::cerr << "Invalid keys in config.\n";
        return false;
    }
    return true;
}

int setup_uinput_device(int simulate_key_code) {
    int ufd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (ufd < 0) {
        perror("open /dev/uinput");
        return -1;
    }

    ioctl(ufd, UI_SET_EVBIT, EV_KEY);
    ioctl(ufd, UI_SET_KEYBIT, simulate_key_code);

    struct uinput_user_dev uidev{};
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "snapkey-uinput");
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor  = 0x1234;
    uidev.id.product = 0xfedc;
    uidev.id.version = 1;

    write(ufd, &uidev, sizeof(uidev));
    ioctl(ufd, UI_DEV_CREATE);

    return ufd;
}

void send_key(int ufd, int keycode) {
    struct input_event ev{};
    gettimeofday(&ev.time, NULL);

    ev.type = EV_KEY;
    ev.code = keycode;
    ev.value = 1;
    write(ufd, &ev, sizeof(ev));

    ev.value = 0;
    write(ufd, &ev, sizeof(ev));

    ev.type = EV_SYN;
    ev.code = SYN_REPORT;
    ev.value = 0;
    write(ufd, &ev, sizeof(ev));
}

int find_keyboard_event() {
    for (int i = 0; i < 32; ++i) {
        char path[64];
        snprintf(path, sizeof(path), "/dev/input/event%d", i);
        int fd = open(path, O_RDONLY | O_NONBLOCK);
        if (fd < 0) continue;

        struct libevdev* dev = nullptr;
        if (libevdev_new_from_fd(fd, &dev) == 0) {
            if (libevdev_has_event_type(dev, EV_KEY)) {
                if (libevdev_has_event_code(dev, EV_KEY, KEY_A)) {
                    std::cout << "Using input device: " << path << "\n";
                    libevdev_free(dev);
                    return fd;
                }
            }
            libevdev_free(dev);
        }
        close(fd);
    }

    std::cerr << "No suitable keyboard device found.\n";
    return -1;
}

int main() {
    int trigger_key = -1, simulate_key = -1;
    if (!loadConfig(trigger_key, simulate_key)) return 1;

    int input_fd = find_keyboard_event();
    if (input_fd < 0) return 1;

    int uinput_fd = setup_uinput_device(simulate_key);
    if (uinput_fd < 0) return 1;

    struct input_event ev{};
    while (true) {
        ssize_t n = read(input_fd, &ev, sizeof(ev));
        if (n <= 0) continue;

        if (ev.type == EV_KEY && ev.code == trigger_key && ev.value == 1) {
            std::cout << "Trigger key pressed. Simulating key.\n";
            send_key(uinput_fd, simulate_key);
        }
    }

    ioctl(uinput_fd, UI_DEV_DESTROY);
    close(uinput_fd);
    close(input_fd);
    return 0;
}
