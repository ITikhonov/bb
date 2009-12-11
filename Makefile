
CFLAGS=-g

all: parse log.i

log.i: test.a
	python i.py > log.i

test.a: js test.js
	./js > log 2>test.a || (cat log; rm test.a; exit 1)

