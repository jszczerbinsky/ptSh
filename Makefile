ver=/tmp/ptsh_ver

all:
	@echo Run \'make install\'

install:
	git rev-parse --short HEAD > $(ver)
	mkdir -p $(DESTDIR)~/.local/share/ptSh
	cp src/config $(DESTDIR)~/.local/share/ptSh/config
	mkdir -p $(DESTDIR)~/.local/bin
	cp src/set_aliases.sh $(DESTDIR)~/.local/bin/ptSh_set_aliases
	cp LICENSE $(DESTDIR)~/.local/share/ptSh/LICENSE
	cp src/logo.txt $(DESTDIR)~/.local/share/ptSh/logo.txt
	cp src/ptLs.sh $(DESTDIR)~/.local/bin/ptls
	cp src/ptPwd.sh $(DESTDIR)~/.local/bin/ptpwd
	cp src/ptMkdir.sh $(DESTDIR)~/.local/bin/ptmkdir
	cp src/ptTouch.sh $(DESTDIR)~/.local/bin/pttouch
	cp src/ptCp.sh $(DESTDIR)~/.local/bin/ptcp
	cp src/ptsh.sh $(DESTDIR)~/.local/bin/ptsh
	mkdir -p $(DESTDIR)~/.config
	mkdir -p $(DESTDIR)~/.config/ptSh
	cp src/config $(DESTDIR)~/.config/ptSh/config
	echo "Version: cloned from " | tee $(DESTDIR)~/.local/share/ptSh/version.txt
	cat $(ver) | tee -a $(DESTDIR)~/.local/share/ptSh/version.txt
	$(DESTDIR)~/.local/bin/ptsh

uninstall:
	rm -rf $(DESTDIR)~/.local/share/ptSh
	rm -rf $(DESTDIR)~/.config/ptSh
	rm $(DESTDIR)~/.local/bin/ptSh_set_aliases
	rm $(DESTDIR)~/.local/bin/ptls
	rm $(DESTDIR)~/.local/bin/ptpwd
	rm $(DESTDIR)~/.local/bin/ptmkdir
	rm $(DESTDIR)~/.local/bin/pttouch
	rm $(DESTDIR)~/.local/bin/ptcp
	rm $(DESTDIR)~/.local/bin/ptsh
