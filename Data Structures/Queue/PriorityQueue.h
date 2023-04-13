#ifndef _PRIORITY_QUEUE_
#define _PRIORITY_QUEUE_

#include "../Node.h"
#include "../../Process/Process.h"
#include <vector>
using namespace std;

class PriorityQueue
{

private:
	PQType type;
	Node<Process*>* backPtr;
	Node<Process*>* frontPtr;
public:
	PriorityQueue(PQType = CT_order);
	bool isEmpty() const;
	bool enqueue(Process*& newEntry);
	bool dequeue(Process*& frntEntry);
	bool peek(Process*& frntEntry)  const;
	~PriorityQueue();

	//copy constructor
	PriorityQueue(const PriorityQueue& LQ);
};
#endif