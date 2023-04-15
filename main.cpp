#include "Scheduler/Scheduler.h"


int main()
{
	Scheduler* schedule_manager = new Scheduler;

	if (schedule_manager)
		delete schedule_manager;

	return 0;
}