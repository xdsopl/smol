CFLAGS = -std=c99 -W -Wall -Ofast
# CFLAGS += -g -fsanitize=address

test: smol
	./smol e < rick.txt | ./smol d | diff -q -s - rick.txt

clean:
	rm -f smol

