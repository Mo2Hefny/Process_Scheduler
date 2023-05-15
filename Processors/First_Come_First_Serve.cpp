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
* @brief Terminates the RUN process.
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
	OverHeat();
	if (state != OVERHEAT)
	{
		CheckSIGKILL();

		NextState();
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
	if (!manager->GetProcessorsInfo().NR) return;
	while (RUN && !RUN->HasParent() && RUN->GetCurrWaitingTime(manager->GetTimeStep()) > manager->GetProcessorsInfo().MaxW)
	{
		if (!manager->AddToRR(RUN)) break;
		AddTimeleft(-(RUN->GetRemainingTime()));
		manager->Increment_FCFSmigration();
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
	new_P_data.AT = new_P_data.RT = manager->GetTimeStep();
	new_P_data.CT = RUN->GetRemainingTime();
	new_P_data.PID = Process::GetForkPID();
	Process* New_Process = new Process(new_P_data, NULL);

	if (!RUN->ForkChild(New_Process))
	{
		delete New_Process;
	}
	else
	{
		manager->IncrementProcessNum();
		manager->IncrementFork();
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
		manager->IncrementKill();
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
		manager->IncrementKill();
		FCFS::KILL_orders.dequeue(latest_order);
		delete latest_order;
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
bool FCFS::Work_Stealing(Process*& process, int mode)
{
	if (!mode)
	{
		for (int i = 0; i < RDY.size(); i++)
		{
			RDY.GetPos(process, i);
			if (!process->HasParent())
			{
				RDY.DeletePos(process, i);
				time_left -= process->GetRemainingTime();
				return true;
			}
		}
		return false;
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
void FCFS::EmptyProcessor()
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
		if (process->HasParent())
		{
			if (manager->GetProcessorsInfo().NF > 1)
				manager->AddToFCFS(process);
			else
			{
				process->Terminate(manager->GetTimeStep());
				manager->AddToList(manager->GetTerminatedList(), process);
			}
		}
		else
			manager->AddToReady(process);
	}
}