#include "First_Come_First_Serve.h"
#include "../Scheduler/Scheduler.h"


void FCFS::AddToRDY(Process* p)
{
	RDY.enqueue(p);
	AddTimeleft(p->GetRemainingTime());
}

/**
* @breif Terminates the RUN process.
*/
void FCFS::TerminateRUN()
{
	AddTimeleft(-(RUN->GetRemainingTime()));
	RUN->Terminate(manager->GetTimeStep());
	manager->AddToList(manager->GetTerminatedList(), RUN);
	state = IDLE;
	RUN = nullptr;
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
		Migrate();
		Fork();
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
* @brief Handles process migration to suitable processors for more
* time efficiency.
*/
void FCFS::Migrate()
{
	while (RUN && RUN->GetRemainingTime() > manager->GetProcessorsInfo().MaxW && manager->GetProcessorsInfo().NR)
	{
		AddTimeleft(-(RUN->GetRemainingTime()));
		manager->AddToRR(RUN);
		if (!RDY.dequeue(RUN))
		{
			state = IDLE;
			RUN = nullptr;
		}
	}
}

/**
* @brief Handles the Forking possibility for the current process in RUN state.
*
* @details A process can fork another process and add it to the shortest RDY queues
* of FCFS processors.
*/
void FCFS::Fork()
{
	int fork_prob = rand() % 100 + 1;
	if (state == IDLE || fork_prob > manager->GetProcessorsInfo().Fork_prob)
		return;

	ProcessInfo new_P_data;
	new_P_data.AT = manager->GetTimeStep();
	new_P_data.CT = RUN->GetRemainingTime();
	new_P_data.PID = rand() % 1000;
	Process* New_Process = new Process(new_P_data, NULL);

	if (!RUN->ForkChild(New_Process))
	{
		delete New_Process;
	}
	else
		manager->AddToFCFS(New_Process);
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
		AddTimeleft(-(RUN->GetRemainingTime()));
		RUN->Terminate(manager->GetTimeStep());
		manager->AddToList(manager->GetTerminatedList(), RUN);
		if (RUN->HasChild())
		{
			RUN = nullptr;
			manager->CheckOrphans();
		}
		if (!RDY.dequeue(RUN))
		{
			state = IDLE;
			RUN = nullptr;
		}
	}
}

/**
* @brief Checks its RUN process and RDY list for the SEGKILL order.
*
* @param ID - Process's ID to kill.
* 
* @returns True if the process is found, False otherwise.
*/
bool FCFS::CheckSIGKILL(int ID)
{
	Process* process = NULL;
	if (RDY.DeleteNode(process, ID))
	{
		AddTimeleft(-(process->GetRemainingTime()));
		process->Terminate(manager->GetTimeStep());
		manager->AddToList(manager->GetTerminatedList(), process);
		if (process->HasChild())
			manager->CheckOrphans();
		return true;
	}
	if (state == BUSY && RUN->GetProcessInfo().PID == ID)
	{
		AddTimeleft(-(RUN->GetRemainingTime()));
		RUN->Terminate(manager->GetTimeStep());
		manager->AddToList(manager->GetTerminatedList(), RUN);
		if (RUN->HasChild())
		{
			RUN = nullptr;
			manager->CheckOrphans();
		}
		if (!RDY.dequeue(RUN))
		{
			state = IDLE;
			RUN = nullptr;
		}
		return true;
	}
	return false;
}