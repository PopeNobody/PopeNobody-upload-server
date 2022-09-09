MAKEFLAGS:=-rR

LD=g++
CXX=g++

exes:= one_upload old-client old-server fd-client fd-server 
all: $(exes)

CPPFLAGS :=
LDFLAGS :=
CFLAGS :=

libsimp.a: checkret.o fixed_buf.o md5.o fd-path.o md5.o
	ar -r $@ $?

.PRECIOUS: $(patsubst %.cc,%.o,$(wildcard *.cc *c))

$(exes): %: %.o libsimp.a ldflags
	g++ -o $@ $< $(shell cat ldflags)


%.i: %.cc checkret.hh fixed_buf.hh cppflags cflags
	g++ -o $@ $< -E $(shell cat cppflags)


%.o: %.cc checkret.hh fixed_buf.hh cppflags cflags
	g++ -o $@ $< -c $(shell cat cppflags)


clean:
	rm -f *.o *.a $(exes) log

include /dev/null $(wildcard *.d)
