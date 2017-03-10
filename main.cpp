#include<iostream>
#include<fstream> 
#include<vector>
#include<string>
#include<list>
#include<sstream>
#include<stdlib.h>
#include<iomanip>
#include"Process.h"

#define t_cs 6
#define t_slice 94

using namespace std;

// <id>|<arrivalTime>|<burstTime>|<numBurst>|<ioTime>

///////////////FUNCTIONS DECLARATIONS//////////////////////////

void readFile(list<Process*> &input, char* fileName);
void parseLine(list<Process*> &input, string &line);
void FCFS(list<Process*> input, char* outputFile, ofstream &file);
void SRT(list<Process*> &input);
void RR(list<Process*> input, char* outputFile, ofstream &file);
void checkArrivals(list<Process*> &input, list<Process*> &toAdd, int currTime, int fcfs);
string printQueue(const list<Process*> &queue);
//bool compare_id(const Process& p1, const Process& p2);
void checkCurrent(list<Process*> &queue, list<Process*> &ioWait, Process* &current, int counter ,int &counterStart, list<Process*> &input);
void checkIoWait(int counter, list<Process*> &ioWait , list<Process*> &queue);
void loadCPU(int &counter, list<Process*> &queue, list<Process*> &ioWait, Process* &current, int &counterStart, list<Process*> &input);
void copyList(list<Process*> &queue, list<Process*> &copyArray);
void freeList(list<Process*> &toFree);
void resultsRR(list<Process*> &finished, int preemptions, int context_switches, int totalWait, int totalTurnaround, int totBursts , ofstream &file);
void printStatistics(ofstream &file, float &averageWait, float &averageBurst, float &averageTurnaround, int &totalPreemptions, int &numContextSwitch);



// function object
struct id_sort : public std::binary_function<Process*, Process*, bool>
{
	bool operator()(const Process* const& p1, const Process* const& p2)
	{
		return (*p1).id < (*p2).id;
	}
};

/////////////////////////MAIN///////////////////////////////////

int main(int argc, char* argv[])
{
	if(argc < 2){
		cerr << "ERROR: Not enough command line arguments" << endl;
		return EXIT_FAILURE;
	}
	//reading file onto a vector
	list<Process*> inputData;
	char* fileName = argv[1];
	char* outputFile = argv[2];
	ofstream file;
	file.open(outputFile);
	readFile(inputData,fileName);
	//////////////FCFS//////////////////
	list<Process*> FCFSList;
	copyList(inputData, FCFSList);
	FCFS(FCFSList, outputFile,file); 
	freeList(FCFSList);
	cout << endl <<endl;
	/////////////SRT///////////////////
	list<Process*> SRTList;
	//copyList(inputData, SRTList);
	//SRT(SRTList);
	freeList(SRTList);
	cout << endl <<endl;
	////////////RR////////////////////
	list<Process*> RRList;
	copyList(inputData, RRList);
	RR(RRList, outputFile,file);
	//freeList(RRList);

	//this will delete all the data from the list<Process*> 
	freeList(inputData);
  	return 1;
}

/////////////////FUNCTIONS/////////////////////////////////

void readFile(list<Process*> &input, char*fileName)
{
	//opening file
	ifstream file;
	file.open(fileName);
	string line;
	char pound = '#';
	char space = ' ';
	// reading line by line and pushing onto a vector
	while(getline(file,line))
	{
		//will get rid of all the line that start with a # or all the lines that are blank
		if(line[0] == space || line[0] == pound || line.size() == 0)
		{
			continue;	
		}
		parseLine(input, line);
	}
	file.close();
}
void parseLine(list<Process*> &input , string &line)
{
	vector<string> vars;
	istringstream ss(line);
	string token;

	while(getline(ss, token, '|')) 
	{
		vars.push_back(token);
		//cout << token << "|";
	}
	//cout <<endl;
	Process* node = new Process(vars[0], atoi(vars[1].c_str()), atoi(vars[2].c_str()), atoi(vars[3].c_str()), atoi(vars[4].c_str()));
	
	input.push_back(node);
}

void FCFS(list<Process*> input, char* outputFile, ofstream &file)
{
	#if 1 
	//All time gone by counter
	int counter = 0;
	int counterStart = 0;
	float averageBurst = 0;
	float averageWait = 0;
	float averageTurnaround = 0;
	int totalPreemptions = 0;
	int numContextSwitch = 0;

	list<Process*>::iterator itr1 = input.begin();
	while(itr1 != input.end())
	{
		averageBurst += (*itr1)->burstTime * (*itr1)->numBurst;
		numContextSwitch += (*itr1)->numBurst;
		itr1++;
	}
	averageBurst /= numContextSwitch;
	cout << "time " << counter << "ms: Simulator started for FCFS [Q <empty>]" << endl;
	//priority q for all arriving process
	list<Process*> queue;
	list<Process*> ioWait;
	list<Process*> toAdd;
	list<Process*>::iterator itr2;
	//the first process to arrive
	Process* current =	NULL; 

	string debug;
	//need to have something on the queue in able for the loop to execute		
	checkArrivals(input,toAdd, counter, 1);
	toAdd.sort(id_sort());
	while(!toAdd.empty())
	{
		//cout << "here" << endl;
		itr2 = toAdd.begin();
		queue.push_back(*itr2);
		if((*itr2)->type == "I/O"){
			cout << "time " << counter << "ms: Process " << (*itr2)->id << " completed I/O; added to ready queue " << printQueue(queue) << endl;  
		}
		else if( (*itr2)->type == "input" ){
			cout << "time " << counter << "ms: Process " << (*itr2)->id << " arrived and added to ready queue " << printQueue(queue) << endl;
		}
		else if( (*itr2)->type == "" ){

		}
		(*itr2)->type = "";
		toAdd.pop_front();
	}
	while(queue.size() > 0 || current != NULL || ioWait.size() > 0)
	{
		//checks for any new arrivals, and puts them in the back of the queue
		
		checkArrivals(input,toAdd, counter, 1);
		//checks for proceces that have expired in the ioWait queue to put in the ready queue
		checkIoWait(counter, ioWait, toAdd);
		toAdd.sort(id_sort());
		while(!toAdd.empty())
		{
			//cout << "here" << endl;
			itr2 = toAdd.begin();
			queue.push_back(*itr2);
			#if 1
			if((*itr2)->type == "I/O"){
				cout << "time " << counter << "ms: Process " << (*itr2)->id << " completed I/O; added to ready queue " << printQueue(queue) << endl;  
			}
			else if( (*itr2)->type == "input" ){
				cout << "time " << counter << "ms: Process " << (*itr2)->id << " arrived and added to ready queue " << printQueue(queue) << endl;
			}
			else if( (*itr2)->type == "" ){

			}
			#endif
			(*itr2)->type = "";
			toAdd.pop_front();
		}

		//checks that the current Process that is being handled has not expiered
		if(current != NULL && ((counter-counterStart) == (*current).burstTime))
		{
			//Checks that the current Process is not expiered		
			checkCurrent(queue, ioWait, current, counter , counterStart, input);
		}

		else if(current  == NULL)
		{

			if(!queue.empty())
			{
				//load works I think
				current = *queue.begin();
				queue.pop_front();
				loadCPU(counter, queue, ioWait, current, counterStart,input);
				counterStart = counter;	
				cout << "time " << counter << "ms: Process " << current->id << " started using the CPU " << printQueue(queue) << endl;
			}		
		}
		#if 0
		else if(((counter-counterStart) > (*current).burstTime))
		{
			//cout << " mutha fvcking error "  << endl;
		}
		#endif
		//checks that no process with io wait 0ms was added to the queue
		averageWait += queue.size();
		
		counter++;
	}
	
	cout << "time "  << (counter + 2) << "ms: Simulator ended for FCFS" << endl;
	#endif
	file << "Algorithm FCFS" << endl;
	printStatistics(file, averageWait, averageBurst, averageTurnaround, totalPreemptions, numContextSwitch);
}

void SRT(list<Process*> &input)
{
	
}

void RR(list<Process*> input, char* outputFile, ofstream &file)
{	
	#if 1 
	//priority q for all arriving process
	list<Process*> queue;
	list<Process*> newArrivals;
	list<Process*> toAdd;
	list<Process*> ioQueue;
	list<Process*> finished;
	Process* current = NULL;
	Process* p_cs = NULL;  //the process to be in the context switch
	//current should = NULL while p_cs has a value
	int cs_counter = 0; //counts up to 3 and 6
	int ts_expire = 0; //time the current time slice will expire upon (process start time + t_slice)
	bool cs = false;
	bool ioAdd = false;
	bool start = false;
	int i = 0;
	int context_switches = 0;
	int preemptions = 0;
	int waitCount = 0;
	int totalTurnaround = 0;
	int totalBursts = 0;
	list<Process*>::iterator itr;
	cout << "time " << i << "ms: Simulator started for RR " << printQueue(queue) << endl;
	while(1){
		//cout << "time " << i << endl;
		checkArrivals(input, newArrivals, i, 0);
		if(cs && cs_counter == t_cs/2){
			++context_switches;
			//if(!queue.empty()){
			if( p_cs!= NULL ){
				totalTurnaround += (i - (*p_cs).turnaroundArrival);
			
				#if 0
				cout << "time " << i << "ms: Turnaround time added: " << (i - (*p_cs).turnaroundArrival) << endl;
				cout << "time " << i << "ms: Total Turnaround time: " << totalTurnaround << endl;
				#endif

				//add p_cs to I/o wait queue
				/*ADD OLD PC_S TO i/o wait queue here */
				if(ioAdd){
					(*p_cs).ioWaitEnd = i + (*p_cs).ioTime;
					ioQueue.push_back(p_cs);
					#if 0
					cout << "time " << i << "ms: IoQueue: " << printQueue(ioQueue) << endl;
					#endif
					//p_cs = NULL;
				}
				else{
					toAdd.push_back(p_cs);
					++(*p_cs).cs_switches;
					//p_cs = NULL;
				}
					p_cs = NULL;
			}
			if(!queue.empty()){
				p_cs = *queue.begin();
				queue.pop_front();
			}
			//}
			if(queue.empty() && input.empty() && ioQueue.empty() && current == NULL && p_cs == NULL){
				printf("time %dms: Simulator ended for RR\n", i);
				resultsRR(finished, preemptions, context_switches, waitCount, totalTurnaround, totalBursts, file);
				return;
			}	
		}
		//if context switch fully completes print statement and start operating on CPU process
		if(cs && cs_counter == 6){
			cs = false;
			cs_counter = 0;
			current = p_cs;
			p_cs = NULL;
			//++waitCount;
			if(current != NULL){
				if( (*current).burstRemain < (*current).burstTime && (*current).burstRemain != 0 ){
					cout << "time " << i << "ms: Process " << (*current).id << " started using the CPU with " << (*current).burstRemain << "ms remaining " << printQueue(queue) << endl;
				}
				else{
					(*current).burstRemain = (*current).burstTime;
					cout << "time " << i << "ms: Process " << (*current).id << " started using the CPU " << printQueue(queue) << endl;
				}
				ts_expire = i + t_slice;
				start = true;
			}
		}	
		//standard CPU operation on current.  --burstremain
		if(!cs && current != NULL){
			if(start){
				start = false;
			}
			else{
				--(*current).burstRemain;
			}
		}	
		//if working on current
		#if 1
		//premption occurs due to time slice (initiate context switch)
		if(!cs && current != NULL && ts_expire == i && (*current).burstRemain != 0){
			if(queue.empty()){
				cout << "time " << i << "ms: Time slice expired; no preemption because ready queue is empty [Q <empty>]" << endl;
				//--(*current).burstRemain; //= (*current).burstTime - t_slice;
				ts_expire = i + t_slice;
				//++context_switches;
			}
			else{
				//--(*current).burstRemain;
				cout << "time " << i << "ms: Time slice expired; process " << (*current).id <<  " preempted with " << (*current).burstRemain << "ms to go " << printQueue (queue) << endl;
				cs = true;
				cs_counter = 0;
				p_cs = current;
				current = NULL;
				ioAdd = false;
				++preemptions;
				//++context_switches;
			}
		}
		#endif
		
		
		//process completes CPU burst/terminates
		if( !cs && current != NULL && (*current).burstRemain == 0 ){
			//set burst remain to I/O time and add to I/O queue
			(*current).ioWaitEnd = i + 3 + (*current).ioTime;
			//Process has more burst remaining
			if( (*current).numBurst > 1 ){
				--(*current).numBurst;
				if((*current).numBurst == 1){
					cout << "time " << i << "ms: Process " << (*current).id <<" completed a CPU burst; " << (*current).numBurst << " burst to go " << printQueue(queue) << endl;
					#if 0
					waitCount += (i - (*current).turnaroundArrival) - (*current).burstTime;
					cout << "time " << i << "ms: Turnaround time added: " << (i - (*current).turnaroundArrival) << endl;
					cout << "time " << i << "ms: waitCount added: " << (i - (*current).turnaroundArrival) - (*current).burstTime << ", Total waitCount: " << waitCount << ", " << printQueue(queue)<< endl;
					#endif
				}
				else{
					cout << "time " << i << "ms: Process " << (*current).id <<" completed a CPU burst; " << (*current).numBurst << " bursts to go " << printQueue(queue) << endl;
					#if 0
					waitCount += (i - (*current).turnaroundArrival) - (*current).burstTime;
					cout << "time " << i << "ms: Turnaround time added: " << (i - (*current).turnaroundArrival) << endl;
					cout << "time " << i << "ms: waitCount added: " << (i - (*current).turnaroundArrival) - (*current).burstTime << ", Total waitCount: " << waitCount << ", " << printQueue(queue)<< endl;
					#endif
				}
				cout << "time " << i << "ms: Process " << (*current).id << " switching out of CPU; will block on I/O until time " << i + (*current).ioTime + 3 <<"ms " << printQueue(queue) << endl;
				p_cs = current;
				#if 1
					waitCount += (i - (*p_cs).ioReturn) - (*p_cs).burstTime;
					//cout << "time " << i << "ms: Turnaround time added: " << (i - (*p_cs).ioReturn) << endl;
					//cout << "time " << i << "ms: waitCount added: " << (i - (*p_cs).ioReturn) - (*p_cs).burstTime << ", Total waitCount: " << waitCount << ", " << printQueue(queue)<< endl;
				#endif
				ioAdd = true;
			}
			//Process has no more bursts remaining
			else{
				
				cout << "time " << i <<"ms: Process " << (*current).id << " terminated " << printQueue(queue) << endl;
				Process* temp = current;
				(*temp).timeTerminated = i;
				totalTurnaround += (i - (*temp).turnaroundArrival) + 3;
				waitCount += (*temp).timeTerminated - (*temp).ioReturn - (*temp).burstTime;
				//cout << "time " << i << "ms: Turnaround time added: " << (i - (*temp).ioReturn) << endl;
				//cout << "time " << i << "ms: waitCount added: " << (*temp).timeTerminated - (*temp).ioReturn - (*temp).burstTime << ", Total waitCount: " << waitCount << ", " << printQueue(queue)<< endl;
				//cout << (*temp).id << "|" << (*temp).burstTime << "|" << (*temp).ioReturn <<  endl;

				current = NULL;
				//delete(temp);
				finished.push_back(temp);
				p_cs = NULL;
			}
			current = NULL;
			cs = true;
			cs_counter = 0;
		}
		//Handle print statements for newArrival processes
		while(!newArrivals.empty()){
			itr = newArrivals.begin();
			totalBursts += (*itr)->numBurst;
			//queue.push_back(*itr);
			(*itr)->type = "input";
			toAdd.push_back(*itr);
			//cout << "time " << i << "ms: Process " << (*itr)->id << " arrived and added to ready queue " << printQueue(queue) << endl;
			newArrivals.pop_front();
			
		}
	#if 0
		if(i == 1600){
			return;
		}
	#endif
		if(cs){
			++cs_counter;
		}
		//updateIOQueue(ioQueue);
		//officially add all processes to queue
		checkIoWait(i, ioQueue, toAdd);
		#if 1
		toAdd.sort(id_sort());
		#endif
		while(!toAdd.empty()){
			itr = toAdd.begin();
			(*itr)->turnaroundArrival = i;
			queue.push_back(*itr);
			#if 1
			if((*itr)->type == "I/O"){
				cout << "time " << i << "ms: Process " << (*itr)->id << " completed I/O; added to ready queue " << printQueue(queue) << endl;  
			}
			else if( (*itr)->type == "input" ){
				cout << "time " << i << "ms: Process " << (*itr)->id << " arrived and added to ready queue " << printQueue(queue) << endl;
			}
			else if( (*itr)->type == "" ){

			}
			#endif
			(*itr)->type = "";
			toAdd.pop_front();
		}
		#if 0
		waitCount += queue.size();
		
		cout << "time " << i << "ms: waitCount added: " << queue.size() << ", Total waitCount: " << waitCount << ", " << printQueue(queue)<< endl;
		#endif


		//start condition (count from 3 to 6)
		if(!queue.empty() && current == NULL && p_cs == NULL && cs == false){
			p_cs = *queue.begin();
			queue.pop_front();
			cs = true;
			cs_counter = 4;
		}
		

		//End Condition
		if(queue.empty() && input.empty() && ioQueue.empty() && current == NULL && p_cs == NULL && !cs){
			printf("time %dms: Simulator ended for RR\n", i);
			resultsRR(finished, preemptions, context_switches, waitCount, totalTurnaround, totalBursts, file);
			return;
		}
		++i;
	}
	#endif
}
void checkArrivals(list<Process*> &input, list<Process*> &toAdd, int currTime, int fcfs){
	#if 1
		if( input.empty() || ((*input.begin())->arrivalTime != currTime) )
		return;
	#endif

	list<Process*>::iterator itr;
	int i = 0;
	for(itr = input.begin(); itr != input.end(); itr++)
	{
		if( (*itr)->arrivalTime == currTime ){
			toAdd.push_back(*itr);
			(*itr)->type = "input";
			if(fcfs)
			{
			
			//cout << "time " << currTime << "ms: Process " << (*(*(itr))).id << " arrived and added to ready queue " << printQueue(toAdd) << endl;
			}
		}
		else if( (*itr)->arrivalTime > currTime )
		{
			break;
		}
		++i;
	}
	for(int j = 0; j < i; ++j)
	{
		input.pop_front();
	}

	#if 0
		cout << "Time: " << currTime << ", Input: " << printQueue(input);
		cout << endl;
		cout << "toAdd: " << printQueue(toAdd);
		cout << endl << endl;
	#endif
}
string printQueue(const list<Process*> &queue){
	string s = "[Q";
	string temp = "";
	if(queue.empty()){
		 s += " <empty>";
	}
	else{
		list<Process*>::const_iterator itr = queue.begin();
		for(; itr != queue.end(); ++itr){
			 s += " " + (*itr)->id;
			 #if 0
			 cout << endl; 
			 (*itr)->print();
			#endif
		}
	}
	s += "]";
	return s;

}

void checkIoWait(int counter, list<Process*> &ioWait , list<Process*> &queue)
{
	list<Process*>::iterator iterator = ioWait.begin();
	//loops thru the list od ioWait Processes
	while (iterator != ioWait.end())
	{
		if((*iterator)->ioWaitEnd == counter)
		{
			//two options the the process has remaining burst in cpu to be completed
				//-> need to check put process back in ready queue
			//or the process is completely done executing
				//-> process needs to be terminated
			if((*iterator)->numBurst > 0)
			{//case 1
				//pushes process to the end of the queue and deletes it from the ioWait 
				// queue
				(*iterator)->type = "I/O";
				queue.push_back(*iterator);
				(*iterator)->ioReturn = counter;
				// alternatively, i = items.erase(i);
				#if 0
				cout << "time " << counter << "ms: Process " << (*iterator)->id << " completed I/O; added to ready queue " << printQueue(queue) << endl; 
				#endif
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

//this function is used in FCFS to handle the case where the current process is done and switching to ioWait or terminating the is handled
void checkCurrent(list<Process*> &queue, list<Process*> &ioWait, Process* &current, int counter ,int &counterStart, list<Process*> &input)
{
		(*current).numBurst--;
		if(current->numBurst > 0 )
		// if the process has cpu burst remaining there are two options 
		// 1. there is another process in the ready queue that can be worked on
		// 2. there are no processes left in the cpu so the current process is set to NULL such that the current program can wait for another process to come out of io time or there are so process left and the program ends
		{
			current->ioWaitEnd = (int)((int)counter + (int)(current->ioTime + 3));
			ioWait.push_back(current);
			
			if((*current).numBurst == 1){
				cout << "time " << counter << "ms: Process " << (*current).id <<" completed a CPU burst; " << (*current).numBurst << " burst to go " << printQueue(queue) << endl;
			}
			else{
				cout << "time " << counter << "ms: Process " << (*current).id <<" completed a CPU burst; " << (*current).numBurst << " bursts to go " << printQueue(queue) << endl;

			}
			loadCPU(counter, queue, ioWait, current, counterStart, input);
			cout << "time " << counter - 3 << "ms: Process " <<  (*current).id << " switching out of CPU; will block on I/O until time " << (*current).ioWaitEnd << "ms " << printQueue(queue) << endl;                                   
			//cout << "here" << endl;

			if(!queue.empty())
			{	
				current = *queue.begin();
				queue.pop_front();	
				loadCPU(counter, queue, ioWait, current, counterStart,input);
				counterStart = counter;
				cout << "time " << counter << "ms: Process " << current->id << " started using the CPU " << printQueue(queue) << endl;
				
			}
			else
			{
				current = NULL;
			}
			
		}
		else
		//if the process has no cpu burst remaining the process is terminated or set to null if no other process are on the ready queue
		{
			cout<< "time " << counter << "ms: Process " << current->id <<  " terminated " << printQueue(queue) << endl;
			loadCPU(counter, queue, ioWait, current, counterStart,input);
			
			
			
			if(!queue.empty())
			{
				current = *queue.begin();
				queue.pop_front();
				loadCPU(counter, queue, ioWait, current, counterStart,input);
				counterStart = counter;
				cout << "time " << counter << "ms: Process " << current->id << " started using the CPU " << printQueue(queue) << endl;
			}	
			else
			{
				current  = NULL;
			}
		}

}
void loadCPU(int &counter, list<Process*> &queue, list<Process*> &ioWait, Process* &current, int &counterStart,list<Process*> &input)
{
	int delay;

	for(delay = 0; delay < 3; delay++)
	{
		checkIoWait(counter, ioWait, queue);
		checkArrivals(input,queue,counter, 1);
		counter++;
	}
}


void copyList(list<Process*> &queue, list<Process*> &copyArray)
{
	//will be used to make a copy list of the Processes
	list<Process*>::iterator itr = queue.begin();
	Process* copy;
	while(itr != queue.end())
	{
		copy = new Process((*(*itr)));
		copyArray.push_back(copy);
		++itr;
	}
	
}

void freeList(list<Process*> & toFree)
{
	//used to free a list of objects from the heap
	list<Process*>::iterator iterator;
	for (iterator = toFree.begin(); iterator != toFree.end(); ++iterator) {
		delete(*iterator);
	}	
}

void resultsRR(list<Process*> &finished, int preemptions, int context_switches, int totalWait, int totalTurnaround, int totBursts , ofstream &file){
	float averageBurst = 0;
	float averageTurnaround = 0;
	float averageWait = 0;

	list<Process*>::iterator itr = finished.begin();
	while(itr != finished.end())
	{
		averageBurst += (*itr)->burstTime * (*itr)->ogNumBurst;
		averageWait += (*itr)->ioTime * ( (*itr)->numBurst - 1 ) ;
		//(*itr)->print();
		++itr;
	}
	averageBurst /= totBursts;
	averageTurnaround = (float)totalTurnaround / totBursts;
	averageWait = (float)totalWait / totBursts;
	//averageWait = totalWait - (context_switches + preemptions)*6;	
	#if 0
	cout << "Algorithm RR" << endl;
	cout << "-- average CPU burst time: " <<  averageBurst << " ms" << endl;
	cout << "-- average wait time: " << averageWait << " ms" << endl;
	cout << "-- average turnaround time: " <<  averageTurnaround << " ms" << endl;
	cout << "-- total number of context switches: " << context_switches << endl;
	cout << "-- total number of preemptions: " << preemptions << endl;
	//cout << "-- total number of bursts: " << totBursts << endl;
	#endif
	file << "Algorithm RR" << endl;
	printStatistics(file, averageWait, averageBurst, averageTurnaround, preemptions, context_switches);
	file.close();
	freeList(finished);
}

void printStatistics(ofstream &file, float &averageWait, float &averageBurst, float &averageTurnaround, int &totalPreemptions, int &numContextSwitch)
{
	file << fixed << setprecision(2) << "-- average CPU burst time: " <<  averageBurst << " ms" << endl;
	file << "-- average wait time: " << averageWait << " ms" << endl;
	file << "-- average turnaround time: " <<  averageTurnaround << " ms" << endl;
	file << "-- total number of context switches: " << numContextSwitch << endl;
	file << "-- total number of preemptions: " << totalPreemptions << endl;
}







