CC = gcc
CFLAGS = -Wall -Wextra -O2 -flto
LDFLAGS = -flto
all: mini_ps mini_top mini_netstat proc_ancestry syscall_snoop

mini_ps: mini_ps.c
	$(CC) $(CFLAGS) -o mini_ps mini_ps.c

mini_top: mini_top.c
	$(CC) $(CFLAGS) -o mini_top mini_top.c

mini_netstat: mini_netstat.c
	$(CC) $(CFLAGS) -o mini_netstat mini_netstat.c

proc_ancestry: proc_ancestry.c
	$(CC) $(CFLAGS) -o proc_ancestry proc_ancestry.c

syscall_snoop: syscall_snoop.c
	$(CC) $(CFLAGS) -o syscall_snoop syscall_snoop.c

clean:
	rm -f mini_ps mini_top mini_netstat proc_ancestry syscall_snoop

