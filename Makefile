all: send.ok

send.ok: send
	./send
	touch send.ok

recv.ok: recv
	./recv
	touch recv.ok

%: %.cc
	g++ -o $@ $< -static -ggdb3
