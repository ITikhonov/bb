
CFLAGS=-g

all: parse ok.test

ok.test: test.a i.py
	python i.py > ok.test 2> log.i < test.a || (cat log.i; rm ok.test; exit 1)

test.a: js test.js
	./js > log 2>test.a || (cat log; rm test.a; exit 1)

