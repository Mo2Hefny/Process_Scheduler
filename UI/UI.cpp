#include "UI.h"
#include "../Scheduler/Scheduler.h"
#include <iostream>
#include <thread>
#include <conio.h>

/**
* @brief UI class constructor.
*
* @param app - Pointer to the scheduling manager.
*/
UI::UI(Scheduler* app)
{
	manager = app; 
	cout << "Please select the program running mode:\n";
	cout << "[1] Interactive Mode.\n";
	cout << "[2] Step-By-Step Mode.\n";
	cout << "[3] Silent Mode.\n";
	int m;
	cin >> m;
	while (m < 1 || m > 3)
	{
		cout << "Invalid answer please try again => ";
		cin >> m;
	}

	switch (m)
	{
	case 1: mode = Interactive; break;
	case 2: mode = Step_by_step; break;
	case 3: mode = Silent; break;
	}
	cout << "Clearing console........";
	// Pause for 1 second
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "\033[2J\033[1;1H";		// Clears Console
}

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
		if (fcfs->GetState() == OVERHEAT)
			printf("\x1B[31m");
		run[i] = Processors[i]->GetRun();
		if (run[i])
			run_size++;
		LinkedList<Process*>* FCFS_RDY = fcfs->GetRDY();
		cout << "processor " << i + 1 << " [FCFS]"<<'('<<fcfs->GetTimeLeft()<<')' << ':' << FCFS_RDY->size() << " RDY: ";
		FCFS_RDY->PrintList();
		
		if (fcfs->GetState() == OVERHEAT)
			printf("\033[0m");
	}
	
	// Print SJF Processors' RDY Lists.
	for (int i = manager->GetProcessorsInfo().NF; i < manager->GetProcessorsInfo().NF + manager->GetProcessorsInfo().NS; i++)
	{
		SJF* sjf = dynamic_cast<SJF*>(Processors[i]);
		if (sjf->GetState() == OVERHEAT)
			printf("\x1B[31m");
		run[i] = Processors[i]->GetRun();
		if (run[i])
			run_size++;
		PriorityQueue<Process*>* SJF_RDY = sjf->GetRDY();
		cout << "processor " << i + 1 << " [" << SJF_NAME << "]" << '(' << sjf->GetTimeLeft() << ')' << ':' << SJF_RDY->size() << " RDY: ";
		SJF_RDY->PrintList();

		if (sjf->GetState() == OVERHEAT)
			printf("\033[0m");
	}
	
	// Print RR Processors' RDY Lists.
	for (int i = manager->GetProcessorsInfo().NF + manager->GetProcessorsInfo().NS; i < manager->GetProcessorsInfo().NT; i++)
	{
		RR* rr = dynamic_cast<RR*>(Processors[i]);
		if (rr->GetState() == OVERHEAT)
			printf("\x1B[31m");
		run[i] = Processors[i]->GetRun();
		if (run[i])
			run_size++;
		LinkedQueue<Process*>* RR_RDY = rr->GetRDY();
		cout << "processor " << i + 1 << " [" << RR_NAME << "]" << '(' << rr->GetTimeLeft() << ')' << ':' << RR_RDY->size() << " RDY: ";
		RR_RDY->PrintList();
		if (rr->GetState() == OVERHEAT)
			printf("\033[0m");
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
	LinkedQueue < Process*>* BLK_LIST = manager->GetBlockList();
	cout << BLK_LIST->size() << " BLK: ";
	BLK_LIST->PrintList();
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
		cout << run[i];
		printf("(P%d)", i + 1);
		run_size--;
		if (run_size)	cout << ", ";
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
	LinkedQueue<Process*>* TRM_LIST = manager->GetTerminatedList();
	cout << TRM_LIST->size() << " TRM: ";
	TRM_LIST->PrintList();
}


/**
* @brief Allows the user to monitor the processes transition between different states.
*/
void UI::PrintOutput()
{
	if (mode == Silent)
	{
		if (manager->GetTimeStep() == 0)
		{
			cout << "Silent Mode.....    Simulation Starts...\n";
			cout << "Simulation ends, Output file created\n";
		}
		return;
	}
	

	int run_size = 0;
	Processor** Processors = manager->GetProcessors();
	Process** run = new Process * [manager->GetProcessorsInfo().NT];
	
	cout << "Current Timestep:" << manager->GetTimeStep() << endl;

	PrintRDY(run_size, Processors, run);
	PrintBLK();
	PrintRUN(run_size, run);
	PrintTRM();

	if (mode == Interactive)
	{
		cout << "PRESS ANY KEY TO MOVE TO THE NEXT STEP !" << endl;
		_getch();								// Waits for any key input
	}
	else
	{
		cout << "PLEASE WAIT FOR 1 SECOND !" << endl;
		// Pause for 1 second
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
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