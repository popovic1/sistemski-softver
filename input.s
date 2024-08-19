# file: handler.s
.extern my_counter
.global handler
.section my_code_handler
handler:
push %r1
push %r2
csrrd %cause, %r1
ld $2, %r2
beq %r1, %r2, my_isr_timer
ld $3, %r2
beq %r1, %r2, my_isr_terminal
.skip 4
# obrada prekida od tajmera
my_isr_timer:
ld $finish, %r1
st %r1, [%r2 + my_isr_timer]
.word my_isr_timer, 0xFFFF, 10
jmp finish
# obrada prekida od terminala
my_isr_terminal:
ld $my_counter, %r1
st %r1, my_isr_timer
ld my_counter, %r1
ld $1, %r2
add %r2, %r1
st %r1, my_counter
finish:
pop %r2
pop %r1
iret
.end