ifdef GCC_PATH
CC := '$(GCC_PATH)/arm-none-eabi-gcc'
CPP:= '$(GCC_PATH)/arm-none-eabi-g++'
AS := '$(GCC_PATH)/arm-none-eabi-as'
CP := '$(GCC_PATH)/arm-none-eabi-objcopy'
SZ := '$(GCC_PATH)/arm-none-eabi-size'
else
CC := arm-none-eabi-gcc
CPP:= arm-none-eabi-g++
AS := arm-none-eabi-as
CP := arm-none-eabi-objcopy
SZ := arm-none-eabi-size
endif

FILE_BUILDINFO := $(DIR_OBJ)/buildinfo.h

DEFINES_C_COMMON :=
# ifeq ($(DEBUG),1) # ALWAYS DEBUG
DEFINES_C_COMMON += DEBUG
# else
# DEFINES_C_COMMON += NDEBUG
# endif

FLAGS_ASM_COMMON := $(FLAGS_MCU) -gdwarf-2 --fatal-warnings 

FLAGS_C_COMMON := $(FLAGS_MCU)
#-marm
FLAGS_C_COMMON += $(addprefix -D,$(DEFINES_C_COMMON))
FLAGS_C_COMMON += -gdwarf-3 -fdata-sections -ffunction-sections
FLAGS_C_COMMON += -MMD -MP
FLAGS_C_COMMON += -ffast-math -funsafe-math-optimizations
# FLAGS_C_COMMON += -flto

# ifeq ($(DEBUG),1)
FLAGS_C_COMMON += -g3
# else
# FLAGS_C_COMMON += -g0
# endif

FLAGS_CPP_COMMON := $(FLAGS_C_COMMON)
FLAGS_CPP_COMMON += -std=c++14
FLAGS_C_COMMON += -std=c11

#######################################
# link by g++, arg pass thru -Xlinker
FLAGS_LD_COMMON := $(FLAGS_MCU)
# FLAGS_LD_COMMON += -lm -lc -lgcc
# FLAGS_LD_COMMON += -flto
FLAGS_LD_COMMON += -specs=nano.specs -specs=nosys.specs
# ifeq ($(CLI_MODE_FLOAT_VARIABLES),1)
# FLAGS_C_COMMON += -DCLI_MODE_FLOAT_VARIABLES
# FLAGS_CPP_COMMON += -DCLI_MODE_FLOAT_VARIABLES
# FLAGS_LD_COMMON += -u _printf_float -u _scanf_float
# endif
#FLAGS_LD_COMMON += --cref # add cross reference to map file
FLAGS_LD_COMMON += -Xlinker --gc-sections
FLAGS_LD_COMMON += -Xlinker --print-memory-usage

#######################################

ifndef BUILD_TAG
BUILD_TAG := undefined
endif

.BUILDINFO:
	mkdir -p $(DIR_OBJ)
	@echo "generating $(FILE_BUILDINFO)..."
	@./prebuild.sh $(FILE_BUILDINFO) "$(BUILD_TAG)"


