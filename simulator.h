#ifndef _simulator_h
#define _simulator_h

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <unistd.h>
#include "Process.h"

std::string printQueue(const list<Process*> &queue);
std::string printVec(const std::vector<Process*> &queue);
bool remove_from_queue(std::vector<Process*> &queue, Process* removeme);

class SRTSimulator
{
public:
	SRTSimulator(std::list<Process*> &list) : clock(-1), idle(true), P(NULL), T(NULL)
	{
		// sorry, had to
		for (std::list<Process*>::const_iterator itr = list.begin(); itr != list.end(); ++itr)
		{
			pslist.push_back(*itr);
		}
	}
	
	~SRTSimulator()
	{
		pslist.clear();
		ready_queue.clear();
		finish_queue.clear();
	}
	Process* ready_process(int tick)
	{
		for (unsigned int i = 0; i < pslist.size(); ++i)
		{
			if (pslist[i]->arrivalTime <= tick)
			{
				return (Process*)pslist[i];
			}
		}
		return NULL;
	}
	void Run();

private:
	int clock;
	bool idle;
	Process* P;
	Process* T;
	std::vector<Process*> pslist;
	std::vector<Process*> ready_queue;
	std::vector<Process*> finish_queue;
};



#endif // !_simulator_h



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
#include "process.h"

class SRTCompare
{
public:
	bool operator() (Process* p1, Process* p2)
	{
		if (p1->burstTime == p2->burstTime)
		{
			return (p1->arrivalTime <= p2->arrivalTime);
		}
		else
		{
			return (p1->burstTime <= p2->burstTime);
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
		Stats() {}
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
private:
	Stats stats;
};

class SRTScheduler : public Scheduler
{
public:
	SRTScheduler() : Scheduler("SRT") {}
	std::queue<Process*> schedule(std::list<Process*> &pslist)
	{
		int start_time = 0;
		int finish_time = 0;
		Process* p, *scheduled, remaining;
		Stats stats = this->GetStats();
		std::queue<Process*> retval;
		std::map<std::string, int> start_times;
		std::map<std::string, int> finish_times;
		std::priority_queue<Process*, std::vector<Process*>, SRTCompare> ready_queue;
		std::priority_queue<Process*, std::vector<Process*>, SRTCompare> waiting_queue;

		while (!pslist.empty() || !ready_queue.empty() || !waiting_queue.empty())
		{
			while (!pslist.empty() && pslist.front()->arrivalTime <= finish_time)
			{
				std::cout << "time " << pslist.front()->arrivalTime << "ms: Process " << pslist.front()->id
					<< " arrived and added to ready queue ";
				ready_queue.push(pslist.front());
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

			if (p->burstTime > 1)
			{
				remaining = *p;
				remaining.burstTime--;
				waiting_queue.push(&remaining);
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

};

#endif // !_simulator_h

