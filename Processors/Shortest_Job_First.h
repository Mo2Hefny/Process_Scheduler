#ifndef _SHORTEST_JOB_FIRST_
#define _SHORTEST_JOB_FIRST_

#include "Processor.h"

class SJF : public Processor
{
	PriorityQueue RDY;

public:
	~SJF();
	//Getters.
	PriorityQueue GetRDY() { return RDY; }

	// Setters.
	virtual void AddToRDY(Process* p) { RDY.enqueue(p); }

	virtual void NextState();
	virtual void Execute();
};

#endif