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
    ;rdi is low, rsi is high
    cmp edi, esi
    jl .partition
    ret

    .partition:
        mov eax, dword[edx + esi*4] ; pivot = arr[high]
        mov ecx, edi
        dec ecx ; i = low - 1

        mov ebx, edi ;j = low

        .for_loop:
            cmp ebx, esi
            jg .end_loop

            mov r8d, dword[edx + ebx*4]
            cmp r8d, eax
            jge .continue_loop

            inc ecx

            mov r9d, dword[edx + ecx*4]
            mov dword[edx + ebx * 4], r9d
            mov dword[edx + ecx * 4], r8d


        .continue_loop:
            inc ebx
            jmp .for_loop
        .end_loop:
            inc ecx
            mov r10d, dword[edx + ecx*4]
            mov dword[edx + ecx * 4], eax
            mov dword[edx + esi * 4], r10d

            mov eax, ecx

cmp eax, edi
    je .right_partition_only
    push rdi
    push rsi
    mov rsi, rax
    dec rsi
    call sort
    pop rsi
    pop rdi

.right_partition_only:
    push rdi
    push rsi
    mov rdi, rax
    inc rdi
    call sort
    pop rsi
    pop rdi
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

    mov edi, 0
    mov rsi, qword [n]
    dec rsi
    mov edx, dword [arr]
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


