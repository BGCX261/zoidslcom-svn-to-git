CPU 686
BITS 32

%define DATA_REG 0x60
%define EOI 0x20
%define PIC1_CMD 0x20
%define PIC2_CMD 0xA0

global _mouse_isr
global _pack_completo

extern _mouse_queue
extern _putGQueue


section .data
_pack_completo dd 0
next_byte dd 1
package db 0, 0, 0



section .text

_mouse_isr:
	push eax
	push ecx
	
	xor eax, eax
	in al, DATA_REG
	
	mov ecx, dword[next_byte]
	
	cmp ecx, 3
	je byte3
	cmp ecx, 2
	je byte2
	
byte1:
	mov byte[package], al
	mov ecx, 2
	jmp fim

byte2:
	mov byte[package + 1], al
	mov ecx, 3
	jmp fim
	
byte3:
	mov byte[package + 2], al
	push package
	push dword[_mouse_queue]
	call _putGQueue
	add esp, 8
	mov ecx, 1

fim:
	mov dword[next_byte], ecx
	mov al, EOI
	out PIC1_CMD, al
	out PIC2_CMD, al
	pop ecx
	pop eax
	iretd
END