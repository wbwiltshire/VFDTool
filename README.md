VFDTool
===

A simple utility to create Virtual Floppy Disk (VFD) files
* Print info
* Create
* Create with boot sector
* List directory
* Add file
* Remove file

Options
---
1. Print VFD info
```
  bin\vfdtool.exe -i BlankFloppy.vfd
```
1. Create VFD
```
  bin\vfdtool.exe -c TestFloppy.vfd
```
1. Create VFD with boot sector
```
  bin\vfdtool.exe -c -b bin\nbootloader.bin TestFloppy.vfd
```
1. List files in directory of a VFD
```
  bin\vfdtool.exe -l TestFloppy.vfd
```
1. Add a file to a VFD
```
  bin\vfdtool.exe -a kernel.sys TestFloppy.vfd
```
1. Remove a file from a VFD
```
  bin\vfdtool.exe -r kernel.sys TestFloppy.vfd
```

To Do
---
1. Initialize FAT table (FAT12/FAT16)
 * Create with boot sector (update messaage: Created; Wrote; Formatted)
1. Write boot sector w/o create
 * Update message (Write, Re-formatted)
 * Reformat FAT tables and Directory entries (leave data alone, but document)
1. Robust command line option validation (combinations/use cases)
1. Refactor opening VFD file
 * Access: read/write (create, write, add, and remove) vs read (info and list)
 * Close in floppydrive destructor
1. Add functionality
 * Determine file size
 * Build cluster chain and write data
 * Add directory entry
1. Remove functionality 
  * mark as deleted with E5
  * reset cluster chain
1. Update print and find directory to skip (E5) entries
1. Refactor all the File I/O to do things once (read or write)

For more information:
* [Reading the FAT](https://www.sqlpassion.at/archive/2022/03/03/reading-files-from-a-fat12-partition/)
* [Reading Files from a FAT12 Partition](http://www.independent-software.com/operating-system-development-file-allocation-table-and-reading-from-disk.html)
* [FAT12 Info](https://wiki.osdev.org/FAT)
* [BIOS Parameter Block](http://brokenthorn.com/Resources/OSDev4.html)
* [FAT12 Access](http://brokenthorn.com/Resources/OSDev6.html)
* [Design of FAT File System](https://en.wikipedia.org/wiki/Design_of_the_FAT_file_system)
* [FAT File System Specification](http://download.microsoft.com/download/1/6/1/161ba512-40e2-4cc9-843a-923143f3456c/fatgen103.doc)
* [GNU MTools - Documentation](https://www.gnu.org/software/mtools/manual/mtools.html)
* [GNU MTools on Github](https://github.com/Distrotech/mtools)