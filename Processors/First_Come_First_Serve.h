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
	static LinkedList<SIGKILL*> KILL_orders;	// SIGKILL orders list shared between all FCFS processors.
	LinkedList<Process*> RDY;
	
public:
	virtual ~FCFS();

	// Getters.
	LinkedList<Process*>* GetRDY() { return &RDY; }

	LinkedList<Process*>& GetRDYref() { return RDY; }
	// Setters.
	virtual void AddToRDY(Process* p);
	static LinkedList<SIGKILL*>* GetSIGKILL() { return &KILL_orders; }

	/**
	* @brief Cleans up the SIGKILL orders from the memory.
	*/
	static void CleanSIGKILL();
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
	*/
	void CheckSIGKILL();

	/**
	* @breif Terminates the RUN process.
	*/
	void TerminateRUN();
};

#endif