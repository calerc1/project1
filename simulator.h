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

	virtual std::queue<Process*> schedule(std::list<Process*> &pslist) = 0;
protected:
	std::string strAlgorithm;
	Stats stats;
};

class SRTScheduler : public Scheduler
{
public:
	SRTScheduler() : Scheduler("SRT") {}
	std::queue<Process*> schedule(std::list<Process*> &input)
	{
		int start_time = 0;
		int finish_time = 0;
		Process* p, remaining;
		std::queue<Process*> retval;
		std::map<std::string, int> start_times;
		std::map<std::string, int> finish_times;
		std::priority_queue<Process*, std::vector<Process*>, SRTCompare> ready_queue;
		std::priority_queue<Process*, std::vector<Process*>, SRTCompare> waiting_queue;
		std::list<Process*> pslist;

		std::cout << "time " << start_time << "ms: Simulator started for SRT " << PQ_Contents(ready_queue) << "\n";

		while (!input.empty() || !ready_queue.empty() || !waiting_queue.empty())
		{
			// check for and add arrival processes
			CheckArrivals(input, pslist, finish_time);
			while (!pslist.empty() && pslist.front()->arrivalTime <= finish_time)
			{
				ready_queue.push(pslist.front());
				std::cout << "time " << finish_time << "ms: Process " << pslist.front()->id
					<< " arrived and added to ready queue " << PQ_Contents(ready_queue) << "\n";
				pslist.pop_front();
			}

			if (ready_queue.empty())
			{
				if (waiting_queue.empty())
				{
					p = pslist.front();
					pslist.pop_front();
				}
				else
				{
					p = waiting_queue.top();
					waiting_queue.pop();
				}
			}
			else if (waiting_queue.empty())
			{
				p = ready_queue.top();
				ready_queue.pop();
			}
			else
			{
				if (ready_queue.top()->burstTime < waiting_queue.top()->burstTime)
				{
					p = (ready_queue.top());
					ready_queue.pop();
				}
				else 
				{ 
					p = (waiting_queue.top()); 
					waiting_queue.pop();
				}
			}

			start_time = std::max((int)std::ceil(p->arrivalTime), finish_time);
			finish_time = start_time + 1;

			if (start_times.find(p->id) == start_times.end())
			{
				start_times.insert(std::make_pair(p->id, start_time));
				stats.waitingTime += (start_time - p->arrivalTime);
				stats.responseTime += (start_time - p->arrivalTime + 1);
			}
			else
			{
				stats.waitingTime += (start_time - finish_times.find(p->id)->second);
			}

			if (p->burstTime > ready_queue.top()->burstTime)
			{
				remaining = *p;
				remaining.burstTime--;

				if (remaining.burstTime == 0)
				{
					waiting_queue.push(&remaining);
					std::cout << "time " << finish_time << "ms: Process " << remaining.id
						<< " arrived and will preempt " << PQ_Contents(ready_queue) << "\n";
				}
				
				finish_times.insert(std::make_pair(remaining.id, finish_time));
			}
			else // process terminates
			{
				stats.numProcesses++;
				stats.turnaroundTime += (finish_time - start_times.find(p->id)->second);
			}
		}

		return retval;
	}
private:
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
};


#endif // !_simulator_h

