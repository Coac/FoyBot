# FoyBot
This program is a packet sniffer.

## Usage
- Modify the dll path in FoyBotInjector.cpp  
- Build it  
- Launch the game  
- Run FoyBotInjector.exe  

## Debug
To see the error message when a crash occur, you can use the VisualStudio debugger, very **very** useful   
In VS, ``Debug`` -> ``Attach To Process`` -> Select the process with the injected dll -> ``Attach``

## Features
- Colorful logging in console
- Send Packet function hooked
- Receive Packet function hooked
- Interact using CLI

## CLI
`send` : allow to send a packet  
Example : `send F3001300436F6163203A2048656C6C6F202100`

![Demo gif](/demo.gif)

## Disclaimer
This code is for educational purposes only, and is not guaranteed in any way. Using automated software may violate terms of service you agreed upon with your game provider.
