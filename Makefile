ver=/tmp/ptsh_ver
install:
	git rev-parse --short HEAD > $(ver)
	sudo mkdir -p /usr/local/share/ptSh
	sudo cp src/config /usr/local/share/ptSh/config
	sudo cp src/set_aliases.sh /usr/local/bin/ptSh_set_aliases
	sudo cp src/ptsh.sh /usr/local/bin/ptsh
	sudo cp LICENSE /usr/local/share/ptSh/LICENSE
	sudo cp src/logo.txt /usr/local/share/ptSh/logo.txt
	sudo cp src/ptLs.sh /usr/local/bin/ptls
	sudo cp src/ptPwd.sh /usr/local/bin/ptpwd
	sudo cp src/ptMkdir.sh /usr/local/bin/ptmkdir
	sudo cp src/ptTouch.sh /usr/local/bin/pttouch
	mkdir -p ~/.config
	mkdir -p ~/.config/ptSh
	cp src/config ~/.config/ptSh/config
	echo "Version: cloned from " | sudo tee /usr/local/share/ptSh/version.txt
	cat $(ver) | sudo tee -a /usr/local/share/ptSh/version.txt
	ptsh
