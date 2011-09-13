include ../linux.defines

#
## Makefile for misc library.
#
#

LIBMISC = libj.a
DYNALIB = libj.so
OBJS    = wrapper.o error.o datetime.o rio.o sockets.o concurrent.o ptrace.o

all: ${DYNALIB} ${LIBMISC}

${DYNALIB}: ${OBJS}
	$(LINK.c) -shared -o ${DYNALIB} $^

${LIBMISC}: ${OBJS}
	${AR} ruv ${LIBMISC} $^
	${RANLIB} ${LIBMISC}

%.o: %.c
	$(COMPILE.c) -fPIC -o $@ $<

-include $(OBJS:.o=.d)
%.d: %.c
	@set -e; rm -f $@; \
		$(COMPILE.c) -MM $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$


.PHONY: clean
clean:
	rm -f ${LIBMISC} ${OBJS} ${TEMPFILES}
