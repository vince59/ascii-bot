export DEBUG=yes
export CC=gcc
#LOG_LEVEL = 1 : error only, 2 : error and warning, 3 : error warning and info
ifeq ($(DEBUG),yes)
	CFLAGS=-I./include/ -W -Wall  -pedantic -g -D LOG_LEVEL=3
else
	CFLAGS=-I./include/ -W -Wall  -pedantic -D LOG_LEVEL=1
endif
export CFLAGS
export LDFLAGS=-lncurses -lpthread -lm
ROBOT_DIR=src/robot
SIMULATOR_DIR=src/simulator
EXEC=bot
MAKEFLAGS += --no-print-directory
all: $(EXEC)

$(EXEC):
ifeq ($(DEBUG),yes)
	@echo "Debug enabled"
else
	@echo "Make in release mode"
endif
	@(cd $(ROBOT_DIR) && $(MAKE))
	@(cd $(SIMULATOR_DIR) && $(MAKE))

clean:
	@(cd $(ROBOT_DIR) && $(MAKE) $@)
	@(cd $(SIMULATOR_DIR) && $(MAKE) $@)
	
mrproper: clean
	@(cd $(ROBOT_DIR) && $(MAKE) $@)
	@(cd $(SIMULATOR_DIR) && $(MAKE) $@)
	@rm -f ./bot
	@rm -f ./simulator
	
