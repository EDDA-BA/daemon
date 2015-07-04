CC = gcc
SOURCE = daemon_functions.c pem_functions.c base64.c desfireAccess.c pinpad.c createAesDesKey.c
OBJECTS = $(SOURCE:%.c=%.o) 
DIRS = tests utils desfire pinpad include
BIN = daemon test init reset delete
LIBS = `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` `pkg-config --libs gtk+-3.0` -lcrypto -lnfc -lfreefare 
VPATH = $(DIRS)

all: $(OBJECTS) $(BIN:%=%.o)
	for i in $(BIN); do $(CC) -o $$i $$i.o $(OBJECTS) $(LIBS); done

%.o: %.c
	$(CC) -c $< $(LIBS)

.PHONY: clean
clean:
	-$(RM) $(OBJECTS) $(BIN:%=%.o) $(BIN)
