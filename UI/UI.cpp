#include "UI.h"
#include "../Scheduler/Scheduler.h"

void UI::PrintOutput()
{
	int run_size=0;
	Processor** Processors = manager->GetProcessors();
	Process** run = new Process * [manager->GetProcessorsInfo().NT];
	cout << "Current Timestep:" << manager->GetTimeStep()<<endl;
	for (int i = 0; i < 15; i++)
		cout << '-' << ' ';
	cout << "   RDY processes    ";
	for (int i = 0; i < 15; i++)
		cout << '-' << ' ';
	cout << endl;
	for (int i = 0; i < manager->GetProcessorsInfo().NF; i++)
	{
		FCFS* fcfs = dynamic_cast<FCFS*>(Processors[i]);
		run[i] = Processors[i]->GetRun();
		if (run[i])
			run_size++;
		cout << "processor " << i+1 << " [FCFS]: " << fcfs->GetRDY().size() << " RDY: ";
		for (int i = 0; i < fcfs->GetRDY().size(); i++)
		{
			Process* process;
			if (fcfs->GetRDY().dequeue(process))
			{
				cout << process->GetProcessInfo().PID << ", ";
				//fcfs->GetRDY().enqueue(process);
			}
		}
		cout << endl;
	}
	for (int i = manager->GetProcessorsInfo().NF; i < manager->GetProcessorsInfo().NF+ manager->GetProcessorsInfo().NS; i++)
	{
		SJF* sjf = dynamic_cast<SJF*>(Processors[i]);
		run[i] = Processors[i]->GetRun();
		if (run[i])
			run_size++;
		cout << "processor " << i+1 << " [SJF ]: " << sjf->GetRDY().size() << " RDY: ";
		for (int i = 0; i < sjf->GetRDY().size(); i++)
		{
			Process* process;
			sjf->GetRDY().dequeue(process);
			cout << process->GetProcessInfo().PID << ", ";
		}
		cout << endl;
	}
	for (int i = manager->GetProcessorsInfo().NF + manager->GetProcessorsInfo().NS; i < manager->GetProcessorsInfo().NT; i++)
	{
		RR* rr = dynamic_cast<RR*>(Processors[i]);
		run[i] = Processors[i]->GetRun();
		if (run[i])
			run_size++;
		cout << "processor " << i+1 << " [RR  ]: " << rr->GetRDY().size() << " RDY: ";
		for (int i = 0; i < rr->GetRDY().size(); i++)
		{
			Process* process;
			if(rr->GetRDY().dequeue(process))
			cout << process->GetProcessInfo().PID << ", ";
		}
		cout << endl;
	}
	for (int i = 0; i < 15; i++)
		cout << '-' << ' ';
	cout << "   BLK processes    ";
	for (int i = 0; i < 15; i++)
		cout << '-' << ' ';
	cout << endl;
	LinkedQueue < Process*> blklist;
	blklist = *(manager->GetBlockList());
	cout << blklist.size() << " BLK: ";
	for (int i = 0; i < blklist.size(); i++)
	{
		Process* process;
		if (blklist.dequeue(process))
		{
			cout << process->GetProcessInfo().PID << ", ";
			//blklist.enqueue(process);
		}
	}
	cout << endl;
	for (int i = 0; i < 15; i++)
		cout << '-' << ' ';
	cout << "   RUN processes    ";
	for (int i = 0; i < 15; i++)
		cout << '-' << ' ';
	cout << endl;
	cout << run_size << " RUN: ";
	for (int i = 0; i < manager->GetProcessorsInfo().NT; i++)
	{
		if (!run[i])	
			continue;
		cout << run[i]->GetProcessInfo().PID << "(P" << i + 1 << "), ";
	}
	cout << endl;
	for (int i = 0; i < 15; i++)
		cout << '-' << ' ';
	cout << "   TRM processes    ";
	for (int i = 0; i < 15; i++)
		cout << '-' << ' ';
	cout << endl;
	LinkedQueue < Process*> trmlist;
	trmlist = *(manager->GetTerminatedList());
	cout << trmlist.size() << " TRM: ";
	for (int i = 0; i < trmlist.size(); i++)
	{
		Process* process;
		if (trmlist.dequeue(process))
		{
			cout << process->GetProcessInfo().PID << ", ";
			//trmlist.enqueue(process); 
		}
	}
	//cout << endl << "PRESS ANY KEY TO MOVE TO THE NEXT STEP !" << endl;
	cout << endl;
	system("pause");
	cout << endl;
}

void UI::PrintNew()
{
	LinkedQueue<Process*>* n = manager->GetNewList();
	while (!n->isEmpty())
	{
		Process* temp;
		n->dequeue(temp);
		ProcessInfo i = temp->GetProcessInfo();
		printf("AT:%d,PID:%d,CT:%d,N:%d\n",
			i.AT, i.PID, i.CT, i.IO_requests);
		IO_process* arr = temp->GetIORequests();
		for (int j = 0; j < i.IO_requests; j++)
		{
			printf("(%d, %d),", arr[j].IO_R, arr[j].IO_D);
		}
		cout << endl;
	}
}

string UI::GetFileName()
{
	string file_name;
	cout << "Enter the file to load: ";
	cin >> file_name;
	return file_name;
}