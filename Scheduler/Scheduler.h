#ifndef _Schedule_Manager_
#define _Schedule_Manager_

#include "../DEFS.h"
#include "../UI/UI.h"
#include "../Processors/Processors.h"
#include "../Process/Process.h"
#include "../Data Structures/DS.h"

class Scheduler
{
	UI* console;
	ProcessorsInfo P_info;
	unsigned timestep;
	LinkedQueue<Process*> New_List;
	LinkedQueue<Process*> Terminated_List;
public:
	Scheduler();
	~Scheduler();

	void AddToNewList(Process*);
	void AddToTerminatedList(Process*);
};

#endif