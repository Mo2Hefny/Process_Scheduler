#include "First_Come_First_Serve.h"
#include "../Scheduler/Scheduler.h"
FCFS::~FCFS()
{
    // Destructor code here
}

void FCFS::Execute()
{
	NextState();
}

/*
* NextState - Generate a random number from 1 to 100 to set the next
* state of each process in RUN state.
*/
void FCFS::NextState()
{
	srand(time(0));
	if (state == BUSY)
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
		if (RDY.dequeue(RUN))
			state = BUSY;
	}
}