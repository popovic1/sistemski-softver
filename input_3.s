.section my_code_handler
ld $0x2323, %r2
ld $0x4455, %r3
ld $3, %r2
handler:
.skip 4
# obrada prekida od tajmera
my_isr_timer:
ld [%r2 + handler], %r1
.skip 4
.end