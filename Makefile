TAG=$(shell git describe --exact-match --tags 2>/dev/null)
COMMIT=$(shell echo "cloned commit: " && git rev-parse --short HEAD 2>/dev/null)

VER=$(or $(TAG),$(COMMIT))

all:
	mkdir -p build
	rm -rf build/*
	mkdir -p build/bin
	mkdir -p build/share
	mkdir -p build/share/ptSh
	mkdir -p build/share/licenses
	mkdir -p build/share/licenses/ptSh
	gcc src/common/*.c src/ptls/*.c -lm -o build/bin/ptls
	gcc src/common/*.c src/ptpwd/*.c -lm -o build/bin/ptpwd
	gcc src/common/*.c src/ptcp/*.c -lm -o build/bin/ptcp
	cp src/ptsh.sh build/bin/ptsh
	cp src/config build/share/ptSh/config
	cp LICENSE build/share/ptSh/LICENSE
	cp LICENSE build/share/licenses/ptSh/LICENSE
	cp src/logo.txt build/share/ptSh/logo.txt
	echo "Version: " | tee build/share/ptSh/version.txt
	echo $(VER) | tee -a build/share/ptSh/version.txt

	
install:
	cp -R build/* $(DESTDIR)/usr
	$(DESTDIR)/usr/bin/ptsh

uninstall:
	rm -rf $(DESTDIR)/usr/share/ptSh
	rm $(DESTDIR)/usr/bin/ptls
	rm $(DESTDIR)/usr/bin/ptpwd
	rm $(DESTDIR)/usr/bin/ptcp
	rm $(DESTDIR)/usr/bin/ptsh
	rm -rf $(DESTDIR)/usr/share/licenses/ptSh
