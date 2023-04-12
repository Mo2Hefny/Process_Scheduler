#ifndef _ROUND_ROBIN_
#define _ROUND_ROBIN_

#include "Processor.h"

class RR : public Processor
{
	CircularList<Process*> RDY;
};

#endif