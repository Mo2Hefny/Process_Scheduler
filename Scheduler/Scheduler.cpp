#include "Scheduler.h"
#include <fstream>
#include <string>
Scheduler::Scheduler()
{
	timestep = k = 0;
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

/*
* AddToList - Adds process to a chosen list.
* 
* @param List - Chosen list.
* @param p - Pointer to the process.
*/
void Scheduler::AddToList(LinkedQueue<Process*>* List, Process* p)
{
	List->enqueue(p);
}

/*
* AddToReady - Schedules the process to a processor's RDY list.
* 
* @param p - Pointer to the process.
*/
void Scheduler::AddToReady(Process* p)
{
	Processors[k]->AddToRDY(p);
	k = (k + 1) % (P_info.NF + P_info.NS + P_info.NR);
}

/*
* LoadFile - Load the Processors and Processes data from an input file.
*/
void Scheduler::LoadFile()
{
	string file_name;
	do
	{
		file_name = console->GetFileName() + ".txt";
		LoadedFile.open(file_name);
	} while (!LoadedFile.is_open());
	LoadedFile >> P_info.NF >> P_info.NS >> P_info.NR;			// No. of processors of each type.
	LoadedFile >> P_info.Time_slice;					// Time slice for RR.
	LoadedFile >> P_info.RTF >> P_info.MaxW >> P_info.STL >> P_info.Fork_prob;
	LoadedFile >> P_info.Num_process;
	for (int j = 0; j < P_info.Num_process; j++)
	{
		ProcessInfo P_data;
		LoadedFile >> P_data.AT >> P_data.PID >> P_data.CT >> P_data.IO_requests;

		string IO_string;
		LoadedFile >> IO_string;
		IO_process* IO = ProcessIORequestsInput(IO_string, P_data.IO_requests);

		Process* New_Process = new Process(P_data, IO);
		AddToList(GetNewList(), New_Process);
	}
	LoadedFile.close();
	console->PrintOutput();
	console->PrintNew();
}

/*
* ProcessIORequestsInput - Extract the I/O data from the given string of pairs.
*
* @param IO_string - The I/O requests string that contains the needed data.
* @param size - Number of the I/O requests of the process.
*
* @return I/O requests array.
*/
IO_process* Scheduler::ProcessIORequestsInput(string IO_string, int size)
{
	IO_process* IO = new IO_process[size];
	string num = "";
	int i = 0;
	for (char c : IO_string) {
		if (isdigit(c)) {
			num += c;
		}
		else if (!num.empty() && (c == ',' || c == ')')) {
			if (i & 1)	IO[i / 2].IO_D = stoi(num);
			else IO[i / 2].IO_R = stoi(num);
			i++;
			num = "";
		}
	}
	return IO;
}

/*
* ReadInput - Read system's information.
*/
void Scheduler::ReadInput()
{
	LoadFile();
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

/*
* Execute - execution of the system.
*/
void Scheduler::Execute()
{
	ReadInput();
	while (Terminated_List.size() < P_info.Num_process)
	{
		for (int i = 0; i < New_List.size(); i++)
		{
			Process* current;
			New_List.dequeue(current);

			if (current->GetArrivalTime() == timestep)
				AddToReady(current);
			else
				New_List.enqueue(current);
		}

		for (int i = 0; i < P_info.NF + P_info.NS + P_info.NR; i++)
		{
			Processors[i]->Execute();
		}

		Process* top = NULL;
		if (BLK_List.peek(top))
		{
			srand(time(0));
			int move_possibility = rand() % 100 + 1;
			if (move_possibility <= 10)
			{
				BLK_List.dequeue(top);
				AddToReady(top);
			}
		}

		console->PrintOutput();

		timestep++;
	}
}