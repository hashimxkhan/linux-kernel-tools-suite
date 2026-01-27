#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Map TCP state code to string */
const char *tcp_state(unsigned int state) {
    switch (state) {
        case 1:  return "ESTABLISHED";
        case 2:  return "SYN_SENT";
        case 3:  return "SYN_RECV";
        case 4:  return "FIN_WAIT1";
        case 5:  return "FIN_WAIT2";
        case 6:  return "TIME_WAIT";
        case 7:  return "CLOSE";
        case 8:  return "CLOSE_WAIT";
        case 9:  return "LAST_ACK";
        case 10: return "LISTEN";
        case 11: return "CLOSING";
        default: return "UNKNOWN";
    }
}

/* Convert hex IP (little-endian) to dotted decimal */
void print_ip(unsigned int hex) {
    printf("%u.%u.%u.%u",
           hex & 0xFF,
           (hex >> 8) & 0xFF,
           (hex >> 16) & 0xFF,
           (hex >> 24) & 0xFF);
}

int main(void) {
    FILE *fp = fopen("/proc/net/tcp", "r");
    if (!fp) {
        perror("fopen /proc/net/tcp");
        return 1;
    }

    char line[512];
    fgets(line, sizeof(line), fp);
    printf("%-22s %-22s %s\n", "LOCAL", "REMOTE", "STATE");

    while (fgets(line, sizeof(line), fp)) {
        unsigned int local_ip, local_port;
        unsigned int remote_ip, remote_port;
        unsigned int state;
        sscanf(line,
               "%*d: %8X:%4X %8X:%4X %2X",
               &local_ip, &local_port,
               &remote_ip, &remote_port,
               &state);

        print_ip(local_ip);
        printf(":%u", local_port);

        printf(" -> ");

        print_ip(remote_ip);
        printf(":%u", remote_port);

        printf(" %-12s\n", tcp_state(state));
    }

    fclose(fp);
    return 0;
}

