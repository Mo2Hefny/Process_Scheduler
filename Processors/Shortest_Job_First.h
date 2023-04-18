#ifndef _SHORTEST_JOB_FIRST_
#define _SHORTEST_JOB_FIRST_

#include "Processor.h"

/**
* @class SJF
*
* @brief Processor working with Shortest Job First algorithm.
*/
class SJF : public Processor
{
	PriorityQueue<Process*> RDY;

public:
	// Setters.
	virtual void AddToRDY(Process* p) { RDY.enqueue(p); }

	//Getters.
	PriorityQueue<Process*>* GetRDY() { return &RDY; }

	/*
	* @brief Generates a random number from 1 to 100 to set the next
	* state of each process in RUN state.
	*/
	virtual void NextState();

	/**
	* @brief The simulation of the processor's algorithm.
	*/
	virtual void Execute();
};

#endif