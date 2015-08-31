OBJECTS = main.o meter_actions.o device_actions.o
ODIR = obj
SDIR = src
IDIR = inc
CFLAGS = -I$(IDIR)
CC = $(CROSS_COMPILE)gcc

OBJ = $(patsubst %,$(ODIR)/%,$(OBJECTS))

.PHONY: all
all: $(ODIR) $(OBJ) meters

$(ODIR):
	mkdir -p $(ODIR)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $^ $(CFLAGS) 

meters: $(OBJ)
	$(CC) -o $@ $^ $(CLFAGS)

.PHONY: clean
clean:
	rm -f meters
	rm -rf $(ODIR)
