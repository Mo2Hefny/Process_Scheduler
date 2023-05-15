#include "Process.h"
#include "../Scheduler/Scheduler.h"
#include <windows.h>

unsigned int Process::total_TRT = 0, Process::total_early = 0;
unsigned int Process::PID;

/**
* @brief Process class constructor.
* 
* @param P_info - The Process Information as AT, CT, PID, etc...
* @param IO_requests - The I/O requests array for this process.
* @param app - Pointer to the Scheduler mannager.
*/
Process::Process(ProcessInfo P_info, IO_process* IO_requests, Scheduler* app)
{
	P_data = P_info;
	IO = IO_requests;
	terminated = late = false;
	manager = app;
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
	late = other.late;
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
		Process::PID++;
		return true;
	}
	r_child = child;
	Process::PID++;

	return true;
}

/**
* @brief Terminates the process and its children.
*/
void Process::Terminate()
{
	if (terminated) return;
	
	int time = manager->GetTimeStep();

	terminated = true;
	if (parent && !parent->IsTerminated())
	{
		if (parent->l_child == this) parent->l_child = nullptr;
		else if (parent->r_child == this) parent->r_child = nullptr;
	}

	P_data.TT = time;
	Process::total_TRT += P_data.TT - P_data.AT + 1;

	if (GetDeadline() > time)
		Process::total_early++;
	else
		late = true;

	if (l_child)	l_child->Terminate();
	if (r_child)	r_child->Terminate();
}

ostream& operator<< (ostream& out, const Process* process)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	GetConsoleScreenBufferInfo(consoleHandle, &consoleInfo);
	WORD currentColor = consoleInfo.wAttributes;
	if (process->GetDeadline() <= process->manager->GetTimeStep() && !process->IsTerminated() || process->late)
	{
		if (process->parent && currentColor != 4)
			printf("\033[3;38;2;239;113;38;48;2;15;99;113m");
		else if (process->HasChild() && currentColor != 4)
			printf("\033[3;38;2;239;113;38;48;2;255;255;255m");
		else
			printf("\x1B[3;38;2;239;113;38;1;4m");
	}
	else if (process->parent && currentColor != 4)
	{
		printf("\033[3;48;2;15;99;113;30m");
	}
	else if (process->HasChild() && currentColor != 4)
	{
		printf("\033[3;48;2;255;255;255;30m");
	}
	else
		printf("\x1B[93m");
	
	out << process->P_data.PID << '(' << process->GetRemainingTime() << ')' << '(' << process->GetDeadline() << ')';

	printf("\033[0m");
	return out;
}