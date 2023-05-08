#include "Processor.h"
#include "../Scheduler/Scheduler.h"
#include <cstdlib>
#include <ctime>


/**
* @brief Processor class constructor.
*/
Processor::Processor()
{
	time_left = time_busy = time_idle = total_TRT = 0;
	state = IDLE;
	RUN = NULL;
	cooldown = 0;
}

Processor::~Processor()
{

}

/**
* @brief Sets a pointer to the Scheduler Manager to call its functions.
* 
* @param m - Pointer to the Scheduler Manager.
*/
void Processor::SetScheduler(Scheduler* m)
{
	manager = m;
}

/**
* @brief  Adds to the busy/idle total time depending on the
* processor's state.
*/
void Processor::AddTime()
{
	if (state == BUSY)
		time_busy++;
	else
		time_idle++;
}

/**
* @brief  Adds to the total time left for the processor's queue.
* 
* @parame time - Time of the remaining CT of the process.
*/
void Processor::AddTimeleft(int time)
{
	time_left += time;
}

/**
* @brief Generates a random number to check if the processor overheats.
*/
void Processor::OverHeat()
{
	if (state == OVERHEAT)
	{
		cooldown--;
		if (!cooldown)
			state = IDLE;
	}
	else
	{
		int random_overheat = rand() % 100 + 1;
		if (random_overheat <= manager->GetProcessorsInfo().Heat_prob)
		{
			state = OVERHEAT;
			cooldown = manager->GetProcessorsInfo().cooldown;
			if (RUN)
			{
				AddTimeleft(-(RUN->GetRemainingTime()));
				manager->AddToReady(RUN);
				RUN = nullptr;
			}
		}
	}
}