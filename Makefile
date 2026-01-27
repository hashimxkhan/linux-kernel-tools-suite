CC = gcc
CFLAGS = -Wall -Wextra -O2

mini_ps: mini_ps.c
	$(CC) $(CFLAGS) -o mini_ps mini_ps.c

mini_top: mini_top.c
	$(CC) $(CFLAGS) -o mini_top mini_top.c

clean:
	rm -f mini_ps mini_top

