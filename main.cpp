#include "Scheduler/Scheduler.h"



int main()
{
	//Scheduler* schedule_manager = new Scheduler;
	CircularList<int> c;
	int n = 0;
	while (n != -1)
	{
		cin >> n;
		c.InsertEnd(n);
	}
	c.PrintList();
	c.pop();
	c.pop();
	c.PrintList();

	//if (schedule_manager)
		//delete schedule_manager;
	return 0;
}