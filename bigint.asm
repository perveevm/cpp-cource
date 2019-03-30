; arithmetical operations with long numbers

                section         .text
                global          _start
_start:
                sub             rsp, 2 * 8 * 256
                lea             rdi, [rsp + 8 * 256]
                mov             rcx, 256
                call            read_long
                mov             rdi, rsp
                call            read_long

                lea             rdi, [rsp + 8 * 256]
                mov             rsi, rsp

                sub             rsp, 8 * 512
                mov             r8, rsp

                call            mul_long_long

                mov             rcx, 512

                call            write_long

                mov             al, 0x0a
                call            write_char

; exits with code 0
exit:
                mov             rax, 60
                xor             rdi, rdi
                syscall

; prints string to stdout
;   rsi -- string
;   rdx -- size
write_string:
                push            rcx
                push            rax
                push            rdi             ; save registers

                mov             rax, 1
                mov             rdi, 1
                syscall                         ; print string

                pop             rdi
                pop             rax
                pop             rcx
                ret                             ; restore registers and return

; prints char to stdout
;   al -- char
write_char:
                push            rcx
                push            rax
                push            rdi
                push            rsi
                push            rdx             ; save registers

                dec             rsp
                mov             [rsp], al       ; add char to stack

                mov             rax, 1
                mov             rdi, 1
                mov             rsi, rsp
                mov             rdx, 1
                syscall                         ; write char to stdout

                inc             rsp             ; pop char from stack

                pop             rdx
                pop             rsi
                pop             rdi
                pop             rax
                pop             rcx
                ret                             ; restore registers and return

; prints long number to stdout
;   rdi -- long number
;   rcx -- size of a number in qwords
write_long:
                push            rcx
                push            r8
                push            r9
                push            rbx
                push            rdx
                push            rsi

                xor             r8, r8
                mov             r9, rsp

.loop:
                mov             rbx, 10
                call            div_long_short
                add             rdx, '0'
                dec             rsp
                inc             r8
                mov             [rsp], dl
                call            is_zero
                jnz             .loop

                mov             rdx, r8
                mov             rsi, rsp
                call            write_string
                mov             rsp, r9

                pop             rsi
                pop             rdx
                pop             rbx
                pop             r9
                pop             r8
                pop             rcx
                ret


; reads char from stdin
; result:
;   rax == -1 if error occurs
;   rax \in [0; 255] if OK
read_char:
                push            rcx
                push            rdi
                push            rsi
                push            rdx             ; save registers

                dec             rsp             ; reserve byte in stack for char

                xor             rax, rax
                xor             rdi, rdi
                mov             rsi, rsp
                mov             rdx, 1
                syscall                         ; read char from stdin

                cmp             rax, 1
                jne             .error          ; check errors

                xor             rax, rax
                mov             al, [rsp]
                inc             rsp             ; move char to rax and pop int from stack

                pop             rdx
                pop             rsi
                pop             rdi
                pop             rcx
                ret                             ; restore registers and return
.error:
                mov             rax, -1
                inc             rsp             ; set -1 to rax if error ocured and pop char from stack

                pop             rdx
                pop             rsi
                pop             rdi
                pop             rcx
                ret                             ; restore registers and return

; reads long number from stdin
;   rdi -- address where to read
;   rcx -- size of long number in qwords
read_long:
                push            rcx
                push            rdi
                call            set_zero
.loop:
                call            read_char

                or              rax, rax
                js              exit

                cmp             rax, 0x09
                je              .eoln
                cmp             rax, 0x0a
                je              .eoln
                cmp             rax, 0x0b
                je              .eoln
                cmp             rax, 0x0c
                je              .eoln
                cmp             rax, 0x0d
                je              .eoln
                cmp             rax, 0x20
                je              .eoln

                cmp             rax, '0'
                jb              .invalid_char
                cmp             rax, '9'
                ja              .invalid_char

                sub             rax, '0'
                mov             rbx, 10
                call            mul_long_short
                call            add_long_short
                jmp             .loop
.invalid_char:
                mov             rsi, bad_msg
                mov             rdx, bad_msg_len
                call            write_string
                call            write_char
                mov             al, 0x0a
                call            write_char

                pop             rdi
                pop             rcx
                jmp             exit
.eoln:
                pop             rdi
                pop             rcx
                ret
; TODO: skip whitespaces

; adds 64-bit number to long number
;   rdi -- address of long number
;   rax -- 64-bit number
;   rcx -- size of long number in qwords
; result:
;   sum is written to rdi
add_long_short:
                push            rdi
                push            rax
                push            rcx
                push            rdx             ; save registers

                xor             rdx, rdx        ; clear rdx -- carry
.loop:
                add             [rdi], rax      ; add current number to long number
                adc             rdx, 0          ; calc carry
                mov             rax, rdx        ; move carry to rax
                xor             rdx, rdx        ; clear rdx to user it on next iteration
                lea             rdi, [rdi + 8]  ; go to next qword
                dec             rcx             ; decrement number size
                jnz             .loop

                pop             rdx
                pop             rcx
                pop             rax
                pop             rdi
                ret                             ; restore registers and return

; multiplies long number by a 64-bit number
;   rdi -- address of long number
;   rbx -- 64-bit number
;   rcx -- size of long number in qwords
; result:
;   product is written to rdi
mul_long_short:
                push            r8
                push            rax
                push            rdi
                push            rdx
                push            rcx             ; save registers

                xor             r8, r8          ; clear r8 -- carry
.loop:
                mov             rax, [rdi]      ; move current qword to rax
                mul             rbx             ; multiply rax by rbx, result -- rdx:rax
                add             rax, r8         ; add carry to rax
                adc             rdx, 0          ; add carry flag to rdx
                mov             [rdi], rax      ; move lower word rax to rdi
                mov             r8, rdx         ; move carry to r8
                lea             rdi, [rdi + 8]  ; go to next qword
                dec             rcx             ; decrement number size
                jnz             .loop

                pop             rcx
                pop             rdx
                pop             rdi
                pop             rax
                pop             r8
                ret                             ; restore registers and return

; divides long number by a 64-bit number
;   rdi -- address of long number
;   rbx -- 64-bit number
;   rcx -- size of long number in qwords
; result:
;   quotent is written to rdi
;   remainder is written in rdx
div_long_short:
                push            rdi
                push            rax
                push            rcx             ; save registers

                lea             rdi, [rdi + 8 * rcx - 8]
                xor             rdx, rdx        ; go to the last qword in number and clear carry
.loop:
                mov             rax, [rdi]      ; move current qword to rax
                div             rbx             ; divide rdx:rax by rbx
                mov             [rdi], rax      ; move divident to rdi
                lea             rdi, [rdi - 8]  ; go to prev qword
                dec             rcx             ; decrement number size
                jnz             .loop

                pop             rcx
                pop             rax
                pop             rdi
                ret                             ; restore registers and return

; sets long number to zero
;   rdi -- address of a long number
;   rcx -- size of a long number in qwords
set_zero:
                push            rdi
                push            rcx
                push            rax             ; save registers

                xor             rax, rax
                rep stosq                       ; fill long number with rax == 0

                pop             rax
                pop             rcx
                pop             rdi
                ret                             ; restore registers and return

; checks if long number is zero
;   rdi -- address of a long number
;   rcx -- sizeof a long number
; result:
;   ZF == 1 if number is zero
is_zero:
                push            rdi
                push            rcx
                push            rax             ; save registers

                xor             rax, rax
                rep scasq                       ; scan long number with rax == 0

                pop             rax
                pop             rcx
                pop             rdi
                ret                             ; restore registers and return

; adds two long numbers
;   rdi -- address of first number
;   rsi -- address of second number
;   rcx -- size of numbers in qwords
; result:
;   sum is written to rdi
add_long_long:
                push            rdi
                push            rsi
                push            rcx
                push            rax             ; save registers

                clc
.loop:
                mov             rax, [rsi]
                lea             rsi, [rsi + 8]
                adc             [rdi], rax
                lea             rdi, [rdi + 8]
                dec             rcx
                jnz             .loop           ; sum loop

                pop             rax
                pop             rcx
                pop             rsi
                pop             rdi
                ret                             ; restore registers and return

; subtracts two long numbers
;   rdi -- address of first number
;   rsi -- address of second number
;   rcx -- size of numbers in qwords
; result:
;   result is written to rdi
sub_long_long:
                push            rdi
                push            rsi
                push            rcx
                push            rax             ; save registers

                clc
.loop:
                mov             rax, [rsi]
                lea             rsi, [rsi + 8]
                sbb             [rdi], rax
                lea             rdi, [rdi + 8]
                dec             rcx
                jnz             .loop           ; sub loop

                pop             rax
                pop             rcx
                pop             rsi
                pop             rdi
                ret                             ; restore registers and return

; multiplies two long numbers
;   rdi -- address of first number
;   rsi -- address of second number
;   rcx -- size of numbers in qwords
;   r8  -- result address
; result:
;   result is written to rdi
mul_long_long:
                push            rsi
                push            rcx
                push            r8
                push            r9              ; save registers

                mov             r9, r8          ; begin address

                push            rdi
                push            rcx
                mov             rdi, r9
                shl             rcx, 1
                call            set_zero
                pop             rcx
                pop             rdi             ; clear memory
.loop:
                mov             rbx, [rsi]      ; current digit
                cmp             rbx, 0
                je              .end_loop       ; if current digit is zero, there's no need to do multiplication

                call            mul_long_short  ; multiply number by a current digit

                push            rdi
                push            rsi
                mov             rsi, rdi
                mov             rdi, r8
                call            add_long_long   ; add current result to answer
                pop             rsi
                pop             rdi

                call            div_long_short  ; divide number by a current digit

.end_loop:
                lea             r8, [r8 + 8]
                lea             rsi, [rsi + 8]
                dec             rcx
                jnz             .loop           ; move pointers and make loop

                mov             rdi, r9

                pop             r9
                pop             r8
                pop             rcx
                pop             rsi
                ret                             ; restore registers and return


                section         .rodata
bad_msg:        db              "Invalid character: "
bad_msg_len     equ             $ - bad_msg
