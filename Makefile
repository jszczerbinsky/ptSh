all:
	mkdir -p build
	rm -rf build/*
	mkdir -p build/bin
	mkdir -p build/share
	mkdir -p build/share/ptSh
	gcc src/common/*.c src/ptls/*.c -lm -o build/bin/ptls
	gcc src/common/*.c src/ptpwd/*.c -lm -o build/bin/ptpwd
	gcc src/common/*.c src/ptcp/*.c -lm -o build/bin/ptcp
	cp src/ptsh.sh build/bin/ptsh
	cp src/config build/share/ptSh/config
	cp LICENSE build/share/ptSh/LICENSE
	cp src/logo.txt build/share/ptSh/logo.txt
	git rev-parse --short HEAD > /tmp/ptsh_ver
	echo "Version: cloned from " | tee build/share/ptSh/version.txt
	cat /tmp/ptsh_ver | tee -a build/share/ptSh/version.txt

install:
	mkdir -p ~/.config
	mkdir -p ~/.config/ptSh
	sudo cp -R build/* /usr
	mkdir -p ~/.config
	mkdir -p ~/.config/ptSh
	cp src/config ~/.config/ptSh/config
	/usr/bin/ptsh

uninstall:
	sudo rm -rf /usr/share/ptSh
	rm -rf ~/.config/ptSh
	sudo rm /usr/bin/ptls
	sudo rm /usr/bin/ptpwd
	sudo rm /usr/bin/ptcp
	sudo rm /usr/bin/ptsh
