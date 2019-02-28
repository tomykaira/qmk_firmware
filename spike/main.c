#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "config_common.h"
#include "i2c.h"

#define SWITCH_PIN B6
#define LED_PIN B2
#define I2C_ADDRESS 10
#define CODE 0x18

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

int main(void) {
	_SFR_IO8((SWITCH_PIN >> 4) + 1) &= ~_BV(SWITCH_PIN & 0xF); // in
	_SFR_IO8((SWITCH_PIN >> 4) + 2) |=  _BV(SWITCH_PIN & 0xF); // pull hi
	_SFR_IO8((LED_PIN    >> 4) + 1) |=  _BV(LED_PIN    & 0xF); // out

	i2c_master_init();
	i2c_slave_init(I2C_ADDRESS);

	_SFR_IO8((LED_PIN >> 4) + 2) &= ~_BV(LED_PIN & 0xF); // LO
	while(1) {
		if ((_SFR_IO8(SWITCH_PIN >> 4) & _BV(SWITCH_PIN & 0xF)) == 0) { // lo
			_SFR_IO8((LED_PIN >> 4) + 2) |= _BV(LED_PIN & 0xF); // hi
			if (i2c_start_write(I2C_ADDRESS)) {
				i2c_reset_state();
				continue;
			}
			if (i2c_master_write(CODE)) {
				i2c_reset_state();
				continue;
			}
			i2c_master_stop();
		}
		if (i2c_slave_buffer[0] != 0) {
			_SFR_IO8((LED_PIN >> 4) + 2) |= _BV(LED_PIN & 0xF); // hi
			i2c_slave_buffer[0] == 0;
		}
		_SFR_IO8((LED_PIN >> 4) + 2) &= ~_BV(LED_PIN & 0xF); // LO
	}
}
