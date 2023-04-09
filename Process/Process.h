#ifndef _PROCESS_
#define _PROCESS_

#include "../DEFS.h"

class Process
{
	ProcessInfo P_data;
	IO_process* IO;			// Array of I/O requests.
	bool terminated;
	Process* child;

public:
	Process();
	~Process();
};

#endif