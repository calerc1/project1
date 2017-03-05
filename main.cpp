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
void checkArrivals(list<Process*> &input, list<Process*> queue);
void FCFS(list<Process*> input);
void SRT(list<Process*> &input);
void RR(list<Process*> input);
void checkArrivalsRR(list<Process*> &input, list<Process*> &toAdd, int currTime);


///////////////MAIN/////////////////////////////////
int main(int argc, char* argv[])
{
	//reading file onto a vector
	list<Process*> inputData;
	char* fileName = argv[1];
	readFile(inputData,fileName);
	/////////FCFS////////////
	FCFS(inputData); 
	SRT(inputData);
	//RR(inputData);
	
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

void checkArrivals(list<Process*> &input, list<Process*> &queue,int counter)
{
	cout << "here" << endl;
	list<Process*>::iterator itr;
	for(itr = input.begin(); itr != input.end(); itr++)
	{
		cout << (*(*itr)).id << endl;
	}
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
	
	while(queue.size() > 0 || current != NULL)
	{
		checkArrivalsRR(input,queue, counter);
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
	#if 0
	//priority q for all arriving process
	list<Process*> queue;
	list<Process*> toAdd;
	Process* current = NULL;
	Process* p_cs1 = NULL;  //the process to be the current process in 3 ms total time
	Process* p_cs2 = NULL; //the process taken away from the current process in 3 ms total time
	//current should = NULL while cs1 and cs2 have values

	int temp = 0;
	int i = 0;
	while(1){
		toAdd.clear();
		checkArrivalsRR(input, toAdd, i);

		#if 1
		if(queue.empty() && input.empty() && current == NULL && p_cs1 == NULL ){
			cout << "RR end condition reached.\node";
			return;
		}
		#endif
		++i;
	}
	#endif
}
void checkArrivalsRR(list<Process*> &input, list<Process*> &toAdd, int currTime){
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
	if(!input.empty()){
		cout << "Time: " << currTime << "Input: ";
		for(itr = input.begin(); itr != input.end(); itr++){
			cout << (*(*itr)).id << ", ";
		}
		cout << endl;
		cout << "toAdd: ";
		for(itr = toAdd.begin(); itr != toAdd.end(); itr++){
			cout << (*(*itr)).id << ", ";
		}
		cout << endl << endl;
	}
	#endif
}


















