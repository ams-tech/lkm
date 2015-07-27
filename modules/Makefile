MODULES = meters


.PHONY: all
all: $(MODULES)

.PHONY: clean
clean:
	rm -rf build

.PHONY: $(MODULES)
$(MODULES):
	$(MAKE) -C $(CURDIR)/$@ LINUX_SRC=$(LINUX_SRC)
	mkdir -p build
	cp -f $@/$@.ko build/.
	cp -rf $@/scripts build/.
