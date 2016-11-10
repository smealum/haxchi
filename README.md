# haxchi

This is a ported version of the haxchi exploit created by smea and others for the european release of brain training.  
In addition to being ported it also includes a homebrew launcher loader as its payload so you can use it for a lot of things.

## install process

haxchi can be very easily installed using iosuhax's wupclient. for example, if hachihachi is installed to the MLC, it suffices to do:
```
  w.up("rom.zip", "/vol/storage_mlc01/usr/title/00050000/10179C00/content/0010/rom.zip")
```
of course, using wupclient to install haxchi permanently requires that redNAND be disabled, unless hachihachi is installed to USB, in which case it can be installed from redNAND using:
```
  w.up("rom.zip", "/vol/storage_usb01/usr/title/00050000/10179C00/content/0010/rom.zip")
```

## contents

* hbl_loader: a loader which will load up the homebrew launcher from sd card
* haxchi_rop.s: hachihachi process ROP which will emit hbl_loader as executable
* haxchi.s: generates a malicious SRL file

## credit

smea, plutoo, yellows8, naehrwert, derrek, FIX94 and dimok

