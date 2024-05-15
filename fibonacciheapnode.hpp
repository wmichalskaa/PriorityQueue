#ifndef FIBONACCI_HEAP_NODE_HPP
#define FIBONACCI_HEAP_NODE_HPP

#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include "Node.hpp"
#include "Queue.hpp"

//representing a node in the Fibonacci heap
template <typename T>
class FibonacciHeapNode
{
public:
	T value;
	FibonacciHeapNode<T>* parent;
	FibonacciHeapNode<T>* child;
	FibonacciHeapNode<T>* left;
	FibonacciHeapNode<T>* right;
	int degree;
	int priority;
	bool mark;

	FibonacciHeapNode(T key, int prio) : value(key), priority(prio), parent(nullptr), child(nullptr), left(this), right(this), degree(0), mark(false) {}
	~FibonacciHeapNode() {}
};

//representing a Fibonacci heap
template <typename T>
class FibonacciHeap : public Queue<T>
{
public:
	FibonacciHeap() : minNode(nullptr), numOfNodes(0) {}
	FibonacciHeap(std::vector<Node<T>>, size_t);
	FibonacciHeap(const FibonacciHeap<T>& copy);
	~FibonacciHeap();
	
	void insert(T key, int priority) override;
	Node<T> exctractMax() override;
	Node<T> findMax() override;
	Node<T> peek();
	void modifyKey(FibonacciHeapNode<T>* node, T newKey) override;
	void modifyKey(T value, int priority) override {}
	size_t getSize() override;
	void display() override;

	T getElement(int index);
	int getPriority(int index);
	void setElement(int index, T value);


private:
	FibonacciHeapNode<T>* minNode;
	size_t numOfNodes;

	void consolidate();
	void link(FibonacciHeapNode<T>* node1, FibonacciHeapNode<T>* node2);
	void cut(FibonacciHeapNode<T>* node, FibonacciHeapNode<T>* parent);
};

//destructor
template <typename T>
FibonacciHeap<T>::~FibonacciHeap()
{
	if (minNode != nullptr)
	{
		FibonacciHeapNode<T>* current = minNode;

		/*
		do
		{
			FibonacciHeapNode<T>* next = current->right;
			delete current;
			current = next;

		}while (current != minNode);
		*/

		while (current != minNode)
		{
			FibonacciHeapNode<T>* next = current->right;
			delete current;
			current = next;
		}
	}
}

//Copy constructor
template <typename T>
FibonacciHeap<T>::FibonacciHeap(const FibonacciHeap<T>& copy)
{
	minNode = copy.minNode;
	numOfNodes = copy.numOfNodes;
}

//constructor
template <typename T>
FibonacciHeap<T>::FibonacciHeap(std::vector<Node<T>> nodes, size_t size) : numOfNodes(size)
{
	minNode = nullptr;
	for (size_t i = 0; i < size; i++)
	{
		FibonacciHeapNode<T>* newNode = new FibonacciHeapNode<T>(nodes[i].value, nodes[i].priority);
		
		if (minNode == nullptr)
		{
			minNode = newNode;
		}
		else
		{
			minNode->left->right = newNode;
			newNode->left = minNode->left;
			minNode->left = newNode;
			newNode->right = minNode;
			if (newNode->priority > minNode->priority)
			{
				minNode = newNode;
			}
		}
	}
}

//Inserts a new node into the Fibonacci heap
template <typename T>
void FibonacciHeap<T>::insert(T value, int priority)
{
	//create a new node
	FibonacciHeapNode<T>* newNode = new FibonacciHeapNode<T>(value, priority);
	//if the heap is empty
	if (minNode == nullptr)
	{
		minNode = newNode;
	}
	else
	{
		//insert the new node into the list of roots
		minNode->left->right = newNode;
		newNode->left = minNode->left;
		minNode->left = newNode;
		newNode->right = minNode;
		//update the min node if necessary
		if (newNode->priority > minNode->priority)
		{
			minNode = newNode;
		}
	}
	numOfNodes++;
}

////Removes and returns the node with the maximum priority
template <typename T>
Node<T> FibonacciHeap<T>::exctractMax()
{
	if (minNode == nullptr)
	{
		return Node<T>();	//returns an empty node
	}
	//creating a copy of the max node
	FibonacciHeapNode<T>* maxNode = minNode;

	if (maxNode->child != nullptr)
	{
		//if the max node has children, add them to the list of roots
		FibonacciHeapNode<T>* start = maxNode->child;
		FibonacciHeapNode<T>* current = maxNode->child;

		/*
		do
		{
			FibonacciHeapNode<T>* next = current->right;

			current->parent = nullptr;
			current->right = minNode->right;
			current->left = minNode;

			minNode->right->left = current;
			minNode->right = current;

			if (current->priority > minNode->priority)
			{
				minNode = current;
			}
			current = next;
		} 
		while (current != start);
		*/

		while (current != start)
		{
			FibonacciHeapNode<T>* next = current->right;
			//disconnect the current node from its parent
			current->parent = nullptr;
			current->right = minNode->right;
			current->left = minNode;

			//insert the current node into the list of roots
			minNode->right->left = current;
			minNode->right = current;

			//update the min node if necessary
			if (current->priority > minNode->priority)
			{
				minNode = current;
			}
			current = next;
		}
	}

	//remove the max node from the list of roots
	maxNode->left->right = maxNode->right;
	maxNode->right->left = maxNode->left;

	Node<T> max;

	//copy the value and priority of the max node
	max.value = maxNode->value;
	max.priority = maxNode->priority;

	if (maxNode == maxNode->right)
	{
		minNode = nullptr;
	}
	else
	{
		//set the min node to the right of the max node
		minNode = maxNode->right;
		consolidate();
	}

	numOfNodes--;

	delete maxNode;
	maxNode = nullptr;
	return max;
}

//Returns the node with the maximum priority
template <typename T>
Node<T> FibonacciHeap<T>::findMax()
{
	/*
	if (minNode == nullptr)
	{
		return Node<T>(); // Zwraca pusty węzeł, jeśli kopiec jest pusty
	}

	FibonacciHeapNode<T>* maxNode = minNode;
	FibonacciHeapNode<T>* current = minNode->right;

	// Przechodzi przez listę korzeni w celu znalezienia maksymalnego węzła
	while (current != minNode)
	{
		if (current->priority > maxNode->priority)
		{
			maxNode = current;
		}
		current = current->right;
	}

	Node<T> max;
	max.value = maxNode->value;
	max.priority = maxNode->priority;

	return max;
	*/

	Node<T> max;
	max.value = minNode->value;
	max.priority = minNode->priority;

	return max;
}

//modyfy the key of a node
template <typename T>
void FibonacciHeap<T>::modifyKey(FibonacciHeapNode<T>* node, T newKey)
{
	//if the new key is greater than the current key
	if (newKey < node->value)
	{
		node->value = newKey;
		FibonacciHeapNode<T>* parent = node->parent;
		if (parent != nullptr && node->priority < parent->priority)
		{
			cut(node, parent);
			while (parent != nullptr && parent->mark)
			{
				cut(parent, parent->parent);
				parent = parent->parent;
			}
		}
		//update the min node if necessary
		if (node->priority > minNode->priority)
		{
			minNode = node;
		}
	}
}

//return the size of fibonacci heap
template <typename T>
size_t FibonacciHeap<T>::getSize()
{
	return numOfNodes;
}

//displaying fibonacci heap
template <typename T>
void FibonacciHeap<T>::display()
{
	if (minNode == nullptr)
	{
		return;
	}
	FibonacciHeapNode<T>* start = minNode;
	FibonacciHeapNode<T>* current = minNode;
	do
	{
		std::cout << current->value << " ";
		current = current->right;
	} while (current != start);
	std::cout << std::endl;
}

//consolidate the heap
template <typename T>
void FibonacciHeap<T>::consolidate()
{
	
	//calculating the maximum degree of a node in the heap
	int maxDegree = static_cast<int>(floor(log(getSize()) / log(2))) + 1;
	FibonacciHeapNode<T>* start = minNode;
	FibonacciHeapNode<T>* current = minNode;

	//vector to store the nodes with the same degree
	std::vector<FibonacciHeapNode<T>*> arr;

	do
	{
		FibonacciHeapNode<T>* next = current->right;
		int degree = current->degree;

		//if the degree of the current node is greater than the maximum degree
		while (degree >= maxDegree)
		{
			maxDegree *= 2;
		}

		arr.assign(maxDegree, nullptr);

		//FibonacciHeapNode<T>* arr[maxDegree] {};

		/*
		for (int i = 0; i < maxDegree; i++)
		{
			arr[i] = nullptr;
		}
		*/

		FibonacciHeapNode<T>* x = current;

		//if there is another node with the same degree as the current node
		while (arr[x->degree] != nullptr)
		{
			FibonacciHeapNode<T>* y = arr[x->degree];

			if (x->priority < y->priority)
			{
				FibonacciHeapNode<T>* temp = x;
				x = y;
				y = temp;
			}

			link(y, x);
			arr[x->degree - 1] = nullptr;
		}

		arr[x->degree] = x;
		current = next;

	} while (current != start);

	minNode = nullptr;

	//insert the nodes with the same degree into the list of roots
	for (int i = 0; i < maxDegree; i++)
	{
		if (arr[i] != nullptr)
		{
			if (minNode == nullptr)
			{
				minNode = arr[i];
				minNode->left = minNode;
				minNode->right = minNode;
			}
			else
			{
				minNode->left->right = arr[i];
				arr[i]->left = minNode->left;
				minNode->left = arr[i];
				arr[i]->right = minNode;

				if (arr[i]->priority > minNode->priority)
				{
					minNode = arr[i];
				}
			}
		}
	}
}

//link two nodes
template <typename T>
void FibonacciHeap<T>::link(FibonacciHeapNode<T>* node1, FibonacciHeapNode<T>* node2)
{
	//remove node1 from the list of roots
	node1->left->right = node1->right;
	node1->right->left = node1->left;
	node1->parent = node2;
	//if node2 has no children
	if (node2->child == nullptr)
	{
		node2->child = node1;
		node1->left = node1;
		node1->right = node1;
	}
	else
	{
		node1->right = node2->child;
		node1->left = node2->child->left;
		node2->child->left->right = node1;
		node2->child->left = node1;
	}
	//updating degree and setting the mark of node1 to false
	node2->degree++;
	node1->mark = false;
}

//cut the node from its parents
template <typename T>
void FibonacciHeap<T>::cut(FibonacciHeapNode<T>* node, FibonacciHeapNode<T>* parent)
{
	//remove the node from the list of children of its parent	
	node->left->right = node->right;
	node->right->left = node->left;
	//decrease the degree of the parent
	parent->degree--;
	//if the node is the child of its parent
	if (parent->child == node)
	{
		parent->child = node->right;
	}
	//if the parent has no children
	if (parent->degree == 0)
	{
		parent->child = nullptr;
	}
	//insert the node into the list of roots
	node->left = minNode;
	node->right = minNode->right;
	minNode->right->left = node;
	minNode->right = node;
	node->parent = nullptr;
	node->mark = false;
}

template <typename T>
T FibonacciHeap<T>::getElement(int index)
{
	FibonacciHeapNode<T>* start = minNode;
	FibonacciHeapNode<T>* current = minNode;
	for (int i = 0; i < index; i++)
	{
		current = current->right;
	}
	return current->value;
}

template <typename T>
int FibonacciHeap<T>::getPriority(int index)
{
	FibonacciHeapNode<T>* start = minNode;
	FibonacciHeapNode<T>* current = minNode;
	for (int i = 0; i < index; i++)
	{
		current = current->right;
	}
	return current->priority;
}

template <typename T>
void FibonacciHeap<T>::setElement(int index, T value)
{
	FibonacciHeapNode<T>* start = minNode;
	FibonacciHeapNode<T>* current = minNode;
	for (int i = 0; i < index; i++)
	{
		current = current->right;
	}
	current->value = value;
}
#endif
