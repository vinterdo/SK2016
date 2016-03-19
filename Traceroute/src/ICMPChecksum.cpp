#include <netinet/ip_icmp.h>

#include "ICMPChecksum.h"

short checksum( short *addr, register int len, short csum)
{
    register int nleft = len; 
     short *w = addr;
    register short answer;
    register int sum = csum;

    while (nleft > 1)  {
        sum += *w++;
        nleft -= 2;
    }

    if (nleft == 1)
        sum += htons(*(char *)w << 8);

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;
    return (answer);
}
