[![testsnap](https://github.com/user-attachments/assets/e9a23ba9-d394-4711-abfc-994932605d86)](https://github.com/cafali/SnapKey/releases)

**Sobre o SnapKey**  
--------------------------------------------------------------------------------------------------  
SnapKey oferece uma alternativa fácil de usar para a função Razer Snap Tap, tornando-a acessível para todos os teclados!

SnapKey é uma ferramenta leve e de código aberto que opera a partir da bandeja do sistema, projetada para rastrear entradas das teclas WASD, sem interferir nos arquivos de jogo. Sua função principal é reconhecer quando essas teclas são pressionadas e liberar automaticamente qualquer comando anteriormente ativado para elas. Isso garante um manuseio responsivo e preciso das entradas. O SnapKey lida com as teclas WASD por padrão, permitindo que você as redefina ao seu gosto através do arquivo de configuração.

**Precisa de mais informações sobre o SnapKey?** Visite o [**SnapKey Wiki**](https://github.com/cafali/SnapKey/wiki)

[![COMPATÍVEL](https://github.com/user-attachments/assets/069a7a23-cfe4-47eb-8ac2-05872fcc2028)](https://github.com/cafali/SnapKey/wiki/Compatibility-List)

Download  
--------------------------------------------------------------------------------------------------  
<p align="center">  
  <a href="https://github.com/cafali/SnapKey/releases">Baixar do GitHub</a> |  
  <a href="https://sourceforge.net/projects/snapkey/">Baixar do SourceForge</a> |  
  <a href="https://www.softpedia.com/get/Tweak/System-Tweak/SnapKey.shtml">Baixar do Softpedia</a>  
</p>

[![latesver](https://github.com/user-attachments/assets/09694f7c-6eeb-4c80-9a02-1d777956d181)](https://github.com/cafali/SnapKey/wiki/Updates)

**Recursos do SnapKey**  
--------------------------------------------------------------------------------------------------  
- Fácil de usar 🧩  
- Documentação detalhada 📖  
- Leve e de código aberto 🌟  
- Acessível pela bandeja do sistema 🖥️  
- Compatível com todos os teclados ✅  
- Não interage com os arquivos do jogo 🎮  
- Ative/Desative pelo menu de contexto ⛔  
- Função Sticky Keys: rastreia o estado de uma tecla pressionada ⌨️  
- Melhora a precisão de movimentos de contra-strafe em jogos 🎯  
- Permite a reconfiguração de teclas usando códigos ASCII especificados no arquivo de configuração 🛠️  
- Suporta uma quantidade ilimitada de teclas compartilhadas entre grupos (padrão AD / WS) 🔄  
- Facilita transições mais suaves entre movimentos para a esquerda e direita sem conflitos de entrada 🚀  
- Não utiliza AutoHotkey ou ferramentas similares; suas funcionalidades dependem apenas das funções da API do Windows 🛡️  

**SnapKey em Ação**  
--------------------------------------------------------------------------------------------------  
![Explain](https://github.com/user-attachments/assets/7d11c221-33d8-43ea-839b-bfe276332593)

- Quando você pressiona e segura a tecla **"A"**, o SnapKey a memoriza.  
- Se você então pressionar a tecla **"D"** enquanto ainda segura **"A"**, o SnapKey libera automaticamente a tecla **"A"** para você.  
- O mesmo acontece se você pressionar **"A"** enquanto segura **"D"** — SnapKey libera a tecla **"D"**.  

**SnapKey previne conflitos de teclas de movimento simultâneas (AD / WS)**

- Em muitos jogos FPS, pressionar as teclas **"A"** e **"D"** simultaneamente geralmente resulta em conflitos de entrada reconhecidos pelo jogo. O SnapKey libera automaticamente a tecla anteriormente pressionada quando uma nova entrada de tecla é detectada.
- As teclas são separadas em dois grupos diferentes: A/D e W/S. Em cada grupo, **"A"** cancela **"D"** e vice-versa, enquanto o mesmo se aplica a **"W"** e **"S"**. Esses grupos não interferem entre si e funcionam separadamente.

**Sticky Keys**

- Sticky Keys é uma funcionalidade que rastreia o estado de uma tecla que você pressionou. Por exemplo, se você segurar a tecla **"A"** e tocar repetidamente na tecla **"D"**, cada pressionamento de **"D"** substituirá temporariamente a tecla **"A"**. Quando você soltar a tecla **"D"**, a ação associada à tecla **"A"** será retomada, desde que você ainda a esteja segurando. O mesmo princípio se aplica se você começar com **"D"** pressionada e depois pressionar **"A"**.

![Snapkey](https://github.com/user-attachments/assets/504ffa5e-50d3-4a77-9016-70f22d143cb1)

**Precisão aprimorada do contra-strafe**

- Libera automaticamente uma tecla anteriormente pressionada quando uma nova tecla (A/D) & (W/S) é pressionada.

<img src="https://github.com/user-attachments/assets/4453aba4-b9bc-45e8-8a80-80caad39347b" width="600" height="338" alt="STRAFE">

**Guia Rápido para Usar o SnapKey**  
--------------------------------------------------------------------------------------------------  
![use](https://github.com/user-attachments/assets/4d9fdf11-ec72-459c-995e-364bf1fc8d66)

1. Obtenha o SnapKey:  
   - [Baixe](https://github.com/cafali/SnapKey/releases), descompacte e inicie SnapKey.exe

<img width="417" alt="image" src="https://github.com/user-attachments/assets/e4606577-6edb-4c12-ad6e-9e5ee01ba18f">

2. Bandeja do Sistema:  
   - O SnapKey aparecerá na sua bandeja do sistema

![tray](https://github.com/user-attachments/assets/26c08735-76f2-4bc0-aa75-44c9a866b453)

3. Pronto para Uso:  
   - O SnapKey está agora em execução e pronto para ser usado
   
4. Reconfigurar Teclas (Opcional):  
   - [Reconfigure suas teclas](https://github.com/cafali/SnapKey/wiki/Rebinding-Keys) se estiver usando um layout de teclado não padrão ou se quiser mapear teclas diferentes

Desativar SnapKey  
--------------------------------------------------------------------------------------------------  
- Clique com o botão direito no ícone do SnapKey na bandeja do sistema e selecione "Desativar SnapKey". Esta opção desativa toda a funcionalidade do SnapKey para as teclas definidas no arquivo de configuração, mantendo o processo do SnapKey e o software em execução na bandeja.

![image](https://github.com/user-attachments/assets/4f9d8d30-4674-4361-bc87-84ee70ce4116)

Sair do SnapKey  
--------------------------------------------------------------------------------------------------  
   - Clique com o botão direito no ícone do SnapKey na bandeja do sistema e selecione "Sair do SnapKey"

![image](https://github.com/user-attachments/assets/c3a9534e-ed8b-4258-be56-37b6117c1814)

**Suporte para Linux**  
--------------------------------------------------------------------------------------------------  
Como o SnapKey não é suportado nativamente no Linux, recomenda-se verificar o guia do @Dillacorn sobre **[executar SnapKey no Linux](https://github.com/cafali/SnapKey/issues/4#issuecomment-2251568839)**.

[![LINUX baner](https://github.com/user-attachments/assets/794a16ed-b0ab-4320-a680-52bda1ca0fd1)](https://github.com/cafali/SnapKey/wiki/Setup-Linux)

Procurando Mais Informações? Tem Perguntas ou Precisa de Ajuda?  
--------------------------------------------------------------------------------------------------  
[<img src="https://github.com/user-attachments/assets/0c6d7564-6471-49f2-9367-64f7bffb7e37" alt="Wikitest" width="50%" />](https://github.com/cafali/SnapKey/wiki)

- **[Sobre ℹ️](https://github.com/cafali/SnapKey/wiki/About)**  
  Saiba mais sobre Snapkey, seus recursos e o que ele pode fazer por você.

- **[Desmembramento do Código 🧠](https://github.com/cafali/SnapKey/wiki/Code-Breakdown)**  
  Mergulhe nos detalhes da estrutura do código do SnapKey.

- **[Lista de Compatibilidade 🎮](https://github.com/cafali/SnapKey/wiki/Compatibility-List)**  
  Jogos que funcionam com o SnapKey e aqueles que não funcionam.

- **[FAQ❓](https://github.com/cafali/SnapKey/wiki/FAQ)**  
  Encontre respostas para perguntas comuns sobre Snapkey.

- **[Licença 📜](https://github.com/cafali/SnapKey/wiki/License)**  
  Visão geral da licença do Snapkey.