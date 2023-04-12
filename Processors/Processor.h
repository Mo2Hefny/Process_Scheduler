#ifndef _PROCESSOR_
#define _PROCESSOR_

#include "../DEFS.h"
#include "../Data Structures/DS.h"
#include "../Process/Process.h"

class Processor
{
	LinkedQueue<Process*> BLK;
	unsigned int time_left;
};

#endif