#ifndef _ROUND_ROBIN_
#define _ROUND_ROBIN_

#include "../DEFS.h"
#include "../Data Structures/Queue/CircularQueue.h"
#include "../Process/Process.h"
class RR : public CircularQueue<Process*>
{
	LinkedQueue<Process*> RDY;
	LinkedQueue<Process*> BLK;
	unsigned int time_left;
};

#endif