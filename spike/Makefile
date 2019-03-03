MCU = atmega32u4
CFLAGS += -mmcu=$(MCU) -Os -I../quantum -I../quantum/audio -DUSE_I2C
CC = avr-gcc
GREP=grep

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

%.s : %.c
	$(CC) -S $(CFLAGS) $< -o $@

%.o : %.S
	$(CC) -c $(CFLAGS) $< -o $@

main.elf: i2c.o main.o
	$(CC) $(CFLAGS) -o $@ $^

main.hex: main.elf
	avr-objcopy -j .text -j .data -O ihex main.elf $@


define EXEC_AVRDUDE
	USB= ;\
		if $(GREP) -q -s Microsoft /proc/version; then \
		echo 'ERROR: AVR flashing cannot be automated within the Windows Subsystem for Linux (WSL) currently. Instead, take the .hex file generated and flash it using AVRDUDE, AVRDUDESS, or XLoader.'; \
		else \
		printf "Detecting USB port, reset your controller now."; \
		ls /dev/tty* > /tmp/1; \
		while [ -z $$USB ]; do \
		sleep 0.5; \
		printf "."; \
		ls /dev/tty* > /tmp/2; \
		USB=`comm -13 /tmp/1 /tmp/2 | $(GREP) -o '/dev/tty.*'`; \
		mv /tmp/2 /tmp/1; \
		done; \
		echo ""; \
		echo "Detected controller on USB port at $$USB"; \
		if $(GREP) -q -s 'MINGW\|MSYS' /proc/version; then \
		USB=`echo "$$USB" | perl -pne 's/\/dev\/ttyS(\d+)/COM.($$1+1)/e'`; \
		echo "Remapped MSYS2 USB port to $$USB"; \
		fi; \
		sleep 1; \
		avrdude -p $(MCU) -c avr109 -P $$USB -U flash:w:main.hex; \
		fi
endef

avrdude: main.hex
	$(call EXEC_AVRDUDE)
