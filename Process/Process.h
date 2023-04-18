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
	int Transition_Time;		// Time of the last list transition.

public:
	Process(const Process&);
	Process(ProcessInfo, IO_process*);
	~Process();

	// Setters.
	void SetTransitionTime(int t) { Transition_Time = t; }

	// Getters.
	int GetArrivalTime() const { return P_data.AT; }
	int GetCPUTime() const { return P_data.CT; }
	int GetTurnAroundDuration() const { return P_data.TT - P_data.AT;  }
	int GetWaitingTime() const { return GetTurnAroundDuration() - P_data.CT; }
	int GetTransitionTime() const { return Transition_Time; }
	ProcessInfo GetProcessInfo() const { return P_data; }
	IO_process* GetIORequests() const { return IO; }

};

#endif