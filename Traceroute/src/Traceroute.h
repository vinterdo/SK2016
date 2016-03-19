
#ifndef TRACEROUTE_H_
#define TRACEROUTE_H_

#include <string.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>

#include "ICMPChecksum.h"
#include "ICMPPacket.h"
#include "SocketHelper.h"
using namespace std;

class Traceroute
{
public:
	Traceroute();
	virtual ~Traceroute();

	void runTraceroute(char** argv);
private:
	int maxhops;
	unsigned char 	buffer[IP_MAXPACKET+1];
	unsigned char* 	bufferPtr;
	ICMPPacket* packets;
	int pid;

	void buildAndSendICMPPacket(sockaddr_in &remoteAddress, int sockfd, int ttl);
	void parsePacket(int seq,string ipAddress,timeval* timers,int i);
        void sendPackets (int sockfd, int ttl, timeval timers[3], sockaddr_in& remoteAddress);
        int processPacket (int sockfd, int ttl, timeval timers[3], int i);
        void printPacket (ICMPPacket& pac);
};


#endif /* TRACEROUTE_H_ */
