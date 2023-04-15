#ifndef _FIRST_COME_FIRST_SERVE_
#define _FIRST_COME_FIRST_SERVE_

#include "Processor.h"

class FCFS : public Processor
{
	LinkedList<Process*> RDY;
	
public:
	~FCFS();
	// Getters.
	LinkedList<Process*>* GetRDY() { return &RDY; }

	// Setters.
	virtual void AddToRDY(Process* p) { RDY.enqueue(p); }

	virtual void NextState();
	virtual void Execute();
};

#endif