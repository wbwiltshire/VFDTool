;*********************************************
;	FAT12.asm
;	A Simple Bootloader in NASM
;
;*********************************************
 
bits		16					; We are still in 16 bit Real Mode

org		0x7c00				; We are loaded by BIOS at 0x7C00

start:
		jmp loader			; jump over OEM block

;*************************************************;
;	OEM Parameter block
;*************************************************;
bpbOEM				DB "DOS5.5  "	; This member must be exactally 8 bytes.
bpbBytesPerSector:		DW 512
bpbSectorsPerCluster:	DB 1
bpbReservedSectors:		DW 1
bpbNumberOfFATs:		DB 2
bpbRootEntries:		DW 224
bpbTotalSectors:		DW 2880
bpbMedia:				DB 0xF0
bpbSectorsPerFAT:		DW 9
bpbSectorsPerTrack:		DW 18
bpbHeadsPerCylinder:	DW 2
bpbHiddenSectors:		DD 0
bpbTotalSectorsBig:		DD 0
bsDriveNumber:			DB 0
bsUnused:				DB 0
bsExtBootSignature:		DB 0x29
bsSerialNumber:		DD 0xa0a1a2a3
bsVolumeLabel:			DB "DOS FLOPPY "
bsFileSystem:			DB "FAT12   "

bootloaderMsg			DB	"Default FAT12 Bootloader started...",13,10,0
finishedMsg			DB	"Bootloader completed successfully	(halted).",13,10,0		

;***************************************
;	Prints a string
;	DS=>SI: 0 terminated string
;***************************************

Print:
	lodsb					; load next byte from string from SI to AL
	or		al, al			; Does AL=0?
	jz		PrintDone			; Yep, null terminator found-bail out
	mov		ah,	0eh			; Nope-Print the character
			int	10h
	jmp		Print			; Repeat until null terminator found
PrintDone:
	ret						; we are done, so return

;*************************************************;
;	Bootloader Entry Point
;*************************************************;

loader:
	xor		ax, ax			; Setup segments to insure they are 0. Remember that
	mov		ds, ax			; we have ORG 0x7c00. This means all addresses are based
	mov		es, ax			; from 0x7c00:0. Because the data segments are within the same
							; code segment.	
	mov		si, bootloaderMsg	; message to print
	call		Print			; call print

	mov		si, finishedMsg	; message to print
	call		Print			; call print
	 
	cli
	hlt
	
times 510 - ($-$$) db 0			; We have to be 512 bytes. Clear the rest of the bytes with 0
 
	DW 0xAA55					; Boot Signiture