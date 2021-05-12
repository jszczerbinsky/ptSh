ver=/tmp/ptsh_ver
install:
	git rev-parse --short HEAD > $(ver)
	sudo mkdir -p ~/.local/share/ptSh
	sudo cp src/config ~/.local/share/ptSh/config
	sudo cp src/set_aliases.sh ~/.local/bin/ptSh_set_aliases
	sudo cp src/ptsh.sh ~/.local/bin/ptsh
	sudo cp LICENSE ~/.local/share/ptSh/LICENSE
	sudo cp src/logo.txt ~/.local/share/ptSh/logo.txt
	sudo cp src/ptLs.sh ~/.local/bin/ptls
	sudo cp src/ptPwd.sh ~/.local/bin/ptpwd
	sudo cp src/ptMkdir.sh ~/.local/bin/ptmkdir
	sudo cp src/ptTouch.sh ~/.local/bin/pttouch
	mkdir -p ~/.config
	mkdir -p ~/.config/ptSh
	cp src/config ~/.config/ptSh/config
	echo "Version: cloned from " | sudo tee ~/.local/share/ptSh/version.txt
	cat $(ver) | sudo tee -a ~/.local/share/ptSh/version.txt
	ptsh
