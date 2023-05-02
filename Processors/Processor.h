#ifndef _PROCESSOR_
#define _PROCESSOR_

#include "../DEFS.h"
#include "../Data Structures/DS.h"
#include "../Process/Process.h"

class Scheduler; //forward class declaration

/**
* @class Processor
* 
* @brief The parent class of the processors which contains the processors data and analytics.
*/
class Processor
{
protected:
	Scheduler* manager;
	unsigned int time_left, time_busy, time_idle, total_TRT;
	Process* RUN;
	ProcessorState state;

public:

	/**
	* @brief Processor class constructor.
	*/
	Processor();
	virtual ~Processor();

	/*
	* @brief Sets a pointer to the Scheduler Manager to call its functions.
	* 
	* @param Pointer to the Scheduler Manager.
	*/
	void SetScheduler(Scheduler*);

	virtual void AddToRDY(Process* p) = 0;

	// Getters
	unsigned int GetPload() const { return time_busy / total_TRT; }
	unsigned int GetPUtil() const { return time_busy / (time_busy + time_idle); }
	unsigned int GetTimeLeft() const { return time_left; }
	Process* GetRun() { return RUN; }

	// Setters

	/*
	* @brief  Adds to the total time left for the processor's queue.
	* 
	* @parame time - Time of the remaining CT of the process.
	*/
	void AddTimeleft(int time);

	/*
	* @brief  Adds to the busy/idle total time depending on the
	* processor's state.
	*/
	void AddTime();

	/*
	* @brief  Decrease from the total time left for the processor's
	* queue each timestep.
	*/
	void DecTimeleft() { (time_left > 0 && state == BUSY) ? time_left-- : 0; };

	virtual void Algorithm() = 0;
	virtual void Execute() = 0;
};

#endif