
#ifndef QUEUE_HPP
#define QUEUE_HPP

template <typename T>
class FibonacciHeapNode;

template <typename T>
class HeapPriorityQueue;

template <typename T>
class FibonacciHeap;

template <typename T>
class Queue
{
public:
	virtual void insert(T value, int index) = 0;
	virtual Node<T> exctractMax() = 0;
	virtual Node<T> findMax() = 0;
	virtual size_t getSize() = 0;
	virtual void display() = 0;

	virtual void modifyKey(T value, int priority) = 0;
	virtual void modifyKey(FibonacciHeapNode<T>* node, T newKey) = 0;
};

#endif