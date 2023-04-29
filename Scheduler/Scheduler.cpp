#include "Scheduler.h"
#include <fstream>
#include <string>

/**
* @brief Scheduler class constructor.
*/
Scheduler::Scheduler()
{
	timestep = 0;
	FCFS_Processors = NULL;
	SJF_Processors = NULL;
	RR_Processors = NULL;
	console = new UI(this);
	Execute();
}

/**
* @brief Scheduler class destructor.
* 
* @details Deallocates the UI console, the Processors, and the processes.
*/
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

	Process* process;
	while (Terminated_List.dequeue(process))
		delete process;
}

/**
* @brief Adds process to a chosen list.
* 
* @param List - Chosen list.
* @param p - Pointer to the process.
*/
void Scheduler::AddToList(LinkedQueue<Process*>* List, Process* p)
{
	//p->SetTransitionTime(timestep);
	List->enqueue(p);
}

/**
* @brief Schedules the process to the processor with the shortest queue.
* 
* @param p - Pointer to the process.
*/
void Scheduler::AddToReady(Process* p)
{
	int shortestqueue_index = 0;
	for (int i = 1; i < P_info.NT; i++)
	{
		if (Processors[i]->GetTimeLeft() < Processors[shortestqueue_index]->GetTimeLeft())
			shortestqueue_index = i;
	}

	//p->SetTransitionTime(timestep);		//MIGHT BE REMOVED//

	Processors[shortestqueue_index]->AddToRDY(p);
}

/**
* @brief Migrates the process to a SJF processor's RDY list.
*
* @param p - Pointer to the process.
*/
void Scheduler::AddToSJF(Process* p)
{
	int shortestqueue_index = 0;
	for (int i = P_info.NF; i < P_info.NF + P_info.NS; i++)
	{
		if (Processors[i]->GetTimeLeft() < Processors[shortestqueue_index]->GetTimeLeft())
			shortestqueue_index = i;
	}

	//p->SetTransitionTime(timestep);		//MIGHT BE REMOVED//

	Processors[shortestqueue_index]->AddToRDY(p);
}

/**
* @brief Migrates the process to a RR processor's RDY list.
*
* @param p - Pointer to the process.
*/
void Scheduler::AddToRR(Process* p)
{
	int shortestqueue_index = 0;
	for (int i = P_info.NF + P_info.NS; i < P_info.NT; i++)
	{
		if (Processors[i]->GetTimeLeft() < Processors[shortestqueue_index]->GetTimeLeft())
			shortestqueue_index = i;
	}

	//p->SetTransitionTime(timestep);		//MIGHT BE REMOVED//

	Processors[shortestqueue_index]->AddToRDY(p);
}

/**
* @brief Load the Processors and Processes data from an input file.
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
	P_info.NT = P_info.NF + P_info.NS + P_info.NR;
	LoadedFile >> P_info.Time_slice;					// Time slice for RR.
	LoadedFile >> P_info.RTF >> P_info.MaxW >> P_info.STL >> P_info.Fork_prob;
	LoadedFile >> P_info.Num_process;
	for (int j = 0; j < P_info.Num_process; j++)
	{
		ProcessInfo P_data;
		LoadedFile >> P_data.AT >> P_data.PID >> P_data.CT >> P_data.IO_requests;

		string IO_string = "";
		IO_process* IO = NULL;
		if (P_data.IO_requests != 0)
		{
			LoadedFile >> IO_string;
			IO = ProcessIORequestsInput(IO_string, P_data.IO_requests);
		}

		Process* New_Process = new Process(P_data, IO);
		AddToList(GetNewList(), New_Process);
	}
	LoadedFile.close();
}

/**
* @brief Extract the I/O data from the given string of pairs.
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

/**
* @brief Read system's information and initialize processors.
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
		RR_Processors[i].SetTimeSlice(P_info.Time_slice);
		Processors[i + P_info.NF + P_info.NS] = &RR_Processors[i];
	}
}

/**
* @brief Simulation of the system.
*/
void Scheduler::Execute()
{
	ReadInput();
	while (Terminated_List.size() < P_info.Num_process)
	{
		int new_size = New_List.size();
		Process* current;
		New_List.peek(current);
		if (current->GetArrivalTime() == timestep)
		{
			New_List.dequeue(current);
			AddToReady(current);
		}

		for (int i = 0; i < P_info.NF + P_info.NS + P_info.NR; i++)
		{
			Processors[i]->Execute();
		}

		int random_ID = rand() % P_info.Num_process + 1;
		for (int i = 0; i < P_info.NF; i++)
		{
			Process* process = NULL;
			FCFS* fcfs = dynamic_cast<FCFS*>(Processors[i]);
			if (fcfs->GetRDYref().DeleteNode(process, random_ID))
			{
				fcfs->SetTimeLeft(fcfs->GetTimeLeft() - process->GetCPUTime());
				process->SetCPUTime(0);
				AddToList(GetTerminatedList(), process);
				break;
			}
		}

		Process* top = NULL;
		if (BLK_List.peek(top))
		{
			IO_process* IO = top->GetIORequests();
			if ((IO + IO->i)->IO_D == timestep - (IO + IO->i)->IO_T)
			{
				IO->i++;
				BLK_List.dequeue(top);
				AddToReady(top);
			}
		}

		console->PrintOutput();

		timestep++;
	}
}