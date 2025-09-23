# List all source files to be compiled; separate with space
LIB_DIR := lib
BUILD_DIR := build
SRC_DIR := src
SOURCE_FILES := $(wildcard $(SRC_DIR)/*.c)
SOURCE_OBJECTS := $(patsubst $(SRC_DIR)/*.c, $(BUILD_DIR)%.o, $(SOURCE_FILES))
LIB_FILES := $(wildcard $(LIB_DIR)/*.c)
LIB_OBJECTS := $(patsubst $(LIB_DIR)/%.c, $(BUILD_DIR)/%.o, $(LIB_FILES))



# Set this flag to "yes" (no quotes) to use JTAG; otherwise ISP (SPI) is used
PROGRAM_WITH_JTAG := yes

# Feel free to ignore anything below this line
PROGRAMMER := atmelice_isp
ifeq ($(PROGRAM_WITH_JTAG), yes)
	PROGRAMMER := atmelice
endif

TARGET_CPU := atmega162
TARGET_DEVICE := m162

CC := avr-gcc
CFLAGS := -O -std=c11 -mmcu=$(TARGET_CPU) -ggdb

OBJECT_FILES = $(SOURCE_FILES:%.c=$(BUILD_DIR)/%.o)

.DEFAULT_GOAL := $(BUILD_DIR)/main.hex

details:
	@ echo "library files" $(LIB_FILES)
	@ echo "library objects" $(LIB_OBJECTS)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

#build dependent files in library folder
$(BUILD_DIR)/%.o: $(LIB_DIR)/%.c
	@ echo building $@...
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/main.hex: $(SOURCE_OBJECTS) | $(BUILD_DIR) $(LIB_OBJECTS)
	@ echo building $@...
	$(CC) $(CFLAGS) $(SOURCE_OBJECTS) $(LIB_OBJECTS) -o $(BUILD_DIR)/a.out
	avr-objcopy -j .text -j .data -O ihex $(BUILD_DIR)/a.out $(BUILD_DIR)/main.hex

.PHONY: flash
flash: $(BUILD_DIR)/main.hex
	sudo avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U flash:w:$(BUILD_DIR)/main.hex:i

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: erase
erase:
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -e
	
.PHONY: debug
debug:
	if pgrep avarice; then pkill avarice; fi
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U flash:w:$(BUILD_DIR)/main.hex:i
	x-terminal-emulator -e avarice --edbg --ignore-intr :4242
	sleep 2
	avr-gdb -tui -iex "target remote localhost:4242" $(BUILD_DIR)/a.out
	killall -s 9 avarice	
