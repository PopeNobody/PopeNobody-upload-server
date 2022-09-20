MAKEFLAGS:=-rR -j

LD=g++
CXX=g++


CPPFLAGS := -ggdb3 -I . -I .
export CPPFLAGS
export LDFLAGS
export CFLAGS

allsrc:=$(wildcard *.cc *.c)

exeexe:= recv send dump one_upload
exesrc:= $(patsubst %,%.cc,$(exeexe))
exeobj:= $(patsubst %.cc,%.oo,$(exesrc))
execpp:= $(patsubst %.cc,%.ii,$(exesrc))
exeall:= $(exesrc) $(exeobj) $(execpp)

libsrc:= $(filter-out $(exesrc), $(allsrc))
libobj:= $(patsubst %.c,%.o,$(patsubst %.cc,%.oo,$(libsrc)))
libcpp:= $(patsubst %.cc,%.ii,$(libsrc))
liball:= $(libsrc) $(libobj) $(libcpp)
all: $(exeexe)

#$(warning libsrc:=$(libsrc))

libsimp.a: $(libobj)
	ar -r $@ $?

.PRECIOUS: $(liball) $(exeall)

$(exeexe): %: %.oo libsimp.a ldflags
	g++ -o $@ $< $(shell cat ldflags)


md5.o: md5.c md5.h config.h cflags
	gcc -o $@ $< -c $(shell cat cflags)

%.oo: %.ii cppflags
	g++ -o $@ $< -c $(shell cat cppflags)

%.ii: %.cc $(wildcard *.hh) cppflags
	g++ -o $@ $< -E $(shell cat cppflags)


clean:
	rm -f *.oo *.a recv send
	rm -fr log
