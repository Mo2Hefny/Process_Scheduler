#include "First_Come_First_Serve.h"
#include "../Scheduler/Scheduler.h"


void FCFS::AddToRDY(Process* p)
{
	RDY.enqueue(p);
	AddTimeleft(p->GetRemainingTime());
}


/**
* @brief The simulation of the processor's algorithm.
*/
void FCFS::Execute()
{
	NextState();

	if (state == BUSY)
	{
		DecTimeleft();	// Decreases the processor's time left.
		Algorithm();
	}
	AddTime();		// Adds to the processor's BUSY/IDLE time.
}

/*
* @brief Generates a random number from 1 to 100 to set the next
* state of each process in RUN state.
*/
void FCFS::NextState()
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
void FCFS::Algorithm()
{
	RUN->ExecutingProcess();

	// If the Process finishes execution.
	if (!RUN->GetRemainingTime())
	{
		RUN->Terminate();
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