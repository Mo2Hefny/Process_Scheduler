#include "Scheduler.h"
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <string>
#include <thread>
#include <conio.h>

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
* @brief Sets the child process to a FCFS processor's RDY list.
*
* @param p - Pointer to the process.
*/
void Scheduler::AddToFCFS(Process* p)
{
	int shortestqueue_index = 0;
	for (int i = 1; i < P_info.NF; i++)
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
	int shortestqueue_index = P_info.NF;
	for (int i = P_info.NF + 1; i < P_info.NF + P_info.NS; i++)
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
	int shortestqueue_index = P_info.NF + P_info.NS;
	for (int i = P_info.NF + P_info.NS + 1; i < P_info.NT; i++)
	{
		if (Processors[i]->GetTimeLeft() < Processors[shortestqueue_index]->GetTimeLeft())
			shortestqueue_index = i;
	}

	//p->SetTransitionTime(timestep);		//MIGHT BE REMOVED//

	Processors[shortestqueue_index]->AddToRDY(p);
}

/**
* @brief The shortest ready queue in the system looks at the longest ready
* queue to see how full it is.
*/
void Scheduler::WorkStealing()
{
	// Set SQF and LQF.
	int shortestqueue_index = 0;
	int longestqueue_index = 0;
	for (int i = 1; i < P_info.NT; i++)
	{
		if (Processors[i]->GetTimeLeft() < Processors[shortestqueue_index]->GetTimeLeft())
			shortestqueue_index = i;
		if (Processors[i]->GetTimeLeft() > Processors[longestqueue_index]->GetTimeLeft())
			longestqueue_index = i;
	}
	LQF = Processors[longestqueue_index];
	SQF = Processors[shortestqueue_index];

	int sqf_time = SQF->GetTimeLeft();
	int lqf_time = LQF->GetTimeLeft();

	float STEAL_LIMIT = (lqf_time - sqf_time) * 100.0 / lqf_time;

	while (STEAL_LIMIT > 40.0)
	{
		Process* process;
		if (LQF->Work_Stealing(process, 0))
		{
			SQF->Work_Stealing(process, 1);
			IncrementSteal();
		}
		sqf_time = SQF->GetTimeLeft();
		lqf_time = LQF->GetTimeLeft();
		STEAL_LIMIT = (float)(lqf_time - sqf_time) / lqf_time;
	}

}

/**
* @brief Checks for orphan processes in the FCFS processors
* after any process ermination.
*/
void Scheduler::CheckOrphans()
{
	LinkedList <Process*> orphans;
	for (int i = 0; i < P_info.NF; i++)
	{
		Process* RUN = FCFS_Processors[i].GetRun();
		if (RUN && RUN->IsTerminated())
		{
			FCFS_Processors[i].TerminateRUN();
		}
		FCFS_Processors[i].GetRDY()->FindOrphans(orphans);
		Process* process;
		while (orphans.dequeue(process))
		{
			FCFS_Processors[i].AddTimeleft(-(process->GetRemainingTime()));
			AddToList(&Terminated_List, process);
		}
	}
}

/**
* @brief Load the Processors and Processes data from an input file.
*/
void Scheduler::LoadFile()
{
	string file_name;
	string path = "Inputs\\";
	do
	{
		file_name = console->GetFileName() + ".txt";
		LoadedFile.open(path + file_name);
	} while (!LoadedFile.is_open());
	LoadedFile >> P_info.NF >> P_info.NS >> P_info.NR;			// No. of processors of each type.
	P_info.NT = P_info.NF + P_info.NS + P_info.NR;
	LoadedFile >> P_info.Time_slice >> P_info.cooldown;					// Time slice for RR && Overheating cooldown for processors.
	LoadedFile >> P_info.RTF >> P_info.MaxW >> P_info.STL >> P_info.Fork_prob >> P_info.Heat_prob;
	LoadedFile >> P_info.Num_process;
	for (int j = 0; j < P_info.Num_process; j++)
	{
		ProcessInfo P_data;
		LoadedFile >> P_data.AT >> P_data.PID >> P_data.CT >> P_data.IO_requests;

		string IO_string = "";
		IO_process* IO = NULL;
		int total = 0;
		if (P_data.IO_requests != 0)
		{
			LoadedFile >> IO_string;
			IO = ProcessIORequestsInput(IO_string, P_data.IO_requests, total);
		}
		P_data.total_IO_D = total;
		Process* New_Process = new Process(P_data, IO);
		AddToList(GetNewList(), New_Process);
	}

	int t, id;
	LinkedList<SIGKILL*>* sigkill_list = FCFS::GetSIGKILL();
	while (LoadedFile >> t >> id) {
		SIGKILL* sigkill = new SIGKILL(t, id);
		sigkill_list->enqueue(sigkill);
	}
	LoadedFile.close();
}

/**
* @brief Extract the I/O data from the given string of pairs.
*
* @param IO_string - The I/O requests string that contains the needed data.
* @param size - Number of the I/O requests of the process.
* @param total - total IO requests durations.
*
* @return I/O requests array.
*/
IO_process* Scheduler::ProcessIORequestsInput(string IO_string, int size, int &total)
{
	IO_process* IO = new IO_process[size];
	string num = "";
	int i = 0;
	for (char c : IO_string) {
		if (isdigit(c)) {
			num += c;
		}
		else if (!num.empty() && (c == ',' || c == ')')) {
			if (i & 1)
			{
				IO[i / 2].IO_D = stoi(num);
				total += stoi(num);
			}
			else
			{
				IO[i / 2].IO_R = stoi(num);
			}
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
			current->SetResponseTime(timestep);
			AddToReady(current);
		}

		if (timestep && timestep % P_info.STL == 0)
			WorkStealing();

		for (int i = 0; i < P_info.NF + P_info.NS + P_info.NR; i++)
		{
			Processors[i]->Execute();
		}
		
		Process* top = NULL;
		if (BLK_List.peek(top))
		{
			IO_process* IO = top->GetIORequests();
			if ((IO + IO->i)->IO_D <= timestep - (IO + IO->i)->IO_T)
			{
				IO->i++;
				BLK_List.dequeue(top);
				AddToReady(top);
			}
		}

		console->PrintOutput();

		timestep++;
	}
	cout << "Creating an output file......\n";
	// Pause for 1 second
	std::this_thread::sleep_for(std::chrono::seconds(1));
	WriteOutput();
}

/**
* @brief Sets the output file name and organizes its path.
*/
void Scheduler::SetOutputFile()
{
	std::cout << "\033[2J\033[1;1H";		// Clears Console
	string file_name;
	cout << "Enter the name of the output file: ";
	cin >> file_name;
	string path = "Outputs\\";
	file_name = file_name + ".txt";
	while (ifstream(path + file_name))
	{
		cout << "File already exists. Do you want to overwrite it? (y / n)\n";
		char c;
		do
		{
			c = getchar();
			c = tolower(c);
		} while (c != 'y' && c != 'n');
		if (c == 'n')
		{
			file_name = console->GetFileName();
			file_name = file_name + ".txt";
		}
		else
			break;
	}
	OutputFile.open(path + file_name);
	if (!OutputFile)
	{
		cout << "ERROR OCCURED\n";
		exit(1);
	}

}

/**
* @brief Write the simulation results data into an output file.
*/
void Scheduler::WriteOutput()
{
	SetOutputFile();
	OutputFile << "TT\t\t\t\tPID\t\t\t\tAT\t\t\t\tCT\t\t\t\tIO_D\t\t\t\t\tWT\t\t\t\tRT\t\t\t\tTRT\n";
	Process* process;
	int total_WT = 0, total_RT = 0, total_TRT = 0;
	while (Terminated_List.dequeue(process))
	{
		ProcessInfo i = process->GetProcessInfo();
		OutputFile << i.TT << "\t\t\t\t";
		OutputFile << i.PID << "\t\t\t\t";
		OutputFile << i.AT << "\t\t\t\t";
		OutputFile << i.CT << "\t\t\t\t";
		OutputFile << i.total_IO_D << " \t\t\t\t\t\t";
		OutputFile << process->GetWaitingTime() << "\t\t\t\t";
		OutputFile << i.RT << "\t\t\t\t";
		OutputFile << process->GetTurnAroundDuration() << "\n";
		total_WT += process->GetWaitingTime();
		total_RT += i.RT;
		total_TRT += process->GetTurnAroundDuration();
		delete process;
	}
	float n = (P_info.Num_process ? P_info.Num_process : 1);
	OutputFile << "\nProcesses: " << P_info.Num_process;
	OutputFile << "\nAvg WT: " << total_WT / (int)n << ", \t\tAvg RT = " << total_RT / (int)n << ", \t\tAvg TRT = " << total_TRT / (int)n;
	OutputFile << "\nMigration %: \t\t\tRTF = " << stats.RR_SJF_migrations * 100.0 / n << "%, \t\tMaxW = " << stats.FCFS_RR_migrations * 100.0 / n << "%";
	OutputFile << "\nWork Steal %: " << stats.total_steal * 100.0 / n << "%";
	OutputFile << "\nForked Process: " << stats.total_fork * 100.0 / n << "%";
	OutputFile << "\nKilled Process: " << stats.total_killed * 100.0 / n << "%";

	OutputFile << "\n\nProcessors: " << P_info.NT << "[" << P_info.NF << " FCFS, " << P_info.NS << " SJR, " << P_info.NR << "RR]";
	OutputFile << "\nProcessors Load\n";
	for (int i = 0; i < P_info.NT; i++)
	{
		OutputFile << "p" << i + 1 << " = " << Processors[i]->GetPload() << "%";
		if (i != P_info.NT - 1)
			OutputFile << ",\t\t";
	}
	float total_util = 0;
	OutputFile << "\n\n\nProcessors Utiliz\n";
	for (int i = 0; i < P_info.NT; i++)
	{
		total_util += Processors[i]->GetPUtil();
		OutputFile << "p" << i + 1 << " = " << Processors[i]->GetPUtil() << "%";
		if (i != P_info.NT - 1)
			OutputFile << ",\t\t";
	}
	OutputFile << "\nAvg utilization = " << total_util / P_info.NT << endl;


	cout << "Output file has been created.\nCheck the \"Outputs\" folder.\n";
	OutputFile.close();
}