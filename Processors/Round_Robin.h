#ifndef _ROUND_ROBIN_
#define _ROUND_ROBIN_

#include "Processor.h"

class RR : public Processor
{
	LinkedQueue<Process*> RDY;

public:
	~RR();
};

#endif