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

	#if 0
		cout << "Time: " << currTime << ", Input: ";
		for(itr = input.begin(); itr != input.end(); itr++){
			cout << (*(*itr)).id << ", ";
		}
		cout << endl;
		cout << "toAdd: ";
		for(itr = toAdd.begin(); itr != toAdd.end(); itr++){
			cout << (*(*itr)).id << ", ";
		}
		cout << endl << endl;
	#endif
}

string printQueue(const list<Process*> &queue){
	string s = "[Q ";
	if(queue.empty()){
		 s += "<empty>]";
	}
	else{
		list<Process*>::const_iterator itr = queue.begin();
		for(; itr != queue.end(); ++itr){

		}
		s += "]";
	}
	return s;
}
















