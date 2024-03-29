#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <arpa/inet.h>

#define IF_NAME "enp0s3"
#define MESSAGE "Ahojte vsetci na AvS!!!"

typedef struct eth_hdr 
{
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    uint16_t ethertype;
    char payload[100];

    
}__attribute__((packed)) ETH_HDR;

int main(void) 
{
    int sock;
    struct sockaddr_ll addr;
    ETH_HDR frame;
    if ((sock = socket(AF_PACKET, SOCK_RAW, 0)) == -1)
    {
        perror("SOCKET ");
        exit(EXIT_FAILURE);
    }
    
    bzero(&addr, sizeof(addr));
    
    addr.sll_family = AF_PACKET;
    addr.sll_protocol = htons(ETH_P_ALL);
    addr.sll_ifindex = (int)if_nametoindex(IF_NAME);
    if (addr.sll_ifindex == 0) 
    {
        close(sock);
        perror("if_nametoindex");
        exit(EXIT_FAILURE);
    }
    

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        close(sock);
        perror("BIND ");
        exit(EXIT_FAILURE);
    }
    
    bzero(&frame, sizeof(frame));

    frame.dst_mac[0] = 0x01;
    frame.dst_mac[1] = 0x02;
    frame.dst_mac[2] = 0x03;
    frame.dst_mac[3] = 0x04;
    frame.dst_mac[4] = 0x05;
    frame.dst_mac[5] = 0x06; 

    frame.src_mac[0] = 0xAA;

    frame.ethertype = htons(0xACDC);
    strcpy(frame.payload, MESSAGE);

    if(write(sock, &frame, sizeof(frame)) < sizeof(frame)) 
    {
        printf("ERROR: Did not send whole frame.");
    }

    close(sock);
    return EXIT_SUCCESS;
}