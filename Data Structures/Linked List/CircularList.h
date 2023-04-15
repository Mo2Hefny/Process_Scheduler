#ifndef _CIRCULAR_LIST_
#define _CIRCULAR_LIST_

#include "../Node.h"

template <typename T>
class CircularList
{
	Node<T>* Head;	//Pointer to the head of the list
	//You can add tail pointer too (depending on your problem)
public:


	CircularList()
	{
		Head = nullptr;
	}

	//List is being desturcted ==> delete all items in the list
	~CircularList()
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

		while (p->getNext() != Head)
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

	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: DeleteAll.
	* Deletes all nodes of the list.
	*/
	void DeleteAll()
	{
		if (!Head)
			return;
		Node<T>* p = Head->getNext();
		while (p->getNext() != Head)
		{
			Node<T>* excess = p;
			p = p->getNext();
			delete excess;
		}
		delete Head;
	}



	////////////////     Requirements   ///////////////////
	//
	// Implement the following member functions


	//[1]InsertEnd 
	//inserts a new node at end if the list
	void InsertEnd(const T& data)
	{
		Node<T>* R = new Node<T>(data);
		R->setNext(Head);					// Tail points to the beginning. (Circular)
		Node<T>* p = Head;
		if (!Head)
		{
			Head = R;
			R->setNext(Head);
			return;
		}
		while (p->getNext() != Head)
		{
			p = p->getNext();
		}
		p->setNext(R);
	}

	//[2]Find 
	//searches for a given value in the list, returns true if found; false otherwise.
	bool Find(int Key);

	//[3]CountOccurance
	//returns how many times a certain value appeared in the list
	int CountOccurance(const T& value){}

	//[4] DeleteFirst
	//Deletes the first node in the list
	Node<T>* pop()
	{
		Node<T>* excess = Head;
		if (!Head)
			return NULL;
		Node<T>* p = Head;
		if (p->getNext() == Head)
		{
			Head = NULL;
			return excess;
		}
		while (p->getNext() != Head)
			p = p->getNext();
		Head = Head->getNext();
		p->setNext(Head);
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
	bool DeleteNodes(const T& value){}

	//[8]Merge
	//Merges the current list to another list L by making the last Node in the current list 
	//point to the first Node in list L
	void Merge(const CircularList& L){}

	//[9] Reverse
	//Reverses the linked list (without allocating any new Nodes)
	void Reverse(){}

	void Rotate()
	{
		if (!Head)
			return;
		Head = Head->getNext();
	}

	Node<T>* GetHead() const { return Head; }

};

#endif