
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



	bool enqueue(T& newEntry,int priority)
	{
		Node<T>* newnode = new Node<T>(newEntry,priority);
		queue_size++;
		if (!frontPtr)
		{
			frontPtr = newnode;
			backPtr = newnode;
			return true;
		}
		if (frontPtr->getPriority() < newnode->getPriority())
		{
			newnode->setNext(frontPtr);
			frontPtr = newnode;
			return true;
		}
		Node<T>* ptr = frontPtr;
		while (ptr->getNext())
		{
			if (ptr->getNext()->getPriority() < newnode->getPriority())
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

	void enqueuelast(T& newEntry)
	{
		Node<T>* newnode = new Node<T>(newEntry);
		if (isEmpty())
			frontPtr = newnode;
		else
			backPtr->setNext(newnode);
		backPtr = newnode;
		queue_size++;
	}

	void PrintList()	const
	{
		Node<Process*>* p = frontPtr;

		while (p)
		{
			cout << p->getItem();
			cout << (p->getNext() ? ", " : " ");
			p = p->getNext();
		}
		printf("\033[0m\n");
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




