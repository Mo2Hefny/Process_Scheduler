#ifndef _PRIORITY_QUEUE_
#define _PRIORITY_QUEUE_

#include "../Node.h"
#include "QueueADT.h"
#include <vector>
using namespace std;

class PriorityQueue :public QueueADT<Process*>
{

private:

	Node<Process*>* backPtr;
	Node<Process*>* frontPtr;
public:
	PriorityQueue();
	bool isEmpty() const;
	bool enqueue(Process*& newEntry);
	bool dequeue(Process*& frntEntry);
	bool peek(Process*& frntEntry)  const;
	~PriorityQueue();

	//copy constructor
	PriorityQueue(const PriorityQueue& LQ);
};
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: Queue()
The constructor of the Queue class.

*/

PriorityQueue::PriorityQueue()
{
	backPtr = nullptr;
	frontPtr = nullptr;

}
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: isEmpty
Sees whether this queue is empty.

Input: None.
Output: True if the queue is empty; otherwise false.
*/
bool PriorityQueue::isEmpty() const
{
	return (frontPtr == nullptr);
}

/////////////////////////////////////////////////////////////////////////////////////////



/*Function:enqueue
Adds newEntry at the back of this queue.

Input: newEntry .
Output: True if the operation is successful; otherwise false.
*/

bool PriorityQueue::enqueue(Process*& newEntry)
{
	Node<Process*>* newNodePtr = new Node<Process*>(newEntry);
	// Insert the new node
	if (isEmpty())//special case if this is the first node to insert
	{
		frontPtr = newNodePtr; // The queue is empty
		backPtr = newNodePtr;
		return true;
	}
	if (!frontPtr->getNext())
	{
		if (newNodePtr->getItem()->GetCPUTime() < frontPtr->getItem()->GetCPUTime())
		{
			newNodePtr->setNext(frontPtr);
			backPtr = frontPtr;
			frontPtr = newNodePtr;
		}
		else
		{
			frontPtr->setNext(newNodePtr);
			backPtr = newNodePtr;
		}
		return true;
	}
	if (newNodePtr->getItem()->GetCPUTime() < frontPtr->getItem()->GetCPUTime())
	{
		frontPtr->setNext(newNodePtr);
		frontPtr = newNodePtr;
		return true;
	}
	Node<Process*>* temp1 = frontPtr, * temp2 = frontPtr->getNext();
	while (temp1 && temp2)
	{
		if (newNodePtr->getItem()->GetCPUTime() <= temp2->getItem()->GetCPUTime() && newNodePtr->getItem()->GetCPUTime() >= temp1->getItem()->GetCPUTime())
		{
			temp1->setNext(newNodePtr);
			newNodePtr->setNext(temp2);
			return true;
		}
		temp1 = temp2;
		temp2 = temp2->getNext();
	}
	backPtr->setNext(newNodePtr);
	backPtr = newNodePtr;
	return true;
} // end enqueue


/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Function: dequeue
Removes the front of this queue. That is, removes the item that was added
earliest.

Input: None.
Output: True if the operation is successful; otherwise false.
*/

bool PriorityQueue::dequeue(Process*& frntEntry)
{
	if (isEmpty())
		return false;

	Node<Process*>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();
	// Queue is not empty; remove front
	if (nodeToDeletePtr == backPtr)	 // Special case: last node in the queue
		backPtr = nullptr;

	// Free memory reserved for the dequeued node
	delete nodeToDeletePtr;

	return true;

}
/////////////////////////////////////////////////////////////////////////////////////////
/*
Function: peek
copies the front of this queue to the passed param. The operation does not modify the queue.

Input: None.
Output: The front of the queue.
*/
bool PriorityQueue::peek(Process*& frntEntry) const
{
	if (isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;

}
///////////////////////////////////////////////////////////////////////////////////
/*
Function: destructor
removes all nodes from the queue by dequeuing them
*/
PriorityQueue::~PriorityQueue()
{
	Process* temp;

	//Free (Dequeue) all nodes in the queue
	while (dequeue(temp));
}

PriorityQueue::PriorityQueue(const PriorityQueue& LQ)
{
	Node<Process*>* NodePtr = LQ.frontPtr;
	if (!NodePtr) //LQ is empty
	{
		frontPtr = backPtr = nullptr;
		return;
	}

	//insert the first node
	Node<Process*>* ptr = new Node<Process*>(NodePtr->getItem());
	frontPtr = backPtr = ptr;
	NodePtr = NodePtr->getNext();

	//insert remaining nodes
	while (NodePtr)
	{
		Node<Process*>* ptr = new Node<Process*>(NodePtr->getItem());
		backPtr->setNext(ptr);
		backPtr = ptr;
		NodePtr = NodePtr->getNext();
	}
}

#endif