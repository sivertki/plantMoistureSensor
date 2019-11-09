# Source files to be compiled
SOURCE_FILES := main.c IO_driver.c Timer.c #UART_driver.c

PROGRAMMER := atmelice_isp

AVRDUDE_VERSION := avrdude #/home/sivert/Software/arduino-1.8.9/hardware/tools/avr/bin/avrdude

BUILD_DIR := build
TARGET_CPU := atmega168
TARGET_DEVICE := m168

CC := avr-gcc
CFLAGS := -O -std=c11 -mmcu=$(TARGET_CPU)

OBJECT_FILES := $(SOURCE_FILES:%.c=$(BUILD_DIR)/%.o)

.DEFAULT_GOAL := $(BUILD_DIR)/main.hex

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/main.hex: $(OBJECT_FILES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(OBJECT_FILES) -o $(BUILD_DIR)/a.out
	avr-objcopy -j .text -j .data -O ihex $(BUILD_DIR)/a.out $(BUILD_DIR)/main.hex

.PHONY: flash
flash: $(BUILD_DIR)/main.hex
	#$(AVRDUDE_VERSION) -v -p $(TARGET_DEVICE) -c $(PROGRAMMER) -C /home/sivert/Software/arduino-1.8.9/hardware/tools/avr/etc/avrdude.conf -P /dev/ttyUSB0 -b 115200 -U flash:w:$(BUILD_DIR)/main.hex:i
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U flash:w:$(BUILD_DIR)/main.hex:i
.PHONY: fuse
fuse:
	$(AVRDUDE_VERSION) -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U efuse:w:0xff:m
	$(AVRDUDE_VERSION) -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U hfuse:w:0xdf:m
	$(AVRDUDE_VERSION) -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U lfuse:w:0x4c:m

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: erase
erase:
	$(AVRDUDE_VERSION) -p $(TARGET_DEVICE) -c $(PROGRAMMER) -e