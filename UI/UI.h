#ifndef _UI_
#define _UI_

#include "../DEFS.h"
#include <fstream>
#include <iostream>

using namespace std;

class Scheduler; //forward class declaration

class UI
{
	ifstream LoadedF;
	Scheduler* manager;
public:
	UI(Scheduler* app) { manager = app; }

	void LoadFile(ProcessorsInfo&);
	ProcessorsInfo GetProcessorsInfo()	const;
	ProcessorsInfo GetProcessorsInfo()	const;
};

#endif