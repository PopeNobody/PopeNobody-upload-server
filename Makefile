MAKEFLAGS:=-rR

LD=g++
CXX=g++

all: recv send dump 

CPPFLAGS := -ggdb3 -I . -I .
export CPPFLAGS
export LDFLAGS
export CFLAGS

libsimp.a: checkret.o fixed_buf.o md5.o
	ar -r $@ $?

.PRECIOUS: $(patsubst %.cc,%.o,$(wildcard *.cc *c))

recv send dump: %: %.o libsimp.a ldflags
	g++ -o $@ $< $$(cat ldflags)


md5.o: md5.c md5.h config.h cflags
	gcc -o $@ $< -c $$(cat cflags)

%.o: %.cc checkret.hh fixed_buf.hh cppflags
	g++ -o $@ $< -c $$(cat cppflags)


clean:
	rm -f *.o *.a recv send
	rm -fr log
