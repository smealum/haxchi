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
make sure to replace YOUR_GAME_TITLE_ID with one of the following:  
10179B00 - US Brain Age  
10179C00 - PAL Brain Training  
10198900 - US Yoshi's Island DS  
10198A00 - PAL Yoshi's Island DS  
101A1F00 - US WarioWare: Touched  
101A2000 - PAL WarioWare: Touched  
101A5600 - US Kirby Squeak Squad  
101A5700 - PAL Kirby Mouse Attack  

## contents

* hbl_loader: a loader which will load up the homebrew launcher from sd card
* haxchi_rop.s: hachihachi process ROP which will emit hbl_loader as executable
* haxchi.s: generates a malicious SRL file

## credit

smea, plutoo, yellows8, naehrwert, derrek, FIX94 and dimok

