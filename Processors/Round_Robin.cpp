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
	OverHeat();
	if (state != OVERHEAT)
	{
		NextState();
		Algorithm();
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
	// Process in RUN state executes for the Time_slice.
	if (state == BUSY && Time_slice)
	{
		Time_slice--;
		RUN->ExecutingProcess();
		DecTimeleft();	// Decreases the processor's time left.
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

/**
* @brief The processor dequeues a process from its RDY list or enqueues it
* depending on the mode.
* 
* @param process - Reference to a pointer to the process.
* @param mode - The processor acts as the donor if 0, acts as the receiver otherwise.\
*
* @returns True on success, false otherwise.
*/
bool RR::Work_Stealing(Process*& process, int mode)
{
	if (!mode)
	{
		if (!RDY.dequeue(process)) return false;

		time_left -= process->GetRemainingTime();
	}
	else
	{
		RDY.enqueue(process);
		time_left += process->GetRemainingTime();
	}
	return true;
}

/**
* @brief Moves all the processes to another processor's list when overheated.
*/
void RR::EmptyProcessor()
{
	if (RUN)
	{
		AddTimeleft(-(RUN->GetRemainingTime()));
		manager->AddToReady(RUN);
		RUN = nullptr;
	}
	Process* process;
	int size = RDY.size();
	while (size--)
	{
		if (!RDY.dequeue(process)) break;
		AddTimeleft(-(process->GetRemainingTime()));
		manager->AddToReady(process);
	}
}