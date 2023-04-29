#include "Round_Robin.h"
#include "../Scheduler/Scheduler.h"

void RR::AddToRDY(Process* p)
{
	RDY.enqueue(p);
	AddTimeleft(p->GetRemainingTime());
}


/**
* @brief The simulation of the processor's algorithm.
*/
void RR::Execute()
{
	NextState();

	if (state == BUSY)
	{
		DecTimeleft();	// Decreases the processor's time left.
	}
	Algorithm();

	AddTime();		// Adds to the processor's BUSY/IDLE time.
}


/*
* @brief Generates a random number from 1 to 100 to set the next
* state of each process in RUN state.
*/
void RR::NextState()
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
			state = BUSY;
	}
}

/**
* @brief The processor's algorithm.
*/
void RR::Algorithm()
{
	//Check Process Migration
	if (state == BUSY && RUN->GetRemainingTime() < manager->GetProcessorsInfo().RTF && manager->GetProcessorsInfo().NS)
	{
		AddTimeleft(-(RUN->GetRemainingTime()));
		manager->AddToSJF(RUN);
		if (!RDY.dequeue(RUN))
		{
			state = IDLE;
			RUN = nullptr;
		}
	}

	// Process in RUN state executes for the Time_slice.
	if (state == BUSY && Time_slice)
	{
		Time_slice--;
		RUN->ExecutingProcess();
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
			SetTimeSlice(manager->GetProcessorsInfo().Time_slice);
		}
	}
	
	if (!Time_slice || state == IDLE)
	{
		// Time Slice finished
		if (RUN)
			RDY.enqueue(RUN);

		if (!RDY.dequeue(RUN))
		{
			state = IDLE;
			RUN = nullptr;
		}
		else
		{
			state = BUSY;
		}
		SetTimeSlice(manager->GetProcessorsInfo().Time_slice);
	}
}