#ifndef _Schedule_Manager_
#define _Schedule_Manager_

#include "../DEFS.h"
#include "../UI/UI.h"
#include "../Processors/Processors.h"
#include "../Process/Process.h"
#include "../Data Structures/DS.h"
#include <iostream>
#include <fstream>

using namespace std;
class Scheduler
{
	UI* console;
	ifstream LoadedFile;
	ProcessorsInfo P_info;
	unsigned timestep;
	LinkedQueue<Process*> New_List;
	LinkedQueue<Process*> BLK_List;
	LinkedQueue<Process*> Terminated_List;
	LinkedQueue<Process*> Parents_List;

	FCFS* FCFS_Processors;
	SJF* SJF_Processors;
	RR* RR_Processors;
	Processor** Processors;

	// TEMPORARY SCHEDULING COUNTER
	int k;
public:
	Scheduler();
	~Scheduler();

	void SetProcessors();

	// Getters
	LinkedQueue<Process*>* GetNewList() {	return &New_List; }
	LinkedQueue<Process*>* GetBlockList() { return &BLK_List; }
	LinkedQueue<Process*>* GetTerminatedList() { return &Terminated_List; }
	LinkedQueue<Process*>* GetParentsList() { return &Parents_List; }
	ProcessorsInfo GetProcessorsInfo() const {	return P_info; }
	unsigned GetTimeStep() { return timestep; }
	Processor** GetProcessors() { return Processors; }

	void AddToList(LinkedQueue<Process*>*, Process*);
	void AddToReady(Process*);
	void ProcessMigration(Process*);

	void Execute();
	void ReadInput();
	void LoadFile();
	IO_process* ProcessIORequestsInput(string IO_string, int size);
};

#endif