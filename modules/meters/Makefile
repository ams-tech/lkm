METERS = ph
LINUX_SRC ?= undefined

.PHONY: all
all: 
	rm -f Kbuild_inc
	NUMMETERS=0; for meter in $(METERS) ; do \
		echo "include $$meter/Kbuild" >> Kbuild_inc ; \
		NUMMETERS=`echo $$NUMMETERS + 1 | bc`; \
	done ; \
	$(MAKE) -C $(LINUX_SRC) M=$(PWD) NUM_METERS=$$NUMMETERS modules
 
