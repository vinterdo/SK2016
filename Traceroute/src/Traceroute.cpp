
#include "Traceroute.h"

Traceroute::Traceroute() {
  maxhops = 30;

  packets = new ICMPPacket[maxhops + 1]();
  pid = getpid();
  bufferPtr = buffer;
}

Traceroute::~Traceroute() {
}

void
Traceroute::sendPackets (int sockfd, int ttl, timeval timers[3],
			 sockaddr_in& remote_address)
{
  for (int i = 0; i < 3; ++i)
    {
      timeval timer;
      buildAndSendICMPPacket (remote_address, sockfd, ttl);
      gettimeofday (&timer, NULL);
      timers[i] = timer;
    }
}

int
Traceroute::processPacket (int sockfd, int ttl, timeval timers[3], int i)
{
  while (true)
    {
      sockaddr_in clientAddress;
      socklen_t len = sizeof(clientAddress);
      fd_set descriptors;
      FD_ZERO(&descriptors);
      FD_SET(sockfd, &descriptors);
      timeval timeoutValue;
      timeoutValue.tv_sec = 1;
      timeoutValue.tv_usec = 0;
      int ready = SocketHelper::Select (sockfd + 1, &descriptors, NULL, NULL, &timeoutValue);

      if (ready)
	{
	  int n = SocketHelper::Recvfrom (sockfd, buffer, IP_MAXPACKET + 1, 0,
					  &clientAddress, &len);
	  if (n == 0)
	    break;

	  bufferPtr = buffer;
	  char ipAddress[20];
	  ip* ipPacket = (ip*) ((bufferPtr));
	  bufferPtr += ipPacket->ip_hl * 4;
	  icmp* icmpPacket = (icmp*) ((bufferPtr));
	  bufferPtr += ICMP_HEADER_SIZE;
	  if (icmpPacket->icmp_type == ICMP_TIME_EXCEEDED && icmpPacket->icmp_code == ICMP_EXC_TTL)
	    {
	      ip* packetOrigin = (ip*) ((bufferPtr));
	      bufferPtr += packetOrigin->ip_hl * 4;
	      if (packetOrigin->ip_p == IPPROTO_ICMP)
		{
		  icmpPacket = (icmp*) ((bufferPtr));
		  char ip_address[20];
		  inet_ntop (AF_INET, &clientAddress.sin_addr, ip_address, sizeof(ip_address));
		  if (icmpPacket->icmp_id == pid)
		    {
		      if (ttl == icmpPacket->icmp_seq)
			{
			  parsePacket(icmpPacket->icmp_seq, ip_address, timers, i);
			  ICMPPacket pac = packets[icmpPacket->icmp_seq];
			  if (pac.allPacketsCame ())
			    {
			      pac.print ();
			      break;
			    }
			}
		      i++;
		    }
		  else
		    {
		      continue;
		    }
		}
	    }
	  else if (icmpPacket->icmp_type == ICMP_ECHOREPLY && icmpPacket->icmp_id == pid)
	    {
	      inet_ntop (AF_INET, &clientAddress.sin_addr, ipAddress,
			 sizeof(ipAddress));
	      ip* packet_orig = (ip*) ((bufferPtr));
	      bufferPtr += packet_orig->ip_hl * 4;
	      parsePacket(icmpPacket->icmp_seq, ipAddress, timers, i);
	      ICMPPacket pac = packets[icmpPacket->icmp_seq];
	      if (pac.allPacketsCame ())
		{
		  pac.print ();
		  std::exit(0);
		}
	      i++;
	    }
	}
      else
	{
	  if (packets[ttl].anyPacketCame ())
	    {
	      packets[ttl].print ();
	      break;
	    }
	  std::cout << "ttl : " << ttl<< " * " << std::endl;
	  break;
	}
    }
  return i;
}

void Traceroute::runTraceroute(char** argv) {
	int sockfd = SocketHelper::Socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	sockaddr_in remote_address;
	bzero(&remote_address, sizeof(remote_address));
	remote_address.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &remote_address.sin_addr);
	for (int i = 0; i < maxhops; ++i) {
		packets[i] = ICMPPacket(i);
	}
	int ttl = 1;
	while (ttl <= maxhops)
	{
		timeval timers[3];
		sendPackets (sockfd, ttl, timers, remote_address);
		int i = 0;
		i = processPacket (sockfd, ttl, timers, i);
		ttl++;
	}
}

void Traceroute::buildAndSendICMPPacket(sockaddr_in &remote_address, int sockfd, int ttl)
{
	icmp icmp_packet;
	icmp_packet.icmp_type = ICMP_ECHO;
	icmp_packet.icmp_code = 0;
	icmp_packet.icmp_id = pid;
	icmp_packet.icmp_seq = ttl;
	icmp_packet.icmp_cksum = 0;
	icmp_packet.icmp_cksum = checksum((short int*)&icmp_packet, 8, 0);
	SocketHelper::Setsockopt (sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));
	SocketHelper::Sendto(sockfd, &icmp_packet, ICMP_HEADER_SIZE, 0, &remote_address, sizeof(remote_address));
}
void Traceroute::parsePacket(int seq,string ip_address,timeval* timers,int i)
{
	ICMPPacket pac = packets[seq];
	pac.setTargetIP(ip_address);
	timeval tim;
	gettimeofday(&tim,NULL);
	double elapsedTime = (tim.tv_sec - timers[i].tv_sec) * 1000.0;
	elapsedTime += (tim.tv_usec - timers[i].tv_usec) / 1000.0;
	pac.onNewPacketCame(elapsedTime);
	packets[seq] = pac;
}
