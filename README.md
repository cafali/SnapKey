<img width="1200" height="675" alt="disconnected_with_ban_centered_aa" src="https://github.com/user-attachments/assets/6785c5cd-f4ab-4752-aa66-b64544b8fb92" />

**About SnapKey** 
--------------------------------------------------------------------------------------------------
SnapKey provides a user-friendly alternative to the Razer Snap Tap function, making it accessible across all keyboards!

SnapKey is a lightweight, open-source tool that operates from the system tray and is designed to track inputs from the WASD keys, without interfering with any game files. Its main role is to recognize when these keys are pressed and automatically release any previously engaged commands for them. This guarantees responsive and precise input handling. SnapKey handles the WASD keys by default and lets you rebind them to your liking via the config file.

**Need More Info on SnapKey?** Visit the [**SnapKey Wiki**](https://github.com/cafali/SnapKey/wiki)

[![COMPATIBLE](https://github.com/user-attachments/assets/069a7a23-cfe4-47eb-8ac2-05872fcc2028)](https://github.com/cafali/SnapKey/wiki/Compatibility-List)

### **New in v1.2.9 – Experimental VAC/CS2 Bypass Modes 🚫🎮**

---

SnapKey v1.2.9 introduces two **experimental bypass modes** aimed at improving compatibility with games that impose stricter input policies, such as **Counter-Strike 2 (CS2)**. These modes can be toggled independently in the configuration file and allow SnapKey to remain active while circumventing CS2’s built-in restrictions on background input automation.

> ⚙️ **VAC Bypass A** — Simulates real-time user input using a lower-level system call chain.
> ⚙️ **VAC Bypass B** — Uses synthetic input injection with randomized timings to mimic human behavior more accurately.

Both modes are entirely optional and **disabled by default**. When enabled, SnapKey will change its tray icon to indicate **VAC bypass mode** is active:

* ✅ A curved arrow wrapping around the "VAC" label appears.
* ⛔ This visually reminds users that advanced compatibility workarounds are enabled.
<img width="239" height="274" alt="image" src="https://github.com/user-attachments/assets/a1019787-0420-49e6-aa87-1e79f78bb66c" />

> \[!WARNING]
> These features are **experimental** and **not officially endorsed by Valve or any game publisher**. While SnapKey does not directly modify any game files or memory, **use of automation tools may still violate Terms of Service** for certain games.
> **Use at your own risk.**

For details on how these bypass modes work and how to enable them, refer to the [**Bypass Modes Wiki Page**](https://github.com/cafali/SnapKey/wiki/VAC-Bypass-Modes).

---

Download
--------------------------------------------------------------------------------------------------
<p align="center">
  <a href="https://github.com/cafali/SnapKey/releases">Download from GitHub</a> |
  <a href="https://sourceforge.net/projects/snapkey/">Download from SourceForge</a> |
  <a href="https://www.softpedia.com/get/Tweak/System-Tweak/SnapKey.shtml">Download from Softpedia</a>
</p>


[![latesver](https://github.com/user-attachments/assets/09694f7c-6eeb-4c80-9a02-1d777956d181)](https://github.com/cafali/SnapKey/wiki/Updates)

**SnapKey Features**
--------------------------------------------------------------------------------------------------
- Easy to use 🧩
- Detailed documentation 📖
- Lightweight and open-source 🌟
- Accessible via the system tray 🖥️
- Compatible with all keyboards ✅
- Does not interact with game files 🎮
- Activate/Deactivate via context menu ⛔
- Double-click the tray icon to disable it 👆👆
- Sticky Keys Feature: tracks the state of a pressed key ⌨️
- Enhances the precision of counter-strafing movements in games 🎯
- Allows key rebinding using ASCII codes specified in the configuration file 🛠️
- Supports unlimited amount of keys shared across groups (default AD / WS) 🔄
- Facilitates smoother transitions between left and right movements without input conflicts 🚀
- Does not use AutoHotkey or similar tools; its features rely solely on Windows API functions 🛡️

**SnapKey in Action**
--------------------------------------------------------------------------------------------------
- When you press and hold down the **"A"** key, SnapKey remembers it.
- If you then press the **"D"** key while still holding down **"A"** SnapKey automatically releases the **"A"** key for you.
- The same happens if you press **"A"** while holding **"D"** — SnapKey releases the **"D"** key.

**SnapKey prevents simultaneous movement key conflicts (AD / WS)**

- In many FPS games, pressing both the **"A"** and **"D"** keys simultaneously typically results in the game recognizing conflicting inputs. SnapKey automatically releases the previously held key when a new key input is detected.
- The keys are separated into two different groups: A/D and W/S. In each group, **"A"** cancels out **"D"** and vice versa, while the same applies to **"W"** and **"S"**. These groups do not interfere with each other and work separately.

**Sticky Keys**

- Sticky Keys is a feature that keeps track of the state of a key you've pressed down. For example, if you 
hold down the **"A"** key and tap the **"D"** key repeatedly, each press of **"D"** will temporarily override 
the **"A"** key. When you release the **"D"** key, the action associated with the **"A"** key will resume, as 
long as you're still holding it down. The same principle applies if you start with **"D"** held down and 
press **"A"** instead.

> [!NOTE]
> SnapKey and similar solutions have been disallowed in certain games; illustrations shown are for demonstrative purposes only.

![Snapkey](https://github.com/user-attachments/assets/504ffa5e-50d3-4a77-9016-70f22d143cb1)

**Enhanced precision of counter-strafing**

- Automatically releases a previously held key when a new key (A/D) & (W/S) is pressed.

<img src="https://github.com/user-attachments/assets/4453aba4-b9bc-45e8-8a80-80caad39347b" width="600" height="338" alt="STRAFE">

**Linux Support**
--------------------------------------------------------------------------------------------------
Since SnapKey isn’t natively supported on Linux, it’s recommended to check out @Dillacorn's guide on **[running SnapKey on Linux](https://github.com/cafali/SnapKey/issues/4#issuecomment-2251568839)**.

[![LINUX baner](https://github.com/user-attachments/assets/794a16ed-b0ab-4320-a680-52bda1ca0fd1)](https://github.com/cafali/SnapKey/wiki/Setup-Linux)

Looking for More Information? Got Questions or Need Help?
--------------------------------------------------------------------------------------------------
[<img src="https://github.com/user-attachments/assets/0c6d7564-6471-49f2-9367-64f7bffb7e37" alt="Wikitest" width="50%" />](https://github.com/cafali/SnapKey/wiki)

- **[About ℹ️](https://github.com/cafali/SnapKey/wiki/About)**  
  Discover SnapKey, explore its features and see how it can benefit you

- **[Code Breakdown 🧠](https://github.com/cafali/SnapKey/wiki/Code-Breakdown)**  
  Dive into the details of SnapKey’s code structure

- **[Compatibility List 🎮](https://github.com/cafali/SnapKey/wiki/Compatibility-List)**  
  Compatibility status of games with SnapKey

- **[FAQ❓](https://github.com/cafali/SnapKey/wiki/FAQ)**  
  Find answers to common questions about SnapKey

- **[License 📜](https://github.com/cafali/SnapKey/wiki/License)**  
  Overview of SnapKey’s licensing

- **[Rebinding Keys ⌨️](https://github.com/cafali/SnapKey/wiki/Rebinding-Keys)**  
  Instructions on how to rebind keys

- **[Setup 🛠️](https://github.com/cafali/SnapKey/wiki/Setup)**  
  General setup instructions for getting Snapkey up and running on your system

- **[Setup Linux 🐧](https://github.com/cafali/SnapKey/wiki/Setup-Linux)**  
  Setting up SnapKey on Linux distributions

- **[System Requirements 🖥️](https://github.com/cafali/SnapKey/wiki/System-Requirements)**  
  SnapKey System Requirements

- **[Troubleshoot 🔧](https://github.com/cafali/SnapKey/wiki/Troubleshoot)**  
  Solutions and tips for troubleshooting common issues with SnapKey

- **[Changelog 🔄](https://github.com/cafali/SnapKey/wiki/Updates)**  
  View SnapKey releases and changes
----

<p align="center">
  SnapKey by
</p>

<p align="center">
  <a href="https://github.com/cafali">@cafali</a> 
  <a href="https://github.com/minteeaa">@minteeaa</a>
  <a href="https://github.com/felix3322">@felix3322</a>
  <a href="https://github.com/Yaw-Dev">@Yaw-Dev</a>
</p>

<p align="center">
  <a href="https://github.com/felix3322/Snapkey/graphs/contributors">
    <img src="https://contrib.rocks/image?repo=felix3322/Snapkey" />
  </a>
</p>