#ifndef _DEFS_
#define _DEFS_

#include <Windows.h>

#define Text "\033[0;38;2;232;230;227;48;2;1;4;9m"
#define Title "\033[0;38;2;232;230;227;48;2;70;133;255m"
#define SmallTitle "\033[0;38;2;232;230;227;48;2;34;29;33m"

#define ParentLate "\033[38;2;226;93;75;48;2;21;103;168m"
#define Parent "\033[38;2;255;254;251;48;2;21;103;168m"

#define ChildLate "\033[38;2;226;93;75;48;2;15;99;113m"
#define Child "\033[38;2;255;254;251;48;2;15;99;113m"

#define Late "\033[38;2;226;93;75;1;48;2;1;4;9m"
#define Early "\x1B[93;48;2;1;4;9m"

#define MigLate "\033[38;2;226;93;75;1;48;2;1;4;9;4;3m"
#define MigEarly "\x1B[93;48;2;1;4;9;4;3m"

struct ProcessInfo
{
	int AT;				// Arrival time.
	int PID;			// Process ID.
	int RT;				// Response Time.
	int CT;				// CPU time.
	int Deadline;		// Deadline.
	int ET;				// Execution time.
	int IO_requests;	// Number of time the process requests I/O
	int TT;				// Termination time.
	int total_IO_D;

	ProcessInfo()
	{
		ET = IO_requests = total_IO_D = 0;
		RT = TT = -1;
	}
};

struct IO_process
{
	int IO_R;	// The execution intercal after which the process requests for I/O resources.
	int IO_D;	// The duration required for a process to perform some I/O task.
	int i;      // Counter to loop over IO array
	int IO_T;   // Timestep process asked for IO

	IO_process()
	{
		i = 0;
	}
};

struct SIGKILL
{
	int time;	// Kill time.
	int ID;		// PID of the process to kill.

	SIGKILL(int t = 0, int id = 0)
	{
		time = t;
		ID = id;
	}
	SIGKILL(const SIGKILL& other)
	{
		ID = other.ID;
		time = other.time;
	}
};

struct ProcessorsInfo
{
	int NF;				// Number of FCFS processors.
	int NS;				// Number of SJF processors.
	int NR;				// Number of RR processors.
	int NE;				// Number of EDF processors.
	int NT;				// Number of processors.
	int Time_slice;		// Time slice for RR.
	int cooldown;		// Overheat Cooldown.
	int RTF;			// Min time Threshold before process migration from RR to SJF.
	int STL;			// Stealing Time Limit.
	int MaxW;			// Max time Threshold before process migration from FCFS to RR.
	int Fork_prob;		// Fork probability.
	int Heat_prob;		// Overheat probability.
	int Num_process;	// Number of processes.

	ProcessorsInfo()
	{
		NF = NS = NR = Num_process = NT = 0;
	}
};

struct Statistics
{
	int RR_SJF_migrations;
	int FCFS_RR_migrations;
	int total_steal;
	int total_fork;
	int total_killed;

	Statistics()
	{
		RR_SJF_migrations = FCFS_RR_migrations = total_steal = total_fork = total_killed = 0;
	}
};

enum ProcessorState
{
	BUSY,
	IDLE,
	OVERHEAT
};

enum ProgramInterface
{
	Interactive,
	Step_by_step,
	Silent
};

#endif