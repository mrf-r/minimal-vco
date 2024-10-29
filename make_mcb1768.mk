DIR_TARGET := target_mcb1768
FLAGS_MCU := -mcpu=cortex-m3
# FLAGS_MCU := -mcpu=cortex-r4f -mfloat-abi=hard -mfpu=vfpv3-d16

include make_common_arm.mk

DIR_RTT := Segger
DIRS_INCLUDE_TARGET := $(DIR_TARGET)/src
DIRS_INCLUDE_TARGET += $(DIR_RTT)
include make_vco.mk
DIRS_INCLUDE_TARGET += $(DIRS_INCLUDE_VCO)
INCLUDES_TARGET := $(addprefix -I,$(DIRS_INCLUDE_TARGET))

SOURCES_C_TARGET := $(wildcard $(DIR_TARGET)/src/*.c)
SOURCES_C_TARGET += $(wildcard $(DIR_RTT)/*.c)
SOURCES_CPP_TARGET := $(wildcard $(DIR_TARGET)/*.cpp)

FLAGS_C_TARGET := $(FLAGS_C_COMMON)
FLAGS_C_TARGET += $(INCLUDES_TARGET)
FLAGS_C_TARGET += -Ofast
FLAGS_C_TARGET += -Wall -Wextra #-Wpedantic
FLAGS_C_TARGET += -Wdouble-promotion
FLAGS_C_TARGET += -Dgcc

FLAGS_CPP_TARGET := $(FLAGS_CPP_COMMON)
FLAGS_CPP_TARGET += $(INCLUDES_TARGET)
FLAGS_CPP_TARGET += -Ofast
FLAGS_CPP_TARGET += -Wall -Wextra -Wpedantic
FLAGS_CPP_TARGET += -Wdouble-promotion
FLAGS_CPP_TARGET += -Dgcc

#######################################

OBJECTS_TARGET := $(addprefix $(DIR_OBJ)/,$(SOURCES_C_TARGET:.c=.o))
OBJECTS_TARGET += $(addprefix $(DIR_OBJ)/,$(SOURCES_CPP_TARGET:.cpp=.o))
DIR_OBJ_TARGET := $(DIR_OBJ)/$(DIR_TARGET)
DIR_OBJ_RTT := $(DIR_OBJ)/$(DIR_RTT)
DIRS_OBJS_TARGET := $(sort $(dir $(OBJECTS_TARGET)))

#######################################

$(DIR_OBJ_TARGET):
	mkdir -p $(DIRS_OBJS_TARGET)

$(DIR_OBJ_TARGET)/%.o: $(DIR_TARGET)/%.c | $(DIR_OBJ_TARGET) #.BUILDINFO
	@echo "MCB1768 C: $(notdir $<)"
	@$(CC) -c $(FLAGS_C_TARGET) $< -o $@

$(DIR_OBJ_RTT)/%.o: $(DIR_RTT)/%.c | $(DIR_OBJ_TARGET) #.BUILDINFO
	@echo "MCB1768 RTT C: $(notdir $<)"
	@$(CC) -c $(FLAGS_C_TARGET) $< -o $@
	
$(DIR_OBJ_TARGET)/%.o: $(DIR_TARGET)/%.cpp | $(DIR_OBJ_TARGET) #.BUILDINFO
	@echo "MCB1768 CPP: $(notdir $<)"
	@$(CPP) -c $(FLAGS_CPP_TARGET) $< -o $@

#######################################
$(OBJECTS_TARGET): make_mcb1768.mk make_common_arm.mk
-include $(filter %.d, $(OBJECTS_TARGET:.o=.d))

#######################################
ARTIFACT := MCB1768

ARTIFACT_BIN := $(DIR_OBJ)/$(ARTIFACT).bin
ARTIFACT_HEX := $(DIR_OBJ)/$(ARTIFACT).hex
ARTIFACT_ELF := $(DIR_OBJ)/$(ARTIFACT).elf

FLAGS_LD += $(FLAGS_LD_COMMON)
# ifeq ($(RAMLINK),1) # ALWAYS RAMLINK
FLAGS_LD += -T$(DIR_TARGET)/_ram.ld
# else
# FLAGS_LD += -T$(DIR_TARGET)/_flash.ld
# endif
FLAGS_LD += -Xlinker -Map=$(DIR_OBJ)/$(ARTIFACT).map
# FLAGS_LD += -Xlinker --cref
# FLAGS_LD += -Xlinker --trace-symbol=fprintf

#######################################
.PHONY: mcb1768
mcb1768: $(ARTIFACT_BIN) $(ARTIFACT_HEX)
	echo $(OBJECTS_VCO)

$(ARTIFACT_BIN) : $(ARTIFACT_ELF)
	@echo "OC> $< $@"
	@$(CP) --output-target=binary $< $@

$(ARTIFACT_HEX) : $(ARTIFACT_ELF)
	@echo "OC> $< $@"
	@$(CP) -O ihex $< $@

$(ARTIFACT_ELF) : $(OBJECTS_TARGET) $(OBJECTS_VCO)
	@echo "LD> $@..."
	@$(CPP) $(FLAGS_LD) $^ -o $@ 

