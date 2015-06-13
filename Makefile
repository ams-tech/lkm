LINUX_SRC=$(CURDIR)/linux

.PHONY: all
all:	linux modules

.PHONY: modules
modules:
	$(MAKE) LINUX_SRC=$(LINUX_SRC) -C modules
	cp -f modules/build/* build/modules/*

.PHONY: clean_modules
clean_modules:
	make -C modules clean

.PHONY: clean
clean:
	rm -rf build
	rm -f Module.*
	rm -f modules.*

linux:
	$(error Directory "linux" does not exist.  You should probably make a symlink to a linux source directory)
