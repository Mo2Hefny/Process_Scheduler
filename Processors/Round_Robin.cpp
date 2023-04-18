#include "Round_Robin.h"
#include "../Scheduler/Scheduler.h"

RR::~RR()
{

}

void RR::Execute()
{
		NextState();
}


/*
* NextState - Generate a random number from 1 to 100 to set the next
* state of each process in RUN state.
*/
void RR::NextState()
{
	if (state == BUSY && RUN->GetTransitionTime() != manager->GetTimeStep())
	{
		int num = rand() % 100 + 1;
		if (num <= 15)
		{
			manager->AddToList(manager->GetBlockList(), RUN);
			if (!RDY.dequeue(RUN))
			{
				state = IDLE;
				RUN = nullptr;
			}
		}
	    else if (num >= 20 && num <= 30)
		{
			AddToRDY(RUN);
			if (!RDY.dequeue(RUN))
			{
				state = IDLE;
				RUN = nullptr;
			}
		}
		else if (num >= 50 && num <= 60)
		{
			manager->AddToList(manager->GetTerminatedList(), RUN);
			if (!RDY.dequeue(RUN))
			{
				state = IDLE;
				RUN = nullptr;
			}
		}
    }
	else
	{
		Process* process;

		// Process has already transitioned in this timestep.
		if (RDY.peek(process) && process->GetTransitionTime() == manager->GetTimeStep())
			return;

		if (RDY.dequeue(RUN))
			state = BUSY;
	}
}