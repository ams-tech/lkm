LINUX_SRC=$(CURDIR)/linux

.PHONY: all
all:	install_all

install:
	echo "Directory 'install' does not exist.  Creating it..."
	mkdir -p install

.PHONY: install_all
install_all: linux modules apps install
	cp -r build/* install/.
	

.PHONY: modules
modules:
	mkdir -p build/modules
	$(MAKE) LINUX_SRC=$(LINUX_SRC) -C modules
	cp -r modules/build/* build/modules/.

.PHONY: clean_modules
clean_modules:
	make -C modules clean

.PHONY: clean_apps
clean_apps:
	make -C apps clean

.PHONY: clean
clean: clean_modules clean_apps
	rm -rf build

.PHONY: apps
apps:
	mkdir -p build/apps
	$(MAKE) LINUX=$(LINUX_SRC) -C apps
	cp apps/build/* build/apps/.

linux:
	$(error Directory "linux" does not exist.  You should probably make a symlink to a linux source directory)
