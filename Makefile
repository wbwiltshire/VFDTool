# VFDTool makefile
# Link: https://docs.microsoft.com/en-us/cpp/build/nmake-reference
CC=cl
AS=nasm
#CFLAGS=/c /EHsc
#Compile with debug info
CFLAGS=/c /EHsc /Zi
ASFLAGS=-f bin
LINK=link
#LFLAGS=
#Link with debug info
LFLAGS= /DEBUG /SUBSYSTEM:CONSOLE /MACHINE:X64
ODIR=bin
ASMDIR=bin
EXE=VFDTool.exe

all: $(ODIR)\$(EXE) $(ASMDIR)\FAT12.bin

.asm{$(ASMDIR)}.bin:  
	$(AS) $(ASLAGS) $** -o$@

.cpp{$(ODIR)}.obj: 
	$(CC) $(CFLAGS) $** /Fo$@

$(ODIR)\$(EXE): $(ODIR)\main.obj $(ODIR)\options.obj $(ODIR)\floppydrive.obj $(ODIR)\biosparmblock.obj $(ODIR)\directory.obj $(ODIR)\fat12.obj
	$(LINK) $** $(LFLAGS) /out:$@

test:
	@bin\$(EXE) -i BlankFloppy.VFD
#	@bin\$(EXE) -c TestFloppy.VFD
#	@bin\$(EXE) -c -b bin\FAT12.bin TestFloppy.VFD
#	@bin\$(EXE) -l TestFloppy.VFD
#	@bin\$(EXE) -a main.cpp TestFloppy.VFD
#	@bin\$(EXE) -r main.cpp TestFloppy.VFD

clean:
	@IF EXIST *.pdb (del *.pdb)
	@IF EXIST bin\*.obj (del bin\*.obj)
	@IF EXIST bin\*.pdb (del bin\*.pdb)
	@IF EXIST bin\*.exe (del bin\*.exe)
	@IF EXIST bin\*.asm (del bin\*.asm)
	@IF EXIST bin\*.ilk (del bin\*.ilk)
	@IF EXIST bin\*.bin (del bin\*.bin)