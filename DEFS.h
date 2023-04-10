#ifndef _DEFS_
#define _DEFS_

struct ProcessInfo
{
	int AT;				// Arrival time.
	int PID;			// Process ID.
	int CT;				// CPU time.
	int IO_requests;	// Number of time the process requests I/O
};

struct IO_process
{
	int IO_R;
	int IO_D;
};

struct ProcessorsInfo
{
	int NF, NS, NR;		// Number of processors.
	int Time_slice;		// Time slice for RR.
	int RTF;			// Min time Threshold before process migration from RR to SJF.
	int STL;			// Stealing Time Limit.
	int MaxW;			// Max time Threshold before process migration from FCFS to RR.
	int Fork_prob;		// Fork probability.
	int Num_process;	// Number of processes.

	ProcessorsInfo()
	{
		NF = NS = NR = Num_process = 0;
	}
};

#endif