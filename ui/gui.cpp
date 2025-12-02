#include <thread>
#include <format>
#include <vector>
#include <Windows.h>
#include <iostream>
#include "gui.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <fstream>
#include <string>
#include <random>
#include <iomanip>
#include <sstream>
#include <TlHelp32.h>
#include <conio.h> // For _getch()
#include "../SnapKey.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler
(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
);

LRESULT CALLBACK WindowProcess(HWND window, UINT message, WPARAM wideParameter, LPARAM longParameter)
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
		return true;

	switch (message)
	{
	case WM_SIZE: {
		if (gui::device && wideParameter != SIZE_MINIMIZED)
		{
			gui::presentParameters.BackBufferWidth = LOWORD(longParameter);
			gui::presentParameters.BackBufferHeight = HIWORD(longParameter);
			gui::ResetDevice();
		}
	}return 0;

	case WM_SYSCOMMAND: {
		if ((wideParameter & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
	}break;

	case WM_DESTROY: {
		PostQuitMessage(0);
	}return 0;

	case WM_LBUTTONDOWN: {
		gui::position = MAKEPOINTS(longParameter); // set click points
	}return 0;

	case WM_MOUSEMOVE: {
		if (wideParameter == MK_LBUTTON)
		{
			const auto points = MAKEPOINTS(longParameter);
			auto rect = ::RECT{ };

			GetWindowRect(gui::window, &rect);

			rect.left += points.x - gui::position.x;
			rect.top += points.y - gui::position.y;

			if (gui::position.x >= 0 &&
				gui::position.x <= gui::WIDTH &&
				gui::position.y >= 0 && gui::position.y <= 19)
				SetWindowPos(
					gui::window,
					HWND_TOPMOST,
					rect.left,
					rect.top,
					0, 0,
					SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
				);
		}

	}return 0;

	}

	return DefWindowProc(window, message, wideParameter, longParameter);
}

void gui::CreateHWindow(const char* windowName) noexcept
{
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_CLASSDC;
	windowClass.lpfnWndProc = WindowProcess;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(0);
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = "class001";
	windowClass.hIconSm = 0;

	RegisterClassEx(&windowClass);

	window = CreateWindowEx(
		0,
		"class001",
		windowName,
		WS_POPUP,
		100,
		100,
		WIDTH,
		HEIGHT,
		0,
		0,
		windowClass.hInstance,
		0
	);

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);
}

void gui::DestroyHWindow() noexcept
{
	DestroyWindow(window);
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool gui::CreateDevice() noexcept
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d)
		return false;

	ZeroMemory(&presentParameters, sizeof(presentParameters));

	presentParameters.Windowed = TRUE;
	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	presentParameters.EnableAutoDepthStencil = TRUE;
	presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&presentParameters,
		&device) < 0)
		return false;

	return true;
}

void gui::ResetDevice() noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	const auto result = device->Reset(&presentParameters);

	if (result == D3DERR_INVALIDCALL)
		IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();
}

void gui::DestroyDevice() noexcept
{
	if (device)
	{
		device->Release();
		device = nullptr;
	}

	if (d3d)
	{
		d3d->Release();
		d3d = nullptr;
	}
}

void gui::CreateImGui() noexcept
{
	// Initialize ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Set up ImGui style
	ImGui::StyleColorsDark(); // Start with the default dark theme

	// Initialize ImGui for DirectX 9
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);
}

void gui::DestroyImGui() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void gui::BeginRender() noexcept
{
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);

		if (message.message == WM_QUIT)
		{
			isRunning = !isRunning;
			return;
		}
	}

	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void gui::EndRender() noexcept
{
	ImGui::EndFrame();

	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	if (device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	const auto result = device->Present(0, 0, 0, 0);

	// Handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();
}



char listen() {
	std::cout << "Press any key: ";

	while (true) {
		// Loop through all virtual-key codes (0x08 = Backspace, 0x20 = Space, 0x41-0x5A = A-Z)
		for (int vk = 8; vk <= 190; vk++) {
			if (GetAsyncKeyState(vk) & 0x8000) { // key is pressed
				// Ignore modifier keys like Shift, Ctrl, Alt
				if (vk == VK_SHIFT || vk == VK_CONTROL || vk == VK_MENU)
					continue;

				// Convert virtual key to char
				BYTE keyboardState[256];
				GetKeyboardState(keyboardState);

				WORD ascii;
				if (ToAscii(vk, MapVirtualKey(vk, 0), keyboardState, &ascii, 0) == 1) {
					char ch = static_cast<char>(ascii);
					ch = std::toupper(static_cast<unsigned char>(ch));
					std::cout << "\nKey pressed: " << ch << std::endl;
					return ch;
				}
			}
		}
		Sleep(10); // small delay to prevent high CPU usage
	}
}

bool UIrunning = true;
void gui::Render() noexcept
{
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
	ImGui::Begin("Snapkey", &isRunning, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
	
	char buf[2];

	// Lambda to safely set the buffer
	auto setBuf = [](char key, char* buffer) {
		if (key < 32 || key > 126) // non-printable ASCII
			buffer[0] = '#';
		else
			buffer[0] = key;
		buffer[1] = '\0';
		};

	// Key 1

	ImGui::Text("GROUP 1:");
	ImGui::Text("Key 1: ");
	setBuf(Key1, buf);
	if (ImGui::Button((std::string(buf) + "##Key1").c_str())) {
		Key1 = listen();
	}

	// Key 2
	ImGui::Text("Key 2: ");
	setBuf(Key2, buf);
	if (ImGui::Button((std::string(buf) + "##Key2").c_str())) {
		Key2 = listen();
	}
	ImGui::Separator();
	ImGui::Text("GROUP 2:");
	// Key 3
	ImGui::Text("Key 3: ");
	setBuf(Key3, buf);
	if (ImGui::Button((std::string(buf) + "##Key3").c_str())) {
		Key3 = listen();
	}

	// Key 4
	ImGui::Text("Key 4: ");
	setBuf(Key4, buf);
	if (ImGui::Button((std::string(buf) + "##Key4").c_str())) {
		Key4 = listen();
	}


	ImGui::SetCursorPos(ImVec2(WIDTH / 2 - 25, WIDTH-230));
	if (ImGui::Button("Save"))
	{
		std::ofstream File("config.cfg");
		File << "[Group]" << "\n";
		File << "key1="<< static_cast<int>(Key1) << "\n";
		File << "key2="<< static_cast<int>(Key2) << "\n";
		File << "\n";
		File << "[Group]" << "\n";
		File << "key3="<<static_cast<int>(Key3) << "\n";
		File << "key4="<< static_cast<int>(Key4) << "\n";
		std::cout << "Saved" << std::endl;
		File.close();

		RestartSnapKey();
	}
	ImGui::End();
}