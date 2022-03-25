ver=/tmp/ptsh_ver

all:
	@echo Run \'make install\'

install:
	mkdir -p ~/.local/share/ptSh
	mkdir -p ~/.local/bin
	mkdir -p build
	gcc src/configParser/configParser.c src/ptls/*.c -lm -o build/ptls
	gcc src/configParser/configParser.c src/ptpwd/*.c -lm -o build/ptpwd
	gcc src/configParser/configParser.c src/ptmove/*.c -lm -o build/ptmove
	cp build/* ~/.local/bin/
	rm -rf build
	cp src/config ~/.local/share/ptSh/config
	cp LICENSE ~/.local/share/ptSh/LICENSE
	cp src/logo.txt ~/.local/share/ptSh/logo.txt
	cp src/ptsh.sh ~/.local/bin/ptsh
	mkdir -p ~/.config
	mkdir -p ~/.config/ptSh
	cp src/config ~/.config/ptSh/config
	~/.local/bin/ptsh

uninstall:
	rm -rf ~/.local/share/ptSh
	rm -rf ~/.config/ptSh
	rm ~/.local/bin/ptls
	rm ~/.local/bin/ptpwd
	rm ~/.local/bin/ptmove
	rm ~/.local/bin/ptsh
