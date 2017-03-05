#include<iostream>
#include<fstream> 
#include<vector>
#include<string>
#include<list>
#include<sstream>
#include<stdlib.h>
#include"Process.h"

#if 1
 Carlos is a cocksucker
# endif

using namespace std;

// <id>|<arrivalTime>|<burstTime>|<numBursts>|<ioTime>

///////////////FUNCTIONS DECLARATIONS//////////////////////////
void readFile(list<Process*> &input, char* fileName);
void parseLine(list<Process*> &input, string &line);
void checkArrivals(list<Process*> &input, list<Process*> queue);
void FCFS(list<Process*> input);
void SRT(list<Process*> &input, string &line);
void RR(list<Process*> &input, string &line);

///////////////MAIN/////////////////////////////////
int main(int argc, char* argv[])
{
	//reading file onto a vector
	list<Process*> inputData;
	char* fileName = argv[1];
	readFile(inputData,fileName);
	/////////FCFS////////////
	FCFS(inputData); 
	
	
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

void checkArrivals(list<Process*> &input, list<Process*> queue)
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
	//All time gone by counter
	int counter =0;
	//priority q for all arriving process
	list<Process*> queue;
	//the first process to arrive
	Process* current = *input.begin();
	input.pop_front();
	while(queue.size() > 0 || current != NULL)
	{
		checkArrivals(input,queue);
		if(counter == (*current).burstTime )
		{
			(*current).numBurst--;
		}	
		counter++;
	}
}

void SRT(list<Process*> &input, string &line)
{
	
}

void RR(list<Process*> &input, string &line)
{
	
}
