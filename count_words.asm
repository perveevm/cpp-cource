                section         .text
                global          _start

_start:
                pop             rax
                cmp             rax, 2
                jne             error

                pop             rax
                pop             rdi
                mov             rax, 2
                xor             rsi, rsi
                xor             rdx, rdx
                syscall
                cmp             rax, 0
                jl              error

                mov             rcx, rax                        ; copy of rax for all next reads
                
                xor             rbx, rbx                        ; words counter

                mov             rdi, rcx
                xor             rax, rax
                mov             rsi, buf
                mov             rdx, buf_size
                syscall

                cmp             rax, 0
                jl              error
                je              exit

                xor             rdi, rdi                        ; current position

skip_spaces:
                mov             r10, 9
                                                                ; check spaces check symbols 9, 10, 11, 12, 13
check_spaces:
                cmp             [buf + rdi], r10b
                je              cycle_end

                inc             r10
                cmp             r10, 14
                jl              check_spaces

                mov             r10, 32
                cmp             [buf + rdi], r10b
                je              cycle_end

                mov             r10, 0
                cmp             [buf + rdi], r10b
                je              print_res
                
                inc             rbx
                jmp             skip_word

skip_word:
                mov             r10, 9
check_spaces2:
                cmp             [buf + rdi], r10b
                je              skip_spaces

                inc             r10
                cmp             r10, 14
                jl              check_spaces2

                mov             r10, 32
                cmp             [buf + rdi], r10b
                je              skip_spaces

                mov             r10, 0
                cmp             [buf + rdi], r10b
                je              print_res

                inc             rdi
                jmp             skip_word               

cycle_end:
                inc             rdi
                jmp             skip_spaces

                                                                        ; print_res prints rax to stdin
print_res:
                mov             rax, rbx
                xor             rdi, rdi
while:
                xor             rdx, rdx                                ; high part = 0
                mov             rbx, 10
                div             rbx
                add             rdx, 48
                mov             [num + rdi], rdx
                inc             rdi

                cmp             rax, 0
                jg              while

                mov             rbx, rdi
                dec             rbx

                                                                        ; real printing
while2:
                mov             rdx, 1                                  ; size of output
                mov             rax, 1
                mov             rdi, 1
                mov             rsi, num
                add             rsi, rbx
                syscall
                dec             rbx

                cmp             rbx, 0
                jnl             while2

                call            exit

error:
                mov             rax, 1
                mov             rdi, 1
                mov             rsi, err
                mov             rdx, 6
                syscall
                call            exit

exit:
                mov             rax, 60
                xor             rdi, rdi
                syscall

                section         .rodata
err:            db              "Error!"

                section         .bss
buf_size:       equ             16 * 1024
num_size:       equ             20
buf:            resb            buf_size
num:            resb            num_size
