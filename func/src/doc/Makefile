#

install=/usr/bin/install

prefix = /home/leechuck
mandir = ${prefix}/share/man
RELEASE = 0.4.8

TARGETS = func.1 func-refin.1 func.ps func-refin.ps func.html func-refin.html 

all: $(TARGETS)

%.1:	%.1.in
	sed -e 's/--RELEASE--/0.4.8/g' -e 's|--prefix--|/home/leechuck|g' < $< > $@

%.ps:	%.1
	-groff -Tps -man $< > $@

%.html:	%.1
	-groff -Thtml -man $< > $@

clean:
	-rm -f $(TARGETS)

dist-clean: clean
	rm -f Makefile

install:
	-$(install) -d $(prefix)/doc/func-$(RELEASE)/
	-$(install) -m 644 func.ps func-refin.ps func.html func-refin.html $(prefix)/doc/func-$(RELEASE)/
	-$(install) -m 644 func.1 $(mandir)/man1/
	-$(install) -m 644 func-refin.1 $(mandir)/man1/
	-$(install) -m 644 ../../README $(prefix)/doc/func-$(RELEASE)/
	-$(install) -m 644 ../../INSTALL $(prefix)/doc/func-$(RELEASE)/
	-$(install) -m 644 ../../COPYING $(prefix)/doc/func-$(RELEASE)/
	-$(install) -m 644 ../../AUTHORS $(prefix)/doc/func-$(RELEASE)/

