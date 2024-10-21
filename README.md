[![testsnap](https://github.com/user-attachments/assets/e9a23ba9-d394-4711-abfc-994932605d86)](https://github.com/cafali/SnapKey/releases)

**About SnapKey** 
--------------------------------------------------------------------------------------------------
SnapKey provides a user-friendly alternative to the Razer Snap Tap function, making it accessible across all keyboards!

SnapKey is a lightweight, open-source tool that operates from the system tray and is designed to track inputs from the WASD keys, without interfering with any game files. Its main role is to recognize when these keys are pressed and automatically release any previously engaged commands for them. This guarantees responsive and precise input handling. SnapKey handles the WASD keys by default and lets you rebind them to your liking via the config file.

[Leia em português | BR](README_pt_BR.md)
[日本語で読む | JP](README_ja_JP.md)

**Need More Info on SnapKey?** Visit the [**SnapKey Wiki**](https://github.com/cafali/SnapKey/wiki)

[![COMPATIBLE](https://github.com/user-attachments/assets/069a7a23-cfe4-47eb-8ac2-05872fcc2028)](https://github.com/cafali/SnapKey/wiki/Compatibility-List)



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
- Sticky Keys Feature: tracks the state of a pressed key ⌨️
- Enhances the precision of counter-strafing movements in games 🎯
- Allows key rebinding using ASCII codes specified in the configuration file 🛠️
- Supports unlimited amount of keys shared across groups (default AD / WS) 🔄
- Facilitates smoother transitions between left and right movements without input conflicts 🚀
- Does not use AutoHotkey or similar tools; its features rely solely on Windows API functions 🛡️

**SnapKey in Action**
--------------------------------------------------------------------------------------------------
![Explain](https://github.com/user-attachments/assets/7d11c221-33d8-43ea-839b-bfe276332593)

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

![Snapkey](https://github.com/user-attachments/assets/504ffa5e-50d3-4a77-9016-70f22d143cb1)

**Enhanced precision of counter-strafing**

- Automatically releases a previously held key when a new key (A/D) & (W/S) is pressed.

<img src="https://github.com/user-attachments/assets/4453aba4-b9bc-45e8-8a80-80caad39347b" width="600" height="338" alt="STRAFE">

**Quick Guide to Using SnapKey**
--------------------------------------------------------------------------------------------------
![use](https://github.com/user-attachments/assets/4d9fdf11-ec72-459c-995e-364bf1fc8d66)



1. Get SnapKey:
   - [Download](https://github.com/cafali/SnapKey/releases), Unzip and Launch SnapKey.exe

<img width="417" alt="image" src="https://github.com/user-attachments/assets/e4606577-6edb-4c12-ad6e-9e5ee01ba18f">




2. System Tray:
   - SnapKey will appear in your system tray

![tray](https://github.com/user-attachments/assets/26c08735-76f2-4bc0-aa75-44c9a866b453)



3. You're All Set:
   - SnapKey is now running and is ready to use
  
   
4. Rebind Keys (Optional):
   - [Rebind your keys](https://github.com/cafali/SnapKey/wiki/Rebinding-Keys) if you're using a nonstandard keyboard layout or if you want to map different keys

Disable SnapKey
--------------------------------------------------------------------------------------------------
- Right-click the SnapKey icon in the system tray and select "Disable SnapKey". This option disables the entire SnapKey functionality of the keys that are set in the config file while keeping the SnapKey process and the software running in the tray.

![image](https://github.com/user-attachments/assets/4f9d8d30-4674-4361-bc87-84ee70ce4116)




Exit SnapKey
--------------------------------------------------------------------------------------------------
   - Right-click the SnapKey icon in the system tray and select "Exit SnapKey"

![image](https://github.com/user-attachments/assets/c3a9534e-ed8b-4258-be56-37b6117c1814)





**Linux Support**
--------------------------------------------------------------------------------------------------
Since SnapKey isn’t natively supported on Linux, it’s recommended to check out @Dillacorn's guide on **[running SnapKey on Linux](https://github.com/cafali/SnapKey/issues/4#issuecomment-2251568839)**.

[![LINUX baner](https://github.com/user-attachments/assets/794a16ed-b0ab-4320-a680-52bda1ca0fd1)](https://github.com/cafali/SnapKey/wiki/Setup-Linux)

Looking for More Information? Got Questions or Need Help?
--------------------------------------------------------------------------------------------------
[<img src="https://github.com/user-attachments/assets/0c6d7564-6471-49f2-9367-64f7bffb7e37" alt="Wikitest" width="50%" />](https://github.com/cafali/SnapKey/wiki)

- **[About ℹ️](https://github.com/cafali/SnapKey/wiki/About)**  
  Learn about Snapkey, its features, and what it can do for you.

- **[Code Breakdown 🧠](https://github.com/cafali/SnapKey/wiki/Code-Breakdown)**  
  Dive into the details of SnapKey’s code structure.

- **[Compatibility List 🎮](https://github.com/cafali/SnapKey/wiki/Compatibility-List)**  
  Games that work with SnapKey and those that don't.

- **[FAQ❓](https://github.com/cafali/SnapKey/wiki/FAQ)**  
  Find answers to common questions about Snapkey.

- **[License 📜](https://github.com/cafali/SnapKey/wiki/License)**  
  Overview of Snapkey’s licensing.

- **[Rebinding Keys ⌨️](https://github.com/cafali/SnapKey/wiki/Rebinding-Keys)**  
  Instructions on how to rebind keys.

- **[Setup 🛠️](https://github.com/cafali/SnapKey/wiki/Setup)**  
  General setup instructions for getting Snapkey up and running on your system.

- **[Setup Linux 🐧](https://github.com/cafali/SnapKey/wiki/Setup-Linux)**  
  Setting up Snapkey on Linux distributions.

- **[System Requirements 🖥️](https://github.com/cafali/SnapKey/wiki/System-Requirements)**  
  SnapKey System Requirments.

- **[Troubleshoot 🔧](https://github.com/cafali/SnapKey/wiki/Troubleshoot)**  
  Solutions and tips for troubleshooting common issues with Snapkey.

- **[Updates 🔄](https://github.com/cafali/SnapKey/wiki/Updates)**  
  SnapKey Version Updates.
----

<p align="center">
  SnapKey by
</p>

<p align="center">
  <a href="https://github.com/cafali">@cafali</a> 
  <a href="https://github.com/minteeaa">@minteeaa</a> 
  <a href="https://github.com/Yaw-Dev">@Yaw-Dev</a>
</p>

<p align="center">
  <a href="https://github.com/cafali/Snapkey/graphs/contributors">
    <img src="https://contrib.rocks/image?repo=cafali/Snapkey" />
  </a>
</p>



