#ifndef _UI_
#define _UI_

#include "../DEFS.h"
#include <fstream>
#include <iostream>
#include "../Process/Process.h"

using namespace std;

class Scheduler; //forward class declaration

class UI
{
	ifstream LoadedFile;
	Scheduler* manager;
public:
	UI(Scheduler* app) { manager = app; }

	void LoadFile(ProcessorsInfo&);
	IO_process* ProcessIORequestsInput(string IO_string, int size);
	void PrintOutput();
};

#endif