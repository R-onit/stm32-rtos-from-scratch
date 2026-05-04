# ── Target ────────────────────────────────────────────────
TARGET := rtos

# ── Directories ───────────────────────────────────────────
BUILD_DIR   := build
OBJ_DIR     := $(BUILD_DIR)/obj

APP_DIR     := app
CORE_DIR    := core
DRIVERS_SRC := drivers/src
DRIVERS_INC := drivers/inc
STARTUP     := startup/startup_stm32f103rbtx.s
LINKER      := STM32F103RBTX_FLASH.ld

# ── Toolchain ─────────────────────────────────────────────
CC      := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
OBJDUMP := arm-none-eabi-objdump
SIZE    := arm-none-eabi-size

# ── Flags ─────────────────────────────────────────────────
CFLAGS := -mcpu=cortex-m3 -mthumb -g -O0 \
           -I$(APP_DIR)     \
           -I$(CORE_DIR)    \
           -I$(DRIVERS_INC) \
           -Icmsis/Include  \
           -Icmsis/Device

LDFLAGS := -T $(LINKER) -nostartfiles -nostdlib -lgcc

# ── OpenOCD ───────────────────────────────────────────────
OPENOCD_INTERFACE := interface/stlink.cfg
OPENOCD_TARGET    := target/stm32f1x.cfg
OPENOCD           := openocd -f $(OPENOCD_INTERFACE) -f $(OPENOCD_TARGET)

# ── Output ────────────────────────────────────────────────
ELF := $(BUILD_DIR)/$(TARGET).elf
BIN := $(BUILD_DIR)/$(TARGET).bin

# ── Sources ───────────────────────────────────────────────
SRC := $(wildcard $(APP_DIR)/*.c)      \
       $(wildcard $(CORE_DIR)/*.c)     \
       $(wildcard $(DRIVERS_SRC)/*.c)

ASM_SRC := $(wildcard core/*.s)
ASM_OBJ := $(ASM_SRC:%.s=$(OBJ_DIR)/%.o)

OBJ := $(SRC:%.c=$(OBJ_DIR)/%.o)
OBJ += $(OBJ_DIR)/startup.o
OBJ += $(ASM_OBJ)     


# ── Rules ─────────────────────────────────────────────────
all: $(BIN)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)/$(APP_DIR)
	@mkdir -p $(OBJ_DIR)/$(CORE_DIR)
	@mkdir -p $(OBJ_DIR)/$(DRIVERS_SRC)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@echo "  CC  $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/startup.o: $(STARTUP) | $(OBJ_DIR)
	@echo "  AS  $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(ELF): $(OBJ)
	@echo "  LD  $@"
	@$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@

$(BIN): $(ELF)
	@echo "  BIN $@"
	@$(OBJCOPY) -O binary $< $@
	@echo "  Done → $(BIN)"

$(OBJ_DIR)/%.o: %.s | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@echo "  AS  $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# ── Utilities ─────────────────────────────────────────────
size: $(ELF)
	@$(SIZE) $(ELF)

dump: $(ELF)
	@$(OBJDUMP) -d $(ELF) > build/dump.txt
	@echo "Disassembly → build/dump.txt"

clean:
	@rm -rf build
	@echo "Cleaned"

flash: $(ELF)
	@echo "Flashing..."
	@$(OPENOCD) -c "program $(ELF) verify reset exit"

openocd:
	@$(OPENOCD)

size_check:
	@arm-none-eabi-gcc $(CFLAGS) -x c - <<< \
	'#include "core/tcb.h"\nint main(){return sizeof(TCB_t);}'

gdb:
	@gdb-multiarch $(ELF) -ex "target remote localhost:3333"

.PHONY: all clean flash openocd gdb size dump