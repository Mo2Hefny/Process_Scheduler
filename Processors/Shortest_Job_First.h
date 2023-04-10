#ifndef _SHORTEST_JOB_FIRST_
#define _SHORTEST_JOB_FIRST_

#include "../DEFS.h"
#include "../Data Structures/Queue/PriorityQueue.h"
#include "../Process/Process.h"
class SJF : public PriorityQueue<Process*>
{
	LinkedQueue<Process*> RDY;
	LinkedQueue<Process*> BLK;
	unsigned int time_left;
};

#endif