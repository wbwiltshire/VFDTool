VFDTool
===

A simple utility to create Virtual Floppy Disk (VFD) files
* Print info
* Create
* Create with boot sector

Options
---
1. Print VFD info
```
  bin\vfdtool.exe -i BlankFloppy.vfd
```
1. Create VFD
```
  bin\vfdtool.exe -c BlankFloppy.vfd
```
1. Create VFD with boot sector
```
  bin\vfdtool.exe -c -b boot.bin BlankFloppy.vfd
```

For more information:
* [FAT12 Info](https://wiki.osdev.org/FAT)
* [BIOS Parameter Block](http://brokenthorn.com/Resources/OSDev4.html)
* [FAT12 Access](http://brokenthorn.com/Resources/OSDev6.html)