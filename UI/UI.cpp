#include "UI.h"
#include "../Scheduler/Scheduler.h"
#include <iostream>
#include <conio.h>

/**
* @brief Print Processors' RDY Lists.
* 
* @param run_size - Number of processes in RUN state.
* @param Processors - System's Processors.
* @param run - RUN List of processors.
*/
void UI::PrintRDY(int& run_size, Processor** Processors, Process**& run)
{
	// HEADER
	for (int i = 0; i < 15; i++)	cout << '-' << ' ';
	cout << "   RDY processes    ";
	for (int i = 0; i < 15; i++)	cout << '-' << ' ';
	cout << endl;

	string SJF_NAME = (manager->GetProcessorsInfo().NF) ? "SJF " : "SJF";
	string RR_NAME = (manager->GetProcessorsInfo().NF) ? "RR  " : (manager->GetProcessorsInfo().NS) ? "RR " : "RR";

	// Print FCFS Processors' RDY Lists.
	for (int i = 0; i < manager->GetProcessorsInfo().NF; i++)
	{
		FCFS* fcfs = dynamic_cast<FCFS*>(Processors[i]);
		run[i] = Processors[i]->GetRun();
		if (run[i])
			run_size++;
		LinkedList<Process*> FCFS_RDY = *(fcfs->GetRDY());
		cout << "processor " << i + 1 << " [FCFS]: " << FCFS_RDY.size() << " RDY: ";
		int FCFS_RDY_SIZE = FCFS_RDY.size();
		for (int i = 0; i < FCFS_RDY_SIZE; i++)
		{
			Process* process = NULL;
			if (FCFS_RDY.dequeue(process))
			{
				cout << process->GetProcessInfo().PID;
				if (!FCFS_RDY.isempty())
					cout << ", ";
			}
		}
		cout << endl;
	}
	
	// Print SJF Processors' RDY Lists.
	for (int i = manager->GetProcessorsInfo().NF; i < manager->GetProcessorsInfo().NF + manager->GetProcessorsInfo().NS; i++)
	{
		SJF* sjf = dynamic_cast<SJF*>(Processors[i]);
		run[i] = Processors[i]->GetRun();
		if (run[i])
			run_size++;
		PriorityQueue<Process*> SJF_RDY = *(sjf->GetRDY());
		cout << "processor " << i + 1 << " [" << SJF_NAME << "]: " << SJF_RDY.size() << " RDY: ";
		int SJF_RDY_SIZE = SJF_RDY.size();
		for (int i = 0; i < SJF_RDY_SIZE; i++)
		{
			Process* process = NULL;
			if (SJF_RDY.dequeue(process))
			{
				cout << process->GetProcessInfo().PID;
				if (!SJF_RDY.isEmpty())
					cout << ", ";
			}
		}
		cout << endl;
	}
	
	// Print RR Processors' RDY Lists.
	for (int i = manager->GetProcessorsInfo().NF + manager->GetProcessorsInfo().NS; i < manager->GetProcessorsInfo().NT; i++)
	{
		RR* rr = dynamic_cast<RR*>(Processors[i]);
		run[i] = Processors[i]->GetRun();
		if (run[i])
			run_size++;
		LinkedQueue<Process*> RR_RDY = *(rr->GetRDY());
		cout << "processor " << i + 1 << " [" << RR_NAME << "]: " << RR_RDY.size() << " RDY: ";
		int RR_RDY_SIZE = RR_RDY.size();
		for (int i = 0; i < RR_RDY_SIZE; i++)
		{
			Process* process = NULL;
			if (RR_RDY.dequeue(process))
			{
				cout << process->GetProcessInfo().PID;
				if (!RR_RDY.isEmpty())
					cout << ", ";
			}
		}
		cout << endl;
	}
}

/**
* @brief Print Processes in BLK List.
*/
void UI::PrintBLK()
{
	// HEADER
	for (int i = 0; i < 15; i++)	cout << '-' << ' ';
	cout << "   BLK processes    ";
	for (int i = 0; i < 15; i++)	cout << '-' << ' ';
	cout << endl;

	// Print Processes in BLK List
	LinkedQueue < Process*> BLK_LIST = *(manager->GetBlockList());
	int BLK_LIST_SIZE = BLK_LIST.size();
	cout << BLK_LIST_SIZE << " BLK: ";
	for (int i = 0; i < BLK_LIST_SIZE; i++)
	{
		Process* process = NULL;
		if (BLK_LIST.dequeue(process))
		{
			cout << process->GetProcessInfo().PID;
			if (!BLK_LIST.isEmpty())
			cout << ", ";
		}
	}
	cout << endl;
}

/**
* @brief Print Processes in RUN state.
*
* @param run_size - Number of processes in RUN state.
* @param run - RUN List of processors.
*/
void UI::PrintRUN(int run_size, Process** run)
{
	// HEADER
	for (int i = 0; i < 15; i++)	cout << '-' << ' ';
	cout << "   RUN processes    ";
	for (int i = 0; i < 15; i++)	cout << '-' << ' ';
	cout << endl;

	// Print Processes in RUN state.
	cout << run_size << " RUN: ";
	for (int i = 0; i < manager->GetProcessorsInfo().NT; i++)
	{
		if (!run[i])
			continue;
		cout << run[i]->GetProcessInfo().PID << "(P" << i + 1;
		run_size--;
		cout << (run_size ? "), " : ")");
	}
	cout << endl;
}

/**
* @brief Print Processes in TRM List.
*/
void UI::PrintTRM()
{
	// HEADER
	for (int i = 0; i < 15; i++)	cout << '-' << ' ';
	cout << "   TRM processes    ";
	for (int i = 0; i < 15; i++)	cout << '-' << ' ';
	cout << endl;

	// Print Processes in TRM List
	LinkedQueue<Process*> TRM_LIST = *(manager->GetTerminatedList());
	int TRM_LIST_SIZE = TRM_LIST.size();
	cout << TRM_LIST_SIZE << " TRM: ";
	for (int i = 0; i < TRM_LIST_SIZE; i++)
	{
		Process* process = NULL;
		if (TRM_LIST.dequeue(process))
		{
			cout << process->GetProcessInfo().PID;
			if (!TRM_LIST.isEmpty())
				cout << ", ";
		}
	}
	cout << endl;
}


/**
* @brief Allows the user to monitor the processes transition between different states.
*/
void UI::PrintOutput()
{
	int run_size = 0;
	Processor** Processors = manager->GetProcessors();
	Process** run = new Process * [manager->GetProcessorsInfo().NT];
	cout << "Current Timestep:" << manager->GetTimeStep() << endl;

	PrintRDY(run_size, Processors, run);
	
	PrintBLK();

	PrintRUN(run_size, run);

	PrintTRM();
	
	cout << "PRESS ANY KEY TO MOVE TO THE NEXT STEP !" << endl;
	_getch();								// Waits for any key input
	std::cout << "\033[2J\033[1;1H";		// Clears Console
}

/**
* @brief Reads the File name from the user.
*
* @return File name.
*/
string UI::GetFileName()
{
	string file_name;
	cout << "Enter the file to load: ";
	cin >> file_name;
	return file_name;
}