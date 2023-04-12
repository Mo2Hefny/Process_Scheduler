#ifndef _SHORTEST_JOB_FIRST_
#define _SHORTEST_JOB_FIRST_

#include "Processor.h"

class SJF : public Processor
{
	PriorityQueue<Process*> RDY;
};

#endif