#include "Scheduler.h"

Scheduler::Scheduler()
{
	timestep = 0;
	console = new UI(this);
	console->LoadFile(P_info, New_List);
}

Scheduler::~Scheduler()
{

	delete console;
}