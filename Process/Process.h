#ifndef _PROCESS_
#define _PROCESS_

#include "../DEFS.h"

class Process
{
	ProcessInfo P_data;
	IO_process* IO;			// Array of I/O requests.
	bool terminated;
	Process* l_child;
	Process* r_child;

public:
	Process(ProcessInfo, IO_process*);
	~Process();

	// Getters.
	int GetArrivalTime() const { return P_data.AT; }
	int GetCPUTime() const { return P_data.CT; }
	int GetTurnAroundDuration() const { return P_data.TT - P_data.AT;  }
	int GetWaitingTime() const { return GetTurnAroundDuration() - P_data.CT; }
	ProcessInfo GetProcessInfo() const { return P_data; }
	IO_process* GetIORequests() const { return IO; }

};

#endif