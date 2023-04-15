#include "Shortest_Job_First.h"
#include "../Scheduler/Scheduler.h"

SJF::~SJF()
{

}

void SJF::Execute()
{
		NextState();
}


/*
* NextState - Generate a random number from 1 to 100 to set the next
* state of each process in RUN state.
*/
void SJF::NextState()
{
	if (state == BUSY)
	{
		srand(time(0));
		int num = rand() % 100 + 1;
		if (num <= 15)
		{
			manager->AddToList(manager->GetBlockList(), RUN);
			if(!RDY.dequeue(RUN))
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
		if (RDY.dequeue(RUN))
			state = BUSY;
	}
}