ver=/tmp/ptsh_ver

all:
	@echo Run \'make install\'

install:
	git rev-parse --short HEAD > $(ver)
	mkdir -p ~/.local/share/ptSh
	mkdir -p ~/.local/share/ptSh/include
	mkdir -p ~/.local/share/ptSh/lib
	mkdir -p ~/.local/bin
	make -C src/libptsh
	make -C src/ptls
	make -C src/ptpwd
	cp src/config ~/.local/share/ptSh/config
	cp src/set_aliases.sh ~/.local/bin/ptSh_set_aliases
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
	rm ~/.local/bin/ptSh_set_aliases
	rm ~/.local/bin/ptls
	rm ~/.local/bin/ptpwd
	rm ~/.local/bin/ptsh
