#include<iostream>
#include<list>
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
	ioWaitEnd = -1;
	type = "";
}


Process::Process(string Id, int ArrivalTime, int BurstTime, int NumBurst, int IoTime)
{
	id = Id;
	arrivalTime = ArrivalTime;
	burstTime = BurstTime;
	numBurst = NumBurst;
	ioTime = IoTime;
	burstRemain = burstTime;
	type = "";

}
Process::Process(const Process& p2)
{
	id = p2.id;
	arrivalTime = p2.arrivalTime;
	burstTime = p2.burstTime;
	numBurst = p2.numBurst;
	ioTime = p2.ioTime;
	burstRemain = p2.burstRemain;
	type = p2.type;

}


bool compare_arrivalTime(const Process& p1, const Process& p2)
{
   if (p1.arrivalTime != p2.arrivalTime) return p1.arrivalTime < p2.arrivalTime;
   return p1.id < p2.id;
}
void Process::print()
{
	cout << this->id << "|" << this->arrivalTime << "|" << this->burstTime << "|" << this->numBurst << "|" << this->ioTime << "|" << this->burstRemain << "|" << ioWaitEnd << endl;
}
bool compare_id(const Process* const& p1, const Process* const& p2)
{
   return (*p1).id < (*p2).id;
	//cout << this->id << " | " << this->arrivalTime << " | " << this->burstTime << " | " << this->numBurst << " | " << this->ioTime << " | " << this->ioWaitEnd << " |"  << endl;

}

//list<Process*>::iterator&



