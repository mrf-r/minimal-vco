MAKE_CLI :=

include $(MAKE_COMMON)

#######################################

DIR_CLI := cli

DIRS_INCLUDE_CLI := $(DIR_CLI)
DIRS_INCLUDE_CLI += $(DIR_CLI)/Segger/Config
DIRS_INCLUDE_CLI += $(DIR_CLI)/Segger/SEGGER

SOURCES_CLI := \
$(DIR_CLI)/cli.c \
$(DIR_CLI)/cli_log.c \
$(DIR_CLI)/Segger/SEGGER/SEGGER_RTT.c

# ifeq ($(DEBUG),1)
# SOURCES_CLI += $(DIR_CLI)/Segger/SEGGER_SYSVIEW.c
# SOURCES_CLI += $(DIR_CLI)/Segger/sv_glue.c
# SOURCES_CLI += $(DIR_CLI)/Segger/Sample/FreeRTOSV10/SEGGER_SYSVIEW_FreeRTOS.c
# endif

FLAGS_C_CLI := $(FLAGS_C_COMMON)
FLAGS_C_CLI += $(addprefix -I,$(DIRS_INCLUDE_CLI))
FLAGS_C_CLI += -Ofast
FLAGS_C_CLI += -Wall -Wpedantic
# FLAGS_C_CLI += -Wdouble-promotion

#######################################
OBJECTS_CLI := $(addprefix $(DIR_OBJ)/,$(SOURCES_CLI:.c=.o))
DIRS_CLI := $(sort $(dir $(SOURCES_CLI)))
DIR_OBJ_CLI := $(addprefix $(DIR_OBJ)/,$(DIR_CLI))
DIRS_OBJ_CLI := $(addprefix $(DIR_OBJ)/,$(DIRS_CLI))

$(DIR_OBJ_CLI):
	mkdir -p $(DIRS_OBJ_CLI)

$(DIR_OBJ_CLI)/%.o: $(DIR_CLI)/%.c | $(DIR_OBJ_CLI)
	@echo "CLI C: $(notdir $<)"
	@$(CC) -c $(FLAGS_C_CLI) $< -o $@

#######################################
$(OBJECTS_CLI): make_cli.mk make_common.mk
-include $(filter %.d, $(OBJECTS_CLI:.o=.d))