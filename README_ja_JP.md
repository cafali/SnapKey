[![testsnap](https://github.com/user-attachments/assets/e9a23ba9-d394-4711-abfc-994932605d86)](https://github.com/cafali/SnapKey/releases)

**SnapKeyについて**  
--------------------------------------------------------------------------------------------------  
SnapKeyは、Razer Snap Tap機能の使いやすい代替手段を提供し、すべてのキーボードで使用可能にします！

SnapKeyは、軽量でオープンソースのツールであり、システムトレイから実行され、ゲームファイルに干渉することなくWASDキーの入力を追跡します。主要な機能は、これらのキーが押されたときに以前に実行されていたコマンドを自動的に解除し、迅速で正確な入力操作を保証することです。SnapKeyはデフォルトでWASDキーを処理しますが、設定ファイルで自由に再バインドできます。

**SnapKeyについて詳しく知りたい方はこちら**  
[**SnapKey Wiki**](https://github.com/cafali/SnapKey/wiki)

[![COMPATIBLE](https://github.com/user-attachments/assets/069a7a23-cfe4-47eb-8ac2-05872fcc2028)](https://github.com/cafali/SnapKey/wiki/Compatibility-List)

### ダウンロード  
--------------------------------------------------------------------------------------------------  
<p align="center">
  <a href="https://github.com/cafali/SnapKey/releases">GitHubからダウンロード</a> |
  <a href="https://sourceforge.net/projects/snapkey/">SourceForgeからダウンロード</a> |
  <a href="https://www.softpedia.com/get/Tweak/System-Tweak/SnapKey.shtml">Softpediaからダウンロード</a>
</p>

[![latesver](https://github.com/user-attachments/assets/09694f7c-6eeb-4c80-9a02-1d777956d181)](https://github.com/cafali/SnapKey/wiki/Updates)

### SnapKeyの機能  
--------------------------------------------------------------------------------------------------  
- 使いやすい 🧩  
- 詳細なドキュメント 📖  
- 軽量でオープンソース 🌟  
- システムトレイからアクセス可能 🖥️  
- すべてのキーボードに対応 ✅  
- ゲームファイルに干渉しない 🎮  
- コンテキストメニューから有効/無効を切り替え ⛔  
- Sticky Keys機能: 押されたキーの状態を追跡 ⌨️  
- ゲーム内のストレイフ操作の精度を向上 🎯  
- 設定ファイルでASCIIコードを使用してキーを再バインド可能 🛠️  
- AD/WSなどのキーグループ間でのスムーズな切り替え 🔄  
- 入力競合なしで左右の移動が可能 🚀  
- AutoHotkeyなどのツールは使用せず、Windows API関数のみを使用 🛡️  

### SnapKeyの使い方  
--------------------------------------------------------------------------------------------------  
![Explain](https://github.com/user-attachments/assets/7d11c221-33d8-43ea-839b-bfe276332593)

- **「A」**キーを押し続けると、SnapKeyが記憶します。  
- その状態で**「D」**キーを押すと、自動的に**「A」**キーが解除されます。  
- 同様に、**「D」**を押している間に**「A」**を押すと、**「D」**が解除されます。  

**SnapKeyは同時押しによる競合を防ぎます（AD/WS）**  

- 多くのFPSゲームでは、**「A」**と**「D」**を同時に押すと、入力の競合が発生します。SnapKeyは新しいキーが押されると、以前のキーを自動的に解除します。  
- キーは「A/D」と「W/S」の2つのグループに分けられ、それぞれのグループ内で一方のキーが他方をキャンセルします。これらのグループは互いに干渉せず、独立して機能します。

### Sticky Keys機能  
- Sticky Keysは押されたキーの状態を追跡する機能です。  
  例えば、**「A」**を押し続けながら**「D」**を何度もタップすると、そのたびに**「A」**が一時的に無効化されます。**「D」**を離すと、**「A」**が再び有効になります。

![Snapkey](https://github.com/user-attachments/assets/504ffa5e-50d3-4a77-9016-70f22d143cb1)

### カウンターストレイフの精度向上  
- **「A/D」**または**「W/S」**の新しいキーが押されると、以前のキーを自動的に解除します。

<img src="https://github.com/user-attachments/assets/4453aba4-b9bc-45e8-8a80-80caad39347b" width="600" height="338" alt="STRAFE">

### クイックガイド  
--------------------------------------------------------------------------------------------------  
![use](https://github.com/user-attachments/assets/4d9fdf11-ec72-459c-995e-364bf1fc8d66)

1. **SnapKeyを取得する**  
   - [ダウンロード](https://github.com/cafali/SnapKey/releases)、解凍し、SnapKey.exeを起動します。

2. **システムトレイに表示されます**  
   - SnapKeyはシステムトレイに表示されます。

3. **準備完了**  
   - SnapKeyは動作を開始し、使用可能です。

4. **キーの再バインド（オプション）**  
   - [再バインド](https://github.com/cafali/SnapKey/wiki/Rebinding-Keys)してカスタマイズ可能です。

### SnapKeyを無効にする  
--------------------------------------------------------------------------------------------------  
- システムトレイのSnapKeyアイコンを右クリックし、「Disable SnapKey」を選択すると、SnapKeyの機能が無効になります。

### SnapKeyを終了する  
--------------------------------------------------------------------------------------------------  
- システムトレイのアイコンを右クリックし、「Exit SnapKey」を選択します。

### Linuxサポート  
--------------------------------------------------------------------------------------------------  
SnapKeyはLinuxをネイティブサポートしていないため、@Dillacornの**[LinuxでのSnapKeyの実行ガイド](https://github.com/cafali/SnapKey/issues/4#issuecomment-2251568839)**を参考にしてください。

[![LINUX banner](https://github.com/user-attachments/assets/794a16ed-b0ab-4320-a680-52bda1ca0fd1)](https://github.com/cafali/SnapKey/wiki/Setup-Linux)

### サポート情報  
--------------------------------------------------------------------------------------------------  
[![Wikitest](https://github.com/user-attachments/assets/0c6d7564-6471-49f2-9367-64f7bffb7e37)](https://github.com/cafali/SnapKey/wiki)

- **[概要 ℹ️](https://github.com/cafali/SnapKey/wiki/About)**  
- **[コードの詳細 🧠](https://github.com/cafali/SnapKey/wiki/Code-Breakdown)**  
- **[互換性リスト 🎮](https://github.com/cafali/SnapKey/wiki/Compatibility-List)**  
- **[FAQ❓](https://github.com/cafali/SnapKey/wiki/FAQ)**  
- **[ライセンス 📜](https://github.com/cafali/SnapKey/wiki/License)**  
- **[キーの再バインド ⌨️](https://github.com/cafali/SnapKey/wiki/Rebinding-Keys)**  
- **[セットアップ 🛠️](https://github.com/cafali/SnapKey/wiki/Setup)**  
- **[Linuxセットアップ 🐧](https://github.com/cafali/SnapKey/wiki/Setup-Linux)**  
- **[システム要件 🖥️](https://github.com/cafali/SnapKey/wiki/System-Requirements)**  
- **[トラブルシューティング 🔧](https://github.com/cafali/SnapKey/wiki/Troubleshoot)**  
- **[更新情報 🔄](https://github.com/cafali/SnapKey/wiki/Updates)**

---

<p align="center">
  SnapKey by  
</p>

<p align="center">
  <a href="https://github.com/cafali">@cafali</a> 
  <a href="https://github.com/minteeaa">@minteeaa</a> 
  <a href="https://github.com/shoboi">@shoboi</a> 
  <a href="https://github.com/sat0ma">@sat0ma</a> 
</p>

---

<p align="center">
  Translated by  
</p>

<p align="center">
  <a href="https://github.com/cafali">@kerubinDev</a>  
</p>
