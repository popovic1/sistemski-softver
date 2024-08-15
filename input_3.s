.section my_code_handler
ld $2, %r2
ld $3, %r2
handler:
.skip 2
# obrada prekida od tajmera
my_isr_timer:
ld [%r2 + handler], %r1
.skip 3
.end