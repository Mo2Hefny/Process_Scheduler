#include "UI.h"
#include <string>
#include "../Scheduler/Scheduler.h"

/*
* LoadFile - Load the Processors and Processes data from an input file.
* 
* @param i - Reference to the Processors info to be updated.
*/
void UI::LoadFile(ProcessorsInfo& i)
{
	
}

/*
* ProcessIORequestsInput - Extract the I/O data from the given string of pairs.
*
* @param IO_string - The I/O requests string that contains the needed data.
* @param size - Number of the I/O requests of the process.
* 
* @return I/O requests array.
*/
IO_process* UI::ProcessIORequestsInput(string IO_string, int size)
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