#include "ICMPPacket.h"
#include <iostream>

bool ICMPPacket::allPacketsCame() 
{ 
    return came == 3; 
}

ICMPPacket::ICMPPacket():ip(""), time(0), ttl(0), came(0)
{
    ICMPPacket(0);
}
ICMPPacket::ICMPPacket(int t) :ip(""), time(0), ttl(t), came(0) {}

void ICMPPacket::setTargetIP(string ipaddress)
{
    if(ip.empty())
    {
        ip = ipaddress;
    }
    else if( ip != ipaddress)
    {
        ip += " " + ipaddress;
    }
}
void ICMPPacket::onNewPacketCame(double usec)
{
    came++;
    time += usec;
}
void ICMPPacket::print()
{
    std::cout << "ttl : " << ttl << " ip: " << ip.c_str();
    if(allPacketsCame())
      {
    std::cout << " time : " << time/3 << std::endl;
      }
    else
      {
	std::cout << " time : " << "*" << std::endl;
      }
}
bool ICMPPacket::anyPacketCame()
{
  return came > 0;
}
