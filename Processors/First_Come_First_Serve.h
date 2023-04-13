#ifndef _FIRST_COME_FIRST_SERVE_
#define _FIRST_COME_FIRST_SERVE_

#include "Processor.h"

class FCFS : public Processor
{
	LinkedQueue<Process*> RDY;
	
public:
	~FCFS();
};

#endif