MAKE_IVL :=

include $(MAKE_COMMON)
include $(MAKE_BSP)
include $(MAKE_CLI)

#######################################
ifeq ($(HW622),1)
DIR_IVL := 622/ivl
else
DIR_IVL := ivl
endif

DIRS_INCLUDE_IVL := $(DIR_IVL)
DIRS_INCLUDE_IVL += $(DIR_IVL)/src
DIRS_INCLUDE_IVL += $(DIRS_INCLUDE_BSP) $(DIR_CLI)

SOURCES_C_IVL := $(wildcard $(DIR_IVL)/src/*.c)
SOURCES_C_IVL += $(wildcard $(DIR_IVL)/src/Breath/*.c)
# SOURCES_C_IVL += $(wildcard $(DIR_IVL)/src/Calibr/*.c)
# SOURCES_CPP_IVL := $(wildcard $(DIR_IVL)/*.cpp)

FLAGS_C_IVL := $(FLAGS_C_COMMON)
FLAGS_C_IVL += $(addprefix -I,$(DIRS_INCLUDE_IVL))
# FLAGS_C_IVL += -Wdouble-promotion
# FLAGS_C_IVL += -Ofast
# FLAGS_C_IVL += -Wall -Wextra -Wpedantic # TODO!!!!
FLAGS_C_IVL += -DV621=1 -DV622=1 -DVerHund=200 -DVertRazg=1
FLAGS_C_IVL += -Wno-unknown-pragmas -Wno-parentheses -Wno-missing-braces -Wno-strict-aliasing -Wno-char-subscripts  -Wno-pointer-sign -Wno-discarded-qualifiers

# FLAGS_CPP_IVL := $(FLAGS_CPP_COMMON)
# FLAGS_CPP_IVL += $(addprefix -I,$(DIRS_INCLUDE_BSP) $(DIRS_INCLUDE_IVL))
# FLAGS_CPP_IVL += -Ofast
# FLAGS_CPP_IVL += -Wall -Wextra -Wpedantic

#######################################

OBJECTS_IVL := $(addprefix $(DIR_OBJ)/,$(SOURCES_C_IVL:.c=.o))
OBJECTS_IVL += $(addprefix $(DIR_OBJ)/,$(SOURCES_CPP_IVL:.cc=.o))
OBJECTS_IVL += $(addprefix $(DIR_OBJ)/,$(SOURCES_ASM_IVL:.S=.o))
DIR_OBJ_IVL := $(addprefix $(DIR_OBJ)/,$(DIR_IVL))
DIRS_OBJ_IVL := $(sort $(dir $(OBJECTS_IVL)))

#######################################

$(DIR_OBJ_IVL):
	mkdir -p $(DIRS_OBJ_IVL)

$(DIR_OBJ_IVL)/%.o: $(DIR_IVL)/%.c | $(DIR_OBJ_IVL) .BUILDINFO
	@echo "IVL C: $(notdir $<)"
	@$(CC) -c $(FLAGS_C_IVL) $< -o $@

$(DIR_OBJ_IVL)/%.o: $(DIR_IVL)/%.cpp | $(DIR_OBJ_IVL)
	@echo "IVL CPP: $(notdir $<)..."
	@$(CPP) -c $(FLAGS_CPP_IVL) $< -o $@

# TODO NO ASM HERE!!
# $(DIR_OBJ_IVL)/%.o: $(DIR_IVL)/%.S | $(DIR_OBJ_IVL)
# 	@echo "IVL AS: $(notdir $<)"
# 	@$(AS) $(FLAGS_ASM_COMMON) $< -o $@
	

#######################################
$(OBJECTS_IVL): make_ivl.mk make_common.mk
-include $(filter %.d, $(OBJECTS_IVL:.o=.d))
