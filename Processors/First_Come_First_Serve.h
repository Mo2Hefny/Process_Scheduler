#ifndef _FIRST_COME_FIRST_SERVE_
#define _FIRST_COME_FIRST_SERVE_

#include "../DEFS.h"
#include "../Data Structures/Queue/LinkedQueue.h"
#include "../Process/Process.h"
class FCFS : public LinkedQueue<Process*>
{
	LinkedQueue<Process*> RDY;
	LinkedQueue<Process*> BLK;
	unsigned int time_left;
};

#endif