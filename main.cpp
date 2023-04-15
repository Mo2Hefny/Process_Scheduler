#include "Scheduler/Scheduler.h"


int main()
{
	/*Scheduler* schedule_manager = new Scheduler;

	if (schedule_manager)
		delete schedule_manager;*/
	LinkedList<int> l,k;
	int x;
	cin >> x;
	while (x != -1)
	{
		l.enqueue(x);
		cin >> x;
	}
	l.PrintList();
	k = l;
	k.PrintList();
	l.dequeue(x);
	l.PrintList();
	k.PrintList();
	/*l.PrintList();
	k.dequeue(x);
	k.PrintList();
	l.PrintList();*/
	return 0;
}