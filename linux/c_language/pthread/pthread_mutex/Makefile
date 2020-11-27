include ../../Makefile.in

EXECFILE = pthread_mutex

OBJS = $(EXECFILE).o

.PHONY: all

all: pthread_mutex

.PHONY: pthread_mutex

pthread_mutex: $(OBJS)
	@$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) $(LIBS) -o $@
	$(info package compiled)

%.o: %.c
	$(CC) -c $< $(CFLAGS) $(LDFLAGS) $(LIBS) -o $@

.PHONY: strip

strip:

.PHONY: libs

libs:

.PHONY: install

install:
	$(shell echo $(DESTDIR) > .install)
	install -d $(DESTDIR)/usr/local/sbin
	install -m 755 $(EXECFILE) $(DESTDIR)/usr/local/sbin
	$(info package installed)

.PHONY: uninstall

uninstall:
	$(eval DESTDIR := $(shell cat .install))
	@for f in $(EXECFILE); \
	do \
		rm -f $(DESTDIR)/usr/local/sbin/$$f; \
	done
	$(info package uninstalled)

.PHONY: clean

clean:
	@rm $(OBJS) $(EXECFILE)
	$(info package cleaned)
