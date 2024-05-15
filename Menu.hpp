#ifndef MENU_HPP
#define MENU_HPP

#include <iostream>

#include "Queue.hpp"
#include "HeapPriorityQueue.hpp"
#include "fibonacciheapnode.hpp"

class Menu
{
protected:
	int userChoice = 0;
	int index = 0;
public:
	virtual void display() const = 0;
	virtual void run() = 0;
};

class DataTypeMenu : public Menu
{
public:
	void display() const override;
	void run() override;
};

template <typename T>
class QueueMenu : public Menu
{
protected:
	std::unique_ptr<Queue<T>> pq;
	const int exitOption = 5;

public:
	void display() const override;
	void run() override;

	template <typename T> friend class OperationMenu;
};

template <typename T>
class OperationMenu : public Menu
{
protected:
	std::unique_ptr<Queue<T>> pq;
	const int exitOption = 7;

public:
	void display() const override;
	void run() override;

	template <typename T> friend class QueueMenu;
};


//////// DataTypeMenu ////////

void DataTypeMenu::display() const
{
	std::cout << "----------\n";
	std::cout << "1. Int\n";
	std::cout << "2. Float\n";
	std::cout << "3. Char\n";
	std::cout << "4. String\n";
	std::cout << "----------\n";

	std::cout << "Choose a data type to use with priority queue: ";
}

void DataTypeMenu::run()
{
	while (true)
	{
		this->display();
		std::cin >> userChoice;

		std::cout << "Press enter to continue";

		// Wait for enter
		std::cin.get();
		std::cin.get();

		system("cls");

		switch (userChoice)
		{
			case 1:
			{
				QueueMenu<int> m1;
				m1.run();
				break;
			}
			case 2:
			{
				QueueMenu<float> m2;
				m2.run();
				break;
			}
			case 3:
			{
				QueueMenu<char> m3;
				m3.run();
				break;
			}
			case 4:
			{
				QueueMenu<std::string> m4;
				m4.run();
				break;
			}
			default:
			{
				std::cerr << "error: chosen option does not exist\n";
				exit(1);
			}
		}
	}

}

//////// QueueMenu ////////

template <typename T>
void QueueMenu<T>::display() const
{
	std::cout << "------------------- Menu -------------------\n";
	std::cout << "1. Create an empty max heap priority queue\n";
	std::cout << "2. Create an empty Fibonacci heap priority queue\n";
	std::cout << "3. Initialize max heap priority queue with data\n";
	std::cout << "4. Initialize Fibonacci heap priority queue with data\n";
	std::cout << "5. Exit program\n";
	std::cout << "--------------------------------------------\n";

	std::cout << "Choose one option from the menu: ";
}

template <typename T>
void QueueMenu<T>::run()
{
	while (userChoice != exitOption)
	{
		this->display();
		std::cin >> userChoice;

		switch (userChoice)
		{
			case 1:
			{
				//HeapPriorityQueue<T> heapPq;
				//pq = &heapPq;
				
				pq = std::make_unique<HeapPriorityQueue<T>>();
				std::cout << "An empty max heap priority queue has been created\n";
				break;
			}

			case 2:
			{
				//FibonacciHeap<T> fibPq;
				//pq = &fibPq;

				pq = std::make_unique<FibonacciHeap<T>>();
				std::cout << "An empty fibonacci heap priority queue has been created\n";
				break;
			}

			case 3:
			{
				std::vector<Node<T>> nodes;
				int size = 0;
				int priority;
				T value;

				std::cout << "Enter desired size of priority queue: ";
				std::cin >> size;

				std::cout << "Enter (value priority) pairs: \n";

				for (int i = 0; i < size; i++)
				{
					std::cin >> value >> priority;
					nodes.push_back({ value, priority });
				}

				//HeapPriorityQueue<T> heapPq(nodes, nodes.size());
				//pq = &heapPq;

				pq = std::make_unique<HeapPriorityQueue<T>>(nodes, nodes.size());
				break;
			}

			case 4:
			{
				std::vector<Node<T>> nodes;
				int size = 0;
				int priority;
				T value;

				std::cout << "Enter desired size of priority queue: ";
				std::cin >> size;

				std::cout << "Enter (value priority) pairs: \n";

				for (int i = 0; i < size; i++)
				{
					std::cin >> value >> priority;
					nodes.push_back({ value, priority });
				}

				//FibonacciHeap<T> fibPq(nodes, nodes.size());
				//pq = &fibPq;

				pq = std::make_unique<FibonacciHeap<T>>(nodes, nodes.size());
				break;
			}

			case 5:
			{
				exit(0);
				break;
			}

			default:
			{
				std::cerr << "error: chosen option does not exist\n";
				exit(1);
			}
		}

		std::cout << "Press enter to continue";

		//Wait for enter
		std::cin.get();
		std::cin.get();
		
		system("cls");

		OperationMenu<T> m1;
		m1.pq = std::move(pq);
		m1.run();
	}
}

//////// OperationMenu ////////

template <typename T>
void OperationMenu<T>::display() const
{
	std::cout << "------ Operation Menu ------\n";
	std::cout << "1. Insert\n";
	std::cout << "2. Extract max\n";
	std::cout << "3. Find max\n";
	std::cout << "4. Modify priority\n";
	std::cout << "5. Display entire queue\n";
	std::cout << "6. Get size\n";
	std::cout << "7. Exit structure menu\n";
	std::cout << "----------------------------\n";

	std::cout << "Choose an operation to perform: ";
}

template <typename T>
void OperationMenu<T>::run()
{
	T value {};
	int priority = 0;
	size_t size = 0;

	while (userChoice != exitOption)
	{
		this->display();
		std::cin >> userChoice;

		switch (userChoice)
		{
			case 1:
			{
				std::cout << "Enter value to be added: ";
				std::cin >> value;
				std::cout << "With priority: ";
				std::cin >> priority;

				pq->insert(value, priority);

				break;
			}

			case 2:
			{
				Node<T> node = pq->exctractMax();

				std::cout << "max node has been removed from queue\n";
				std::cout << "max value: " << node.value << '\n';
				std::cout << "max priority: " << node.priority << '\n';

				break;
			}

			case 3:
			{
				Node <T> node = pq->findMax();

				std::cout << "max value: " << node.value << '\n';
				std::cout << "max priority: " << node.priority << '\n';

				break;
			}

			case 4:
			{
				std::cout << "Enter value that you want to change: ";
				std::cin >> value;
				std::cout << "Enter new priority: ";
				std::cin >> priority;

				pq->modifyKey(value, priority);
				break;
			}

			case 5:
			{
				pq->display();
				break;
			}

			case 6:
			{
				size = pq->getSize();

				std::cout << "Size of queue is: " << size << '\n';

				break;
			}

			case 7:
			{
				break;
			}

			default:
			{
				std::cerr << "error: chosen option does not exist\n";
				exit(1);
			}
		}

		std::cout << "Press enter to continue";

		// Wait for enter
		std::cin.get();
		std::cin.get();

		system("cls");
	}
}

#endif