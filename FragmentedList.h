#ifndef _FRAG_LIST_H_
#define _FRAG_LIST_H_

#include <iostream>
using std::cout;

template <class T>
struct _fragListNode
{
	struct _fragListNode *prev;
	struct _fragListNode *next;
	T data;
};

template <class T>
class FragmentedList
{
public:

	typedef struct _fragListNode<T> fragListNode;
	FragmentedList()
	{
		list = NULL;
		head = NULL;
		Size = 0;
		fragmented = false;
	}

	FragmentedList(int initialSize)
	{
		resize(initialSize);	
	}

	void resize(int nSize)
	{
		Size = nSize;
		blockSize = Size;
		list = new fragListNode[nSize];
		head = list;
		link();
	}

	void link()
	{
		//Link each node together, even though they are contiguous
		list[0].prev = NULL;
		for(int i = 0; i < Size-1; i++)
		{
			list[i].next = &list[i+1];
			list[i+1].prev = &list[i];
		}
		list[Size-1].next = NULL;
	}

	int size()
	{
		return Size;
	}

	void defrag()
	{
		/*
		 * An attempt to just move the memory up 
		if(blockSize > Size)
		{
			T tempA;
			T tempB;
			fragListNode *it = head;
			int i = 0;
			
			for(; i < Size && it == &list[i]; i++, it = it->next);

			
		}*/
		
		fragListNode *temp = new fragListNode[Size];
		fragListNode *it = head;

		for(int i = 0; i < Size; i++)
		{
			temp[i].data = it->data;
			it = it->next;
		}
		head = temp;
		delete[] list;
		list = head;
		link();	
		blockSize = Size;		
		fragmented = false;	
	}

	T &operator[] (int index)
	{
		return at(index)->data;
	}

	fragListNode *at(int index)
	{
		if(!fragmented) return &list[index];
		else
		{
			if(index < lastIti)
			{
				lastIti = 0;
				lastIt = head;
			}			
			for(;lastIti < index; lastIti++, lastIt = lastIt->next);
			return lastIt;
		
		}
	
	}

	void push_front(T item);
	void push_back(T item);

	void insert(T item, int index)
	{
		fragListNode *nnode = new fragListNode;
		nnode->data = item;
		fragListNode *temp = at(index);
		nnode->next = temp;
		if(index > 0)
		{
			temp->prev->next = nnode;
			nnode->prev = temp->prev;
			temp->prev = nnode;
		}
		else
		{
			nnode->prev = NULL;
			head = nnode;
		}
		fragmented = true;
		Size++;
	}

	void erase(unsigned int index)
	{
		fragListNode *n = at(index);

		if(n->next == NULL)
		{
			n->prev->next = NULL;
		}
		else if(n->prev == NULL)
		{
			//keep separate memory pointer
			fragmented = true;
			n->next->prev = NULL;
			head = n->next;
		}
		else
		{
			fragmented = true;
			n->next->prev = n->prev;
			n->prev->next = n->next;
		}

		if(!(n >= list && n <= list + (sizeof(fragListNode) * blockSize)))
		{	
			delete n;
		}
		Size--;		
		if(index < lastIti)
		{
			lastIti--;
			return;
		}
		if(index == lastIti)
		{
			lastIti = 0;
			lastIt = head;
		}
	}
private:

	fragListNode *list; //a pointer to the lists main block of memory
	fragListNode *head;	//a pointer to the first item in the list
	fragListNode *lastIt; //a pointer to the last iterator used
	int lastIti; //index of the last iterator used
	int Size; //number of items in the list
	int blockSize; //size (in items) of the lists main memory block
	bool fragmented; 
};

#endif