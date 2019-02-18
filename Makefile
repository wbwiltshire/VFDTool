# VFDTool makefile
# Link: https://docs.microsoft.com/en-us/cpp/build/nmake-reference
CC=cl
#CFLAGS=/c /EHsc
#Compile with debug info
CFLAGS=/c /EHsc /Zi
LINK=link
#LFLAGS=
#Link with debug info
LFLAGS= /DEBUG /SUBSYSTEM:CONSOLE /MACHINE:X64
ODIR=bin
EXE=VFDTool.exe

all: $(ODIR)\$(EXE)

.cpp{$(ODIR)}.obj: 
	$(CC) $(CFLAGS) $** /Fo$@

$(ODIR)\$(EXE): $(ODIR)\main.obj $(ODIR)\options.obj $(ODIR)\biosparmblock.obj
	$(LINK) $** $(LFLAGS) /out:$@

test:
	@bin\$(EXE) -i BlankFloppy.VFD
#	@bin\$(EXE) -c BlankFloppy.VFD
#	@bin\$(EXE) -c -s settings.json BlankFloppy.VFD

clean:
	@IF EXIST *.pdb (del *.pdb)
	@IF EXIST bin\*.obj (del bin\*.obj)
	@IF EXIST bin\*.pdb (del bin\*.pdb)
	@IF EXIST bin\*.exe (del bin\*.exe)
	@IF EXIST bin\*.asm (del bin\*.asm)
	@IF EXIST bin\*.ilk (del bin\*.ilk)