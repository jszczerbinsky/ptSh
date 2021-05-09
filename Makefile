install:
	sudo mkdir -p /usr/share/ptSh
	sudo cp src/config /usr/share/ptSh/config
	sudo cp src/ptLs.sh /usr/bin/ptls
	sudo cp src/ptPwd.sh /usr/bin/ptpwd
	sudo cp src/ptMkdir.sh /usr/bin/ptmkdir
	mkdir -p ~/.config
	mkdir -p ~/.config/ptSh
	cp src/config ~/.config/ptSh/.config
