CPU 686
BITS 32

%define EOI 20h
%define PIC1_CMD 20h
%define DATA_REG 60h

global _kbd_isr
extern _queuePut
extern _keys_queue
;global _key

section .data
;_key dd 0

section .text
_kbd_isr:
	push eax ; todos os registos utilizados na rotina
	; instruções da rotina

	mov eax, 0
	in al, DATA_REG
	;mov dword[_key], eax
	
	push eax
	push dword _keys_queue
	call _queuePut
	add esp, 8

	mov al, EOI ; sinaliza EOI para o PIC-1
	out PIC1_CMD, al
	pop eax ; todos os registos guardados
	iretd
END