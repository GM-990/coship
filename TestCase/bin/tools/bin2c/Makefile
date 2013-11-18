
# Makefile for the genromfs program.

all: bin2c

PACKAGE = bin2c
VERSION = 0.5.2
CC = gcc
CFLAGS = -O2 -Wall -DVERSION=\"$(VERSION)\"#-g#
LDFLAGS = -s#-g

DISTDIR = $(PACKAGE)-$(VERSION)

FILES = COPYING NEWS ChangeLog Makefile \

prefix = /usr
bindir = $(prefix)/bin
mandir = $(prefix)/man

bin2c: bin2c.o
	$(CC) $(LDFLAGS) bin2c.o -o bin2c
	cp bin2c $(MMCP_HOME)/bin -f

.c.o:
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm -f bin2c *.o

distclean: clean
	rm -rf $(DISTDIR) $(DISTDIR).tar.gz

dist:
	./checkdist $(VERSION)
	rm -rf $(DISTDIR).tar.gz $(DISTDIR)
	mkdir $(DISTDIR);
	for i in $(FILES); do \
		cp $$i $(DISTDIR)/; \
	done; \
	tar --owner=root --group=root -zcf $(DISTDIR).tar.gz $(DISTDIR);
	rm -rf $(DISTDIR)

install: all install-bin install-man

install-bin:
	mkdir -p $(PREFIX)$(bindir)
	install -m 755 genromfs $(PREFIX)$(bindir)/

install-man:
	# genromfs 0.5 installed the man page in this file,
	# remove it before someone notices :)
	if [ -f $(PREFIX)$(bindir)/man8 ]; then \
		rm -f $(PREFIX)$(bindir)/man8; \
	fi
	mkdir -p $(PREFIX)$(mandir)/man8
	install -m 644 genromfs.8 $(PREFIX)$(mandir)/man8/

