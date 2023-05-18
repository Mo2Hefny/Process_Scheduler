#ifndef _PROCESS_
#define _PROCESS_

#include "../DEFS.h"
#include <iostream>
using namespace std;

class Scheduler; //forward class declaration

/**
* @class Process
* 
* @brief The encapsulation of the input processes from the Loaded File.
*/
class Process
{
	ProcessInfo P_data;
	IO_process* IO;			// Array of I/O requests.
	bool terminated;
	bool late;
	bool RR_mig, SJF_mig;
	static unsigned int total_TRT, PID, total_early;
	Scheduler* manager;
	//int Transition_Time;		// Time of the last list transition.

	// Forking.
	Process* parent;
	Process* l_child;
	Process* r_child;

public:

	/**
	* @brief Process class copy constructor.
	*
	* @param other - The other constructor from which it copies the data.
	*/
	Process(const Process&);

	/**
	* @brief Process class constructor.
	*
	* @param P_info - The Process Information as AT, CT, PID, etc...
	* @param IO_requests - The I/O requests array for this process.
	* @param app - Pointer to the Scheduler mannager.
	*/
	Process(ProcessInfo, IO_process*, Scheduler*);

	/**
	* @brief Process class destructor.
	*
	* @details Deallocates the I/O requests array.
	*/
	~Process();

	// Setters.
	void SetResponseTime(int t) { P_data.RT = t; }
	static void SetForkPID(unsigned int id) { Process::PID = id; }
	void SetLate(bool b) { late = b; }
	void SetRRMig(bool b) { RR_mig = b; }
	void SetSJFMig(bool b) { SJF_mig = b; }
	//void SetTransitionTime(int t) { Transition_Time = t; }

	// Getters.
	int GetArrivalTime() const { return P_data.AT; }
	int GetResponseTime() const { return P_data.RT; }
	int GetCPUTime() const { return P_data.CT; }
	int GetDeadline() const { return P_data.Deadline; }
	bool GetLate() const { return late; }
	bool GetRRMig() const { return RR_mig; }
	bool GetSJFMig() const { return SJF_mig; }
	Scheduler* GetScheduler() const { return manager; }
	int GetRemainingTime() const { return P_data.CT - P_data.ET; }
	int GetExecutedTime() const { return P_data.ET; }
	int GetTurnAroundDuration() const { return P_data.TT - P_data.AT + 1; }
	int GetWaitingTime() const { return GetTurnAroundDuration() - P_data.ET; }
	int GetCurrWaitingTime(int TimeStep) const { return TimeStep - P_data.AT - P_data.ET; }
	static unsigned int GetTotalTRT() { return Process::total_TRT; }
	static unsigned int GetForkPID() { return Process::PID; }
	static unsigned int GetTotalEarly() { return Process::total_early; }

	//int GetTransitionTime() const { return Transition_Time; }
	ProcessInfo GetProcessInfo() const { return P_data; }
	IO_process* GetIORequests() { return IO; }

	bool HasChild() const {	return l_child || r_child; }
	bool HasParent() const { return parent; }
	bool IsTerminated() const { return terminated; }

	bool ForkChild(Process* child);

	/**
	* @brief Adds to the total execution time for the process.
	*/
	void ExecutingProcess() { P_data.ET++; }

	/**
	* @brief Terminates the process and its children.
	*/
	void Terminate();

	friend ostream& operator<< (ostream& out, const Process* process);

};

#endif