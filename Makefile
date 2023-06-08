MAKEFLAGS += --silent

BUILD_DIR = build
APP = $(BUILD_DIR)/libraria-test

default: $(APP)

$(APP): config
	cmake --build build

debug:
	gdb $(APP)

config-debug: config
	cmake -S . -B build/ -D CMAKE_BUILD_TYPE=Debug

config-release: config
	cmake -S . -B build/ -D CMAKE_BUILD_TYPE=Release

config: $(BULD_DIR)
	cmake -E make_directory build
	cmake -E chdir build cmake ..

clean:
	cmake --clean build

remove:
	-rm -rf build

.PHONY: $(APP) debug config config-debug config-release remove clean
