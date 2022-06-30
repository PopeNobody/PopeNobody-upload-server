MAKEFLAGS:=-r

all: recv send dump


libsimp.a: checkret.o fixed_buf.o md5.o
	ar -r $@ $^

%: %.o libsimp.a -ggdb3 -lsimp -L.
	g++ -o $@ $< 


md5.o: md5.c md5.h
	gcc -o $@ $< -c -ggdb3 -I .

%.o: %.cc checkret.hh
	g++ -o $@ $< -c -ggdb3

clean:
	rm -f *.o *.a recv send
