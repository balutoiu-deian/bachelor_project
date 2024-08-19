section .data
    filename_read db "../input_array.txt", 0   ; Input file name
    filename_write db "../output_array.txt", 0  ; Output file name
    format db "%d", 0
section .bss
    n resq 1
    timing resd 1
    arr resd 1


section .text
    extern readNumbersFromFile
    extern writeArrayToFile
    extern printf
    extern fflush
    extern malloc
    extern free
    extern atoi
    global _start

sort:
    dec edi
    .outer_loop:
        xor ecx, ecx
	.inner_loop:
	    mov eax, dword[esi + ecx*4]
	    mov ebx, dword[esi + (ecx + 1)*4]

	    cmp eax, ebx
	    jle .no_swap

	    mov dword[esi + ecx*4], ebx
	    mov dword[esi + (ecx + 1)*4], eax

	.no_swap:
	    inc ecx
	    cmp ecx, edi
   	    jl .inner_loop

        dec edi
        jnz .outer_loop

    ret

handle_overflow:
    cmp r12, r13
    jg overflow

    sub r13, r12
    jmp print_timing

overflow:
    mov rax, 0xffffffffffffffff
    sub rax, r12
    add r13, rax
    jmp print_timing

_start:
    mov rdi, [rsp + 16]
    call atoi
    mov qword [n], rax    ; make argv[1] into an int

    mov rdi, qword[n]
    imul rdi, 4
    call malloc
    mov dword [arr], eax

    mov rdi, filename_read
    mov esi, dword [arr]
    mov rdx, qword[n]
    call readNumbersFromFile

    rdtscp
    mov r12, rax

    mov rdi, qword [n]
    mov esi, dword [arr]
    call sort

    rdtscp
    mov r13, rax

    mov rdi, filename_write
    mov esi, dword [arr]
    mov rdx, qword [n]
    call writeArrayToFile

    jmp handle_overflow

print_timing:
    mov dword[timing], r13d
    mov rdi, format
    mov rsi, [timing]
    xor rax, rax
    call printf
    xor rdi, rdi
    call fflush
exit:
    mov eax, dword[arr]
    mov edi, eax
    call free
    mov rax, 60
    xor rdi, rdi
    syscall
