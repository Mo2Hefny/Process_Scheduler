#include "First_Come_First_Serve.h"
#include "../Scheduler/Scheduler.h"

/**
* @brief The simulation of the processor's algorithm.
*/
void FCFS::Execute()
{
	NextState();
}

/*
* @brief Generates a random number from 1 to 100 to set the next
* state of each process in RUN state.
*/
void FCFS::NextState()
{
	if (state == BUSY && RUN->GetTransitionTime() != manager->GetTimeStep())
	{
		IO_process* IO=RUN->GetIORequests();
		if (IO&& (IO+IO->i)->IO_R == RUN->GetProcessInfo().CT - RUN->GetCPUTime())
		{
			(IO + IO->i)->IO_T = manager->GetTimeStep();
			time_left -= RUN->GetCPUTime();
			manager->AddToList(manager->GetBlockList(), RUN);
			if (RDY.peek(RUN)&& RUN->GetTransitionTime() != manager->GetTimeStep())
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
		else if (RUN->GetCPUTime()==0)
		{
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
		Process* process;

		// Process has already transitioned in this timestep.
		if (RDY.peek(process) && process->GetTransitionTime() == manager->GetTimeStep())
			return;

		if (RDY.dequeue(RUN))
		{
			state = BUSY;
			RUN->SetCPUTime(RUN->GetCPUTime()-1);
			time_left--;
		}
	}
}