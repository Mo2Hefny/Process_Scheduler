#ifndef _PROCESSOR_
#define _PROCESSOR_

#include "../DEFS.h"
#include "../Data Structures/DS.h"
#include "../Process/Process.h"

class Scheduler; //forward class declaration

/**
* @class Processor
* 
* @brief The parent class of the processors which contains the processors data and analytics.
*/
class Processor
{
protected:
	Scheduler* manager;
	unsigned int time_left, time_busy, time_idle, cooldown;
	Process* RUN;
	ProcessorState state;

public:

	/**
	* @brief Processor class constructor.
	*/
	Processor();
	virtual ~Processor();

	/**
	* @brief Sets a pointer to the Scheduler Manager to call its functions.
	* 
	* @param Pointer to the Scheduler Manager.
	*/
	void SetScheduler(Scheduler*);

	virtual void AddToRDY(Process* p) = 0;

	// Getters
	unsigned int GetCooldown() const { return cooldown; }
	float GetPload() const { return time_busy * 100.0/ Process::GetTotalTRT(); }
	float GetPUtil() const { return time_busy * 100.0 / (time_busy + time_idle); }
	unsigned int GetTimeLeft() const { return time_left; }
	ProcessorState GetState() const { return state; }
	Process* GetRun() { return RUN; }

	// Setters
	void SetState(ProcessorState STATE) { state = STATE; }
	void SetRun(Process* p) { RUN = p; }

	/**
	* @brief  Adds to the total time left for the processor's queue.
	* 
	* @parame time - Time of the remaining CT of the process.
	*/
	void AddTimeleft(int time);

	/**
	* @brief  Adds to the busy/idle total time depending on the
	* processor's state.
	*/
	void AddTime();

	/**
	* @brief  Decrease from the total time left for the processor's
	* queue each timestep.
	*/
	void DecTimeleft() { (time_left > 0 && state == BUSY) ? time_left-- : 0; };

	/**
	* @brief Generates a random number to check if the processor overheats.
	*/
	void OverHeat();

	/**
	* @brief Moves all the processes to another processor's list when overheated.
	*/
	virtual void EmptyProcessor() = 0;

	/**
	* @brief The processor dequeues a process from its RDY list or enqueues it
	* depending on the mode.
	*
	* @param process - Reference to a pointer to the process.
	* @param mode - The processor acts as the donor if 0, acts as the receiver otherwise.
	*
	* @returns True on success, false otherwise.
	*/
	virtual bool Work_Stealing(Process*& process, int mode) = 0;
	virtual void Algorithm() = 0;
	virtual void Execute() = 0;
};

#endif