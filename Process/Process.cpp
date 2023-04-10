#include "Process.h"

Process::Process(ProcessInfo P_info, IO_process* IO_requests)
{
	P_data = P_info;
	IO = IO_requests;
	terminated = false;
}

Process::~Process()
{
	if (IO)
		delete[] IO;
}