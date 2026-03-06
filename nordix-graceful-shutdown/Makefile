CC       = clang
CFLAGS   = -O3 -Wall -Wextra -march=native -mtune=native -flto=thin
PREFIX   = /usr
LIBDIR   = $(PREFIX)/lib/nordix
BINDIR   = $(PREFIX)/bin
UNITDIR  = /etc/systemd/system

DAEMON   = nordix-graceful-shutdown
CLIENT   = nordix-shutdown-client

POPUPS   = nordix-shutdown-scrub.py \
           nordix-shutdown-vm.py    \
           nordix-shutdown-download.py

.PHONY: all clean install uninstall enable disable

all: $(DAEMON) $(CLIENT)

$(DAEMON): $(DAEMON).c
	$(CC) $(CFLAGS) -o $@ $<

$(CLIENT): $(CLIENT).c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(DAEMON) $(CLIENT)

install: all
	install -Dm755 $(DAEMON)             $(DESTDIR)$(LIBDIR)/$(DAEMON)
	install -Dm755 $(CLIENT)             $(DESTDIR)$(BINDIR)/$(CLIENT)
	install -Dm755 $(POPUPS:%=$(DESTDIR)$(LIBDIR)/%) || true
	$(foreach p,$(POPUPS),install -Dm644 $(p) $(DESTDIR)$(LIBDIR)/$(p);)
	install -Dm644 $(DAEMON).service     $(DESTDIR)$(UNITDIR)/$(DAEMON).service
	install -Dm644 $(DAEMON).socket      $(DESTDIR)$(UNITDIR)/$(DAEMON).socket

uninstall:
	rm -f $(DESTDIR)$(LIBDIR)/$(DAEMON)
	rm -f $(DESTDIR)$(BINDIR)/$(CLIENT)
	$(foreach p,$(POPUPS),rm -f $(DESTDIR)$(LIBDIR)/$(p);)
	rm -f $(DESTDIR)$(UNITDIR)/$(DAEMON).service
	rm -f $(DESTDIR)$(UNITDIR)/$(DAEMON).socket
	-rmdir $(DESTDIR)$(LIBDIR) 2>/dev/null

enable:
	systemctl daemon-reload
	systemctl enable $(DAEMON).socket
	systemctl start  $(DAEMON).socket

disable:
	systemctl stop    $(DAEMON).socket
	systemctl disable $(DAEMON).socket
	systemctl daemon-reload