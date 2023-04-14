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
		cout << "\nprinting list contents:\n\n";
		Node<T>* p = Head;

		while (p)
		{
			cout << "[ " << p->getItem() << " ]";
			cout << "--->";
			p = p->getNext();
		}
		cout << "NULL\n";
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
		list_size = 0;
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
	}



	////////////////     Requirements   ///////////////////
	//
	// Implement the following member functions


	//[1]Enqueue 
	//inserts a new node at end if the list
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

	//[2]Find 
	//searches for a given value in the list, returns true if found; false otherwise.
	bool Find(int Key){}

	//[3]CountOccurance
	//returns how many times a certain value appeared in the list
	int CountOccurance(const T& value);

	//[4] DeleteFirst
	//Deletes the first node in the list
	T dequeue()
	{
		if (!Head)
			return nullptr;
		Node<T>* p = Head;
		Head = Head->getNext();
		T excess = p->getItem();
		p->setItem(NULL);
		delete p;
		list_size--;
		return excess;
	}


	//[5] DeleteLast
	//Deletes the last node in the list
	void DeleteLast(){}

	//[6] DeleteNode
	//deletes the first node with the given value (if found) and returns true
	//if not found, returns false
	//Note: List is not sorted
	bool DeleteNode(const T& value){}

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

	T DeletePos(int pos)
	{
		Node<T>* dummy = new Node<T>;
		dummy->setNext(Head);
		Node<T>* prev = dummy, *p = Head;
		while (p->getNext() && --pos)
		{
			p = p->getNext();
			prev = prev->getNext();
		}
		prev->setNext(p->getNext());
		delete dummy;
		T excess = p->getItem();
		p->setItem(NULL);
		delete p;
		list_size--;
		return excess;
	}

	int size() const { return list_size; }
};

#endif	
