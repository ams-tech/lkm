MODULES = phmeter


.PHONY: all
all: $(MODULES)

.PHONY: $(MODULES)
$(MODULES):
	$(MAKE) -c $(LINUX_SRC) M=$(PWD)/$@ modules
	cp -f $@/$@.ko build/.
