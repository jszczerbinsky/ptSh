install:
	sudo mkdir -p /usr/share/ptSh
	sudo cp src/config /usr/share/ptSh/config
	sudo cp src/success_failure_cmd.sh /usr/share/ptSh/success_failure_cmd.sh
	sudo cp src/set_aliases.sh /usr/bin/ptSh_set_aliases
	sudo cp src/ptLs.sh /usr/bin/ptls
	sudo cp src/ptPwd.sh /usr/bin/ptpwd
	sudo cp src/ptMkdir.sh /usr/bin/ptmkdir
	sudo cp src/ptTouch.sh /usr/bin/pttouch
	mkdir -p ~/.config
	mkdir -p ~/.config/ptSh
	cp src/config ~/.config/ptSh/config
