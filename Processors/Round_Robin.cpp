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
		Algorithm();
		DecTimeleft();	// Decreases the processor's time left.
	}

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
		if (IO && (IO + IO->i)->IO_R == RUN->GetProcessInfo().CT - RUN->GetCPUTime())
		{
			(IO + IO->i)->IO_T = manager->GetTimeStep();
			time_left -= RUN->GetCPUTime();
			manager->AddToList(manager->GetBlockList(), RUN);
			if (RDY.peek(RUN) && RUN->GetTransitionTime() != manager->GetTimeStep())
			{
				RDY.dequeue(RUN);
				RUN->SetCPUTime(RUN->GetCPUTime() - 1);
				time_left--;
			}
			else
			{
				state = IDLE;
				RUN = nullptr;
			}
		}
		else if (RUN->GetCPUTime() == 0)
		{
			AddTimeleft(-(RUN->GetRemainingTime()));
			manager->AddToList(manager->GetTerminatedList(), RUN);
			if (RDY.peek(RUN) && RUN->GetTransitionTime() != manager->GetTimeStep())
			{
				RDY.dequeue(RUN);
				RUN->SetCPUTime(RUN->GetCPUTime() - 1);
				time_left--;
			}
			else
			{
				state = IDLE;
				RUN = nullptr;
			}
		}
		else
		{
			RUN->SetCPUTime(RUN->GetCPUTime() - 1);
			time_left--;
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
	if (RUN->GetRemainingTime() < manager->GetProcessorsInfo().RTF && manager->GetProcessorsInfo().NS)
	{
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
			manager->AddToList(manager->GetTerminatedList(), RUN);
			if (!RDY.dequeue(RUN))
			{
				state = IDLE;
				RUN = nullptr;
			}
			SetTimeSlice(manager->GetProcessorsInfo().Time_slice);
		}
	}
	else
	{
		// Time Slice finished
		if (RUN)
			RDY.enqueue(RUN);

		if (!RDY.dequeue(RUN))
		{
			state = IDLE;
			RUN = nullptr;
		}
		SetTimeSlice(manager->GetProcessorsInfo().Time_slice);
	}
}