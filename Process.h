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
	int ioTime;
	int burstRemain;
	int ioWaitEnd;
	int timeTerminated;
	void print();
};
