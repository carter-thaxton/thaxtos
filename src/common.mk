CC = gcc
AR = ar
LD = ld
AS = as
SHELL = bash

CFLAGS ?= -nostdlib -nostdinc -fno-builtin -I../libth
EXEFLAGS ?= -nostdlib -ffreestanding -no-pie -L../libth
SOFLAGS ?= -nostdlib -shared
ROOT ?= ../../root
EXE_DEST ?= /bin
LIB_DEST ?= /lib

SRC ?= $(shell shopt -s nullglob; echo *.c *.s)
OBJ := $(SRC:.c=.o)
OBJ := $(OBJ:.s=.o)

ifdef TARGET_EXE
	OBJ += ../libth/start.o
endif

TARGETS = $(TARGET_EXE)
ifdef TARGET_LIB
	TARGETS += $(TARGET_LIB).a $(TARGET_LIB).so
endif

all: $(TARGETS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $< -o $@

$(TARGET_EXE): $(OBJ)
	$(CC) $(EXEFLAGS) -o $(TARGET_EXE) $(OBJ) $(LIB)

$(TARGET_LIB).a: $(OBJ)
	$(AR) rs $(TARGET_LIB).a $(OBJ)

$(TARGET_LIB).so: $(OBJ)
	$(LD) $(SOFLAGS) -o $(TARGET_LIB).so $(OBJ)

install: $(TARGETS)
ifdef TARGET_EXE
	cp $(TARGET_EXE) $(ROOT)/$(EXE_DEST)/
endif
ifdef TARGET_LIB
	cp $(TARGET_LIB).a $(ROOT)/$(LIB_DEST)/
	cp $(TARGET_LIB).so $(ROOT)/$(LIB_DEST)/
endif

clean:
	rm -f *.o *.a *.so $(TARGETS)
