#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "split_util.h"
#include "matrix.h"
#include "keyboard.h"
#include "config.h"
#include "timer.h"
#include "protocol/ps2_mouse.h"

#ifdef USE_I2C
#  include "i2c.h"
#else
#  include "serial.h"
#endif

volatile bool isLeftHand = true;

static void setup_handedness(void) {
  #ifdef EE_HANDS
    isLeftHand = eeprom_read_byte(EECONFIG_HANDEDNESS);
  #else
    // I2C_MASTER_RIGHT is deprecated, use MASTER_RIGHT instead, since this works for both serial and i2c
    #if defined(I2C_MASTER_RIGHT) || defined(MASTER_RIGHT)
      isLeftHand = !has_usb();
    #else
      isLeftHand = has_usb();
    #endif
  #endif
}

static void keyboard_master_setup(void) {
#ifdef USE_I2C
    i2c_master_init();
#ifdef SSD1306OLED
    matrix_master_OLED_init ();
#endif
#else
    serial_master_init();
#endif
}

static void keyboard_slave_setup(void) {
  timer_init();
#ifdef USE_I2C
    i2c_slave_init(SLAVE_I2C_ADDRESS);
#else
    serial_slave_init();
#endif
}

static void ps2_mouse_slave_put_data(void);

bool has_usb(void) {
   USBCON |= (1 << OTGPADE); //enables VBUS pad
   _delay_us(5);
   return (USBSTA & (1<<VBUS));  //checks state of VBUS
}

void split_keyboard_setup(void) {
   setup_handedness();

   if (has_usb()) {
      keyboard_master_setup();
   } else {
      keyboard_slave_setup();
   }
   sei();
}

void keyboard_slave_loop(void) {
   matrix_init();
   ps2_mouse_init();

   while (1) {
      matrix_slave_scan();
      ps2_mouse_task(true);
      ps2_mouse_slave_put_data();
   }
}

// this code runs before the usb and keyboard is initialized
void matrix_setup(void) {
    split_keyboard_setup();

    if (!has_usb()) {
        keyboard_slave_loop();
    }
}

static uint8_t mouse_buffer[4];

void ps2_mouse_slave_send(report_mouse_t *mouse_report) {
    mouse_buffer[0] = mouse_report->x;
    mouse_buffer[1] = mouse_report->y;
    mouse_buffer[2] = mouse_report->v;
    mouse_buffer[3] = mouse_report->h;
}

static void ps2_mouse_slave_put_data(void) {
	// now disabled
	/*
    int offset = 5; // (MATRIX_ROWS/2); // see matrix.c matrix_slave_scan()
    for (int i = 0; i < 4; ++i) {
        i2c_slave_buffer[offset + i] = mouse_buffer[i];
        mouse_buffer[i] = 0;
    }
    */
}
