ver=/tmp/ptsh_ver
install:
	git rev-parse --short HEAD > $(ver)
	mkdir -p ~/.local/share/ptSh
	cp src/config ~/.local/share/ptSh/config
	cp src/set_aliases.sh ~/.local/bin/ptSh_set_aliases
	cp src/ptsh.sh ~/.local/bin/ptsh
	cp LICENSE ~/.local/share/ptSh/LICENSE
	cp src/logo.txt ~/.local/share/ptSh/logo.txt
	cp src/ptLs.sh ~/.local/bin/ptls
	cp src/ptPwd.sh ~/.local/bin/ptpwd
	cp src/ptMkdir.sh ~/.local/bin/ptmkdir
	cp src/ptTouch.sh ~/.local/bin/pttouch
	mkdir -p ~/.config
	mkdir -p ~/.config/ptSh
	cp src/config ~/.config/ptSh/config
	echo "Version: cloned from " | tee ~/.local/share/ptSh/version.txt
	cat $(ver) | tee -a ~/.local/share/ptSh/version.txt
	ptsh
