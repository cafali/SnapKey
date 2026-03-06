#include "InputState.h"

// =========================
// EMIT ONLY ON CHANGE
// =========================
void EmitIfChanged() {
    if (lW != prevW) SendKey('W', lW);
    if (lA != prevA) SendKey('A', lA);
    if (lS != prevS) SendKey('S', lS);
    if (lD != prevD) SendKey('D', lD);

    prevW = lW;
    prevA = lA;
    prevS = lS;
    prevD = lD;
}

// =========================
// SOCD RESOLUTION (AXIS AWARE)
// =========================
void ResolveSOCD() {

    // -------- HORIZONTAL (A / D) --------
    if (g_socd_enabled && g_socd_x_enabled) {
        if (pA && pD) {
            sA = (lastH == 'A');
            sD = (lastH == 'D');
        } else {
            sA = pA;
            sD = pD;
        }
    } else {
        sA = pA;
        sD = pD;
    }

    // -------- VERTICAL (W / S) --------
    if (g_socd_enabled && g_socd_y_enabled) {
        if (pW && pS) {
            sW = (lastV == 'W');
            sS = (lastV == 'S');
        } else {
            sW = pW;
            sS = pS;
        }
    } else {
        sW = pW;
        sS = pS;
    }
}

// =========================
// MAIN UPDATE
// =========================
void Update() {

    // Fully disabled = transparent
    if (!g_enabled) {
        lW = lA = lS = lD = false;
        EmitIfChanged();
        return;
    }

    // -------- SOCD --------
    ResolveSOCD();

    // -------- MOUSE OVERRIDE (AXIS AWARE) --------
    if (g_mouse_override_enabled && pFire) {

        // Horizontal
        if (g_mouse_override_x_enabled) {
            lA = false;
            lD = false;
        } else {
            lA = sA;
            lD = sD;
        }

        // Vertical
        if (g_mouse_override_y_enabled) {
            lW = false;
            lS = false;
        } else {
            lW = sW;
            lS = sS;
        }

    } else {
        // No mouse override → pass SOCD output
        lW = sW;
        lA = sA;
        lS = sS;
        lD = sD;
    }

    EmitIfChanged();
}
