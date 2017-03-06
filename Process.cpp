#include<iostream>
#include<string>
#include"Process.h"
using namespace std;
/////////CONSTRUCTORS/////////////////////

Process::Process()
{
	id = "";
	arrivalTime = 0;
	burstTime = 0;
	numBurst = 0;
	ioTime = 0;
	burstRemain = burstTime;
}


Process::Process(string Id, int ArrivalTime, int BurstTime, int NumBurst, int IoTime)
{
	id = Id;
	arrivalTime = ArrivalTime;
	burstTime = BurstTime;
	numBurst = NumBurst;
	ioTime = IoTime;
	burstRemain = burstTime;

}
Process::Process(const Process& p2)
{
	id = p2.id;
	arrivalTime = p2.arrivalTime;
	burstTime = p2.burstTime;
	numBurst = p2.numBurst;
	ioTime = p2.ioTime;
	burstRemain = p2.burstRemain;

}


bool compare_arrivalTime(const Process& p1, const Process& p2)
{
   if (p1.arrivalTime != p2.arrivalTime) return p1.arrivalTime < p2.arrivalTime;
   return p1.id < p2.id;
}

bool compare_id(const Process& p1, const Process& p2)
{
   return p1.id < p2.id;
}


void Process::print()
{
	cout << this->id << " | " << this->arrivalTime << " | " << this->burstTime << " | " << this->numBurst << " | " << this->ioTime << " | " << endl;
}
