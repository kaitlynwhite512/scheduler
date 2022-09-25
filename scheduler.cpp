#include <stdio.h>
//#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <queue>
#include <string>
#include <vector>
#include <iostream>
//compile with g++

using namespace std;

struct process {
	string processName;
	double arrivalTime;
	double executionTime;
};
class scheduler {
	//process* inputProcesses;
	double* startTime = NULL; double* stopTime = NULL;
public:
	void FCFS(vector<process> p);
	void preemptiveSJF(vector<process> p);
};
void scheduler::FCFS(vector<process> p) {
	//write code here to print the schedule for given processes
	int currentProcess;
	int x;
	int firstProcessIndex = 0;
	int startTime = 0;
	int stopTime = 0;
	while (p.size() > 1) {
		currentProcess = p[0].arrivalTime;
		for (int i = 0; i < p.size(); i++) {
			if (p[i].arrivalTime < currentProcess) {
				currentProcess = p[i].arrivalTime;
				firstProcessIndex = i;
			}
		}
		if (startTime < p[firstProcessIndex].arrivalTime) {
			startTime = p[firstProcessIndex].arrivalTime;
			stopTime = startTime + p[firstProcessIndex].executionTime;
		}
		else {
			stopTime = stopTime + p[firstProcessIndex].executionTime;
		}
		cout << "=====" << endl;
		cout << "Scheduler for " << p[firstProcessIndex].processName << endl;
		cout << "Start time		Stop time" << endl;
		cout << startTime << "			" << stopTime << endl;
		p.erase(p.begin() + firstProcessIndex);
		startTime = stopTime;
	}
	if (startTime < p[0].arrivalTime) {
		startTime = p[0].arrivalTime;
		stopTime = startTime + p[0].executionTime;
	}
	else {
		stopTime = stopTime + p[0].executionTime;
	}
	cout << "=====" << endl;
	cout << "Scheduler for " << p[0].processName << endl;
	cout << "Start time		Stop time" << endl;
	cout << startTime << "			" << stopTime << endl;
}
void scheduler::preemptiveSJF(vector<process> p) {
	//write code here to print the schedule for given processes
	vector<process> sorted;
	int currentProcess;
	int firstProcessIndex = 0;
	int ctr = 0;
	while (p.size() > 1) {
		currentProcess = p[0].arrivalTime;
		for (int i = 0; i < p.size(); i++) {
			if (p[i].arrivalTime < currentProcess) {
				currentProcess = p[i].arrivalTime;
				firstProcessIndex = i;
			}
		}
		sorted.push_back(process());
		sorted[ctr] = p[firstProcessIndex];
		p.erase(p.begin() + firstProcessIndex);
		ctr++;
	}
	sorted.push_back(process());
	sorted[ctr] = p[0];

	vector<process> available;
	vector<process> output;
	int totalseconds = 0;
	for (int i = 0; i < sorted.size(); i++) {
		totalseconds = totalseconds + sorted[i].executionTime;
	}
	vector<int> executionTimeList;
	for (int i = 0; i < sorted.size(); i++) {
		executionTimeList.push_back(sorted[i].executionTime);
	}
	int outputindex = 0;
	int secondspassed = 0;
	int smallest;
	int smallestIndex;
	while (secondspassed < totalseconds) {
		for (int i = 0; i < sorted.size(); i++) {
			//if the arrival time is smaller then seconds passed
			//it goes into a vector to get sorted for smallest execution time
			if (sorted[i].arrivalTime <= secondspassed) {
				available.push_back(process());
				available[i] = sorted[i];
			}
		}
		smallest = available[0].executionTime;
		smallestIndex = 0;
		for (int i = 0; i < available.size(); i++) {
			//records process with smallest execution time
			if (available[i].executionTime < smallest) {
				smallest = available[i].executionTime;
				smallestIndex = i;
			}
		}
		//clears available vector in preparation for next loop
		available.clear();
		//if the current process still has the origional execution time
		//records start time as current time
		if (sorted[smallestIndex].executionTime == executionTimeList[smallestIndex]) {
			output.push_back(process());
			output[outputindex].processName = sorted[smallestIndex].processName;
			output[outputindex].arrivalTime = secondspassed;
			outputindex++;
		}

		//subtracts a second from the execution time of current process
		sorted[smallestIndex].executionTime = sorted[smallestIndex].executionTime - 1;

		//when run time becomes zero it records seconds passed as stop time
		if (sorted[smallestIndex].executionTime == 0) {
			for (int i = 0; i < output.size(); i++) {
				if (output[i].processName == sorted[smallestIndex].processName) {
					output[i].executionTime = secondspassed + 1;
				}
			}
			sorted.erase(sorted.begin() + smallestIndex);
			executionTimeList.erase(executionTimeList.begin() + smallestIndex);
		}
		secondspassed++;
	}
	for (int i = 0; i < output.size(); i++) {

		cout << "=====" << endl;
		cout << "Scheduler for " << output[i].processName << endl;
		cout << "Start time		Stop time" << endl;
		cout << output[i].arrivalTime << "			" << output[i].executionTime << endl;
	}
}
int main(int argc, char** argv) {
	//write code here to create schedule for the input processes
	string choice = argv[argc - 1];
	vector<process> processList;
	
	int i = 1;
	int loopctr = 0;
	while (i < argc - 2) {
		processList.push_back(process());
		processList[loopctr].processName = argv[i];
		i++;
		processList[loopctr].arrivalTime = strtol(argv[i], NULL, 10);
		i++;
		processList[loopctr].executionTime = strtol(argv[i], NULL, 10);
		i++;
		loopctr++;
	}

	scheduler x;

	if (choice == "FCFS") {
		x.FCFS(processList);
	}
	if (choice == "SJF") {
		x.preemptiveSJF(processList);
	}
}