.section my_code_handler
handler:
push %r1
push %r2
csrrd %cause, %r1
ld $2, %r2
bne %r1, %r2, my_isr_timer
ld $3, %r2
.skip 2
# obrada prekida od tajmera
my_isr_timer:
.skip 3
.end