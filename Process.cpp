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
}


Process::Process(string Id, int ArrivalTime, int BurstTime, int NumBurst, int IoTime)
{
	id = Id;
	arrivalTime = ArrivalTime;
	burstTime = BurstTime;
	numBurst = NumBurst;
	ioTime = IoTime;

}
