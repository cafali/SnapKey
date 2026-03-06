<p align="center">
  <img src="https://raw.githubusercontent.com/SAPNXTDOOR/SnapKey-Pro/main/banner.png" alt="SnapKey Pro Banner">
</p>

<p align="center">
  <a href="https://github.com/SAPNXTDOOR/SnapKey-Pro/releases">
    <img src="https://img.shields.io/badge/Download-SnapKey%20Pro-2ea44f?style=for-the-badge&logo=github&logoColor=white">
  </a>
</p>

<p align="center">
  <img src="https://img.shields.io/github/downloads/SAPNXTDOOR/SnapKey-Pro/total?style=flat-square">
  <img src="https://img.shields.io/github/license/SAPNXTDOOR/SnapKey-Pro?style=flat-square">
</p>

# SnapKey Pro

SnapKey Pro provides a user-friendly alternative to the **Razer Snap Tap** feature, making it accessible on **any keyboard**.

It is a lightweight open-source Windows utility that runs in the **system tray** and manages movement key inputs to prevent conflicts. SnapKey Pro listens to keyboard inputs and automatically releases previously held movement keys when an opposite key is pressed, ensuring **responsive and precise movement control** in games.

SnapKey Pro does **not modify or interact with any game files** and works entirely through the **Windows API**.

---

## Features

* Easy to use 🧩
* Lightweight and open-source 🌟
* Runs from the system tray 🖥️
* Compatible with all keyboards ✅
* Does not interact with game files 🎮
* Enable / Disable features via tray menu ⛔
* Independent **X-axis and Y-axis SOCD control**
* **Mouse override support** for improved shooting accuracy
* **Sticky Keys system** for smoother movement recovery
* **Customizable global hotkeys** via `hotkeys.txt`
* **Edit Hotkeys** option directly from the tray menu
* **Restart option** to reload configuration instantly
* **Settings persistence** (remembers your toggle states)
* Uses only **Windows API** — no AutoHotkey or external scripting tools

---

## How SnapKey Pro Works

### Basic Example

1. Hold **A**
2. Press **D**
3. SnapKey Pro automatically releases **A**
4. Only **D** remains active

If you press **A** again while holding **D**, SnapKey Pro releases **D**.

This ensures there is **never conflicting input between opposite movement keys**.

---

## SOCD Handling (AD / WS)

Movement keys are divided into two independent groups:

| Axis       | Keys  |
| ---------- | ----- |
| Horizontal | A / D |
| Vertical   | W / S |

Within each group:

* **A cancels D**
* **D cancels A**
* **W cancels S**
* **S cancels W**

The two groups operate **independently**, so pressing W and A together works normally.

---

## Sticky Keys System

Sticky Keys keeps track of held keys and restores them after temporary overrides.

Example:

1. Hold **A**
2. Tap **D**
3. D temporarily overrides A
4. Release D
5. **A resumes automatically**

This makes movement feel **smooth and responsive** during rapid direction changes.

The same behavior applies to **mouse override**:

* When **Left Click** is pressed while moving, movement inputs can temporarily pause.
* Once the click is released, movement resumes if the key is still held.

---

## Mouse Override

Mouse override prevents unintended movement while firing.

When **Left Click** is pressed:

* Movement inputs can be temporarily disabled
* This improves **shooting accuracy** in certain scenarios.

---

## Hotkeys Configuration

SnapKey Pro supports **custom global hotkeys**.

Hotkeys are configured in:

```
hotkeys.txt
```

Example:

```
toggle_snapkey=Ctrl+Alt+S
toggle_socd_x=Ctrl+L
toggle_socd_y=Ctrl+Shift+1
toggle_mouse_override=F13
toggle_mouse_override_x=Alt+F14
toggle_mouse_override_y=MouseX2
```

Supported keys:

* A-Z
* 0-9
* F1–F24
* Ctrl
* Alt
* Shift
* Win
* MouseLeft
* MouseRight
* MouseMiddle
* MouseX1
* MouseX2

Hotkeys can combine multiple modifiers:

```
Ctrl+Shift+F12
Alt+F14
Ctrl+Alt+S
```

After editing hotkeys:

1. Right-click the tray icon
2. Click **Restart**

or restart the application.

---

## Tray Menu Options

Right-click the tray icon to access:

* Toggle SnapTap
* Toggle SOCD X
* Toggle SOCD Y
* Toggle Mouse Override
* Toggle Mouse Override X
* Toggle Mouse Override Y
* **Edit Hotkeys**
* **Restart**
* Get Help
* Check for Updates
* About
* Exit

---

## Installation

1. Download the latest release.
2. Extract the folder.
3. Ensure these files are together:

```
SnapKey Pro.exe
settings.ini
hotkeys.txt
```

4. Run:

```
SnapKey Pro.exe
```

The application will appear in the **system tray**.

---

## Run at Startup (Optional)

1. Press **Win + R**
2. Type:

```
shell:startup
```

3. Place a shortcut of **SnapKey Pro.exe** inside the folder.

---

## Disclaimer

Some games may restrict or disallow software that modifies input behavior.
SnapKey Pro is intended for **educational and demonstration purposes**.

Please ensure that using this tool complies with the **rules of the games you play**.

---

## Author

SnapKey Pro by
**@SAPNXTDOOR**

---

## License

Open-source project.
See repository for license information.
