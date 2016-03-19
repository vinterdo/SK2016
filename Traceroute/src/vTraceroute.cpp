
#include "Traceroute.h"

int main (int argc, char** argv)
{
	if (argc != 2)
	  {
	    std::cout << "missing ip address" << std::endl;
	    return 0;
	  }

	Traceroute t;
	t.runTraceroute(argv);
	return 0;
}

