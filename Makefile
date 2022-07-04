MAKEFLAGS:=-rR

LD=g++
CXX=g++

all: recv send dump 

CPPFLAGS := -ggdb3 -I . -I .
export CPPFLAGS
export LDFLAGS
export CFLAGS

ifneq ($(CPPFLAGS),$(XPPFLAGS))
$(shell rm -f cppflags)
endif

LDFLAGS:=-lsimp -ggdb3 -L .
ifneq ($(LDFLAGS),$(shell cat ldflags))
$(shell rm -f ldflags)
endif

CFLAGS:= -ggdb3 -I .
ifneq ($(CFLAGS),$(shell cat cflags))
$(shell rm -f cflags)
endif

libsimp.a(%):%
	ar -r $@ $^

cppflags:
	bash flags.sh $@ $(CPPFLAGS)

cflags:
	bash flags.sh cflags $(CFLAGS)

ldflags:
	bash flags.sh ldflags $(LDFLAGS)


libsimp.a: libsimp.a(checkret.o) libsimp.a(fixed_buf.o) libsimp.a(md5.o)
.PRECIOUS: $(patsubst %.cc,%.o,$(wildcard *.cc *c))

recv send dump: %: %.o libsimp.a ldflags
	g++ -o $@ $< $$(cat ldflags)


md5.o: md5.c md5.h config.h
	gcc -o $@ $< -c $$(cat cflags)

%.o: %.cc checkret.hh fixed_buf.hh cppflags
	g++ -o $@ $< -c $$(cat cppflags)


clean:
	rm -f *.o *.a recv send
