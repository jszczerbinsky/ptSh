ver=/tmp/ptsh_ver

all:
	@echo Run \'make install\'

install:
	mkdir -p ~/.local/share/ptSh
	cp src/config ~/.local/share/ptSh/config
	mkdir -p ~/.local/bin
	cp src/set_aliases.sh ~/.local/bin/ptSh_set_aliases
	cp LICENSE ~/.local/share/ptSh/LICENSE
	cp src/logo.txt ~/.local/share/ptSh/logo.txt
	cp src/ptLs.sh ~/.local/bin/ptls
	cp src/ptPwd.sh ~/.local/bin/ptpwd
	cp src/ptMkdir.sh ~/.local/bin/ptmkdir
	cp src/ptTouch.sh ~/.local/bin/pttouch
	cp src/ptCp.sh ~/.local/bin/ptcp
	cp src/ptMv.sh ~/.local/bin/ptmv
	cp src/ptRm.sh ~/.local/bin/ptrm
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
	rm ~/.local/bin/ptmkdir
	rm ~/.local/bin/pttouch
	rm ~/.local/bin/ptcp
	rm ~/.local/bin/ptmv
	rm ~/.local/bin/ptrm
	rm ~/.local/bin/ptsh
