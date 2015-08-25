CPU 686
BITS 32

%define EOI 20h
%define PIC1_CMD 20h

global _handler
global _time_tick
global _sec_tick

section .data
_time_tick dd 0
_sec_tick dd 0 
_contador dd 1000

section .text
_handler:
	push eax
	inc dword [_time_tick]
	dec dword [_contador]

	cmp dword [_contador], 0
	jne FIM
	inc dword [_sec_tick]
	mov dword [_contador], 1000
FIM:
	mov al, EOI
	out PIC1_CMD, al
	pop eax
	iretd
END