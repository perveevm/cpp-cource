                section         .text
                global          _start
_start:
                pop             rax
                pop             rax
                pop             rdi

                mov             rax, 2
                xor             rsi, rsi
                xor             rdx, rdx
                syscall                                         ; open file

                mov             r15, rax

                xor r10, r10
input_loop:
                xor             rax, rax
                mov             rdi, r15
                mov             rsi, text_buffer
                mov             rdx, 1024
                syscall                                         ; read bytes

                cmp             rax, 0
                je              exit

                mov             r9, text_buffer                 ; buffer pointer
                xor             r8, r8                          ; number of checked bytes
string_loop:
                mov             bl, 0x0a
                mov             cl, [r9]
                cmp             bl, cl
                jne             not_eoln
                inc             r10
not_eoln:
                inc             r9
                inc             r8
                cmp             r8, rax
                jl              string_loop

                jmp             input_loop

exit:
                mov             rax, r10
                call            print_number

                mov             rax, 60
                xor             rdi, rdi
                syscall                                         ; exit

print_number:
                mov             [buffer + 20], word 0x0a
                mov             r9,  buffer + 19                ; buffer ptr
                mov             r8,  2                          ; number of digits
loop:
                xor             rdx, rdx
                mov             rbx, 10
                div             rbx                             ; dividing number by 10

                add             rdx, '0'                
                mov             [r9], dl                        ; loading digit to buffer
                dec             r9
                inc             r8                              ; updating variables

                dec             r10

                cmp             rax, 0
                je              end
                jg				loop
end:
                inc             r8

                mov             rax, 1
                mov             rdi, 1
                mov             rsi, r9
                mov             rdx, r8
                syscall                                         ; printing number

                ret

                section .bss
buffer:         resb            22
text_buffer:    resb            1024