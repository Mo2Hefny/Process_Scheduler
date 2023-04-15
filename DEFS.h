#ifndef _DEFS_
#define _DEFS_

struct ProcessInfo
{
	int AT;				// Arrival time.
	int PID;			// Process ID.
	int CT;				// CPU time.
	int IO_requests;	// Number of time the process requests I/O
	int TT;				// Termination time.
};

struct IO_process
{
	int IO_R;	// The execution intercal after which the process requests for I/O resources.
	int IO_D;	// The duration required for a process to perform some I/O task.
};

struct ProcessorsInfo
{
	int NF;				// Number of FCFS processors.
	int NS;				// Number of SJF processors.
	int NR;				// Number of RR processors.
	int NT;				// Number of processors.
	int Time_slice;		// Time slice for RR.
	int RTF;			// Min time Threshold before process migration from RR to SJF.
	int STL;			// Stealing Time Limit.
	int MaxW;			// Max time Threshold before process migration from FCFS to RR.
	int Fork_prob;		// Fork probability.
	int Num_process;	// Number of processes.

	ProcessorsInfo()
	{
		NF = NS = NR = Num_process = NT = 0;
	}
};

enum ProcessorState
{
	BUSY,
	IDLE
};

enum PQType
{
	AT_order,
	CT_order
};

enum ProgramInterface
{
	Interactive,
	Step_by_step,
	Silent
};

#endif