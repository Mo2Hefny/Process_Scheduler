#include "Scheduler.h"
#include <fstream>

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

void Scheduler::LoadFile()
{
	string file_name;
	do
	{
		cout << "Enter the file to load: ";
		cin >> file_name;
		file_name += ".txt";
		LoadedFile.open(file_name);
	} while (!LoadedFile.is_open());
	LoadedFile >> i.NF >> i.NS >> i.NR;			// No. of processors of each type.
	LoadedFile >> i.Time_slice;					// Time slice for RR.
	LoadedFile >> i.RTF >> i.MaxW >> i.STL >> i.Fork_prob;
	LoadedFile >> i.Num_process;
	for (int j = 0; j < i.Num_process; j++)
	{
		ProcessInfo P_data;
		LoadedFile >> P_data.AT >> P_data.PID >> P_data.CT >> P_data.IO_requests;

		string IO_string;
		LoadedFile >> IO_string;
		IO_process* IO = ProcessIORequestsInput(IO_string, P_data.IO_requests);

		Process* New_Process = new Process(P_data, IO);
		manager->AddToList(manager->GetNewList(), New_Process);
	}
	LoadedFile.close();
	PrintOutput();
	PrintNew();
}

void Scheduler::ReadInput()
{
	console->LoadFile(P_info);
	if (P_info.NF > 0)
		FCFS_Processors = new FCFS[P_info.NF];
	if (P_info.NS > 0)
		SJF_Processors = new SJF[P_info.NS];
	if (P_info.NR > 0)
		RR_Processors = new RR[P_info.NR];
	if (P_info.NF > 0 || P_info.NS > 0 || P_info.NR > 0)
	Processors = new Processor*[P_info.NF + P_info.NS + P_info.NR];
	for (int i = 0; i < P_info.NF; i++)
	{
		FCFS_Processors[i].SetScheduler(this);
		Processors[i] = &FCFS_Processors[i];
	}
	for (int i = 0; i < P_info.NS; i++)
	{
		SJF_Processors[i].SetScheduler(this);
		Processors[i + P_info.NF] = &SJF_Processors[i];
	}
	for (int i = 0; i < P_info.NR; i++)
	{
		RR_Processors[i].SetScheduler(this);
		Processors[i + P_info.NF + P_info.NS] = &RR_Processors[i];
	}
}

void Scheduler::Execute()
{
	ReadInput();
}