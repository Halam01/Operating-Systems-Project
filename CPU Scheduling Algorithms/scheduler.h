#pragma once
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <vector>

using namespace std;



struct process {
	int arrival;
	int service;
	int turnAround;
	bool finished;
	int MLF;
	int slotsUsed;
	int pid;
};


struct result {
	vector<process> p;
	float avgtat;
};

class Scheduler {
public:
	Scheduler();
	~Scheduler();
	void addProcess(char* arrival, char* service);
	result FIFO();
	result SJF();
	result SRT();
	result MLF();
	vector<process> pList;
	ofstream outfile;
};