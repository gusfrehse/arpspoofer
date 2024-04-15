#include <stdio.h>
#include <string.h>

#include <sys/socket.h> 
#include <net/if.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <interface>\n", argv[0]);
        return 1;
    }

    unsigned int interface_index = if_nametoindex(argv[1]);

    if (!interface_index) {
        perror("if_nametoindex");
        return 1;
    }

    printf("Interface %s is index %d.\n", argv[1], interface_index);

    struct sockaddr_ll address = {};

    int s = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ARP));

    struct ifreq req = {0};
    strncpy(req.ifr_name, argv[1], IF_NAMESIZE-1);

    if (ioctl(s, SIOCGIFHWADDR, req) == -1) {
        perror("ioctl");
        return 1;
    }

    struct sockaddr hwaddr = req.ifr_hwaddr;

    printf("hwaddr is %02x:%02x:%02x:%02x:%02x:%02x\n",
            hwaddr.sa_data[0],
            hwaddr.sa_data[1],
            hwaddr.sa_data[2],
            hwaddr.sa_data[3],
            hwaddr.sa_data[4],
            hwaddr.sa_data[5]); 
    
    return 0;
}

