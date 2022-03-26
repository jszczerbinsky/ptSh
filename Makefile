ver=/tmp/ptsh_ver

all:
	@echo Run \'make install\'

install:
	git rev-parse --short HEAD > $(ver)
	mkdir -p ~/.local/share/ptSh
	mkdir -p ~/.local/bin
	mkdir -p build
	gcc src/common/*.c src/ptls/*.c -lm -o build/ptls
	gcc src/common/*.c src/ptpwd/*.c -lm -o build/ptpwd
	gcc src/common/*.c src/ptcp/*.c -lm -o build/ptcp
	cp build/* ~/.local/bin/
	rm -rf build
	cp src/config ~/.local/share/ptSh/config
	cp LICENSE ~/.local/share/ptSh/LICENSE
	cp src/logo.txt ~/.local/share/ptSh/logo.txt
	cp src/ptsh.sh ~/.local/bin/ptsh
	mkdir -p ~/.config
	mkdir -p ~/.config/ptSh
	cp src/config ~/.config/ptSh/config
	echo "Version: cloned from " | tee ~/.local/share/ptSh/version.txt
	cat $(ver) | tee -a ~/.local/share/ptSh/version.txt
	~/.local/bin/ptsh

uninstall:
	rm -rf ~/.local/share/ptSh
	rm -rf ~/.config/ptSh
	rm ~/.local/bin/ptls
	rm ~/.local/bin/ptpwd
	rm ~/.local/bin/ptmove
	rm ~/.local/bin/ptsh
