.PHONY: all obj ptls ptcp ptpwd

CC=cc
LDFLAGS=-lm
PREFIX=/usr/local

TAG=$(shell git describe --exact-match --tags 2>/dev/null)
COMMIT=$(shell echo "cloned commit: " && git rev-parse --short HEAD 2>/dev/null)

VER=$(or $(TAG),$(COMMIT))

SRC=$(shell find src -iname "*.c")
OBJ=$(subst src/,obj/,$(SRC:.c=.o))

TARGETS=ptls ptcp ptpwd
BIN=$(addprefix build/bin/,$(TARGETS))

obj/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) -c -DPREFIX=\"$(PREFIX)\" $(CFLAGS) $(LDFLAGS) -o $@ $^

build/bin/%: obj/%/*.o
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -DPREFIX=\"$(PREFIX)\" $(LDFLAGS) obj/common/*.o -o $@ $^

all: obj
	$(MAKE) $(BIN) CC=$(CC) CFLAGS=$(CFLAGS) LDFLAGS=$(LDFLAGS) PREFIX="$(PREFIX)"
	mkdir -p build
	mkdir -p build/bin
	mkdir -p build/share
	mkdir -p build/share/ptSh
	mkdir -p build/share/licenses
	mkdir -p build/share/licenses/ptSh
	cp src/ptsh.sh build/bin/ptsh
	cp src/config build/share/ptSh/config
	cp LICENSE build/share/ptSh/LICENSE
	cp LICENSE build/share/licenses/ptSh/LICENSE
	cp src/logo.txt build/share/ptSh/logo.txt
	echo "Version: " | tee build/share/ptSh/version.txt
	echo $(VER) | tee -a build/share/ptSh/version.txt

obj: $(OBJ)

clean:
	rm -rf build/*
	rm -rf obj/*

install:
	cp -R build/* $(PREFIX)
	$(PREFIX)/bin/ptsh

uninstall:
	rm -rf $(PREFIX)/share/ptSh
	rm $(PREFIX)/bin/ptls
	rm $(PREFIX)/bin/ptpwd
	rm $(PREFIX)/bin/ptcp
	rm $(PREFIX)/bin/ptsh
	rm -rf $(PREFIX)/share/licenses/ptSh
