MAKEFLAGS:=-rR -j

LD=g++
CXX=g++

<<<<<<< HEAD
=======
exes:= upload old-client old-server fd-client fd-server 
all: $(exes)
>>>>>>> 9899ab75c0e6f772679c18c88f8a140ffb9a7eb0

CPPFLAGS :=
LDFLAGS :=
CFLAGS :=

<<<<<<< HEAD
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
=======
libsimp.a: checkret.o fixed_buf.o md5.o fd-path.o md5.o
>>>>>>> 9899ab75c0e6f772679c18c88f8a140ffb9a7eb0
	ar -r $@ $?

.PRECIOUS: $(liball) $(exeall)

<<<<<<< HEAD
$(exeexe): %: %.oo libsimp.a ldflags
=======
$(exes): %: %.o libsimp.a ldflags
>>>>>>> 9899ab75c0e6f772679c18c88f8a140ffb9a7eb0
	g++ -o $@ $< $(shell cat ldflags)


%.i: %.cc checkret.hh fixed_buf.hh cppflags cflags
	g++ -o $@ $< -E $(shell cat cppflags)

<<<<<<< HEAD
%.oo: %.ii cppflags
=======

%.o: %.cc checkret.hh fixed_buf.hh cppflags cflags
>>>>>>> 9899ab75c0e6f772679c18c88f8a140ffb9a7eb0
	g++ -o $@ $< -c $(shell cat cppflags)

%.ii: %.cc $(wildcard *.hh) cppflags
	g++ -o $@ $< -E $(shell cat cppflags)


clean:
<<<<<<< HEAD
	rm -f *.oo *.a recv send
	rm -fr log
=======
	rm -f *.o *.a $(exes) log

include /dev/null $(wildcard *.d)
>>>>>>> 9899ab75c0e6f772679c18c88f8a140ffb9a7eb0
