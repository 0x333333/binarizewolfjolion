ifneq (, $(findstring Windows, $(OS)))
WINDOWS=1
CC=gcc
endif

CFLAGS=-Wall -pedantic
ifneq ($(WINDOWS), 1)
CFLAGS+=-ansi
endif

FOPT=-O3 -fomit-frame-pointer

FI686=-m32 -march=i686 -mtune=i686
FAMD64=-m64
FSPARC=-m64 -mcpu=niagara -mtune=niagara
ifeq ($(WINDOWS), 1)
ARCH=i686
FARCH=$(FI686)
else
ARCH=$(shell uname -m)
ifeq ($(ARCH), i686)
FARCH=$(FI686)
endif
ifeq ($(ARCH), x86_64)
FARCH=$(FAMD64)
endif
ifeq ($(ARCH), sparc64)
FARCH=$(FSPARC)
endif
endif

.PHONY: all debug static clean

all: fcbo

debug: fcbo-debug

static: fcbo-static

fcbo: %: %.c
ifeq ($(WINDOWS), 1)
	$(CC) $(CFLAGS) $(FOPT) $(FARCH) -o $@.exe $^
	strip $@.exe
else
	$(CC) $(CFLAGS) $(FOPT) $(FARCH) -o $@ $^
	strip $@
endif

fcbo-debug: %-debug: %.c
ifeq ($(WINDOWS), 1)
	$(CC) $(CFLAGS) -g $(FARCH) -o $@.exe $^
else
	$(CC) $(CFLAGS) -g $(FARCH) -o $@ $^
endif

fcbo-static: %-static: %.c
ifeq ($(WINDOWS), 1)
	$(CC) $(CFLAGS) $(FOPT) $(FARCH) -static -o $@-windows-$(ARCH).exe $^
	strip $@-windows-$(ARCH).exe
else
	$(CC) $(CFLAGS) $(FOPT) $(FARCH) -static -o $@-linux-$(ARCH) $^
	strip $@-linux-$(ARCH)
endif

clean:
	rm -f fcbo fcbo-debug fcbo-static* *.exe
