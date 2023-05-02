#include "Process.h"

/**
* @brief Process class constructor.
* 
* @param P_info - The Process Information as AT, CT, PID, etc...
* @param IO_requests - The I/O requests array for this process.
*/
Process::Process(ProcessInfo P_info, IO_process* IO_requests)
{
	P_data = P_info;
	IO = IO_requests;
	terminated = false;
	//Transition_Time = -1;

	parent = nullptr;
	l_child = nullptr;
	r_child = nullptr;
}

/**
* @brief Process class copy constructor.
* 
* @param other - The other constructor from which it copies the data.
*/
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
	//Transition_Time = other.Transition_Time;
}

/**
* @brief Process class destructor.
*
* @details Deallocates the I/O requests array.
*/
Process::~Process()
{
	if (IO)
		delete[] IO;
}

bool Process::ForkChild(Process* child)
{
	if (l_child && r_child)
		return false;
	child->parent = this;
	if (!l_child)
	{
		l_child = child;
		return true;
	}
	r_child = child;

	return true;
}

/**
* @breif Terminates the process and its children.
* 
* @parameter time - Termination time.
*/
void Process::Terminate(int time)
{
	terminated = true;
	if (parent && !parent->IsTerminated())
	{
		if (parent->l_child == this) parent->l_child = nullptr;
		else if (parent->r_child == this) parent->r_child = nullptr;
	}


	P_data.TT = time;
	if (l_child)	l_child->Terminate(time);
	if (r_child)	r_child->Terminate(time);
}