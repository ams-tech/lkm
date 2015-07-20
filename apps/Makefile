APPS = meters
CLEAN_APPS = $(patsubst %,%_clean,$(APPS))

.PHONY: all
all: $(APPS)

.PHONY: clean
clean:
	

.PHONY: $(APPS)
$(APPS):
	$(MAKE) -C $(CURDIR)/$@
	mkdir -p build
	cp -f $@/$@ build/.
