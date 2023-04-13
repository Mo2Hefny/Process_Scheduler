#include "Round_Robin.h"
#include "../Scheduler/Scheduler.h"

RR::~RR()
{

}

void RR::Execute()
{
	if (state == BUSY)
		NextState();
	

}


/*
* NextState - Generate a random number from 1 to 100 to set the next
* state of each process in RUN state.
*/
void RR::NextState()
{
	srand(time(0));
	int num = rand() % 100 + 1;
	if (num <= 15)
	{
		manager->AddToList(manager->GetBlockList(), RUN);
		RUN = NULL;
		state = IDLE;
	}
	else if (num >= 20 && num <= 30)
	{
		AddToRDY(RUN);
		RUN = NULL;
		state = IDLE;
	}
	else if (num >= 50 && num <= 60)
	{
		manager->AddToList(manager->GetTerminatedList(), RUN);
		RUN = NULL;
		state = IDLE;
	}
}