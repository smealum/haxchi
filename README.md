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
10179A00 - JPN Brain Age  
10179B00 - US Brain Age  
10179C00 - PAL Brain Training  
10179D00 - JPN Yoshi Touch and Go  
10179E00 - US Yoshi Touch and Go  
10179F00 - PAL Yoshi Touch and Go  
10195600 - JPN Mario Kart DS  
10195700 - US Mario Kart DS  
10195800 - PAL Mario Kart DS  
10195900 - JPN New Super Mario Bros  
10195A00 - US New Super Mario Bros  
10195B00 - PAL New Super Mario Bros  
10198800 - JPN Yoshi's Island DS  
10198900 - US Yoshi's Island DS  
10198A00 - PAL Yoshi's Island DS  
101A1E00 - JPN WarioWare: Touched  
101A1F00 - US WarioWare: Touched  
101A2000 - PAL WarioWare: Touched  
101A5500 - JPN Kirby Squeak Squad  
101A5600 - US Kirby Squeak Squad  
101A5700 - PAL Kirby Mouse Attack  
101AC000 - JPN Star Fox Command  
101AC100 - US Star Fox Command  
101AC200 - PAL Star Fox Command  
101C3300 - JPN Super Mario 64 DS  
101C3400 - US Super Mario 64 DS  
101C3500 - PAL Super Mario 64 DS  
101C3600 - JPN Zelda Phantom Hourglass  
101C3700 - US Zelda Phantom Hourglass  
101C3800 - PAL Zelda Phantom Hourglass  

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

