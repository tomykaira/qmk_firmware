	.file	"main.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.global	timer_init
	.type	timer_init, @function
timer_init:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r24,lo8(2)
	out 0x24,r24
	ldi r25,lo8(3)
	out 0x25,r25
	ldi r25,lo8(-6)
	out 0x27,r25
	sts 110,r24
/* epilogue start */
	ret
	.size	timer_init, .-timer_init
	.section	.text.startup,"ax",@progbits
.global	main
	.type	main, @function
main:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	cbi 0x4,6
	sbi 0x5,6
	sbi 0x4,2
	call timer_init
	ldi r24,lo8(10)
	call i2c_slave_init
/* #APP */
 ;  95 "main.c" 1
	sei
 ;  0 "" 2
/* #NOAPP */
.L9:
	cbi 0x5,2
.L3:
	sbic 0x3,6
	rjmp .L4
	ldi r24,lo8(20)
	call i2c_master_start
	tst r24
	breq .L5
	cpi r24,lo8(1)
	brne .L6
	sbi 0x5,2
.L6:
	call i2c_reset_state
	rjmp .L3
.L5:
	ldi r24,0
	call i2c_master_write
	cpse r24,__zero_reg__
	rjmp .L6
	ldi r24,lo8(24)
	call i2c_master_write
	cpse r24,__zero_reg__
	rjmp .L6
	call i2c_master_stop
	ldi r24,lo8(3999)
	ldi r25,hi8(3999)
1:	sbiw r24,1
	brne 1b
	rjmp .
	nop
.L4:
	lds r24,i2c_slave_buffer
	cpi r24,lo8(24)
	brne .L9
	sbi 0x5,2
	lds r24,i2c_slave_buffer
	ldi r25,lo8(3199999)
	ldi r18,hi8(3199999)
	ldi r24,hlo8(3199999)
1:	subi r25,1
	sbci r18,0
	sbci r24,0
	brne 1b
	rjmp .
	nop
	rjmp .L9
	.size	main, .-main
	.text
.global	__vector_36
	.type	__vector_36, @function
__vector_36:
	__gcc_isr 1
	push r25
	push r30
	push r31
/* prologue: Signal */
/* frame size = 0 */
/* stack size = 3...7 */
.L__stack_usage = 3 + __gcc_isr.n_pushed
	lds r24,185
	andi r24,lo8(-8)
	cpi r24,lo8(96)
	breq .L11
	brsh .L12
	tst r24
	brne .+2
	rjmp .L13
.L14:
	lds r24,188
	ori r24,lo8(69)
	rjmp .L21
.L12:
	cpi r24,lo8(-128)
	breq .L15
	cpi r24,lo8(-72)
	brne .L14
	lds r30,slave_buffer_pos
	ldi r31,0
	subi r30,lo8(-(i2c_slave_buffer))
	sbci r31,hi8(-(i2c_slave_buffer))
	ld r24,Z
	sts 187,r24
	rjmp .L22
.L11:
	sts slave_has_register_set,__zero_reg__
.L20:
	lds r24,188
	ori r24,lo8(-59)
.L21:
	sts 188,r24
/* epilogue start */
	pop r31
	pop r30
	pop r25
	__gcc_isr 2
	reti
.L15:
	lds r24,slave_has_register_set
	cpse r24,__zero_reg__
	rjmp .L18
	lds r24,187
	sts slave_buffer_pos,r24
	lds r24,slave_buffer_pos
	cpi r24,lo8(16)
	brlo .L19
	lds r24,188
	andi r24,lo8(-65)
	sts 188,r24
	sts slave_buffer_pos,__zero_reg__
.L19:
	ldi r24,lo8(1)
	sts slave_has_register_set,r24
	rjmp .L20
.L18:
	lds r30,slave_buffer_pos
	ldi r31,0
	lds r24,187
	subi r30,lo8(-(i2c_slave_buffer))
	sbci r31,hi8(-(i2c_slave_buffer))
	st Z,r24
.L22:
	lds r24,slave_buffer_pos
	ldi r25,0
	adiw r24,1
	andi r24,15
	clr r25
	sts slave_buffer_pos,r24
	rjmp .L20
.L13:
	sts 188,__zero_reg__
	rjmp .L14
	__gcc_isr 0,r24
	.size	__vector_36, .-__vector_36
	.local	slave_has_register_set
	.comm	slave_has_register_set,1,1
	.local	slave_buffer_pos
	.comm	slave_buffer_pos,1,1
	.comm	i2c_slave_buffer,16,1
	.ident	"GCC: (GNU) 8.2.0"
.global __do_clear_bss
