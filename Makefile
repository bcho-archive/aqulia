PROJECT_NAME = aqulia

CMAKE = `which cmake`
TARGET_DIR = build
SOURCE_DIR = $(CURDIR)
SRC_DIR = src
PORT_DIR = port

ifeq ($(DEBUG), 1)
	CMAKE_VARS := -DCMAKE_BUILD_TYPE=Debug
endif

all: build

prepare:
	mkdir -p $(TARGET_DIR)
	cd $(TARGET_DIR); $(CMAKE) $(CMAKE_VARS) $(CURDIR)

build: prepare
	cd $(TARGET_DIR); $(MAKE); $(MAKE) install

pack:
	zip $(PORT_DIR).zip $(PORT_DIR)/*

prepare_port:
	rm -rf $(PORT_DIR)
	mkdir -p $(PORT_DIR)
	cp -r $(SRC_DIR)/* $(PORT_DIR)
	-cp `find $(PORT_DIR) -name '*.[ch]'` $(PORT_DIR)/

generate_prj:
	rm -rf $(PORT_DIR)/$(PROJECT_NAME).prj
	find $(PORT_DIR)/*.c > $(PORT_DIR)/$(PROJECT_NAME).prj
	sed -i -e 's/$(PORT_DIR)\///' $(PORT_DIR)/$(PROJECT_NAME).prj

port: prepare_port generate_prj
	find $(PORT_DIR) -name '*.[ch]' | xargs sed -i -e 's/inline //'
	find $(PORT_DIR) -name '*.*' | xargs sed -i -e 's/\r\+$$/\r/'

clean:
	rm -rf $(TARGET_DIR)

.PHONY: all prepare build clean pack prepare_port generate_prj port
