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
		IO_process*	IO = ProcessIORequestsInput(IO_string, P_data.IO_requests);

		Process* New_Process = new Process(P_data, IO);
		manager->AddToList(manager->GetNewList(), New_Process);
	}
	LoadedFile.close();
	PrintOutput();
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