#ifndef _PROCESS_
#define _PROCESS_

#include "../DEFS.h"
#include <iostream>
using namespace std;

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
	*/
	Process(ProcessInfo, IO_process*);

	/**
	* @brief Process class destructor.
	*
	* @details Deallocates the I/O requests array.
	*/
	~Process();

	// Setters.
	void SetResponseTime(int t) { P_data.RT = t; }
	//void SetTransitionTime(int t) { Transition_Time = t; }

	// Getters.
	int GetArrivalTime() const { return P_data.AT; }
	int GetResponseTime() const { return P_data.RT; }
	int GetCPUTime() const { return P_data.CT; }
	int GetRemainingTime() const { return P_data.CT - P_data.ET; }
	int GetExecutedTime() const { return P_data.ET; }
	int GetTurnAroundDuration() const { return P_data.TT - P_data.AT; }
	int GetWaitingTime() const { return GetTurnAroundDuration() - P_data.CT; }
	//int GetTransitionTime() const { return Transition_Time; }
	ProcessInfo GetProcessInfo() const { return P_data; }
	IO_process* GetIORequests() { return IO; }

	bool HasChild() const {	return l_child || r_child; }
	bool HasParent() const { return parent; }
	bool IsTerminated() const { return terminated; }

	bool ForkChild(Process* child);

	/**
	* @breif Adds to the total execution time for the process.
	*/
	void ExecutingProcess() { P_data.ET++; }

	/**
	* @breif Terminates the process and its children.
	* 
	* @parameter time - Termination time.
	*/
	void Terminate(int time);

	friend ostream& operator<< (ostream& out, const Process* process);

};

#endif