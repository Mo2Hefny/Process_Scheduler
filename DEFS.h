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
	int RTF;			// Min time Threshold before process migration from RR to SJF.
	int STL;			// Stealing Time Limit.
	int MaxW;			// Max time Threshold before process migration from FCFS to RR.
	int Fork_prob;		// Fork probability.
};

#endif