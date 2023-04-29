#include "Shortest_Job_First.h"
#include "../Scheduler/Scheduler.h"

void SJF::AddToRDY(Process* p)
{
	RDY.enqueue(p);
	AddTimeleft(p->GetCPUTime());
}


/**
* @brief The simulation of the processor's algorithm.
*/
void SJF::Execute()
{
	NextState();

	AddTime();		// Adds to the processor's BUSY/IDLE time.
	DecTimeleft();	// Decreases the processor's time left.
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
		{
			state = BUSY;
			RUN->SetCPUTime(RUN->GetCPUTime() - 1);
			time_left--;
		}
	}
}

/**
* @brief The processor's algorithm.
*/
void SJF::Algorithm()
{

}