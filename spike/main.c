#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>
#include <util/delay.h>
#include <util/twi.h>
#include "config_common.h"
#include "i2c.h"

#define SWITCH_PIN B6
#define LED_PIN B2
#define I2C_ADDRESS 10
#define CODE 0x18

#ifndef TIMER_PRESCALER
#   if F_CPU > 16000000
#       define TIMER_PRESCALER      256
#   elif F_CPU > 2000000
#       define TIMER_PRESCALER      64
#   elif F_CPU > 250000
#       define TIMER_PRESCALER      8
#   else
#       define TIMER_PRESCALER      1
#   endif
#endif
#define TIMER_RAW_FREQ      (F_CPU/TIMER_PRESCALER)
#define TIMER_RAW           TCNT0
#define TIMER_RAW_TOP       (TIMER_RAW_FREQ/1000)

#if (TIMER_RAW_TOP > 255)
#   error "Timer0 can't count 1ms at this clock freq. Use larger prescaler."
#endif

/**
 * Program
 * - when switch is pushed, send a signal once to the other MCU
 * - when the signal comes, turn LED on for several cycles
 * - use I2C for signaling
 */

// _SFR_IO8((pin >> 4) + 1) &= ~_BV(pin & 0xF); // IN
// _SFR_IO8((pin >> 4) + 1) |=  _BV(pin & 0xF); // OUT
// _SFR_IO8((pin >> 4) + 2) |=  _BV(pin & 0xF); // HI
// _SFR_IO8((pin >> 4) + 2) &= ~_BV(pin & 0xF); // LOW

void timer_init(void)
{
#if TIMER_PRESCALER == 1
	uint8_t prescaler = 0x01;
#elif TIMER_PRESCALER == 8
	uint8_t prescaler = 0x02;
#elif TIMER_PRESCALER == 64
	uint8_t prescaler = 0x03;
#elif TIMER_PRESCALER == 256
	uint8_t prescaler = 0x04;
#elif TIMER_PRESCALER == 1024
	uint8_t prescaler = 0x05;
#else
#   error "Timer prescaler value is NOT vaild."
#endif

#ifndef __AVR_ATmega32A__
	// Timer0 CTC mode
	TCCR0A = 0x02;

	TCCR0B = prescaler;

	OCR0A = TIMER_RAW_TOP;
	TIMSK0 = (1<<OCIE0A);
#else
	// Timer0 CTC mode
	TCCR0 = (1 << WGM01) | prescaler;

	OCR0 = TIMER_RAW_TOP;
	TIMSK = (1 << OCIE0);
#endif
}



int main(void) {
	_SFR_IO8((SWITCH_PIN >> 4) + 1) &= ~_BV(SWITCH_PIN & 0xF); // in
	_SFR_IO8((SWITCH_PIN >> 4) + 2) |=  _BV(SWITCH_PIN & 0xF); // pull hi
	_SFR_IO8((LED_PIN    >> 4) + 1) |=  _BV(LED_PIN    & 0xF); // out

#ifdef MASTER
	i2c_master_init();
#else
	sei();
#endif
	timer_init();
	i2c_slave_init(I2C_ADDRESS);

	_SFR_IO8((LED_PIN >> 4) + 2) &= ~_BV(LED_PIN & 0xF); // LO
	while(1) {
		if ((_SFR_IO8(SWITCH_PIN >> 4) & _BV(SWITCH_PIN & 0xF)) == 0) { // lo
			uint8_t err = i2c_master_start((I2C_ADDRESS << 1) | I2C_WRITE); // i2c_start_write(I2C_ADDRESS);
			if (err) {
				i2c_reset_state();
				continue;
			}
			if (i2c_master_write(0x0)) {
				i2c_reset_state();
				continue;
			}
			if (i2c_master_write(CODE)) {
				i2c_reset_state();
				continue;
			}
			i2c_master_stop();
			_SFR_IO8((LED_PIN >> 4) + 2) |= _BV(LED_PIN & 0xF); // hi
			_delay_ms(1);
		}
		if (i2c_slave_buffer[0] == CODE) {
			_SFR_IO8((LED_PIN >> 4) + 2) |= _BV(LED_PIN & 0xF); // hi
			i2c_slave_buffer[0] == 0;
			_delay_ms(1000);
		}
		_SFR_IO8((LED_PIN >> 4) + 2) &= ~_BV(LED_PIN & 0xF); // LO
	}
}

#define BUFFER_POS_INC() (slave_buffer_pos = (slave_buffer_pos+1)%SLAVE_BUFFER_SIZE)

volatile uint8_t i2c_slave_buffer[SLAVE_BUFFER_SIZE];

static volatile uint8_t slave_buffer_pos;
static volatile bool slave_has_register_set = false;

ISR(TWI_vect) {
	uint8_t ack = 1;
	switch(TW_STATUS) {
		case TW_SR_SLA_ACK:
			// this device has been addressed as a slave receiver
			slave_has_register_set = false;
			TWCR |= (1<<TWIE) | (1<<TWINT) | (ack<<TWEA) | (1<<TWEN);
			return;

		case TW_SR_DATA_ACK:
			// this device has received data as a slave receiver
			// The first byte that we receive in this transaction sets the location
			// of the read/write location of the slaves memory that it exposes over
			// i2c.  After that, bytes will be written at slave_buffer_pos, incrementing
			// slave_buffer_pos after each write.
			if(!slave_has_register_set) {
				slave_buffer_pos = TWDR;
				// don't acknowledge the master if this memory loctaion is out of bounds
				if ( slave_buffer_pos >= SLAVE_BUFFER_SIZE ) {
					TWCR &= ~(1<<TWEA);
					slave_buffer_pos = 0;
					TWCR |= (1<<TWIE) | (1<<TWINT) | (1<<TWEN);
					return;
				}
				slave_has_register_set = true;
			} else {
				i2c_slave_buffer[slave_buffer_pos] = TWDR;
				BUFFER_POS_INC();
			}
			TWCR |= (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
			return;

		case TW_ST_DATA_ACK:
			// master has addressed this device as a slave transmitter and is
			// requesting data.
			TWDR = i2c_slave_buffer[slave_buffer_pos];
			BUFFER_POS_INC();
			TWCR |= (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
			return;

		case TW_BUS_ERROR: // something went wrong, reset twi state
			TWCR = 0;
			TWCR |= (1<<TWIE) | (1<<TWEA) | (1<<TWEN);
			return;
		default:
			TWCR |= (1<<TWIE) | (1<<TWEA) | (1<<TWEN);
			return;
	}
	// Reset everything, so we are ready for the next TWI interrupt
}
