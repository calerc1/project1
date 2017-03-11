#ifndef _simulator_h
#define _simulator_h

#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <queue>
#include <map>
#include <algorithm>
#include <utility>
#include "Process.h"

// function object
struct burst_remain_sort : public std::binary_function<Process*, Process*, bool>
{
	bool operator()(const Process* const& p1, const Process* const& p2)
	{
		if (p1->burstRemain == p2->burstRemain)
		{
			if (p1->arrivalTime < p2->arrivalTime)
			{
				return true;
			}
			else
			{
				return (p1->id < p2->id);
			}
		}
		else
		{
			return (p1->burstRemain < p2->burstRemain);
		}
	}
};

class SRTCompare
{
public:
	bool operator() (Process* p1, Process* p2)
	{
		if (p1->burstTime == p2->burstTime)
		{
			if (p1->arrivalTime > p2->arrivalTime)
			{
				return true;
			}
			else
			{
				return (p1->id > p2->id);
			}
		}
		else
		{
			return (p1->burstTime > p2->burstTime);
		}
	}
};

class Scheduler
{
public:
	Scheduler(const std::string &strAlgo = "") : strAlgorithm(strAlgo) {}
	virtual ~Scheduler() {}
	class Stats
	{
	public:
		Stats(): turnaroundTime(0), waitingTime(0), responseTime(0), numProcesses(0) {}
		Stats(const Stats &s) : turnaroundTime(s.turnaroundTime), waitingTime(s.waitingTime), responseTime(s.responseTime), numProcesses(s.numProcesses)
		{

		}
		double AvgTurnaroundTime() const
		{
			return turnaroundTime / (double)numProcesses;
		}
		double AvgWaitingTime() const
		{
			return waitingTime / (double)numProcesses;
		}
		double AvgResponseTime() const
		{
			return responseTime / (double)numProcesses;
		}
		int turnaroundTime;
		int waitingTime;
		int responseTime;
		int numProcesses;
		int numContextSwitches;
	};

	Stats GetStats() const { return stats; }

	void PrintStats()
	{
		std::cout << "Algorithm " << strAlgorithm << "\n";
		std::cout << "-- average burst time: " << " ms\n";
		std::cout << "-- average wait time: " << stats.AvgWaitingTime() << " ms\n";
		std::cout << "-- average turnaround time: " << stats.AvgTurnaroundTime() << " ms\n";
		std::cout << "-- total number of context switches: " << "\n";
		std::cout << "-- total number of preemptions: " << "\n";
	}

	virtual void schedule(std::list<Process*> &pslist) = 0;
protected:
	std::string strAlgorithm;
	Stats stats;
};

class SRTScheduler : public Scheduler
{
public:
	SRTScheduler() : Scheduler("SRT") {}
	void schedule(std::list<Process*> &input)
	{
		int start_time = 0;
		int finish_time = 0;
		bool ctx_switch = false;
		Process* p = NULL, *pswitch = NULL;
		std::map<std::string, int> start_times;
		std::map<std::string, int> finish_times;
		std::list<Process*> ready_queue;
		std::list<Process*> waiting_queue;
		std::list<Process*> pslist; // process queue
		std::list<Process*> io_queue;

		std::cout << "time " << start_time << "ms: Simulator started for SRT " << LL_Contents(ready_queue) << "\n";

		CheckArrivals(input, pslist, finish_time);
		pslist.sort(burst_remain_sort());
		while (!pslist.empty())
		{
			std::list<Process*>::iterator itr = pslist.begin();
			ready_queue.push_back(*itr);
			if ((*itr)->type == "I/O")
			{
				std::cout << "time " << finish_time << "ms: Process " << (*itr)->id
					<< " completed I/O; added to ready queue " << LL_Contents(ready_queue) << "\n";
			}
			else if ((*itr)->type == "input")
			{
				std::cout << "time " << finish_time << "ms: Process " << (*itr)->id
					<< " arrived and added to ready queue " << LL_Contents(ready_queue) << "\n";
			}
			else
			{

			}
			(*itr)->type = "";
			pslist.pop_front();
		}

		while (!ready_queue.empty() || !waiting_queue.empty() || p != NULL)
		{
			// check for and add arrival processes
			CheckArrivals(input, pslist, finish_time);
			CheckIoWait(finish_time, waiting_queue, ready_queue);
			pslist.sort(burst_remain_sort());
			while (!pslist.empty() && pslist.front()->arrivalTime <= finish_time)
			{
				std::list<Process*>::iterator itr = pslist.begin();
				ready_queue.push_back(*itr);
				if ((*itr)->type == "I/O")
				{
					std::cout << "time " << finish_time << "ms: Process " << (*itr)->id
						<< " completed I/O; added to ready queue " << LL_Contents(ready_queue) << "\n";
				}
				else if ((*itr)->type == "input")
				{
					std::cout << "time " << finish_time << "ms: Process " << (*itr)->id
						<< " arrived and added to ready queue " << LL_Contents(ready_queue) << "\n";
				}
				else
				{

				}
				(*itr)->type = "";
				pslist.pop_front();
			}

			if (p != NULL && (finish_time - start_time == (*p).burstTime))
			{
				CheckCurrent(ready_queue, waiting_queue, p, finish_time, start_time, input);
			}
			else if (p == NULL)
			{
				if (!ready_queue.empty())
				{
					LoadCPU(finish_time, ready_queue, waiting_queue, p, start_time, input);
					p = *ready_queue.begin();
					start_time = finish_time;
					ready_queue.pop_front();
					cout << "time " << finish_time << "ms: Process " << p->id << " started using the CPU " << LL_Contents(ready_queue) << endl;
				}
			}
			++finish_time;
		}
		std::cout << "time " << finish_time << "ms: Simulator ended for SRT\n";
	}
private:
	void CheckCurrent(list<Process*> &queue, list<Process*> &ioWait, Process* &current, int counter, int &counterStart, list<Process*> &input)
	{
		(*current).numBurst--;
		if (current->numBurst > 0)
			// if the process has cpu burst remaining there are two options 
			// 1. there is another process in the ready queue that can be worked on
			// 2. there are no processes left in the cpu so the current process is set to NULL such that the current program can wait for another process to come out of io time or there are so process left and the program ends
		{
			current->ioWaitEnd = (int)((int)counter + (int)(current->ioTime + 3));
			ioWait.push_back(current);

			if ((*current).numBurst == 1) {
				cout << "time " << counter << "ms: Process " << (*current).id << " completed a CPU burst; " << (*current).numBurst << " burst to go " << LL_Contents(queue) << endl;
			}
			else {
				cout << "time " << counter << "ms: Process " << (*current).id << " completed a CPU burst; " << (*current).numBurst << " bursts to go " << LL_Contents(queue) << endl;

			}
			LoadCPU(counter, queue, ioWait, current, counterStart, input);
			cout << "time " << counter - 3 << "ms: Process " << (*current).id << " switching out of CPU; will block on I/O until time " << (*current).ioWaitEnd << "ms " << LL_Contents(queue) << endl;

			if (!queue.empty())
			{
				LoadCPU(counter, queue, ioWait, current, counterStart, input);
				current = *queue.begin();
				queue.pop_front();
				counterStart = counter;
				cout << "time " << counter << "ms: Process " << current->id << " started using the CPU " << LL_Contents(queue) << endl;

			}
			else
			{
				current = NULL;
			}

		}
		else
			//if the process has no cpu burst remaining the process is terminated or set to null if no other process are on the ready queue
		{
			cout << "time " << counter << "ms: Process " << current->id << " terminated " << LL_Contents(queue) << endl;
			LoadCPU(counter, queue, ioWait, current, counterStart, input);


			if (!queue.empty())
			{
				LoadCPU(counter, queue, ioWait, current, counterStart, input);
				current = *queue.begin();
				queue.pop_front();
				counterStart = counter;
				cout << "time " << counter << "ms: Process " << current->id << " started using the CPU " << LL_Contents(queue) << endl;
			}
			else
			{
				current = NULL;
			}
		}

	}
	void CheckArrivals(list<Process*> &input, list<Process*> &toAdd, int currTime) 
	{
		if (input.empty() || ((*input.begin())->arrivalTime != currTime))
			return;

		list<Process*>::iterator itr;
		int i = 0;
		for (itr = input.begin(); itr != input.end(); itr++)
		{
			if ((*itr)->arrivalTime == currTime) {
				toAdd.push_back(*itr);
				(*itr)->type = "input";
			}
			else if ((*itr)->arrivalTime > currTime)
			{
				break;
			}
			++i;
		}
		for (int j = 0; j < i; ++j)
		{
			input.pop_front();
		}
	}
	void CheckIoWait(int counter, list<Process*> &ioWait, list<Process*> &queue)
	{
		list<Process*>::iterator iterator = ioWait.begin();
		//loops thru the list od ioWait Processes
		while (iterator != ioWait.end())
		{
			if ((*iterator)->ioWaitEnd == counter)
			{
				//two options the the process has remaining burst in cpu to be completed
				//-> need to check put process back in ready queue
				//or the process is completely done executing
				//-> process needs to be terminated
				if ((*iterator)->numBurst > 0)
				{//case 1
				 //pushes process to the end of the queue and deletes it from the ioWait 
				 // queue
					(*iterator)->type = "I/O";
					queue.push_back(*iterator);
					// alternatively, i = items.erase(i);
					ioWait.erase(iterator++);

				}
				// case two
				else
				{
					cout << (*iterator)->id << " is done and has been removed from precesses" << endl;
					ioWait.erase(iterator++);

				}
			}
			//just increment the iterator
			else
			{
				++iterator;
			}
		}
	}
	void LoadCPU(int &counter, list<Process*> &queue, list<Process*> &ioWait, Process* &current, int &counterStart, list<Process*> &input)
	{
		int delay;

		for (delay = 0; delay < 3; delay++)
		{
			CheckIoWait(counter, ioWait, queue);
			CheckArrivals(input, queue, counter);
			counter++;
		}
	}
	std::string PQ_Contents(const std::priority_queue<Process*, std::vector<Process*>, SRTCompare> &pq) const
	{
		std::string retval = "[Q";

		// queue is empty; no need for that costly operation ahead
		if (pq.empty())
		{
			retval += " <empty>]";
			return retval;
		}
		// Queues are normally not meant to be traversed for its contents, but for this assignment, this bulky memory copy is necessary
		std::priority_queue<Process*, std::vector<Process*>, SRTCompare> priorq = pq;

		while (!priorq.empty())
		{
			retval += " " + priorq.top()->id;
			priorq.pop();
		}

		retval += "]";
		return retval;
	}

	std::string LL_Contents(const std::list<Process*> &pq) const
	{
		std::string retval = "[Q";

		// queue is empty; no need for that costly operation ahead
		if (pq.empty())
		{
			retval += " <empty>]";
			return retval;
		}
		
		for (std::list<Process*>::const_iterator itr = pq.begin(); itr != pq.end(); ++itr)
		{
			retval += " " + (*itr)->id;
		}

		retval += "]";
		return retval;
	}
};


#endif // !_simulator_h

