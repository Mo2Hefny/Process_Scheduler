#ifndef _FIRST_COME_FIRST_SERVE_
#define _FIRST_COME_FIRST_SERVE_

#include "Processor.h"

/**
* @class FCFS
*
* @brief Processor working with First Come First Serve algorithm.
*/
class FCFS : public Processor
{
	LinkedList<Process*> RDY;
	
public:
	// Getters.
	LinkedList<Process*>* GetRDY() { return &RDY; }

	LinkedList<Process*>& GetRDYref() { return RDY; }
	// Setters.
	virtual void AddToRDY(Process* p);

	/*
	* @brief Generates a random number from 1 to 100 to set the next
	* state of each process in RUN state.
	*/
	virtual void NextState();

	/**
	* @brief The processor's algorithm.
	*/
	virtual void Algorithm();

	/**
	* @brief The simulation of the processor's algorithm.
	*/
	virtual void Execute();
};

#endif