#include "Shortest_Job_First.h"
#include "../Scheduler/Scheduler.h"

void SJF::AddToRDY(Process* p)
{
	RDY.enqueue(p);
	AddTimeleft(p->GetRemainingTime());
}


/**
* @brief The simulation of the processor's algorithm.
*/
void SJF::Execute()
{
	OverHeat();
	if (state == OVERHEAT)	return;

	NextState();

	Algorithm();
	AddTime();		// Adds to the processor's BUSY/IDLE time.
}


/*
* @brief Generates a random number from 1 to 100 to set the next
* state of each process in RUN state.
*/
void SJF::NextState()
{
	if (state == BUSY)
	{
		IO_process* IO = RUN->GetIORequests();

		// Checks for IO requests for the executed process by the CPU.
		if (IO && (IO + IO->i)->IO_R == RUN->GetExecutedTime())
		{
			// Sends process to the BLK list.
			(IO + IO->i)->IO_T = manager->GetTimeStep();
			time_left -= RUN->GetRemainingTime();
			manager->AddToList(manager->GetBlockList(), RUN);

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
		{
			state = BUSY;
		}
	}
}

/**
* @brief The processor's algorithm.
*/
void SJF::Algorithm()
{
	if (state == IDLE)	return;
	RUN->ExecutingProcess();
	DecTimeleft();	// Decreases the processor's time left.
	// If the Process finishes execution.
	if (!RUN->GetRemainingTime())
	{
		RUN->Terminate(manager->GetTimeStep());
		manager->AddToList(manager->GetTerminatedList(), RUN);
		if (RUN->HasChild())
			manager->CheckOrphans();
		if (!RDY.dequeue(RUN))
		{
			state = IDLE;
			RUN = nullptr;
		}
	}
}