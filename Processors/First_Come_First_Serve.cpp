#include "First_Come_First_Serve.h"
#include "../Scheduler/Scheduler.h"


void FCFS::AddToRDY(Process* p)
{
	RDY.enqueue(p);
	AddTimeleft(p->GetCPUTime());
}


/**
* @brief The simulation of the processor's algorithm.
*/
void FCFS::Execute()
{
	NextState();

	AddTime();		// Adds to the processor's BUSY/IDLE time.
	DecTimeleft();	// Decreases the processor's time left.
}

/*
* @brief Generates a random number from 1 to 100 to set the next
* state of each process in RUN state.
*/
void FCFS::NextState()
{
	if (state == BUSY)
	{
		int num = rand() % 100 + 1;
		if (num <= 15)
		{
			AddTimeleft(-(RUN->GetRemainingTime()));
			manager->AddToList(manager->GetBlockList(), RUN);
			if (!RDY.dequeue(RUN))
			{
				state = IDLE;
				RUN = nullptr;
			}
		}
		else if (num >= 20 && num <= 30)
		{
			AddTimeleft(-(RUN->GetRemainingTime()));
			AddToRDY(RUN);
			if (!RDY.dequeue(RUN))
			{
				state = IDLE;
				RUN = nullptr;
			}
		}
		else if (num >= 50 && num <= 60)
		{
			AddTimeleft(-(RUN->GetRemainingTime()));
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

/**
* @brief The processor's algorithm.
*/
void FCFS::Algorithm()
{

}