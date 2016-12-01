# Haxchi

This is the continuation of the POC Haxchi exploit by smea.  
It features compatibility with a lot of DS VC and can be easly installed and further configured.

## Installation

If you already have a installation of v1.7 or v1.8 then this wont bring anything new for you, it just makes it far simpler for new people.  
It now comes with its own installer and does not need iosuhax, wupclient or any additional scripts anymore, just your wiiu, a DS VC game and a way to run the homebrew launcher.  
If you happen to have a DS VC title that is listed in this readme then go ahead and grab both haxchi .elf and .zip from the "releases" tab, make sure to NOT click on "source code" but on haxchi.elf and haxchi.zip.  
The .elf goes into sd:/wiiu/apps and the .zip should just be extracted to sd:/haxchi with all its contents inside. That content right now just consists of a simple replacement icon, logo and replacing the game title with "Haxchi", its example config.txt will boot homebrew launcher by default and a fw.img on your sd card when holding A. For a full list of all compatible buttons that you can use for the config.txt go [here](dsrom/option_select/main.c#L57-L75).  
The content of this haxchi folder can be changed to your liking - if you want to you can also add in an alternative bootSound.btsnd to replace the original startup sound which I did not do in this example haxchi folder.  
After setting up the content to your liking all you have to do is run the haxchi .elf in homebrew launcher, select the game you want to install it on and that is it! If you ever want to make changes to the content folder it installed to then just re-run the haxchi .elf and install it again, you dont have to reinstall the game beforehand, it'll just overwrite the previous haxchi installation with your new data.  
Please note, this will ONLY WORK WITH DS VC GAMES ON NAND, if you have a ds vc game on USB you want to use then please move it to your NAND first and ideally detach your usb device before using this installer .elf, if you dont remove your usb devices it may freeze up on exiting or not install properly.  
This also ONLY LOADS THE .ELF VERSION OF THE HOMBEBREW LAUNCHER which as of right now is v1.4 so make sure to keep that on your sd card or you will just get error -5 when starting your haxchi channel. Once you are in the homebrew launcher it is also perfectly compatible with loading .rpx files, you just cant use haxchi itself to load .rpx files.    

The following games right now are supported by the installer:  

| Game | JPN | US | PAL |
|---|:---:|:---:|:---:|
|Brain Age (Training)|10179A00|10179B00|10179C00|
|Yoshi Touch and Go|10179D00|10179E00|10179F00|
|Mario Kart DS|10195600|10195700|10195800|
|New Super Mario Bros|10195900|10195A00|10195B00|
|Yoshi's Island DS|10198800|10198900|10198A00|
|Big Brain Academy|10198B00|10198C00|10198D00|
|WarioWare: Touched|101A1E00|101A1F00|101A2000|
|Mario and Luigi: Partners in Time|101A2100|101A2200|101A2300|
|DK Jungle Climber|101A5200|101A5300|101A5400|
|Kirby Squeak Squad(Mouse Attack)|101A5500|101A5600|101A5700|
|Wario Master of Disguise|101ABD00|101ABE00|101ABF00|
|Star Fox Command|101AC000|101AC100|101AC200|
|Kirby Canvas Curse|101B8800|101B8900|101B8A00|
|Zelda Spirit Tracks|101B8B00|101B8C00|101B8D00|
|Super Mario 64 DS|101C3300|101C3400|101C3500|
|Zelda Phantom Hourglass|101C3600|101C3700|101C3800|
|Kirby Mass Attack|101C8600|101C8700|101C8800|
|Pokemon Ranger: Shadows of Almia|101E0F00|101E1000|101E1100|

## Dependencies

To properly compile this project yourself you will need the latest libiosuhax from dimok789's github.  

## credit

smea, plutoo, yellows8, naehrwert, derrek, FIX94 and dimok
