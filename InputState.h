#pragma once
#include <windows.h>

// =========================
// PHYSICAL STATE
// =========================
extern bool pW, pA, pS, pD;
extern bool pFire;

// =========================
// FEATURE TOGGLES
// =========================
extern bool g_socd_enabled;
extern bool g_mouse_override_enabled;

// =========================
// AXIS-SPECIFIC SOCD TOGGLES
// =========================
extern bool g_socd_x_enabled; // A / D
extern bool g_socd_y_enabled; // W / S

// =========================
// AXIS-SPECIFIC MOUSE OVERRIDE TOGGLES
// =========================
extern bool g_mouse_override_x_enabled; // A / D
extern bool g_mouse_override_y_enabled; // W / S


// =========================
// SOCD-RESOLVED STATE
// =========================
extern bool sW, sA, sS, sD;

// =========================
// FINAL OUTPUT STATE
// =========================
extern bool lW, lA, lS, lD;

// =========================
// PREVIOUS OUTPUT STATE
// =========================
extern bool prevW, prevA, prevS, prevD;

// =========================
// LAST PRESSED (SOCD)
// =========================
extern char lastH;
extern char lastV;

// =========================
// APP STATE
// =========================
extern bool g_enabled;

// =========================
// CORE
// =========================
void Update();

// =========================
// HOOKS
// =========================
void InstallKeyboardHook();
void InstallMouseHook();

// =========================
// EMISSION
// =========================
void SendKey(WORD vk, bool down);
