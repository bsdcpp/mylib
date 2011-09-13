include ../linux.defines

PROGS  = echoclient echoserver \
		 thread thread2 thread3 thread4 thread5 \
		 badcnt \
		 sbuf \
		 signal testc curses sigaction file hashtable hash
			
all:  ${PROGS}

-include $(addsuffix .d,${PROGS})
%.d: %.c
	@set -e; rm -f $@; \
		$(COMPILE.c) -MM $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$


#  curses: curses.o
#      $(CC) -o $@ $< -lcurses


.PHONY: clean
clean:
	rm -f ${PROGS} ${TEMPFILES}
