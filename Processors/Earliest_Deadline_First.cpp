#include "Earliest_Deadline_First.h"
#include "../Scheduler/Scheduler.h"

void EDF::AddToRDY(Process* p)
{
	RDY.enqueue(p, p->GetDeadline());
	AddTimeleft(p->GetRemainingTime());
}


/**
* @brief The simulation of the processor's algorithm.
*/
void EDF::Execute()
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
void EDF::NextState()
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
void EDF::Algorithm()
{
	if (state == IDLE)	return;
	Process* temp;
	if(RDY.peek(temp) && RUN->GetDeadline() > temp->GetDeadline())
	{
		RDY.enqueue(RUN, RUN->GetDeadline());
		RDY.dequeue(RUN);
	}
	RUN->ExecutingProcess();
	DecTimeleft();	// Decreases the processor's time left.
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

/**
* @brief The processor dequeues a process from its RDY list or enqueues it
* depending on the mode.
*
* @param process - Reference to a pointer to the process.
* @param mode - The processor acts as the donor if 0, acts as the receiver otherwise.\
*
* @returns True on success, false otherwise.
*/
bool EDF::Work_Stealing(Process*& process, int mode)
{
	if (!mode)
	{
		if (!RDY.dequeue(process)) return false;

		time_left -= process->GetRemainingTime();
	}
	else
	{
		RDY.enqueue(process, process->GetDeadline());
		time_left += process->GetRemainingTime();
	}
	return true;
}

/**
* @brief Moves all the processes to another processor's list when overheated.
*/
void EDF::EmptyProcessor()
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