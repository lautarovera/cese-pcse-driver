#############################################################################
#                     GNU ARM Embedded Toolchain Makefile                   #
#############################################################################

# Project name
PROJ_NAME = lis2dw12_app

#############################################################################
#                                SETUP TOOLS                                #
#############################################################################

# Toolchain path
TOOLS_DIR = /opt/gcc-arm-embedded/gcc-arm-none-eabi-10.3-2021.10/bin

CC      = $(TOOLS_DIR)/arm-none-eabi-gcc
OBJCOPY = $(TOOLS_DIR)/arm-none-eabi-objcopy
GDB     = $(TOOLS_DIR)/arm-none-eabi-gdb
AS      = $(TOOLS_DIR)/arm-none-eabi-as

MCU     = STM32F429xx

# Port for st-util
STUTIL_PORT = 4242

# Directories to be searched for header files
INCLUDE = $(addprefix -I,$(INC_DIR))

DEFS    = -D$(MCU) -DUSE_HAL_DRIVER

# Assembly flags
AFLAGS  = -mcpu=cortex-m4
AFLAGS += -mthumb
AFLAGS += -mthumb-interwork
AFLAGS += -mlittle-endian
AFLAGS += -mfloat-abi=hard
AFLAGS += -mfpu=fpv4-sp-d16

# Compiler flags
CFLAGS  = -ggdb
CFLAGS += -O0
CFLAGS += -Wall -Wextra -Warray-bounds -Wno-unused-parameter
CFLAGS += $(AFLAGS)

# Linker flags
LFLAGS  = -T"./lnk/stm32f429zitx.ld"

###############################################################################
#                              SETUP SOURCES                                  #
###############################################################################

# This is the directory containing the firmware package
STM_ROOT     = $$HOME/STM32Cube/Repository/STM32Cube_FW_F4_V1.27.0

# This is where the source files are located,
# which are not in the current directory
# (the sources of the standard peripheral library, which we use)
# see also "info:/make/Selective Search" in Konqueror
DRV_SRC_DIR  = ./driver_LIS2DW12/src
STM_SRC_DIR  = $(STM_ROOT)/Drivers/STM32F4xx_HAL_Driver/Src
STM_SRC_DIR += $(STM_ROOT)/Drivers/BSP/STM32F4xx_Nucleo_144
STM_SRC_DIR += $(STM_ROOT)/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates

# Tell make to look in that folder if it cannot find a source
# in the current directory
vpath %.c $(DRV_SRC_DIR)
vpath %.c $(STM_SRC_DIR)

################################################################################
#                         SETUP HEADER FILES                                   #
################################################################################

# The header files we use are located here
INC_DIR  = ./driver_LIS2DW12/inc
INC_DIR += $(STM_ROOT)/Drivers/STM32F4xx_HAL_Driver/Inc
INC_DIR += $(STM_ROOT)/Drivers/BSP/STM32F4xx_Nucleo_144
INC_DIR += $(STM_ROOT)/Drivers/CMSIS/Include
INC_DIR += $(STM_ROOT)/Drivers/CMSIS/Device/ST/STM32F4xx/Include
INC_DIR += ./inc

################################################################################
#                   SOURCE FILES TO COMPILE                                    #
################################################################################

SRCS_DIR = ./src
ASRCS_DIR = ./startup

# My source files
SRCS  = main.c

# Contains initialisation code and must be compiled into
# our project. This file is in the current directory and
# was writen by ST.
SRCS += system_stm32f4xx.c
#SRCS += stm32f4xx_hal_msp.c
#SRCS += stm32f4xx_it.c
SRCS += syscalls.c
SRCS += sysmem.c

# Startup file written by ST
# The assembly code in this file is the first one to be
# executed. Normally you do not change this file.
ASRCS = startup_stm32f429zitx.s

SOURCE = $(addprefix $(SRCS_DIR)/,$(SRCS))
ASOURCE = $(addprefix $(ASRCS_DIR)/,$(ASRCS))

OUT_DIR = ./build
# in case we have to many sources and don't want
# to compile all sources every time
COBJS = $(SRCS:.c=.o)
AOBJS = $(ASRCS:.s=.o)
COBJS := $(addprefix $(OUT_DIR)/,$(COBJS))
AOBJS := $(addprefix $(OUT_DIR)/,$(AOBJS))

######################################################################
#                         SETUP TARGETS                              #
######################################################################

BIN_DIR = ./bin

.PHONY: all

all: $(BIN_DIR)/$(PROJ_NAME).elf

$(OUT_DIR)/%.o: $(SRCS_DIR)/%.c
	@echo "[Compiling...  ]  $^"
	@mkdir -p $(dir $@)
	@$(CC) -c $(CFLAGS) $(INCLUDE) $(DEFS) -o $@ $^

$(OUT_DIR)/%.o: $(ASRCS_DIR)/%.s
	@echo "[Assembling... ]  $^"
	@$(AS) $(AFLAGS) $< -o $@

$(BIN_DIR)/$(PROJ_NAME).elf: $(COBJS) $(AOBJS)
	@echo "[Linking...    ]  $@"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(LFLAGS) $(foreach file, $^, $(file)) -o $@
	@$(OBJCOPY) -O ihex $@ $(dir $@)/$(PROJ_NAME).hex
	@$(OBJCOPY) -O binary $@ $(dir $@)/$(PROJ_NAME).bin
	@echo "Done!"

clean:
	@echo "[Cleaning...   ]  $^"
	@rm -f $(BIN_DIR)/$(PROJ_NAME).elf
	@rm -f $(BIN_DIR)/$(PROJ_NAME).hex
	@rm -f $(BIN_DIR)/$(PROJ_NAME).bin
	@rm -f $(BIN_DIR)/$(PROJ_NAME).map
	@find $(OUT_DIR) -type f -name '*.o' -print0 | xargs -0 -r rm
	@echo "Done!"

flash: all
	@st-flash write $(BIN_DIR)/$(PROJ_NAME).bin 0x8000000

server:
	@echo "On another terminal run \"make debug\""
	@st-util -p $(STUTIL_PORT)
	
# Before you start GDB, you must start st-util with "make server"
debug:
	$(GDB) -tui $(BIN_DIR)/$(PROJ_NAME).elf

driver_doc:
	@echo "[Generating LIS2DW12 driver documentation ]"
	@doxygen
	@echo "Done!"
