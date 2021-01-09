/*
 * WARNING: be careful changing this code, it is very timing dependent
 */

#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include "serial.h"

#ifndef USE_I2C

// Serial pulse period in microseconds. Its probably a bad idea to lower this
// value.
#define SERIAL_DELAY 24

uint8_t volatile serial_slave_buffer[SERIAL_SLAVE_BUFFER_LENGTH] = {0};
uint8_t volatile serial_master_buffer[SERIAL_MASTER_BUFFER_LENGTH] = {0};

#define SLAVE_DATA_CORRUPT (1<<0)
volatile uint8_t status = 0;

inline static
void serial_delay(void) {
  _delay_us(SERIAL_DELAY);
}

inline static
void serial_output(void) {
  SERIAL_PIN_DDR |= SERIAL_PIN_MASK;
}

// make the serial pin an input with pull-up resistor
inline static
void serial_input(void) {
  SERIAL_PIN_DDR  &= ~SERIAL_PIN_MASK;
  SERIAL_PIN_PORT |= SERIAL_PIN_MASK;
}

inline static
uint8_t serial_read_pin(void) {
  return !!(SERIAL_PIN_INPUT & SERIAL_PIN_MASK);
}

inline static
void serial_low(void) {
  SERIAL_PIN_PORT &= ~SERIAL_PIN_MASK;
}

inline static
void serial_high(void) {
  SERIAL_PIN_PORT |= SERIAL_PIN_MASK;
}

void serial_master_init(void) {
  serial_output();
  serial_high();
}

void serial_slave_init(void) {
  serial_input();

  // Enable INT0
  EIMSK |= _BV(INT0);
  // Trigger on falling edge of INT0
  EICRA &= ~(_BV(ISC00) | _BV(ISC01));
}

// Used by the master to synchronize timing with the slave.
static
void sync_recv(void) {
  serial_input();
  // This shouldn't hang if the slave disconnects because the
  // serial line will float to high if the slave does disconnect.
  while (!serial_read_pin());
  serial_delay();
}

// Reads a byte from the serial line
static
uint8_t serial_read_byte(void) {
  uint8_t byte = 0;
  serial_input();
  for ( uint8_t i = 0; i < 8; ++i) {
    byte = (byte << 1) | serial_read_pin();
    serial_delay();
    _delay_us(1);
  }

  return byte;
}

// Sends a byte with MSB ordering
static
void serial_write_byte(uint8_t data) {
  uint8_t b = 8;
  serial_output();
  while( b-- ) {
    if(data & (1 << b)) {
      serial_high();
    } else {
      serial_low();
    }
    serial_delay();
  }
}

static
void send_uart(uint8_t data) {
  // start bit
  serial_low();
  serial_delay();

  uint8_t b = 8;
  uint8_t p = 1; // odd parity
  while( b-- ) {
    if(data & (1 << b)) {
      serial_high();
      p = !p;
    } else {
      serial_low();
    }
    serial_delay();
  }
  // parity
  if(p) {
    serial_high();
  } else {
    serial_low();
  }
  serial_delay();

  // stop (with minimum delay)
  serial_high();
  serial_delay();
}

// interrupt handle to be used by the slave device
ISR(SERIAL_PIN_INTERRUPT) {
  // Set high before start bit
  serial_output();
  serial_high();
  // wait 2 serial delay
  serial_delay();
  serial_delay();

  // start uart
  for (int i = 0; i < SERIAL_SLAVE_BUFFER_LENGTH; ++i) {
    send_uart(serial_slave_buffer[i]);
  }

  serial_input(); // end transaction
}

inline
bool serial_slave_DATA_CORRUPT(void) {
  return status & SLAVE_DATA_CORRUPT;
}

// Copies the serial_slave_buffer to the master and sends the
// serial_master_buffer to the slave.
//
// Returns:
// 0 => no error
// 1 => slave did not respond
int serial_update_buffers(void) {
  // this code is very time dependent, so we need to disable interrupts
  cli();

  // signal to the slave that we want to start a transaction
  serial_output();
  serial_low();
  _delay_us(1);

  // wait for the slaves response
  serial_input();
  serial_high();
  _delay_us(SERIAL_DELAY);

  // check if the slave is present
  if (serial_read_pin()) {
    // slave failed to pull the line low, assume not present
    sei();
    return 1;
  }

  // if the slave is present syncronize with it
  sync_recv();

  uint8_t checksum_computed = 0;
  // receive data from the slave
  for (int i = 0; i < SERIAL_SLAVE_BUFFER_LENGTH; ++i) {
    serial_slave_buffer[i] = serial_read_byte();
    sync_recv();
    checksum_computed += serial_slave_buffer[i];
  }
  uint8_t checksum_received = serial_read_byte();
  sync_recv();

  if (checksum_computed != checksum_received) {
    sei();
    return 1;
  }

  uint8_t checksum = 0;
  // send data to the slave
  for (int i = 0; i < SERIAL_MASTER_BUFFER_LENGTH; ++i) {
    serial_write_byte(serial_master_buffer[i]);
    sync_recv();
    checksum += serial_master_buffer[i];
  }
  serial_write_byte(checksum);
  sync_recv();

  // always, release the line when not in use
  serial_output();
  serial_high();

  sei();
  return 0;
}

#endif
