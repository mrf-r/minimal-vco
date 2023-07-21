
.DEFAULT_GOAL := help
DIR_OBJ := build

ifeq ($(MAKECMDGOALS),mcb1768)
    -include make_mcb1768.mk
endif
ifeq ($(MAKECMDGOALS),stm32f4)
    -include make_stm32f4_my.mk
endif
ifeq ($(MAKECMDGOALS),stm32g0)
    -include make_stm32g0.mk
endif

.PHONY: clean
clean:  ; rm -fR $(DIR_OBJ)

.PHONY: help
help : 
	@echo "usage: make stm32g0 / mcb1768 / clean -j -O ..."
	@echo "use variable GCC_PATH to customize path to ../gcc_arm_none_eabi/bin"
	@echo "use DEBUG=1 most of the time"
	@echo "use RAMLINK=1 to speed up tings"
	@echo "don't use CLI_MODE_FLOAT_VARIABLES=1 ))"
	@echo "you may use BUILD_TAG=..."
# @echo $(FLAGS_C_TARGET)
# @echo "help: $(foreach odir, $(DIRS_OBJ_HAL), $(wildcard $(odir)/*.d))"
