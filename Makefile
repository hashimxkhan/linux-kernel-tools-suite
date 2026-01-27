CC = gcc
CFLAGS = -Wall -Wextra -O2
all: mini_ps mini_top mini_netstat

mini_ps: mini_ps.c
	$(CC) $(CFLAGS) -o mini_ps mini_ps.c

mini_top: mini_top.c
	$(CC) $(CFLAGS) -o mini_top mini_top.c

mini_netstat: mini_netstat.c
	$(CC) $(CFLAGS) -o mini_netstat mini_netstat.c

clean:
	rm -f mini_ps mini_top mini_netstat

