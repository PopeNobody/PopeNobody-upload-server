all: recv.ok

send.ok: send
	./send
	touch send.ok

recv.ok: recv
	./recv
	touch recv.ok

libsimp.a: checkret.o
	ar -r $@ $<

%: %.cc libsimp.a
	g++ -o $@ $< -static -ggdb3 -lsimp -L.

%.o: %.cc
	g++ -o $@ $< -c -ggdb3
