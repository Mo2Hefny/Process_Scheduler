#include "Scheduler.h"

Scheduler::Scheduler()
{
	timestep = 0;
	FCFS_Processors = NULL;
	SJF_Processors = NULL;
	RR_Processors = NULL;
	console = new UI(this);
	Execute();
}

Scheduler::~Scheduler()
{
	if (console)
		delete console;
	if (FCFS_Processors)
		delete[] FCFS_Processors;
	if (SJF_Processors)
		delete[] SJF_Processors;
	if (RR_Processors)
		delete[] RR_Processors;
}

void Scheduler::AddToList(LinkedQueue<Process*>* List, Process* p)
{
	List->enqueue(p);
}

void Scheduler::ReadInput()
{
	console->LoadFile(P_info);
	FCFS_Processors = new FCFS[P_info.NF];
	SJF_Processors = new SJF[P_info.NS];
	RR_Processors = new RR[P_info.NR];
}

void Scheduler::Execute()
{
	ReadInput();
}