BUILD_DIR         := build
MAKEFILE          := $(BUILD_DIR)/Makefile
TARGET            := $(BUILD_DIR)/bin/vbc
BUILD_TYPE        ?= Debug
CMAKE_FLAGS       := -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

all: config build

$(MAKEFILE): config

$(TARGET): build

config:
	@echo "- CMake configure"
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake $(CMAKE_FLAGS) ..

build: config
	@echo "- Build target"
	@cd $(BUILD_DIR) && make

run: $(TARGET)
	@echo '-- run ------'
	@$(TARGET)

gdb: $(TARGET)
	@echo '-- gdb ------'
	@gdb $(TARGET)

clean:
	-rm -rf $(BUILD_DIR)

.PHONY: all config build run gdb clean

