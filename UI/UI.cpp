#include "UI.h"
#include "../Scheduler/Scheduler.h"

void UI::PrintOutput()
{
	ProcessorsInfo i = manager->GetProcessorsInfo();
	cout << "NF    NS    NR    TS    RTF    MaxW    STL    Forkprob    Processes\n";
	cout << i.NF << "    " << i.NS << "    " << i.NR << "    " << i.Time_slice << "    " << i.RTF << "    "
		<< i.MaxW << "    " << i.STL << "    " << i.Fork_prob << "    " << i.Num_process << "    " << endl;
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