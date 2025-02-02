CFLAGS = -std=c99 -W -Wall -Ofast
# CFLAGS += -g -fsanitize=address

test: smol
	echo "Hello World!" | ./smol e | ./smol d

clean:
	rm -f smol

