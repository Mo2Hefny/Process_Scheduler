#ifndef _UI_
#define _UI_

#include "../DEFS.h"
#include <string>
#include "../Process/Process.h"
#include "../Processors/Processors.h"

using namespace std;

class Scheduler; //forward class declaration

class UI
{
	
	Scheduler* manager;
public:
	UI(Scheduler* app) { manager = app; }

	void LoadFile(ProcessorsInfo&);
	


	// Testing
	void PrintOutput();
	void PrintRDY(int&, Processor**, Process**&);
	void PrintBLK();
	void PrintRUN(int, Process**);
	void PrintTRM();
	void PrintNew();

	string GetFileName();
};

#endif