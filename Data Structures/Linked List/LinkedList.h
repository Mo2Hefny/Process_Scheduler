#ifndef _LINKEDLIST
#define _LINKEDLIST

#include "../Node.h"
#include <iostream>
using namespace std;
template <typename T>
class LinkedList
{
private:
	Node<T>* Head;	//Pointer to the head of the list
	int list_size;
	//You can add tail pointer too (depending on your problem)
public:


	LinkedList()
	{
		Head = nullptr;
		list_size = 0;
	}

	//List is being desturcted ==> delete all items in the list
	~LinkedList()
	{
		DeleteAll();
	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: PrintList.
	* prints the values of all nodes in a linked list.
	*/
	void PrintList()	const
	{
		Node<T>* p = Head;

		while (p)
		{
			cout << p->getItem();
			cout << (p->getNext() ? ", " : " ");
			p = p->getNext();
		}
		printf("\033[0m\n");
	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: InsertBeg.
	* Creates a new node and adds it to the beginning of a linked list.
	*
	* Parameters:
	*	- data : The value to be stored in the new node.
	*/
	void InsertBeg(const T& data)
	{
		Node<T>* R = new Node<T>(data);
		R->setNext(Head);
		Head = R;
		list_size++;
	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: DeleteAll.
	* Deletes all nodes of the list.
	*/
	void DeleteAll()
	{
		Node<T>* P = Head;
		while (Head)
		{
			P = Head->getNext();
			delete Head;
			Head = P;
		}
		list_size = 0;
	}



	////////////////     Requirements   ///////////////////
	//
	// Implement the following member functions


	//[1]Enqueue 
	//inserts a new node at end of the list
	void enqueue(const T& data)
	{
		if (!Head)
		{
			InsertBeg(data);
			return;
		}

		Node<T>* R = new Node<T>(data);
		Node<T>* p = Head;
		while (p->getNext())
			p = p->getNext();
		p->setNext(R);
		list_size++;
	}

	bool peek(T& frntEntry)	const
	{
		if (isempty())	return false;
		frntEntry = Head->getItem();
		return true;
	}

	//[2]Find 
	//searches for a given value in the list, returns true if found; false otherwise.
	bool Find(int Key){}

	//[3]CountOccurance
	//returns how many times a certain value appeared in the list
	int CountOccurance(const T& value){}

	//[4] DeleteFirst
	//Deletes the first node in the list
	bool dequeue(T& frntEntry)
	{
		if (isempty())
			return false;

		Node<T>* nodeToDeletePtr = Head;
		frntEntry = Head->getItem();
		Head->setItem(NULL);
		Head = Head->getNext();
		// Queue is not empty;
		// Free memory reserved for the dequeued node
		delete nodeToDeletePtr;
		list_size--;
		return true;

	}


	//[5] DeleteLast
	//Deletes the last node in the list
	bool DeleteLast(T& frntEntry)
	{
		if (isempty())
			return false;

		Node<T>* nodeToDeletePtr = Head;
		Node<T>* dummy = new Node<T>;
		dummy->setNext(Head);
		Node<T>* prev = dummy;
		while (nodeToDeletePtr->getNext())
		{
			prev = prev->getNext();
			nodeToDeletePtr = nodeToDeletePtr->getNext();
		}
		if (Head == nodeToDeletePtr)	Head = NULL;
		frntEntry = nodeToDeletePtr->getItem();
		nodeToDeletePtr->setItem(NULL);
		prev->setNext(NULL);
		// Queue is not empty;
		// Free memory reserved for the dequeued node
		delete nodeToDeletePtr;
		delete dummy;
		list_size--;
		return true;
	}

	//[6] DeleteNode
	//deletes the first node with the given value (if found) and returns true
	//if not found, returns false
	//Note: List is not sorted
	bool DeleteNode(T& frntEntry, const int& ID)
	{
		if (!Head)	return false;
		Node<T>* dummy = new Node<T>;
		dummy->setNext(Head);
		Node<T>* p = Head, *prev = dummy;
		while (p)
		{
			if (p->getItem()->GetProcessInfo().PID == ID)
			{
				frntEntry = p->getItem();
				if (p == Head)	Head = Head->getNext();
				prev->setNext(p->getNext());
				p->setItem(NULL);
				delete p;
				delete dummy;
				list_size--;
				return true;
			}
			prev = prev->getNext();
			p = p->getNext();
		}
		delete dummy;
		return false;
	}

	//[7] DeleteNodes
	//deletes ALL node with the given value (if found) and returns true
	//if not found, returns false
	//Note: List is not sorted
	bool DeleteNodes(const T& value)
	{}

	//[8]Merge
	//Merges the current list to another list L by making the last Node in the current list 
	//point to the first Node in list L
	void Merge(const LinkedList& L)
	{}

	//[9] Reverse
	//Reverses the linked list (without allocating any new Nodes)
	void Reverse()
	{}

	bool GetPos(T& frntEntry, const int& index)
	{
		if (!Head || index > list_size)	return false;
		Node<T>* p = Head;
		int i = 0;
		while (p)
		{
			if (i == index)
			{
				frntEntry = p->getItem();
				return true;
			}
			p = p->getNext();
			i++;
		}
		return false;
	}

	bool DeletePos(T& frntEntry, const int& index)
	{
		if (!Head || index > list_size)	return false;
		Node<T>* dummy = new Node<T>;
		dummy->setNext(Head);
		Node<T>* p = Head, * prev = dummy;
		int i = 0;
		while (p)
		{
			if (i == index)
			{
				frntEntry = p->getItem();
				if (p == Head)	Head = Head->getNext();
				prev->setNext(p->getNext());
				p->setItem(NULL);
				delete p;
				delete dummy;
				list_size--;
				return true;
			}
			prev = prev->getNext();
			p = p->getNext();
			i++;
		}
		delete dummy;
		return false;
	}

	void FindOrphans(LinkedList <T>& orph_list) {
		Node<T>* dummy = new Node<T>;
		dummy->setNext(Head);
		Node<T>* prev = dummy, * p = Head;
		while (p)
		{
			if (p->getItem()->IsTerminated())
			{
				list_size--;
				if (p == Head)
					Head = Head->getNext();
				T item = p->getItem();
				orph_list.InsertBeg(item);
				prev->setNext(p->getNext());
				p->setItem(NULL);
				delete p;
				p = prev->getNext();
			}
			else
			{
				prev = prev->getNext();
				p = p->getNext();
			}
		}
		delete dummy;
	}

	int size() const { return list_size; }


	bool isempty()	const
	{
		if (!Head)
			return true;
		else
			return false;
	}

	LinkedList(const LinkedList<T>& LQ)
	{
		Head = nullptr;
		list_size = LQ.list_size;
		Node<T>* NodePtr = LQ.Head;
		if (!NodePtr) //LQ is empty
		{
			Head = nullptr;
			return;
		}

		while (NodePtr)
		{
			Node<T>* newnode = new Node<T>(NodePtr->getItem());
			if (!Head)
				Head = newnode;
			else
			{
				Node<T>* temp = Head;
				while (temp->getNext())
				{
					temp = temp->getNext();
				}
				temp->setNext(newnode);
			}
			NodePtr = NodePtr->getNext();
		}
	}
};

#endif	
