CFLAGS=-Wall -fPIC -DPIC -march=native -Werror

debug: CFLAGS+=-g -Og
debug: build

release: CFLAGS+=-O2
release: build

build: limit.o libjudge.o logger.o
	gcc $(CFLAGS) -shared -Wl,-soname,libjudge.so.0 -o libjudge.so.0 limit.o libjudge.o logger.o -lc

language:
	python ../generator/lang.py

all: release

install:
	test -f libjudge.so.0
	cp libjudge.so.0 /usr/lib/
	ln -fs /usr/lib/libjudge.so.0 /usr/lib/libjudge.so
	ldconfig

uninstall:
	-rm -f /usr/lib/libjudge.so.0 /usr/lib/libjudge.so

clean:
	-rm *.o
	-rm libjudge.so.0
