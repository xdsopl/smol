CFLAGS = -std=c99 -W -Wall -Ofast
# CFLAGS += -g -fsanitize=address

test: smol
	./smol

clean:
	rm -f smol

