	.file	"main.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
	.section	.text.startup,"ax",@progbits
.global	main
	.type	main, @function
main:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	cbi 0x4,6
	cbi 0x5,6
	sbi 0xa,5
	sbi 0xb,5
.L2:
	sbi 0xb,5
	cbi 0xb,5
	rjmp .L2
	.size	main, .-main
	.ident	"GCC: (GNU) 8.2.0"
