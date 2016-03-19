#ifndef __ICMP_H
#define __ICMP_H

#define ICMP_HEADER_SIZE 8

short checksum( short *addr, register int len, short csum);

#endif
