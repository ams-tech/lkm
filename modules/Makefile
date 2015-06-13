MODULES = meters


.PHONY: all
all: $(MODULES)

.PHONY: $(MODULES)
$(MODULES):
	$(MAKE) -C $(CURDIR)/$@ LINUX_SRC=$(LINUX_SRC)
	mkdir -p build
	cp -f $@/$@.ko build/.
