SRC_PATH=src
BUILD_PATH=build
TEST_PATH=tests

SRCS := $(wildcard $(SRC_PATH)/*.c)
HEADERS := $(wildcard $(SRC_PATH)/*.h)
OBJS := $(patsubst $(SRC_PATH)/%.c,$(BUILD_PATH)/%.o,$(SRCS))

TEST_SRCS := $(wildcard $(TEST_PATH)/*.test.c)
TEST_OBJS := $(patsubst $(TEST_PATH)/%.test.c,$(BUILD_PATH)/%.test.o,$(TEST_SRCS))

PROJECT=sunrise_alarm_avr

.PHONY: all burn clean readfuse

all: $(BUILD_PATH)/$(PROJECT).hex

$(BUILD_PATH)/$(PROJECT).hex: $(BUILD_PATH)/$(PROJECT).elf
	avr-objcopy -O ihex -j .text -j .data -R .eeprom $(BUILD_PATH)/$(PROJECT).elf $(BUILD_PATH)/$(PROJECT).hex

$(BUILD_PATH)/$(PROJECT).elf: $(OBJS)
	avr-gcc -mmcu=atmega328p -Os -o $(BUILD_PATH)/$(PROJECT).elf $(OBJS)

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.c $(HEADERS)
	@mkdir -p $(BUILD_PATH)
	avr-gcc -mmcu=atmega328p -D__AVR_ATmega328P__ -Wall -Os -c $< -o $@

burn: all
	avrdude -p m328p -v -c usbasp -P usb -B 32 -U flash:w:$(BUILD_PATH)/$(PROJECT).hex:i

clean:
	rm -f $(BUILD_PATH)/*

readfuse:
	avrdude -p m328p -c usbasp -U lfuse:r:-:h -U hfuse:r:-:h -U efuse:r:-:h


test: all ${TEST_OBJS}
	bash ./run_tests.sh

$(BUILD_PATH)/%.test.o: $(TEST_PATH)/%.test.c $(SRC_PATH)/%.h $(SRC_PATH)/%.c
	@mkdir -p $(BUILD_PATH)
	gcc -Wall -Os -o $@ $^
	chmod +x $@


