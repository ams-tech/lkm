METERS = ph
LINUX_SRC ?= undefined

.PHONY: all
all: 
	rm -f Kbuild_inc
	EXTERNS=""; INITS="{"; NUMMETERS=0; for meter in $(METERS) ; do \
		echo "include \$$(src)/$$meter/Kbuild" >> Kbuild_inc ; \
		NUMMETERS=`echo $$NUMMETERS + 1 | bc`; \
		EXTERNS="$$EXTERNS extern meter_dev_t $${meter}_meter_dev;" ; \
		INITS="$$INITS &$${meter}_meter_dev," ; \
	done ; INITS="$$INITS }" ; \
	echo "$$EXTERNS" ; \
	echo "$$INITS" ; \
	$(MAKE) -C $(LINUX_SRC) M=$(CURDIR) MY_INITS="$$INITS" MY_EXTERNS="$$EXTERNS" NUM_METERS=$$NUMMETERS modules
 
