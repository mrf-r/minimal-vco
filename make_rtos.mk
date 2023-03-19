MAKE_RTOS :=

include $(MAKE_COMMON)
include $(MAKE_CLI)

#######################################

DIR_RTOS := freertos-lts
DIR_RTOS_PORT := $(DIR_RTOS)/portable/GCC/CortexR4f

DIRS_INCLUDE_RTOS := $(DIR_RTOS)/include
DIRS_INCLUDE_RTOS += $(DIR_RTOS_PORT)

SOURCES_ASM_RTOS := $(wildcard $(DIR_RTOS_PORT)/*.S)
SOURCES_C_RTOS := $(wildcard $(DIR_RTOS_PORT)/*.c)
SOURCES_C_RTOS += $(wildcard $(DIR_RTOS)/*.c)
SOURCES_C_RTOS := $(filter-out $(DIR_RTOS)/stream_buffer.c, $(SOURCES_C_RTOS))
SOURCES_C_RTOS := $(filter-out $(DIR_RTOS_PORT)/heap_4.c, $(SOURCES_C_RTOS))

FLAGS_C_RTOS := $(FLAGS_C_COMMON)
FLAGS_C_RTOS += $(addprefix -I,$(DIRS_INCLUDE_RTOS))
FLAGS_C_RTOS += $(addprefix -I,$(DIRS_INCLUDE_HAL)) # SEGGER HERE !!!!
FLAGS_C_RTOS += $(addprefix -I,$(DIRS_INCLUDE_CLI)) # SEGGER HERE !!!!
FLAGS_C_RTOS += -Ofast

#######################################
DIR_OBJ_RTOS := $(DIR_OBJ)/$(DIR_RTOS)
OBJECTS_RTOS := $(SOURCES_C_RTOS:.c=.o)
OBJECTS_RTOS += $(SOURCES_ASM_RTOS:.S=.o)
OBJECTS_RTOS := $(addprefix $(DIR_OBJ)/,$(OBJECTS_RTOS))

DIRS_OBJ_RTOS := $(sort $(dir $(OBJECTS_RTOS)))

$(DIR_OBJ_RTOS):
	mkdir -p $(DIRS_OBJ_RTOS)

$(DIR_OBJ_RTOS)/%.o: $(DIR_RTOS)/%.c | $(DIR_OBJ_RTOS)
	@echo "RTOS C> $(notdir $<)"
	@$(CC) -c $(FLAGS_C_RTOS) $< -o $@

$(DIR_OBJ_RTOS)/%.o: $(DIR_RTOS)/%.S | $(DIR_OBJ_RTOS)
	@echo "RTOS AS> $(notdir $<)"
	@$(AS) $(FLAGS_ASM_COMMON) $< -o $@

#######################################
$(OBJECTS_CLI): make_rtos.mk make_common.mk
-include $(filter %.d, $(OBJECTS_RTOS:.o=.d))