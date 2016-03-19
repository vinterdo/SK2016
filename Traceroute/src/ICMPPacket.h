#include <string>
using namespace std;

class ICMPPacket
{
    string ip;
    double time;
public:
    int ttl;
    int came;
    bool allPacketsCame(); 
    ICMPPacket();
    ICMPPacket(int t);
    void setTargetIP(string ipAddr);
    void onNewPacketCame(double usec);
    void print();
    bool anyPacketCame();
};
