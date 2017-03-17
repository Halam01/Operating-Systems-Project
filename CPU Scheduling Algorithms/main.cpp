#include "scheduler.h"
using namespace std;

Scheduler::Scheduler() {};
Scheduler::~Scheduler() {};

void Scheduler::addProcess(char* arrival, char* service) {
	process p1 = { atoi(arrival), atoi(service) };
	p1.finished = false;
	p1.MLF = 4;
	p1.slotsUsed = 0;
	p1.pid = pList.size() + 1;
	pList.push_back(p1);
};
result Scheduler::FIFO() {
	vector<process> pcopy = pList;
	float avgTurnAround = 0.0;
	int size = pcopy.size();
	int timer = 0;
	int size_flag = pcopy.size();
	process *active = NULL;
	bool found = false;

	while (size_flag != 0) {
		if (active != NULL) {
			if (active->service == 0) {
				active->finished = true;
				active->turnAround = timer - active->arrival;
				avgTurnAround += timer - active->arrival;
				size_flag -= 1;
				active = NULL;
			}
			else {
				active->service--;
				timer++;
			}
		}
		else {
			for (vector<process>::iterator it = pcopy.begin(); it != pcopy.end(); it++) {
				if (it->arrival <= timer && it->finished == false) {
					active = &*it;
					found = true;
					break;
				}
			}
			if (!found) {
				timer++;
			}
			found = false;
		}
	}


	avgTurnAround = avgTurnAround / size;
	cout << setprecision(2) << fixed << floor(100 * avgTurnAround) / 100 << " ";
	for (vector<process>::iterator it = pcopy.begin(); it != pcopy.end(); it++) {
		cout << it->turnAround << " ";
	}
	cout << endl;

	result r = { pcopy, avgTurnAround };

	return r;

	
};
result Scheduler::SJF() {
	int timer = 0;
	int min = 10000000000;
	float avgTurnAround = 0.0;
	vector<process> pcopy = pList;
	int size = pcopy.size();
	int size_flag = pcopy.size();
	process *active = NULL;
	bool found = false;

	while (size_flag != 0) {
		if (active != NULL) {
			if (active->service == 0) {
				active->finished = true;
				active->turnAround = timer - active->arrival;
				avgTurnAround += timer - active->arrival;
				size_flag -= 1;
				active = NULL;
				min = 10000000000;
				for (vector<process>::iterator it = pcopy.begin(); it != pcopy.end(); it++) {
					if (it->arrival <= timer && it->service < min && it->finished == false) {
						min = it->service;
						active = &*it;
					}
				}
			}
			else {
				active->service--;
				timer++;
			}
		}
		else {
			for (vector<process>::iterator it = pcopy.begin(); it != pcopy.end(); it++) {
				if (it->arrival <= timer && it->service < min && it->finished == false) {
					min = it->service;
					active = &*it;
					found = true;
				}
			}
			if (!found) {
				timer++;
			}
			found = false;
		}
	}

	avgTurnAround = avgTurnAround / size;

	cout << setprecision(2) << fixed << floor(100 * avgTurnAround) / 100 << " ";
	for (vector<process>::iterator it = pcopy.begin(); it != pcopy.end(); it++) {
		cout << it->turnAround << " ";
	}
	cout << endl;

	result r = { pcopy, avgTurnAround };
	return r;
};
result Scheduler::SRT() {
	int timer = 0;
	int min = 10000000000;
	float avgTurnAround = 0.0;
	vector<process> pcopy = pList;
	int size = pcopy.size();
	int size_flag = pcopy.size();
	process *active = NULL;
	bool found = false;

	while (size_flag != 0) {
		if (active != NULL) {
			if (active->service == 0) {
				active->finished = true;
				active->turnAround = timer - active->arrival;
				avgTurnAround += timer - active->arrival;
				size_flag -= 1;
				active = NULL;
				min = 10000000000;
			}
			else {
				active->service--;
				timer++;
			}
			for (vector<process>::iterator it = pcopy.begin(); it != pcopy.end(); it++) {
				if (it->arrival <= timer && it->service < min && it->finished == false) {
					min = it->service;
					active = &*it;
				}
			}
		}
		else {
			for (vector<process>::iterator it = pcopy.begin(); it != pcopy.end(); it++) {
				if (it->arrival <= timer && it->service < min && it->finished == false) {
					min = it->service;
					active = &*it;
					found = true;
				}
			}
			if (!found) {
				timer++;
			}
			found = false;
		}
	}

	avgTurnAround = avgTurnAround / size;

	cout << setprecision(2) << fixed << floor(100 * avgTurnAround) / 100 << " ";
	for (vector<process>::iterator it = pcopy.begin(); it != pcopy.end(); it++) {
		cout << it->turnAround << " ";
	}
	cout << endl;

	result r = { pcopy, avgTurnAround };
	return r;
};
result Scheduler::MLF() {
	int timer = 0;
	float avgTurnAround = 0.0;
	vector<process> pcopy = pList;
	int size = pcopy.size();
	int size_flag = pcopy.size();
	process *active = NULL;
	bool found = false;
	vector<process> *mlfqueues = new vector<process>[5]();
	vector<process> finishedProcesses;
	
	for (vector<process>::iterator it = pcopy.begin(); it != pcopy.end(); it++) {
		mlfqueues[it->MLF].push_back(*it);
	}

	while (size_flag != 0) {
		if (active != NULL) {
			if (active->service == 0) {
				active->finished = true;
				active->turnAround = timer - active->arrival;
				avgTurnAround += timer - active->arrival;
				size_flag -= 1;
				for (vector<process>::iterator it = mlfqueues[active->MLF].begin(); it != mlfqueues[active->MLF].end(); it++) {
					if (it->pid == active->pid) {
						finishedProcesses.push_back(*it);
						mlfqueues[active->MLF].erase(it);
						break;
					}
				}
				active = NULL;
			}
			else if(active->MLF == 4 && active->slotsUsed == 1){
				active->MLF--;
				active->slotsUsed = 0;
				mlfqueues[active->MLF].push_back(*active);
				mlfqueues[4].erase(mlfqueues[4].begin());
				active = NULL;
			}
			else if (active->MLF == 3 && active->slotsUsed == 2) {
				active->MLF--;
				active->slotsUsed = 0;
				mlfqueues[active->MLF].push_back(*active);
				mlfqueues[3].erase(mlfqueues[3].begin());
				active = NULL;
			}
			else if (active->MLF == 2 && active->slotsUsed == 4) {
				active->MLF--;
				active->slotsUsed = 0;
				mlfqueues[active->MLF].push_back(*active);
				mlfqueues[2].erase(mlfqueues[2].begin());
				active = NULL;
			}
			else if (active->MLF == 1 && active->slotsUsed == 8) {
				active->MLF--;
				active->slotsUsed = 0;
				mlfqueues[active->MLF].push_back(*active);
				mlfqueues[1].erase(mlfqueues[1].begin());
				active = NULL;
			}
			else if (active->MLF == 0 && active->slotsUsed == 16) {
				active->slotsUsed = 0;
				mlfqueues[active->MLF].push_back(*active);
				mlfqueues[0].erase(mlfqueues[0].begin());
				active = NULL;
			}
			else {
				active->service--;
				timer++;
				active->slotsUsed++;
				for (int i = 4; i > -1; i--) {
					for (vector<process>::iterator it = mlfqueues[i].begin(); it != mlfqueues[i].end(); it++) {
						if (it->arrival <= timer && it->finished == false && it->MLF > active->MLF) {
						
							active = &*it;
							break;
						}
					}
				}
			}
			
		}
		else {
			for (int i = 4; i > -1; i--) {
				for (vector<process>::iterator it = mlfqueues[i].begin(); it != mlfqueues[i].end(); it++) {
					if (it->arrival <= timer && it->finished == false) {
						active = &*it;
						found = true;
						break;
					}
				}
				if (found) {
					break;
				}
			}
			if (!found) {
				timer++;
			}
			found = false;
		}
	}

	avgTurnAround = avgTurnAround / size;

	cout << setprecision(2) << fixed << floor(100 * avgTurnAround) / 100 << " ";
	for (vector<process>::iterator pit = pcopy.begin(); pit != pcopy.end(); pit++) {
		for (vector<process>::iterator it = finishedProcesses.begin(); it != finishedProcesses.end(); it++) {
			if (pit->pid == it->pid) {
				pit->turnAround = it->turnAround;
			}
		}
	}
	for (vector<process>::iterator pit = pcopy.begin(); pit != pcopy.end(); pit++) {
		cout << pit->turnAround << " ";
	}

	cout << endl;

	result r = { pcopy, avgTurnAround };
	return r;
};




void main() {
	string inputpathname, outputpathname, line;
	ifstream inputfile;
	ofstream outputfile;
	char *command[2];
	char *token;

	cout << "Enter test file pathname to open: ";
	cin >> inputpathname;
	cout << "Enter results output file pathname: ";
	cin >> outputpathname;
	inputfile.open(inputpathname);
	outputfile.open(outputpathname);
	Scheduler schedule;
	if (inputfile.is_open()) {
		while (getline(inputfile, line)) {
			command[0] = "";
			command[1] = "";
			char *linetp = new char[line.length() + 1];
			strcpy(linetp, line.c_str());
			token = strtok(linetp, " ");
			int i = 0;
			while (token != NULL) {
				command[i] = token;
				//cout << command[i] << endl;
				if (i == 1) {
					schedule.addProcess(command[0], command[1]);
				}
				i = (i + 1) % 2;
				token = strtok(NULL, " ");
			}
			//for (vector<process>::iterator it = schedule.pList.begin(); it != schedule.pList.end(); it++) {
			//	cout << it->arrival << " " << it->service << endl;
			//}

			//FIFO
			result fifo = schedule.FIFO();
			outputfile << setprecision(2) << fixed << floor(100*fifo.avgtat)/100 << " ";
			for (vector<process>::iterator it = fifo.p.begin(); it != fifo.p.end(); it++) {
				outputfile << it->turnAround << " ";
			}
			outputfile << endl;

			//SJF
			result sjf = schedule.SJF();
			outputfile << setprecision(2) << fixed << floor(100 * sjf.avgtat) / 100 << " ";
			for (vector<process>::iterator it = sjf.p.begin(); it != sjf.p.end(); it++) {
				outputfile << it->turnAround << " ";
			}
			outputfile << endl;

			//SRT
			result srt = schedule.SRT();
			outputfile << setprecision(2) << fixed << floor(100 * srt.avgtat) / 100 << " ";
			for (vector<process>::iterator it = srt.p.begin(); it != srt.p.end(); it++) {
				outputfile << it->turnAround << " ";
			}
			outputfile << endl;

			//MLF
			result mlf = schedule.MLF();
			outputfile << setprecision(2) << fixed << floor(100 * mlf.avgtat) / 100 << " ";
			for (vector<process>::iterator it = mlf.p.begin(); it != mlf.p.end(); it++) {
				outputfile << it->turnAround << " ";
			}
			outputfile << endl;

			inputfile.close();
			outputfile.close();
		}
	}
}