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

// <id>|<arrivalTime>|<burstTime>|<numBursts>|<ioTime>

///////////////FUNCTIONS DECLARATIONS//////////////////////////

void readFile(list<Process*> &input, char* fileName);
void parseLine(list<Process*> &input, string &line);
void FCFS(list<Process*> input);
void SRT(list<Process*> &input);
void RR(list<Process*> input);
void checkArrivals(list<Process*> &input, list<Process*> &toAdd, int currTime);
string printQueue(const list<Process*> &queue);
void checkCurrent(list<Process*> &queue, list<Process*> &ioWait, Process* &current, int counter ,int &counterStart);
void checkIoWait(int counter, list<Process*> &ioWait , list<Process*> &queue);

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
	readFile(inputData,fileName);
	//////////////FCFS//////////////////
	FCFS(inputData); 
	SRT(inputData);
	//RR(inputData);
	
	//this will delete all the data from the list<Process*> 
	list<Process*>::iterator iterator;
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

	while(getline(ss, token, '|')) 
	{
		vars.push_back(token);
	}
	Process* node = new Process(vars[0], atoi(vars[1].c_str()), atoi(vars[2].c_str()), atoi(vars[3].c_str()), atoi(vars[4].c_str()));
	
	input.push_back(node);
}


void FCFS(list<Process*> input)
{
	#if 1  
	//All time gone by counter
	int counter = 0;
	int counterStart = 0;
	cout << "time " << counter << "ms: Simulator started for FCFS [Q <empty>]" << endl;
	//priority q for all arriving process
	list<Process*> queue;
	list<Process*> ioWait;
	//the first process to arrive
	Process* current = *input.begin();
	input.pop_front();
	string debug;
	if(queue.size()  == 0 && ioWait.size() > 0)
	{
		cout << "REEEEEEEEEEEEEEEEEEEEEEEEEEee" << endl;
	}
	
	while(queue.size() > 0 || current != NULL || ioWait.size() > 0)
	{

		//checks for any new arrivals, and puts them in the back of the queue
		checkArrivals(input,queue, counter);
		//checks for proceces that have expired in the ioWait queue to put in the ready queue
		checkIoWait(counter, ioWait, queue);
		//checks that the current Process that is being handled has not expiered
			
		if(current != NULL && ((counter-counterStart) == (*current).burstTime))
		{
			cout << "at Process done" << endl;
			cin >> debug;
			checkCurrent(queue,ioWait, current, counter , counterStart);	
		}
		else if(current  == NULL)
		{
			
			if(queue.size() > 0)
			{
			current = *queue.begin();
			counterStart = counter;	
			}		
		}
		else if(((counter-counterStart) > (*current).burstTime))
		{
			cout << " mutha fvcking error "  << endl;
		}
		//checks that no process with io wait 0ms was added to the queue
		counter++;
	}
	#endif
}

void SRT(list<Process*> &input)
{
	
}

void RR(list<Process*> input)
{	
	#if 0
	//priority q for all arriving process
	list<Process*> queue;
	list<Process*> toAdd;
	Process* current = NULL;
	Process* p_cs = NULL;  //the process to be in the context switch
	//current should = NULL while cs has a value
	int cs_counter = 0; //counts up to 3
	bool cs = true;
	int i = 0;
	cout << "time " << i << "ms: Simulator started for RR " << printQueue(queue) << endl;
	while(1){
		toAdd.clear();
		checkArrivals(input, toAdd, i);
		if(cs_counter == 3){
			cs_counter = 0;
		}
		#if 1
		if(queue.empty() && input.empty() && current == NULL && p_cs == NULL ){
			cout << "Time: " << i << ", RR end condition reached." << endl;
			return;
		}
		#endif
		++i;
	}
	#endif
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
			cout << "time " << currTime << "ms: Process " << (*(*(itr))).id << " arrived and added to ready queue " << printQueue(toAdd) << endl;
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

string printQueue(const list<Process*> &queue)
{
	string s = "[Q";
	if(queue.empty())
	{
 		s += " <empty>]";
	}
	#if 1 
	else{
		list<Process*>::const_iterator itr;
		for(itr = queue.begin(); itr != queue.end(); ++itr)
		{
			s += " ";	
			s += (*(*itr)).id;
		}
		s += "]";
	}
	#endif
	return s;
}

void checkIoWait(int counter, list<Process*> &ioWait , list<Process*> &queue)
{
	list<Process*>::iterator iterator = ioWait.begin();
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
				cout << "before" << endl << printQueue(queue) << "  " << printQueue(ioWait) << endl;
				(*iterator)->print();
				//pushes process to the end of the queue and deletes it from the ioWait 
				// queue
				queue.push_back(*iterator);
				// alternatively, i = items.erase(i);
				cout << "time " << counter << "ms: Process " << (*iterator)->id << " completed I/O; added to ready queue " << printQueue(queue) << endl; 
				ioWait.erase(iterator++);  
				
				cout << "after" << endl << printQueue(queue) << "  " << printQueue(ioWait) << endl;
				
			
			}
			// case two
			else
			{
				cout << (*iterator)->id << " is done and has been removed from precesses" << endl;	
				ioWait.erase(iterator++);  // alternatively, i = items.erase(i);

			}
		}
		//just increment the iterator
		else
		{
			++iterator;
		}
	}
}




void checkCurrent(list<Process*> &queue, list<Process*> &ioWait, Process* &current, int counter ,int &counterStart)
{
		(*current).numBurst--;
		if(current->numBurst > 0 )
		{
			current->ioWaitEnd = (int)((int)counter + (int)(current->ioTime));
			ioWait.push_back(current);
			

			cout << "time " << counter << "ms: Process " << (*current).id <<" completed a CPU burst; " << (*current).numBurst << " bursts to go" << printQueue(queue) << endl;
			cout << "time " << counter << "ms: Process " <<  (*current).id << " switching out of CPU; will block on I/O until time " << (*current).ioWaitEnd << "ms " << printQueue(queue) << endl;                                   

			if(queue.size() > 0)
			{	
				current = *queue.begin();
				queue.pop_front();	
				counterStart = counter;
			}
			else
			{
				current == NULL;
			}
			
			cout << "time " << counter << "ms: Process " << current->id << " started using the CPU " << printQueue(queue) << endl;
		}
		else
		{
			cout<< "time " << counter << "ms: Process " << current->id <<  "terminated " << printQueue(queue) << endl;
			
			current = *queue.begin();
			queue.pop_front();
			counterStart = counter;
			
			cout << "time " << counter << "ms: Process " << current->id << " started using the CPU " << printQueue(queue) << endl;


		}

}
