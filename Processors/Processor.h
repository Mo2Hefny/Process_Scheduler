#ifndef _PROCESSOR_
#define _PROCESSOR_

#include "../DEFS.h"
#include "../Data Structures/DS.h"
#include "../Process/Process.h"

class Scheduler; //forward class declaration

class Processor
{
protected:
	Scheduler* manager;
	unsigned int time_left, time_busy, time_idle, total_TRT;
	Process* RUN;
	ProcessorState state;

public:
	Processor();

	// Setters
	void SetScheduler(Scheduler*);
	virtual void AddToRDY(Process* p) = 0;

	// Getters
	unsigned int GetPload() const { return time_busy / total_TRT; }
	unsigned int GetPUtil() const { return time_busy / (time_busy + time_idle); }
	unsigned int GetTimeLeft() const { return time_left; }

	void AddTime();
	void NextState();
	virtual void Execute() = 0;
};

#endif