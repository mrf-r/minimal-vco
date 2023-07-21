DIR_TARGET := target_stm32f411/Core
DIR_EXT := target_stm32f411/Drivers
FLAGS_MCU := -mcpu=cortex-m4 -mfpu=vfpv4 -mfloat-abi=hard
# FLAGS_MCU := -mcpu=cortex-r4f -mfloat-abi=hard -mfpu=vfpv3-d16

include make_common_arm.mk

DIR_RTT := Segger
DIRS_INCLUDE_TARGET :=
DIRS_INCLUDE_TARGET += $(DIR_RTT)
DIRS_INCLUDE_TARGET += $(DIRS_INCLUDE_VCO)
DIRS_INCLUDE_TARGET += $(DIR_TARGET)
DIRS_INCLUDE_TARGET += $(DIR_EXT)/CMSIS/Include
DIRS_INCLUDE_TARGET += $(DIR_EXT)/CMSIS/Device/ST/STM32F4xx/Include
DIRS_INCLUDE_TARGET += $(DIR_EXT)/STM32F4xx_HAL_Driver/Inc
include make_vco.mk
INCLUDES_TARGET := $(addprefix -I,$(DIRS_INCLUDE_TARGET))

SOURCES_C_TARGET :=
SOURCES_C_TARGET += $(wildcard $(DIR_RTT)/*.c)
SOURCES_C_TARGET += $(wildcard $(DIR_TARGET)/*.c)

SOURCES_C_EXT += $(wildcard $(DIR_EXT)/STM32F4xx_HAL_Driver/Src/*.c)
SOURCES_C_EXT := $(filter-out $(DIR_EXT)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_timebase_rtc_alarm_template.c,$(SOURCES_C_EXT))
SOURCES_C_EXT := $(filter-out $(DIR_EXT)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_timebase_rtc_wakeup_template.c,$(SOURCES_C_EXT))
SOURCES_C_EXT := $(filter-out $(DIR_EXT)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_timebase_tim_template.c,$(SOURCES_C_EXT))

SOURCES_CPP_TARGET := $(wildcard $(DIR_TARGET)/*.cpp)
SOURCES_ASM_TARGET := $(DIR_TARGET)/../startup_stm32f411xe.s

FLAGS_C_TARGET := $(FLAGS_C_COMMON)
FLAGS_C_TARGET += $(INCLUDES_TARGET)
FLAGS_C_TARGET += -Ofast
FLAGS_C_TARGET += -Wall -Wextra -Wpedantic
FLAGS_C_TARGET += -Wdouble-promotion
FLAGS_C_TARGET += -Dgcc
FLAGS_C_TARGET += -DUSE_HAL_DRIVER -DSTM32F411xE

FLAGS_C_EXT := $(FLAGS_C_COMMON)
FLAGS_C_EXT += $(INCLUDES_TARGET)
FLAGS_C_EXT += -Ofast
FLAGS_C_EXT += -Wdouble-promotion
FLAGS_C_EXT += -Dgcc
FLAGS_C_EXT += -DUSE_HAL_DRIVER -DSTM32F411xE

FLAGS_CPP_TARGET := $(FLAGS_CPP_COMMON)
FLAGS_CPP_TARGET += $(INCLUDES_TARGET)
FLAGS_CPP_TARGET += -Ofast
FLAGS_CPP_TARGET += -Wall -Wextra -Wpedantic
FLAGS_CPP_TARGET += -Wdouble-promotion
FLAGS_CPP_TARGET += -Dgcc

#######################################

OBJECTS_TARGET := $(addprefix $(DIR_OBJ)/,$(SOURCES_C_TARGET:.c=.o))
OBJECTS_TARGET += $(addprefix $(DIR_OBJ)/,$(SOURCES_CPP_TARGET:.cpp=.o))
OBJECTS_TARGET += $(addprefix $(DIR_OBJ)/,$(SOURCES_ASM_TARGET:.s=.o))

OBJECTS_EXT := $(addprefix $(DIR_OBJ)/,$(SOURCES_C_EXT:.c=.o))

DIR_OBJ_TARGET := $(DIR_OBJ)/$(DIR_TARGET)
DIR_OBJ_RTT := $(DIR_OBJ)/$(DIR_RTT)
DIR_OBJ_EXT := $(DIR_OBJ)/$(DIR_EXT)
DIRS_OBJS_TARGET := $(sort $(dir $(OBJECTS_TARGET) $(OBJECTS_EXT)))


#######################################

$(DIR_OBJ_TARGET):
	mkdir -p $(DIRS_OBJS_TARGET)

$(DIR_OBJ_TARGET)/%.o: $(DIR_TARGET)/%.c | $(DIR_OBJ_TARGET) #.BUILDINFO
	@echo "STM32F4 C: $(notdir $<)"
	@$(CC) -c $(FLAGS_C_TARGET) $< -o $@

$(DIR_OBJ_TARGET)/%.o: $(DIR_TARGET)/%.cpp | $(DIR_OBJ_TARGET) #.BUILDINFO
	@echo "STM32F4 CPP: $(notdir $<)"
	@$(CPP) -c $(FLAGS_CPP_TARGET) $< -o $@

$(DIR_OBJ_TARGET)/%.o: $(DIR_TARGET)/%.s | $(DIR_OBJ_TARGET) #.BUILDINFO
	@echo "STM32F4 ASM: $(notdir $<)"
	@$(AS) -c $(FLAGS_MCU) $< -o $@

$(DIR_OBJ_RTT)/%.o: $(DIR_RTT)/%.c | $(DIR_OBJ_TARGET) #.BUILDINFO
	@echo "STM32F4 RTT C: $(notdir $<)"
	@$(CC) -c $(FLAGS_C_TARGET) $< -o $@

$(DIR_OBJ_EXT)/%.o: $(DIR_EXT)/%.c | $(DIR_OBJ_TARGET) #.BUILDINFO
	@echo "STM32F4 EXT C: $(notdir $<)"
	@$(CC) -c $(FLAGS_C_EXT) $< -o $@

#######################################
# $(OBJECTS_TARGET): make_mcb1768.mk make_common_arm.mk
# -include $(filter %.d, $(OBJECTS_TARGET:.o=.d))

#######################################
ARTIFACT := IMAGE

ARTIFACT_BIN := $(DIR_OBJ)/$(ARTIFACT).bin
ARTIFACT_HEX := $(DIR_OBJ)/$(ARTIFACT).hex
ARTIFACT_ELF := $(DIR_OBJ)/$(ARTIFACT).elf

FLAGS_LD += $(FLAGS_LD_COMMON)
ifeq ($(RAMLINK),1)
FLAGS_LD += -T$(DIR_TARGET)/../STM32F401VCTx_RAM.ld
else
FLAGS_LD += -T$(DIR_TARGET)/../STM32F401VCTx_FLASH.ld
endif
FLAGS_LD += -Xlinker -Map=$(DIR_OBJ)/$(ARTIFACT).map
# FLAGS_LD += -Xlinker --cref
# FLAGS_LD += -Xlinker --trace-symbol=fprintf

#######################################
.PHONY: stm32f4
stm32f4: $(ARTIFACT_BIN) $(ARTIFACT_HEX)
	echo $(OBJECTS_VCO)

$(ARTIFACT_BIN) : $(ARTIFACT_ELF)
	@echo "OC> $< $@"
	@$(CP) --output-target=binary $< $@

$(ARTIFACT_HEX) : $(ARTIFACT_ELF)
	@echo "OC> $< $@"
	@$(CP) -O ihex $< $@

$(ARTIFACT_ELF) : $(OBJECTS_EXT) $(OBJECTS_TARGET) $(OBJECTS_VCO)
	@echo "LD> $@..."
	@$(CPP) $(FLAGS_LD) $^ -o $@ 

