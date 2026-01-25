# ===== CI Makefile for TM4C123 (Cortex-M4F) =====
# Builds with arm-none-eabi-gcc on Linux (Jenkins container)

TARGET := rtos_ci
BUILD  := build

CC  := arm-none-eabi-gcc
LD  := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
SIZE := arm-none-eabi-size

# Repo root is one level up from ci/
ROOT := ..

# MCU flags for TM4C123 (Cortex-M4F with FPU)
CPUFLAGS := -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard

# Compile flags (CI-friendly)
CFLAGS := $(CPUFLAGS) -O0 -g3 -ffunction-sections -fdata-sections \
          -Wall -Wextra \
          -Wno-error -Wno-unused-parameter -Wno-unused-variable -Wno-format \
          -Wno-sign-compare -Wno-builtin-declaration-mismatch -Wno-return-type

INCLUDES := -I$(ROOT)

# Include CI compatibility header automatically
CFLAGS += -include ci_compat.h

# Linker flags (we are already in ci/, so linker.ld is local)
LDFLAGS := $(CPUFLAGS) -Wl,--gc-sections -T linker.ld -Wl,-Map=$(BUILD)/$(TARGET).map

# Source files:
# - main sources are in repo root (..)
# - stubs/startup/syscalls are in this ci/ folder
C_SRCS := \
  $(ROOT)/rtos.c \
  $(ROOT)/uart0.c \
  $(ROOT)/wait.c \
  startup_ci.c \
  asm_stubs.c \
  syscalls.c

C_OBJS := \
  $(BUILD)/rtos.o \
  $(BUILD)/uart0.o \
  $(BUILD)/wait.o \
  $(BUILD)/startup_ci.o \
  $(BUILD)/asm_stubs.o \
  $(BUILD)/syscalls.o

all: $(BUILD)/$(TARGET).elf $(BUILD)/$(TARGET).bin

$(BUILD):
	mkdir -p $(BUILD)

# Compile root C files (../*.c)
$(BUILD)/%.o: $(ROOT)/%.c | $(BUILD)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compile ci-local C files (*.c)
$(BUILD)/%.o: %.c | $(BUILD)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Link
$(BUILD)/$(TARGET).elf: $(C_OBJS) linker.ld | $(BUILD)
	$(LD) $(C_OBJS) $(LDFLAGS) -o $@
	$(SIZE) $@

# Create binary
$(BUILD)/$(TARGET).bin: $(BUILD)/$(TARGET).elf | $(BUILD)
	$(OBJCOPY) -O binary $< $@

clean:
	rm -rf $(BUILD)

.PHONY: all clean
