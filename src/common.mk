CC = gcc
AR = ar
LD = ld
AS = as
SHELL = bash

CFLAGS ?= -nostdlib -nostdinc -fno-builtin -mno-outline-atomics -O2 -Wall -Werror -I../libth
EXEFLAGS ?= -nostdlib -ffreestanding -no-pie -L../libth
SOFLAGS ?= -nostdlib -shared

ROOT ?= ../../root
EXE_DEST ?= /bin
LIB_DEST ?= /lib

SRC ?= $(shell shopt -s nullglob; echo *.c *.s)
OBJ := $(SRC:.c=.o)
OBJ := $(OBJ:.s=.o)

ifdef EXE_TARGET
	OBJ += ../libth/start.o
endif

TARGETS = $(EXE_TARGET)
ifdef LIB_TARGET
	TARGETS += $(LIB_TARGET).a $(LIB_TARGET).so
endif

all: $(TARGETS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $< -o $@

$(EXE_TARGET): $(OBJ)
	$(CC) $(EXEFLAGS) -o $(EXE_TARGET) $(OBJ) $(LIB)

$(LIB_TARGET).a: $(OBJ)
	$(AR) rs $(LIB_TARGET).a $(OBJ)

$(LIB_TARGET).so: $(OBJ)
	$(LD) $(SOFLAGS) -o $(LIB_TARGET).so $(OBJ)

install: $(TARGETS)
ifdef EXE_TARGET
	cp $(EXE_TARGET) $(ROOT)/$(EXE_DEST)/
endif
ifdef LIB_TARGET
	cp $(LIB_TARGET).a $(ROOT)/$(LIB_DEST)/
	cp $(LIB_TARGET).so $(ROOT)/$(LIB_DEST)/
endif

clean:
	rm -f *.o *.a *.so $(TARGETS)
