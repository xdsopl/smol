CFLAGS = -std=c99 -W -Wall -Ofast
# CFLAGS += -g -fsanitize=address

test: smol
	./smol e < rick.txt | ./smol d | diff -q -s - rick.txt
	./smol e 11 < smol | ./smol d | diff -q -s - smol

%: %.c *.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f smol

