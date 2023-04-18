#ifndef _ROUND_ROBIN_
#define _ROUND_ROBIN_

#include "Processor.h"

/**
* @class RR
* 
* @brief Processor working with Round Robin algorithm.
*/
class RR : public Processor
{
	LinkedQueue<Process*> RDY;

public:
	// Setters.
	virtual void AddToRDY(Process* p) { RDY.enqueue(p); }

	//Getters.
	LinkedQueue<Process*>* GetRDY() { return &RDY; }

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