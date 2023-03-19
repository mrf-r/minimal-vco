
DIR_VCO := vco

DIRS_INCLUDE_VCO := $(DIR_VCO) $(DIRS_INCLUDE_TARGET)

SOURCES_C_VCO := $(wildcard $(DIR_VCO)/*.c)

FLAGS_C_VCO := $(FLAGS_C_COMMON)
FLAGS_C_VCO += $(addprefix -I,$(DIRS_INCLUDE_VCO))
FLAGS_C_VCO += -Ofast
FLAGS_C_VCO += -Wall -Wextra -Wpedantic
FLAGS_C_VCO += -Wdouble-promotion
FLAGS_C_VCO += -Dgcc

#######################################
OBJECTS_VCO := $(SOURCES_C_VCO:.c=.o)
OBJECTS_VCO := $(addprefix $(DIR_OBJ)/, $(OBJECTS_VCO))

DIRS_OBJ_VCO := $(sort $(dir $(OBJECTS_VCO)))
DIR_OBJ_VCO := $(DIR_OBJ)/$(DIR_VCO)

#######################################
$(DIR_OBJ_VCO):
	mkdir -p $(DIRS_OBJ_VCO)

$(DIR_OBJ_VCO)/%.o: $(DIR_VCO)/%.c | $(DIR_OBJ_VCO)
	@echo "VCO C> $(notdir $<)"
	@$(CC) -c $(FLAGS_C_VCO) $< -o $@

#######################################
$(OBJECTS_CLI): make_vco.mk make_common.mk
#-include $(filter %.d, $(OBJECTS_VCO:.o=.d))
-include $(foreach odir, $(DIRS_OBJ_VCO), $(wildcard $(odir)/*.d))