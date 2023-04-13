#ifndef _UI_
#define _UI_

#include "../DEFS.h"
#include <string>
#include "../Process/Process.h"

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
	void PrintNew();

	string GetFileName();
};

#endif