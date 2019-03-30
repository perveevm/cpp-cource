                section         .text
                global          _start
_start:

                pop             rax
                cmp             rax, 2
                jne             error                           ; bad arguments

                pop             rax
                pop             rax
                mov             rdi, rax
                mov             rax, 2
                xor             rsi, rsi
                xor             rdx, rdx
                syscall                                         ; open file
                
                mov             r15, rax                        ; file descriptor
                xor             r14, r14                        ; answer
                mov             r11, 1                          ; 1 if prev is whitespace, 0 else

read_loop:
                mov             rax, 0
                mov             rdi, r15
                mov             rsi, data_buffer
                mov             rdx, 1024
                syscall                                         ; read data

                cmp             rax, 0
                jl              error
                je              print_answer

                mov             r13, rax                        ; rest bytes
                mov             r12, data_buffer                ; data pointer
handler_loop:
                xor             rax, rax
                mov             al, [r12]
                call check_byte

                cmp             rax, 1
                je              handler_loop_end
                cmp             r12, data_buffer
                je              good_1
                cmp             r11, 1
                jne             handler_loop_end
good_1:
                inc             r14                             ; current is whitespace, prev is not 

handler_loop_end:
                mov             r11, rax
                inc             r12
                dec             r13
                cmp             r13, 0
                jne             handler_loop

                jmp             read_loop

print_number:
                cmp             r11, 1
                je              end1
                inc             r14

end1:
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

check_byte:
                mov             rbx, 1

                cmp             al, 0x09
                je              check_byte_end
                cmp             al, 0x0a
                je              check_byte_end
                cmp             al, 0x0b
                je              check_byte_end
                cmp             al, 0x0c
                je              check_byte_end
                cmp             al, 0x0d
                je              check_byte_end
                cmp             al, 0x20
                je              check_byte_end

                xor             rbx, rbx

check_byte_end:
                mov             rax, rbx
                ret

print_answer:
                mov             rax, r14
                call            print_number
                jmp             exit

exit:
                mov             rax, 60
                xor             rdi, rdi
                syscall                                         ; exit

error:
                mov             rax, 1
                mov             rdi, 1
                mov             rsi, error_msg
                mov             rdx, error_msg_size
                syscall
                jmp exit

                section .bss
buffer:         resb            22
data_buffer:    resb            1024

                section .rodata
error_msg:      db              "Error!",0x0a
error_msg_size: equ             $ - error_msg