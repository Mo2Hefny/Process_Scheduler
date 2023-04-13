#include "Processor.h"
#include "../Scheduler/Scheduler.h"

Processor::Processor()
{
	time_left = time_busy = time_idle = 0;
	state = IDLE;
}

/*
* SetScheduler - Sets a pointer to the Scheduler Manager to call its functions.
*/
void Processor::SetScheduler(Scheduler* m)
{
	manager = m;
}

/*
* AddTime -  adds to the busy/idle total time depending on the
* processor's state.
*/
void Processor::AddTime()
{
	if (state == BUSY)
		time_busy++;
	else
		time_idle++;
}