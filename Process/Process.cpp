#include "Process.h"

Process::Process(ProcessInfo P_info, IO_process* IO_requests)
{
	P_data = P_info;
	IO = IO_requests;
	terminated = false;
	Transition_Time = -1;
}

Process::Process(const Process& other)
{
	P_data = other.P_data;
	IO = new IO_process[other.P_data.IO_requests];
	for (int i = 0; i < other.P_data.IO_requests; i++)
	{
		IO[i].IO_R = other.IO[i].IO_R;
		IO[i].IO_D = other.IO[i].IO_D;
	}
	terminated = other.terminated;
	l_child = other.l_child;
	r_child = other.r_child;
	Transition_Time = other.Transition_Time;
}

Process::~Process()
{
	if (IO)
		delete[] IO;
}