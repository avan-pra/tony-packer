section .data

section .text
global _start
_start:
	push 0x41
	mov rdi, 1
	mov rsi, rsp
	mov rdx, 4
	mov rax, 1
	syscall
	add rsp, 4
