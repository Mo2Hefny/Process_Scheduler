#ifndef _Schedule_Manager_
#define _Schedule_Manager_

#include "../DEFS.h"
#include "../UI/UI.h"
#include "../Processors/Processors.h"
#include "../Process/Process.h"
#include "../Data Structures/DS.h"
#include <iostream>
#include <fstream>

using namespace std;
/**
* @class Scheduler
* 
* @brief Responsible for calling the processors, scheduling processes,
* and the whole structure of the simulation.
*/
class Scheduler
{
	UI* console;
	ifstream LoadedFile;
	ProcessorsInfo P_info;
	unsigned timestep;

	// Lists.
	LinkedQueue<Process*> New_List;
	LinkedQueue<Process*> BLK_List;
	LinkedQueue<Process*> Terminated_List;
	LinkedQueue<Process*> Parents_List;

	// Processors.
	FCFS* FCFS_Processors;
	SJF* SJF_Processors;
	RR* RR_Processors;
	Processor** Processors;

	// TEMPORARY SCHEDULING COUNTER
	int k;
public:
	/**
	* @brief Scheduler class constructor.
	*/
	Scheduler();

	/**
	* @brief Scheduler class destructor.
	*
	* @details Deallocates the UI console, the Processors, and the processes.
	*/
	~Scheduler();

	// Getters
	LinkedQueue<Process*>* GetNewList() {	return &New_List; }
	LinkedQueue<Process*>* GetBlockList() { return &BLK_List; }
	LinkedQueue<Process*>* GetTerminatedList() { return &Terminated_List; }
	LinkedQueue<Process*>* GetParentsList() { return &Parents_List; }
	ProcessorsInfo GetProcessorsInfo() const {	return P_info; }
	unsigned GetTimeStep() { return timestep; }
	Processor** GetProcessors() { return Processors; }

	/**
	* @brief Adds process to a chosen list.
	*
	* @param List - Chosen list.
	* @param p - Pointer to the process.
	*/
	void AddToList(LinkedQueue<Process*>*, Process*);

	/**
	* @brief Schedules the process to a processor's RDY list.
	*
	* @param p - Pointer to the process.
	*/
	void AddToReady(Process*);

	/**
	* @brief Extract the I/O data from the given string of pairs.
	*
	* @param IO_string - The I/O requests string that contains the needed data.
	* @param size - Number of the I/O requests of the process.
	*
	* @return I/O requests array.
	*/
	IO_process* ProcessIORequestsInput(string IO_string, int size);

	/**
	* @brief Load the Processors and Processes data from an input file.
	*/
	void LoadFile();

	/**
	* @brief Read system's information and initialize processors.
	*/
	void ReadInput();

	/**
	* @brief Simulation of the system.
	*/
	void Execute();
};

#endif