#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <cstdlib>
#include <ctime>
#include <vector>
#include "HeapPriorityQueue.hpp"
#include "fibonacciheapnode.hpp"
#include "Timer.hpp"
#include "Node.hpp"

std::vector<int> generateIntDataSet(int size, int seed = time(NULL), int start = 0, int end = 10000)
{
	srand(seed);
	std::vector<int> dataSet;

	for (int i = 1; i <= size; i++)
	{
		int random = (rand() % end) + start;
		dataSet.push_back(random);
	}

	return dataSet;
}

void measurePerformance(int repetitions, int dataSetSize)
{
	//Generate priorities
	std::vector<int> priorities = generateIntDataSet(dataSetSize, 1, 0, 1000000);

	//Generate elements
	std::vector<int> elements = generateIntDataSet(dataSetSize, 2);

	std::vector<Node<int>> nodes;

	for (int i = 0; i < dataSetSize; i++)
	{
		nodes.push_back({ elements[i], priorities[i] });
	}

	FibonacciHeap<int> pq(nodes, dataSetSize);
	FibonacciHeap<int> pqCopy(pq);

	double averageTime = 0.0;
	double sum = 0.0;
	double duration = 0.0;

	//int priorityToInsert = pq.getPriority(0);
	//int elementToInsert = pq.getElement(0);

	//int valueToFind = -1;
	//int priorityToSet = pq.getPriority(dataSetSize) - 1;
	//pq.setElement(0, valueToFind);

	// measure time of operation
	for (int i = 0; i < repetitions; i++)
	{
		Timer timer;

		timer.start();
		//pqCopy.insert(elementToInsert, priorityToInsert);
		//pqCopy.exctractMax();
		//pqCopy.findMax();
		pq.getSize();
		//pq.modifyKey(valueToFind, priorityToSet);
		timer.stop();

		duration = timer.getDuration();
		sum += duration;

		//pqCopy = pq;
	}

	averageTime = sum / repetitions;

	std::cout << "Average time: " << averageTime << "ns\n";
}

#endif