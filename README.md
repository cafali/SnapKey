![DFSDSSAS](https://github.com/user-attachments/assets/db8591f7-dca1-4649-bd11-2e6ae257634e)

**About SnapKey**
--------------------------------------------------------------------------------------------------
SnapKey offers a simple and intuitive alternative to the Razer Snap Tap function, making it accessible across all keyboards!

SnapKey is a lightweight open source utility that runs in the system tray and is designed to monitor user inputs for the A and D keys, without engaging with any game files. 

Its primary function is to detect when these keys are pressed and promptly cancel any previously held commands for them. 

This ensures that inputs are responsive and accurate. It manages the A and D key inputs; you can rebind the keys at any time in the config file using the provided ASCII codes.

Download
--------------------------------------------------------------------------------------------------
**[Download SnapKey from GitHub](https://github.com/cafali/SnapKey/releases)**

**SnapKey Features**
--------------------------------------------------------------------------------------------------

- Lightweight and open-source
- Compatible with all keyboards
- Accessible via the system tray
- Facilitates smoother transitions between left and right movements without input conflicts
- Enhances the precision of counter-strafing movements in games
- Allows key rebinding using ASCII codes specified in the configuration file
- Sticky Keys Feature: keeps track of the state of a key you've pressed down

--------------------------------------------------------------------------------------------------
**Here’s how it works:**
--------------------------------------------------------------------------------------------------
- When you press and hold down the "A" key, SnapKey remembers it.
- If you then press the "D" key while still holding down "A," SnapKey automatically releases the "A" key for you.
- The same happens if you press "A" while holding "D" — SnapKey releases the "D" key.

**Preventing simultaneous pressing of both the A and D keys**

- In many FPS games, pressing both the A and D keys simultaneously typically results in the game recognizing conflicting inputs. SnapKey automatically releases the previously held key when a new key input is detected.

![test](https://github.com/user-attachments/assets/20c0bad9-a33b-4c5a-8f85-bb8a21f0d951)



**Sticky Keys**

- Sticky Keys is a feature that keeps track of the state of a key you've pressed down. For example, if you 
hold down the "A" key and tap the "D" key repeatedly, each press of "D" will temporarily override 
the "A" key. When you release the "D" key, the action associated with the "A" key will resume, as 
long as you're still holding it down. The same principle applies if you start with "D" held down and 
press "A" instead. 

![Snapkey](https://github.com/user-attachments/assets/504ffa5e-50d3-4a77-9016-70f22d143cb1)

**Enhanced precision of counter-strafing**

- Automatically releases a previously held key when a new key (A/D) is pressed.

<img src="https://github.com/user-attachments/assets/4453aba4-b9bc-45e8-8a80-80caad39347b" width="600" height="338" alt="STRAFE">


--------------------------------------------------------------------------------------------------

**Rebind Keys**
--------------------------------------------------------------------------------------------------
To rebind your keys, follow these steps:

1. **Open the Config File**: Use any text editor to open the **config.cfg** file, which is located in the main SnapKey directory.

2. **Find the Key Settings**: Look for these lines:
   ```
   KeyA=65    (A is 65 in ASCII)
   KeyD=68    (D is 68 in ASCII)
   ```

3. **Find ASCII Codes**: The ASCII codes you need can be found in the list provided **below** in the **cfg file**.

4. **Change the Numbers**: Replace the numbers with the ASCII codes of the keys you want:
   - For example, to set 'F' (ASCII 70) for KeyA, change `KeyA=65` to `KeyA=70`.
   - To set 'G' (ASCII 71) for KeyD, change `KeyD=68` to `KeyD=71`.

5. **Your Config File Should Look Like This**:
   ```
   KeyA=70
   KeyD=71
   ```

6. **Restart Required**: After making these changes, restart Snapkey for the new key bindings to take effect.

**Warning**: Do not change `KeyA=` or `KeyD=` to any other letters, for example `KeyF=`. You are **only allowed to modify the number** according to the **ASCII table**. Changing these labels will break the functionality of the config file, causing it to stop working.

![cfg](https://github.com/user-attachments/assets/0a39b349-3326-44f0-ac08-81052990ec96)





**Quick Guide to Using SnapKey**
--------------------------------------------------------------------------------------------------

1. Launch SnapKey:
   - Open SnapKey

![image](https://github.com/user-attachments/assets/b420fe49-bd7d-432d-a68f-1e66f468a648)



2. System Tray:
   - SnapKey will appear in your system tray

![tray](https://github.com/user-attachments/assets/26c08735-76f2-4bc0-aa75-44c9a866b453)



3. You're All Set:
   - SnapKey is now running and is ready to use

Close SnapKey
--------------------------------------------------------------------------------------------------
   - Right-click the SnapKey icon in the system tray and select "Exit SnapKey"

![exit](https://github.com/user-attachments/assets/d9266b0c-75f0-4c07-9d55-4924a2515b82)

**Linux Support**
--------------------------------------------------------------------------------------------------
Since SnapKey isn’t natively supported on Linux, it’s recommended to check out @Dillacorn's guide on **[running SnapKey on Linux](https://github.com/cafali/SnapKey/issues/4#issuecomment-2251568839)**.

![SpanLin](https://github.com/user-attachments/assets/b47339e8-3ebf-4658-a07e-28c2df22a6c8)

SnapKey Wiki
--------------------------------------------------------------------------------------------------
Need Help with Troubleshooting and More?

[<img src="https://github.com/user-attachments/assets/0c6d7564-6471-49f2-9367-64f7bffb7e37" alt="Wikitest" width="50%" />](https://github.com/cafali/SnapKey/wiki)



