#include "Round_Robin.h"
#include "../Scheduler/Scheduler.h"

void RR::AddToRDY(Process* p)
{
	RDY.enqueue(p);
	AddTimeleft(p->GetCPUTime());
}


/**
* @brief The simulation of the processor's algorithm.
*/
void RR::Execute()
{
	NextState();

	if (state == BUSY)
		Algorithm();
	else
	{
		SetTimeSlice(manager->GetProcessorsInfo().Time_slice);
	}

	AddTime();		// Adds to the processor's BUSY/IDLE time.
	DecTimeleft();	// Decreases the processor's time left.
}


/*
* @brief Generates a random number from 1 to 100 to set the next
* state of each process in RUN state.
*/
void RR::NextState()
{
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
		Process* process;

		// Process has already transitioned in this timestep.
		if (RDY.peek(process) && process->GetTransitionTime() == manager->GetTimeStep())
			return;

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
	if (RUN->GetCPUTime() < manager->GetProcessorsInfo().RTF)
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
		if (RUN->IsFinished())
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