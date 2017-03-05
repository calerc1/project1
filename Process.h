using namespace std;
class Process{
public:
    Process();
    Process(string Id, int ArrivalTime, int BurstTime, int NumBurst, int IoTime);
	string id;
	int arrivalTime;
	int burstTime;
	int numBurst;
	int ioTime;
};
