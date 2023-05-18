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
	int Time_slice;

public:
	// Setters.
	virtual void AddToRDY(Process* p);
	void SetTimeSlice(int TS) { Time_slice = TS; }


	//Getters.
	LinkedQueue<Process*>* GetRDY() { return &RDY; }
	int GetTimeSliceLeft() const { return Time_slice; }

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

	/**
	* @brief The processor dequeues a process from its RDY list or enqueues it
	* depending on the mode.
	*
	* @param process - Reference to a pointer to the process.
	* @param mode - The processor acts as the donor if 0, acts as the receiver otherwise.
	*
	* @returns True on success, false otherwise.
	*/
	virtual bool Work_Stealing(Process*& process, int mode);

	/**
	* @brief Moves all the processes to another processor's list when overheated.
	*/
	virtual void EmptyProcessor();
};

#endif