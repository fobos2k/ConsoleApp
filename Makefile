BUILD_DIR = build

all: build

rebuild: clean build

config:
	@echo "Configuration..."
	@cmake -B $(BUILD_DIR) -G "Unix Makefiles"

build: config
	@echo "Building..."
	@cmake --build $(BUILD_DIR)



clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD_DIR)

distclean: clean
	@echo "Dist clean..."
	@git clean -xdf

