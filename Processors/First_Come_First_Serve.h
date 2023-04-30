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
	* @brief Handles process migration to suitable processors for more
	* time efficiency.
	*/
	void Migrate();

	/**
	* @brief Handles the Forking possibility for the current process in RUN state.
	* 
	* @details A process can fork another process and add it to the shortest RDY queues
	* of FCFS processors.
	*/
	void Fork();

	/**
	* @brief The processor's algorithm.
	*/
	virtual void Algorithm();

	/**
	* @brief The simulation of the processor's algorithm.
	*/
	virtual void Execute();

	/**
	* @breif Checks its RUN process and RDY list for the SEGKILL order.
	* 
	* @param ID - Process's ID to kill.
	* 
	* @returns True if the process is found, False otherwise.
	*/
	bool CheckSIGKILL(int ID);

	/**
	* @breif Terminates the RUN process.
	*/
	void TerminateRUN();
};

#endif