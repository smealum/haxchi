# haxchi

haxchi is an exploit for the Nintendo DS virtual console emulator on Wii U (hachihachi). it is possible due to "contenthax", a vulnerability in the wii u's title integrity design: only code and critical descriptors are signed, with all other contents left at the mercy of attackers. this can be exploited simply by asking IOSU to copy over files in /content/ directories on either MLC or USB. contenthax can also be exploited from powerpc userland by using the MCP_CopyTitle command. as there is no integrity data for that content, CopyTitle cannot validate the malicious content and will therefore happily copy it from SD card to MLC or USB if asked.

it is likely that virtually all apps can be exploited in some way through contenthax, due to developers being less likely to program defensively against content that they should be the only ones to have control over. the Nintendo DS virtual console app was selected for this exploit because it has the ability to dynamically emit executable code. as a nice bonus, hachihachi includes symbols for its code. haxchi exploits a bug in the emulator's rom loader, and basically gets it to perform arbitrary memcpy operations. from there, achieving code execution is trivial given that there is no ASLR in place.

note that haxchi was my first time doing PPC ROP so... yeah

## coldboothax

haxchi (and indeed any other contenthax) can be used to achieve persistent automatic unsigned code execution on the wii u. this is due to the fact that the wii u can be configured to boot into any given title simply by modifying a file on the SLC. the file in question is sys/config/system.xml, and <default_title_id type="hexBinary" length="8">...</default_title_id> can be set to any arbitrary title ID, such as hachihachi's.

**NOTE**: it is *very* easy to brick a wii u by messing with this file, so don't do it unless you really know what you're doing.

## install process

haxchi can be very easily installed using iosuhax's wupclient. for example, if hachihachi is installed to the MLC, it suffices to do:
```
  w.up("rom.zip", "/vol/storage_mlc01/usr/title/00050000/101A5600/content/0010/rom.zip")
```
of course, using wupclient to install haxchi permanently requires that redNAND be disabled, unless hachihachi is installed to USB, in which case it can be installed from redNAND using:
```
  w.up("rom.zip", "/vol/storage_usb01/usr/title/00050000/101A5600/content/0010/rom.zip")
```
coldboothax can be installed by downloading system.xml as so:
```
  w.dl("/vol/system/config/system.xml")
```
modifying it, and then uploading it back:
```
  w.up("system.xml", "/vol/system/config/system.xml")
```
## contents

* haxchi_code: basic demo (native code)
* haxchi_rop.s: hachihachi process ROP which will emit haxchi_code as executable
* haxchi.s: generates a malicious SRL file

## credit

smea, plutoo, yellows8, naehrwert and derrek

