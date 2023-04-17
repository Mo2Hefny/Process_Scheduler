
#ifndef _PRIORITY_QUEUE_T
#define _PRIORITY_QUEUE_T

#include "../Node.h"
#include "../../Process/Process.h"
#include <vector>
using namespace std;

template<typename T>
class PriorityQueue
{

private:

	Node<T>* backPtr;
	Node<T>* frontPtr;
	int queue_size;
public:


	PriorityQueue()
	{
		backPtr = nullptr;
		frontPtr = nullptr;
		queue_size = 0;
	}



	bool isEmpty() const
	{
		return (frontPtr == nullptr);
	}



	bool enqueue(T& newEntry)
	{
		Node<T>* newnode = new Node<T>(newEntry);
		if (!frontPtr)
		{
			frontPtr = newnode;
			backPtr = newnode;
			return true;
		}
		if (frontPtr->getItem() < newnode->getItem())
		{
			newnode->setNext(frontPtr);
			frontPtr = newnode;
			return true;
		}
		Node<T>* ptr = frontPtr;
		while (ptr->getNext())
		{
			if (ptr->getNext()->getItem() < newnode->getItem())
			{
				newnode->setNext(ptr->getNext());
				ptr->setNext(newnode);
				return true;
			}
			ptr = ptr->getNext();
		}
		backPtr->setNext(newnode);
		backPtr = newnode;
		return true;
	}


	bool dequeue(T& frntEntry)
	{
		if (isEmpty())
			return false;
		Node<T>* nodeToDeletePtr = frontPtr;
		frntEntry = frontPtr->getItem();
		frontPtr->setItem(NULL);
		frontPtr = frontPtr->getNext();
		// Queue is not empty; remove front
		if (nodeToDeletePtr == backPtr)	 // Special case: last node in the queue
			backPtr = nullptr;

		// Free memory reserved for the dequeued node
		delete nodeToDeletePtr;
		queue_size--;

		return true;

	}



	bool peek(T& frntEntry)  const
	{
		{
			if (isEmpty())
				return false;

			frntEntry = frontPtr->getItem();
			return true;

		}
	}



	int size()
	{
		return queue_size;
	}


	~PriorityQueue()
	{
		T temp;

		//Free (Dequeue) all nodes in the queue
		while (dequeue(temp));
	}



	//copy constructor
	PriorityQueue(const PriorityQueue& LQ)
	{
		queue_size = LQ.queue_size;
		Node<T>* NodePtr = LQ.frontPtr;
		if (!NodePtr) //LQ is empty
		{
			frontPtr = backPtr = nullptr;
			return;
		}

		//insert the first node
		Node<T>* ptr = new Node<T>(NodePtr->getItem());
		frontPtr = backPtr = ptr;
		NodePtr = NodePtr->getNext();

		//insert remaining nodes
		while (NodePtr)
		{
			Node<T>* ptr = new Node<T>(NodePtr->getItem());
			backPtr->setNext(ptr);
			backPtr = ptr;
			NodePtr = NodePtr->getNext();
		}
	}


};
#endif




#ifndef _PRIORITY_QUEUE_
#define _PRIORITY_QUEUE_


template<>
class PriorityQueue <Process*>
{

private:
	PQType type;
	Node<Process*>* backPtr;
	Node<Process*>* frontPtr;
	int queue_size;
public:


	PriorityQueue(PQType t= CT_order)
	{
		backPtr = nullptr;
		frontPtr = nullptr;
		type = t;
		queue_size = 0;
	}



	bool isEmpty() const
	{
		return (frontPtr == nullptr);
	}



	bool enqueue(Process*& newEntry)
	{
		queue_size++;
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
			if (type == CT_order)
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
			}
			else
			{
				if (newNodePtr->getItem()->GetArrivalTime() < frontPtr->getItem()->GetArrivalTime())
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
			}
			return true;
		}

		if (type == CT_order && newNodePtr->getItem()->GetCPUTime() < frontPtr->getItem()->GetCPUTime())
		{
			frontPtr->setNext(newNodePtr);
			frontPtr = newNodePtr;
			return true;
		}
		else if (type == AT_order && newNodePtr->getItem()->GetArrivalTime() < frontPtr->getItem()->GetArrivalTime())
		{
			frontPtr->setNext(newNodePtr);
			frontPtr = newNodePtr;
			return true;
		}

		Node<Process*>* temp1 = frontPtr, * temp2 = frontPtr->getNext();
		while (temp1 && temp2)
		{
			if (type == CT_order && newNodePtr->getItem()->GetCPUTime() <= temp2->getItem()->GetCPUTime())
			{
				temp1->setNext(newNodePtr);
				newNodePtr->setNext(temp2);
				return true;
			}
			else if (type == AT_order && newNodePtr->getItem()->GetArrivalTime() <= temp2->getItem()->GetArrivalTime())
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
	}


	bool dequeue(Process*& frntEntry)
	{
		if (isEmpty())
			return false;
		Node<Process*>* nodeToDeletePtr = frontPtr;
		frntEntry = frontPtr->getItem();
		frontPtr->setItem(NULL);
		frontPtr = frontPtr->getNext();
		// Queue is not empty; remove front
		if (nodeToDeletePtr == backPtr)	 // Special case: last node in the queue
			backPtr = nullptr;

		// Free memory reserved for the dequeued node
		delete nodeToDeletePtr;
		queue_size--;

		return true;

	}



	bool peek(Process*& frntEntry)  const
	{
		{
			if (isEmpty())
				return false;

			frntEntry = frontPtr->getItem();
			return true;

		}
	}



	int size()
	{
		return queue_size;
	}


	~PriorityQueue()
	{
		Process* temp;

	//Free (Dequeue) all nodes in the queue
	while (dequeue(temp));
    }



	//copy constructor
	PriorityQueue(const PriorityQueue& LQ)
	{
		queue_size = LQ.queue_size;
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


};
#endif