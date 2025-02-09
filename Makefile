CFLAGS = -std=c99 -W -Wall -Ofast
# CFLAGS += -g -fsanitize=address
LDFLAGS = -ldivsufsort

test: smol
	./smol e < rick.txt | ./smol d | diff -q -s - rick.txt

%: %.c *.h
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

clean:
	rm -f smol

