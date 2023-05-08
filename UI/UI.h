#ifndef _UI_
#define _UI_

#include "../DEFS.h"
#include <string>
#include "../Process/Process.h"
#include "../Processors/Processors.h"

using namespace std;

class Scheduler; //forward class declaration
/**
* @class UI
* @brief The User Interface of the simulation.
* 
* @details Responsible for the communication between the user and
* the Scheduler.
*/
class UI
{
	Scheduler* manager;
	ProgramInterface mode;
public:
	/**
	* @brief UI class constructor.
	* 
	* @param app - Pointer to the scheduling manager.
	*/
	UI(Scheduler* app);

	/**
	* @brief Allows the user to monitor the processes transition between different states.
	*/
	void PrintOutput();

	/**
	* @brief Print Processors' RDY Lists.
	*
	* @param run_size - Number of processes in RUN state.
	* @param Processors - System's Processors.
	* @param run - RUN List of processors.
	*/
	void PrintRDY(int&, Processor**, Process**&);

	/**
	* @brief Print Processes in BLK List.
	*/
	void PrintBLK();

	/**
	* @brief Print Processes in RUN state.
	*
	* @param run_size - Number of processes in RUN state.
	* @param run - RUN List of processors.
	*/
	void PrintRUN(int, Process**);

	/**
	* @brief Print Processes in TRM List.
	*/
	void PrintTRM();

	/**
	* @brief Reads the File name from the user.
	*
	* @return File name.
	*/
	string GetFileName();
};

#endif