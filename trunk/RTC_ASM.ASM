CPU 686
BITS 32

%define RTC_ADD_REG 70h
%define RTC_DATA_REG 71h

%define RTC_SEC 0
%define RTC_MIN 2
%define RTC_HOUR 4

%define RTC_STAT_A 10
%define RTC_STAT_B 11
%define RTC_STAT_C 12
%define RTC_STAT_D 13

%define RTC_SET 10000000b

; function arguments, assuming return adddress and old basepointer (ebp) in stack
%define arg1 [ebp+8]
%define arg2 [ebp+12]
%define arg3 [ebp+16]
%define arg4 [ebp+20]

global _get_time
global _get_second_of_day

extern _rtc_valid
extern _read_rtc
extern _write_rtc
extern _bcd2dec

;-----------------------------------------------

section .data

temp_hour dd 0
temp_min dd 0
temp_sec dd 0
not_bcd dd 0

;-----------------------------------------------

section .text

_get_time:
	push ebp
	mov ebp, esp
	push ebx
	
	;colocar rtc_set a 1

	call _rtc_valid
	push RTC_STAT_B
	call _read_rtc
	add esp, 4
	or eax, RTC_SET
	push eax
	push RTC_STAT_B
	call _write_rtc
	add esp, 8

	;verificar bcd

	push RTC_STAT_B
	call _read_rtc
	add esp, 4
	and eax, 100b
	cmp eax, 0
	je passo
	mov dword[not_bcd], 1
	
	
passo:	push RTC_HOUR
	call _read_rtc
	add esp, 4
	cmp dword[not_bcd], 1
	je passo2
	push eax
	call _bcd2dec
	add esp, 4
passo2: mov ebx, arg1
	mov [ebx], eax
	
	push RTC_MIN
	call _read_rtc
	add esp, 4
	cmp dword[not_bcd], 1
	je passo3
	push eax
	call _bcd2dec
	add esp, 4
passo3:	mov ebx, arg2
	mov [ebx], eax
	
	push RTC_SEC
	call _read_rtc
	add esp, 4
	cmp dword[not_bcd], 1
	je passo4
	push eax
	call _bcd2dec
	add esp, 4
passo4:	mov ebx, arg3
	mov [ebx], eax
	
	;por rtc_set a 0

	push RTC_STAT_B
	call _read_rtc
	add esp, 4
	mov ebx, RTC_SET
	not ebx
	and eax, ebx
	push eax
	push RTC_STAT_B
	call _write_rtc
	add esp, 8
	
	pop ebx
	mov esp, ebp
	pop ebp
	ret


_get_second_of_day:
	push ebp
	mov ebp, esp
	
	push temp_sec
	push temp_min
	push temp_hour
	call _get_time
	add esp, 12
	
	mov ecx, 0
	mov ecx, [temp_sec]
	
	mov eax, [temp_min]
	mov edx, 60
	mul edx
	add ecx, eax
	
	mov eax, [temp_hour]
	mov edx, 3600
	mul edx
	add ecx, eax
	
	mov eax, ecx
	
	mov esp, ebp
	pop ebp
	ret
END
	
	