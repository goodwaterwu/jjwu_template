include ../../Makefile.in

EXECFILE = posix_message_queue_attribute

OBJS = $(EXECFILE).o

.PHONY: all

all: posix_message_queue_attribute

.PHONY: posix_message_queue_attribute

posix_message_queue_attribute: $(OBJS)
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
		@rm -f $(DESTDIR)/usr/local/sbin/$$f; \
	done
	rm -f .install
	$(info package uninstalled)

.PHONY: clean

clean:
	@rm -f $(OBJS) $(EXECFILE)
	$(info package cleaned)
