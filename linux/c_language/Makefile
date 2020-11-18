include Makefile.in

SUBDIRS = \
	./pthread/pthread_create \
	./pthread/pthread_exit \
	./pthread/multi_pthread \
	./pthread/pthread_attribute \
	./pthread/pthread_mutex \
	./pthread/pthread_condition \
	./signal/signal_install_handler \
	./signal/signal_send_by_kill \
	./signal/signal_send_by_raise \
	./signal/signal_set_mask \
	./signal/sigaction_install_handler \
	./timer/setitimer \
	./timer/alarm \
	./timer/posix_timer_signal \
	./timer/posix_timer_abstime \
	./timer/posix_timer_thread \
	./timer/nanosleep \
	./timer/clock_nanosleep \
	./message_queue/message_queue_create \
	./message_queue/message_queue_create_private \
	./message_queue/message_queue_msqid_ds \
	./message_queue/message_queue_one_write_multi_read \
	./message_queue/message_queue_priority \
	./message_queue/message_queue_snd_rcv \
	./message_queue/posix_message_queue_attribute \
	./message_queue/posix_message_queue_notify_signal \
	./message_queue/posix_message_queue_notify_thread \
	./message_queue/posix_message_queue_priority \
	./message_queue/posix_message_queue_rw \
	./daemon/daemon_posix_message_queue \
	./syslog/syslog

.PHONY: all

all:
	$(info compile packages)
	@for dir in $(SUBDIRS); \
	do \
		$(MAKE) -C $$dir; \
	done

.PHONY: strip

strip:
	$(info strip packages)
	@for dir in $(SUBDIRS); \
	do \
		$(MAKE) -C $$dir strip; \
	done

.PHONY: libs

libs:
	$(info make libraries)
	@for dir in $(SUBDIRS); \
	do \
		$(MAKE) -C $$dir libs; \
	done


.PHONY: install

install:
	$(info install packages)
	@for dir in $(SUBDIRS); \
	do \
		$(MAKE) -C $$dir install; \
	done

.PHONY: uninstall

uninstall:
	$(info uninstall packages)
	@for dir in $(SUBDIRS); \
	do \
		$(MAKE) -C $$dir uninstall; \
	done

.PHONY: clean

clean:
	$(info clean packages)
	@for dir in $(SUBDIRS); \
	do \
		$(MAKE) -C $$dir clean; \
	done
	@rm -rf $(INSTALLDIR)
