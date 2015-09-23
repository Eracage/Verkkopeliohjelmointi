#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>

std::mutex coutMutex;

void PrintHelloWorld(int index, int count, int milliseconds)
{
	for (int i = 0; i < count; i++)
	{
		coutMutex.lock();
		std::cout << "Hello world\n";
		coutMutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
	}
}

void PrintHelloWorld2(int index, int count, int milliseconds)
{
	for (int i = 0; i < count; i++)
	{
		coutMutex.lock();
		//std::cout << "Hello" << " " << "world" << " " << "from" << " " << "thread" << " " << index << std::endl;
		std::cout << "Hello world from thread " << index << std::endl;
		coutMutex.unlock();
	}
}

void Ex6()
{
	std::thread thread(PrintHelloWorld, 1, 10, 1000);

	thread.join();
}

void Ex7()
{
	int threadcount = 3;
	std::vector<std::thread> threads;
	threads.reserve(threadcount);
	for (int i = 0; i < threadcount; i++)
		threads.push_back(std::thread(PrintHelloWorld2, i, 10, 1000));

	for (auto &thread : threads)
	{
		thread.join();
	}
}

void Ex8()
{

}

int main(int argc, char **argv)
{
	Ex7();
}