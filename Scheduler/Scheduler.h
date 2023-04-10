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
	LinkedQueue<Process*> Parents_List;

	FCFS* FCFS_Processors;
	SJF* SJF_Processors;
	RR* RR_Processors;

public:
	Scheduler();
	~Scheduler();

	void SetProcessors();

	// Getters
	LinkedQueue<Process*>* GetNewList() {	return &New_List; }
	LinkedQueue<Process*>* GetTerminatedList() { return &Terminated_List; }
	LinkedQueue<Process*>* GetParentsList() { return &Parents_List; }
	ProcessorsInfo GetProcessorsInfo() const {	return P_info; }

	void AddToList(LinkedQueue<Process*>*, Process*);

	void Execute();
	void ReadInput();
};

#endif