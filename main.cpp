#include<iostream>
#include<fstream> 
#include<vector>
#include<string>
#include<list>
#include<sstream>
#include<stdlib.h>
#include"Process.h"

#define t_cs 6
#define t_slice 94

using namespace std;

// <id>|<arrivalTime>|<burstTime>|<numBurst>|<ioTime>

///////////////FUNCTIONS DECLARATIONS//////////////////////////
void readFile(list<Process*> &input, char* fileName);
void parseLine(list<Process*> &input, string &line);
void FCFS(list<Process*> input);
void SRT(list<Process*> &input);
void RR(list<Process*> input);
void checkArrivals(list<Process*> &input, list<Process*> &toAdd, int currTime);
string printQueue(const list<Process*> &queue);
//bool compare_id(const Process& p1, const Process& p2);


///////////////MAIN/////////////////////////////////
int main(int argc, char* argv[])
{
	if(argc < 2){
		cerr << "ERROR: Not enough command line arguments" << endl;
		return EXIT_FAILURE;
	}
	//reading file onto a vector
	list<Process*> inputData;
	char* fileName = argv[1];
	readFile(inputData,fileName);
	/////////FCFS////////////
	FCFS(inputData); 
	SRT(inputData);
	RR(inputData);
	
	//this will delete all the data from the list<Process*> 
	list<Process*>::const_iterator iterator;
	for (iterator = inputData.begin(); iterator != inputData.end(); ++iterator) {

		delete(*iterator);
	}	
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

	while(getline(ss, token, '|')) {
		vars.push_back(token);
}
	Process* node = new Process(vars[0], atoi(vars[1].c_str()), atoi(vars[2].c_str()), atoi(vars[3].c_str()), atoi(vars[4].c_str()));
	
	input.push_back(node);
	//cout << input.size() << endl;
}


void FCFS(list<Process*> input)
{
	#if 0
	//All time gone by counter
	int counter = 0;
	int counter =0;
	int counterStart = 0;
	cout << "time " << counter << "ms: Simulator started for FCFS [Q <empty>]" << endl;
	//priority q for all arriving process
	list<Process*> queue;
	list<Process*> ioWait;
	//the first process to arrive
	Process* current = *input.begin();
	input.pop_front();
	
	while(queue.size() > 0 || current != NULL)
	{
		checkArrivals(input,queue, counter);
		if((counter-counterStart) == (*current).burstTime)
		{
			ioWait.push_back(current);
			(*current).numBurst--;
			current = *queue.begin();
			queue.pop_front();	
			counterStart = counter;
					
		}
		counter++;
		counterStart++;
	}
	#endif
}

void SRT(list<Process*> &input)
{
	
}

void RR(list<Process*> input)
{	
	//priority q for all arriving process
	list<Process*> queue;
	list<Process*> newArrivals;
	list<Process*> toAdd;
	Process* current = NULL;
	Process* p_cs = NULL;  //the process to be in the context switch
	//current should = NULL while cs has a value
	int cs_counter = 0; //counts up to 3
	int ts_expire = 0; //time the current time slice will expire upon (process start time + t_slice)
	bool cs = false;
	//bool begin = true;
	int i = 0;
	list<Process*>::iterator itr;
	cout << "time " << i << "ms: Simulator started for RR " << printQueue(queue) << endl;
	while(1){
		cout << "time " << i << endl;
		checkArrivals(input, newArrivals, i);
		if(cs && cs_counter == t_cs/2){
			if(!queue.empty()){
				if( p_cs!= NULL ){
					//add p_cs to I/o wait queue
					/*ADD OLD PC_S TO i/o wait queue here */
				}
				p_cs = *queue.begin();
				queue.pop_front();
			}
		}
		if(cs && cs_counter == 6){
			cs = false;
			cs_counter = 0;
			current = p_cs;
			p_cs = NULL;
			cout << "time " << i << "ms: Process " << (*current).id << " started using the CPU " << printQueue(queue) << endl;
			if( (*current).burstRemain == 0 ){
				(*current).burstRemain = (*current).burstTime;
			}
			ts_expire = i + t_slice;
		}




		//process completes CPU burst/terminates
		if( !cs && current != NULL && (*current).burstRemain == 0 ){
			//set burst remain to I/O time and add to I/O queue

		
			(*current).burstRemain = (*current).ioTime;
			//Process has more burst remaining
			if( (*current).numBurst > 0 ){
				--(*current).numBurst;
				cout << "time " << i << "ms: Process " << (*current).id << " completed a CPU burst; " << (*current).numBurst << " bursts to go " << printQueue(queue) << endl;
			}
			//Process has no more bursts remaining
			else{
				cout << "time " << i <<"ms: Process " << (*current).id << " terminated " << printQueue(queue) << endl;
			}
			current = NULL;
			cs = true;
			cs_counter = 0;
		}

		#if 0
		//premption occurs due to time slice
		if(!cs && ts_expire == i){

		}
		#endif
		

		//Handle print statements for newArrival processes
		while(!newArrivals.empty()){
			itr = newArrivals.begin();
			cout << "time " << i << "ms: Process " << (*itr)->id <<" arrived and added to ready queue " << printQueue(queue) << endl;
			newArrivals.pop_front();
			toAdd.push_back(*itr);
		}





	#if 1
		if(i == 100){
			return;
		}
	#endif
		if(cs){
			++cs_counter;
		}

		//officially add all processes to queue
		#if 0
		sort(toAdd.begin(),toAdd.end(), id_sort());
		#endif
		while(!toAdd.empty()){
			itr = toAdd.begin();
			#if 0
			cout << "time " << i << "ms: Process " << (*itr)->id <<" added to ready queue " << printQueue(queue) << endl;
			#endif
			toAdd.pop_front();
			queue.push_back(*itr);
		}



		//start condition (count from 3 to 6)
		if(!queue.empty() && current == NULL && p_cs == NULL && cs == false){
			p_cs = *queue.begin();
			queue.pop_front();
			cs = true;
			cs_counter = 4;
		}



		#if 1
		if(queue.empty() && input.empty() && current == NULL && p_cs == NULL ){
			//cout << "Time: " << i << ", RR end condition reached." << endl;
			printf("time %dms: Simulator ended for RR\n", i);
			return;
		}
		#endif
		++i;
	}
}
void checkArrivals(list<Process*> &input, list<Process*> &toAdd, int currTime){
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

		}
		else if( (*itr)->arrivalTime > currTime ){
			break;

		}
		++i;
	}
	for(int j = 0; j < i; ++j){
		input.pop_front();
	}

	#if 1
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
		}
		
	}
	s += "]";
	return s;
}














