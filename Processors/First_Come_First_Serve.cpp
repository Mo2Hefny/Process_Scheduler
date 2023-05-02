#include "First_Come_First_Serve.h"
#include "../Scheduler/Scheduler.h"

FCFS::~FCFS()
{
	CleanSIGKILL();
}

/**
* @brief Cleans up the SIGKILL orders from the memory.
*/
void FCFS::CleanSIGKILL()
{
	SIGKILL* sigkill;
	while (KILL_orders.dequeue(sigkill))
		delete sigkill;
}

// Define the static member variable KILL_orders
LinkedList<SIGKILL*> FCFS::KILL_orders;

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
	CheckSIGKILL();

	NextState();
	Migrate();
	Fork();
	Algorithm();
	
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
	{
		manager->IncrementProcessNum();
		manager->AddToFCFS(New_Process);
	}
}

/**
* @brief The processor's algorithm.
*/
void FCFS::Algorithm()
{
	if (state == IDLE) return;
	RUN->ExecutingProcess();
	DecTimeleft();	// Decreases the processor's time left.
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
*/
void FCFS::CheckSIGKILL()
{
	Process* process = NULL;
	SIGKILL* latest_order;
	int timestep = manager->GetTimeStep(), ID = -1;

	if (!FCFS::KILL_orders.peek(latest_order))	return;

	// If the processor was overheated and didn't check past SIGKILLs
	while (latest_order->time < timestep)
	{
		FCFS::KILL_orders.dequeue(latest_order);
		delete latest_order;
		if (!FCFS::KILL_orders.peek(latest_order))	return;
	}
	// The SIGKILL order's time didn't come yet.
	if (latest_order->time != timestep)
		return;

	// If the time matches then it searches for the process.
	ID = latest_order->ID;
	
	

	if (RDY.DeleteNode(process, ID))
	{
		AddTimeleft(-(process->GetRemainingTime()));
		process->Terminate(manager->GetTimeStep());
		manager->AddToList(manager->GetTerminatedList(), process);
		if (process->HasChild())
			manager->CheckOrphans();
		// Deletes order after executing it.
		FCFS::KILL_orders.dequeue(latest_order);
		delete latest_order;
		return;
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
		// Deletes order after executing it.
		FCFS::KILL_orders.dequeue(latest_order);
		delete latest_order;
	}
}