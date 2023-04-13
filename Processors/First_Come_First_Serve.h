#ifndef _FIRST_COME_FIRST_SERVE_
#define _FIRST_COME_FIRST_SERVE_

#include "Processor.h"

class FCFS : public Processor
{
	LinkedQueue<Process*> RDY;
	
public:
	~FCFS();

	// Setters.
	virtual void AddToRDY(Process* p) { RDY.enqueue(p); }

	virtual void Execute();
};

#endif