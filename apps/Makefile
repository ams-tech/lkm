APPS = meters

.PHONY: all
all: $(APPS)

.PHONY: $(APPS)
$(APPS):
	$(MAKE) -C $(CURDIR)/$@
	mkdir -p build
	cp -f $@/$@ build/.
