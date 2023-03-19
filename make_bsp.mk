MAKE_BSP :=

include $(MAKE_COMMON)
include $(MAKE_HAL)
include $(MAKE_RTOS)
include $(MAKE_CLI)

#######################################
ifeq ($(HW622),1)
DIR_BSP := 622/bsp
else
DIR_BSP := bsp
endif

DIRS_INCLUDE_BSP := $(DIR_BSP)
INCLUDES_BSP := $(addprefix -I,$(DIRS_INCLUDE_BSP) $(DIRS_INCLUDE_HAL) $(DIRS_INCLUDE_CLI) $(DIRS_INCLUDE_RTOS))

SOURCES_C_BSP := $(wildcard $(DIR_BSP)/*.c)
SOURCES_CPP_BSP := $(wildcard $(DIR_BSP)/*.cpp)

ifeq ($(RAMLINK),1)
SOURCES_C_BSP := $(filter-out $(DIR_BSP)/bsp_ivl_main.c, $(SOURCES_C_BSP))
SOURCES_CPP_BSP := $(filter-out $(DIR_BSP)/bsp_ivl_main.cpp, $(SOURCES_CPP_BSP))
else
SOURCES_C_BSP := $(filter-out $(DIR_BSP)/bsp_ivlhwtest.c, $(SOURCES_C_BSP))
SOURCES_CPP_BSP := $(filter-out $(DIR_BSP)/bsp_ivlhwtest.cpp, $(SOURCES_CPP_BSP))
endif

FLAGS_C_BSP := $(FLAGS_C_COMMON)
FLAGS_C_BSP += $(INCLUDES_BSP)
FLAGS_C_BSP += -Ofast
FLAGS_C_BSP += -Wall -Wextra -Wpedantic
FLAGS_C_BSP += -Wdouble-promotion
FLAGS_C_BSP += -Dgcc

FLAGS_CPP_BSP := $(FLAGS_CPP_COMMON)
FLAGS_CPP_BSP += $(INCLUDES_BSP)
FLAGS_CPP_BSP += -Ofast
FLAGS_CPP_BSP += -Wall -Wextra -Wpedantic
FLAGS_CPP_BSP += -Wdouble-promotion
FLAGS_CPP_BSP += -Dgcc

#######################################

OBJECTS_BSP := $(addprefix $(DIR_OBJ)/,$(SOURCES_C_BSP:.c=.o))
OBJECTS_BSP += $(addprefix $(DIR_OBJ)/,$(SOURCES_CPP_BSP:.cpp=.o))
DIR_OBJ_BSP := $(DIR_OBJ)/$(DIR_BSP)
DIRS_OBJ_BSP := $(sort $(dir $(OBJECTS_BSP)))

#######################################
FILE_MD5 := $(DIR_BSP)/bsp_nvm_tbl_md5.h

$(FILE_MD5): $(DIR_BSP)/bsp_nvm_tbl_hwid.c $(DIR_BSP)/bsp_nvm_tbl_opt.c $(DIR_BSP)/bsp_nvm_tbl_calib.c
	./prebuild-nvm.sh

$(DIR_OBJ_BSP):
	mkdir -p $(DIRS_OBJ_BSP)

$(DIR_OBJ_BSP)/%.o: $(DIR_BSP)/%.c | $(DIR_OBJ_BSP) .BUILDINFO $(FILE_MD5)
	@echo "BSP C: $(notdir $<)"
	@$(CC) -c $(FLAGS_C_BSP) $< -o $@
	
$(DIR_OBJ_BSP)/%.o: $(DIR_BSP)/%.cpp | $(DIR_OBJ_BSP) .BUILDINFO $(FILE_MD5)
	@echo "BSP CPP: $(notdir $<)"
	@$(CPP) -c $(FLAGS_CPP_BSP) $< -o $@

#######################################
$(OBJECTS_BSP): make_bsp.mk make_common.mk
-include $(filter %.d, $(OBJECTS_BSP:.o=.d))
