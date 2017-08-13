TARGET = picoc.elf
CC = arm-none-eabi-gcc
STDLIB = $(EMBEDDED)

OPENCM_DIR = opencm3/libopencm3
OPENCM_LIB = opencm3_stm32f1

LIBS =   -L$(OPENCM_DIR)/lib -l$(OPENCM_LIB) -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group
CFLAGS += -DSTM32F1 -mthumb -mcpu=cortex-m3 -g
CFLAGS += -fno-common -ffunction-sections -fdata-sections  -I$(OPENCM_DIR)/include
LDFLAGS += -nostartfiles -static -T$(HOST_DIR)/stm32f103.ld -Wl,-Map=$(*).map -Wl,--gc-sections -Wl,--print-gc-sections

HOSTLIB = $(OPENCM_DIR)/lib/lib$(OPENCM_LIB).a
$(TARGET): $(HOSTLIB)
$(HOSTLIB):
	$(MAKE) -C $(OPENCM_DIR) CFLAGS=-std=c99

reallyclean: clean
	$(MAKE) -C opencm3/libopencm3 clean
