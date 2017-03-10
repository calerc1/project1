#include <string>


using namespace std;
class Process{
public:
    Process();
    Process(string Id, int ArrivalTime, int BurstTime, int NumBurst, int IoTime);
    Process(const Process& p2);
	~Process()
	{
	
	}
	string id;
	int arrivalTime;
	int burstTime;
	int numBurst;
	int ogNumBurst;
	int ioTime;
	int burstRemain;
	int ioWaitEnd;
	string type;
	int turnaroundArrival;
	int timeTerminated;
	int cs_switches;
	int ioReturn;
	void print();
};

