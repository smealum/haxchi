# haxchi

This is a ported version of the haxchi exploit created by smea and others for all sorts of different ds vc games.  
In addition to being ported it also includes a homebrew launcher loader as its payload so you can use it for a lot of things.

## install process

haxchi can be very easily installed using iosuhax's wupclient. for example, if hachihachi is installed to the MLC, it suffices to do:
```
  w.up("rom.zip", "/vol/storage_mlc01/usr/title/00050000/YOUR_GAME_TITLE_ID/content/0010/rom.zip")
```
of course, using wupclient to install haxchi permanently requires that redNAND be disabled, unless hachihachi is installed to USB, in which case it can be installed from redNAND using:
```
  w.up("rom.zip", "/vol/storage_usb01/usr/title/00050000/YOUR_GAME_TITLE_ID/content/0010/rom.zip")
```

To upload a config.txt as well you will have to do the following if the title is installed on NAND:
```
  w.up("config.txt", "/vol/storage_mlc01/usr/title/00050000/YOUR_GAME_TITLE_ID/content/config.txt")  
  w.chmod("/vol/storage_mlc01/usr/title/00050000/YOUR_GAME_TITLE_ID/content/config.txt", 0x644)  
```
and for the title being installed to USB you do the following:
```
  w.up("config.txt", "/vol/storage_usb01/usr/title/00050000/YOUR_GAME_TITLE_ID/content/config.txt")  
  w.chmod("/vol/storage_usb01/usr/title/00050000/YOUR_GAME_TITLE_ID/content/config.txt", 0x644)  
```

make sure to replace YOUR_GAME_TITLE_ID with one of the following:  

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
|Super Mario 64 DS|101C3300|101C3400|101C3500|
|Zelda Phantom Hourglass|101C3600|101C3700|101C3800|
|Kirby Mass Attack|101C8600|101C8700|101C8800|

a config.txt can look like this for example:
```
a=fw.img
b=rednand/fw.img
x=wiiu/apps/ftpiiu/ftpiiu.elf
down=wiiu/apps/snes9x2010_libretro/snes9x2010_libretro.elf
default=wiiu/apps/homebrew_launcher/homebrew_launcher.elf
```
please note though that if you want to load a fw.img the name HAS to be named fw.img in whatever folder you use, nothing else will work.  

## contents

* hbl_loader: a loader which will load up the homebrew launcher from sd card
* haxchi_rop.s: hachihachi process ROP which will emit hbl_loader as executable
* haxchi.s: generates a malicious SRL file

## credit

smea, plutoo, yellows8, naehrwert, derrek, FIX94 and dimok
