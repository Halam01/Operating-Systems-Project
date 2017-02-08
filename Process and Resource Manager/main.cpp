#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include "Scheduler.h"
#include <cstdlib>
#include <algorithm>
#include <iterator>
using namespace std;


Scheduler::Scheduler() {
	init();
}
void Scheduler::init() {
	
	R1 = Resource();
	R1.rid = "R1";
	R1.status = 1;
	R1.max = 1;
	R1.waitingProcesses = vector<ProcessNode*>();
	R2 = Resource();
	R2.rid = "R2";
	R2.status = 2;
	R2.max = 2;
	R2.waitingProcesses = vector<ProcessNode*>();
	R3 = Resource();
	R3.rid = "R3";
	R3.status = 3;
	R3.max = 3;
	R3.waitingProcesses = vector<ProcessNode*>();
	R4 = Resource();
	R4.rid = "R4";
	R4.status = 4;
	R4.max = 4;
	R4.waitingProcesses = vector<ProcessNode*>();
	resources = vector<Resource>();
	resources.push_back(R1);
	resources.push_back(R2);
	resources.push_back(R3);
	resources.push_back(R4);

	
	status = "ready";

	ProcessNode *p = new ProcessNode();
	p->pid = "init";
	p->parent = "init";
	p->priority = 0;
	p->status = "running";
	p->pos = 0;
	p->children = vector<ProcessNode*>();
	p->otherResources = vector<OtherResources>();

	processList = vector<ProcessNode*>();
	processList.push_back(p);
	currentProcess = p;


};
void Scheduler::find_nextp() {
	bool set_new_curr = false;
	if (processList.size() <= 1) {
		currentProcess = processList[0];
		currentProcess->status = "running";
		set_new_curr = true;
	}
	else {
		if (curr_del) {
			for (vector<ProcessNode*>::iterator it = processList.begin(); it != processList.end(); ++it) {
				if ((*it)->priority == 2 && (*it)->status == "ready") {
					currentProcess = (*it);
					currentProcess->status = "running";
					set_new_curr = true;
					break;
				}
			}
if (!set_new_curr) {
	for (vector<ProcessNode*>::iterator it = processList.begin(); it != processList.end(); ++it) {
		if ((*it)->priority == 1 && (*it)->status == "ready") {
			currentProcess = (*it);
			currentProcess->status = "running";
			set_new_curr = true;
			break;
		}
	}
}
		}
		else {
			for (vector<ProcessNode*>::iterator it = processList.begin(); it != processList.end(); ++it) {
				if ((*it)->priority >= currentProcess->priority && (*it)->status == "ready" && (*it) != currentProcess) {
					currentProcess = (*it);
					currentProcess->status = "running";
					set_new_curr = true;
					break;
				}
			}
			if (!set_new_curr) {
				if (currentProcess->status == "blocked") {
					currentProcess = processList[0];
					currentProcess->status = "running";
					set_new_curr = true;
				}
				else {
					currentProcess->status = "running";
				}
			}
		}
	}
};
bool Scheduler::cr(char* name, int priority) {
	bool is_in = false;
	if (strcmp(name, "init") == 0) {
		return false;
	}
	if (priority > 2 || priority < 1) {
		return false;
	}
	for (vector<ProcessNode*>::iterator it = processList.begin(); it != processList.end(); ++it) {
		if (strcmp((*it)->pid, name) == 0) {
			is_in = true;
			break;
		}
	}
	if (!is_in) {
		ProcessNode *newp = new ProcessNode();
		newp->pid = name;
		newp->status = "ready";
		newp->priority = priority;
		newp->pos = currentProcess->pos + 1; //
		newp->parent = currentProcess->pid;	//processList[0]
		newp->otherResources = vector<OtherResources>();
		newp->children = vector<ProcessNode*>();
		currentProcess->children.push_back(newp);
		processList.push_back(newp);
		if (newp->priority > currentProcess->priority) {
			newp->status = "running";
			currentProcess->status = "ready";
			currentProcess = newp;
		}
		return true;
	}
	else {
		cout << "cannot create duplicate process with same name" << endl;
		return false;
	}

};

bool Scheduler::de(char* name) {
	bool is_in = false;
	int d1 = 0;
	int d2 = 0;
	for (vector<ProcessNode*>::iterator it = processList.begin(); it != processList.end(); ++it) {
		if (strcmp((*it)->pid, name) == 0) {
			is_in = true;
			break;
		}
	}
	if (is_in) {
		for (vector<ProcessNode*>::iterator git = processList.begin(); git != processList.end(); ++git) {
			if (strcmp((*git)->pid, currentProcess->pid) == 0) {
				
				d1 = (*git)->pos;
				//cout << (*git)->pid << " d1 " << d1 << endl;
			}
			else if(strcmp((*git)->pid, name) == 0){
				d2 = (*git)->pos;
				//cout << (*git)->pid << " d2 " << d2 << endl;
			}
			if (d1 != 0 && d2 != 0) {
				//cout << d1 - d2 << endl;
				//cout << "Current proc " << currentProcess->pid << endl;
				if (d2 <= d1) {
					//cout << "Cannot delete a process higher than current process" << endl;
					return false;
				}
				if (d2 > d1) {
					break;
				}
			}
		}
		curr_del = false;
		vector<OtherResources> rel_res = vector<OtherResources>();
		for (vector<ProcessNode*>::iterator it = processList.begin(); it != processList.end();) {
			if (strcmp((*it)->pid, name) == 0 || strcmp((*it)->parent, name) == 0) {
				if (*it == currentProcess) {
					curr_del = true;
				}
				for (vector<OtherResources>::iterator rit = (*it)->otherResources.begin(); rit != (*it)->otherResources.end(); ++rit) {
					for (vector<Resource>::iterator qit = resources.begin(); qit != resources.end(); ++qit) {
						if (strcmp((rit)->rid, qit->rid) == 0) {
							qit->status += (rit)->numUnits;
						}
					}
					
				}
				
				//cout << "deleting " << (*it)->pid << endl;
				delete *it;
				it = processList.erase(it);
				
			}
			else {
				++it;
			}
		}
		for (vector <Resource>::iterator pit = resources.begin(); pit != resources.end(); ++pit) {
			for (vector<ProcessNode*>::iterator wit = (pit)->waitingProcesses.begin(); wit != (pit)->waitingProcesses.end(); ++wit) {
				if ((*wit)->numWaiting <= pit->status) {
					pit->status = pit->status - (*wit)->numWaiting;
					(*wit)->numWaiting = 0;
					(*wit)->status = "ready";
				}
			}
		}
		if (curr_del) {
			find_nextp();
		}
		//cout << "process list size " << processList.size() << endl;
		return true;
	}
	else {
		cout << "that process does not exist" << endl;
		return false;
	}
};
bool Scheduler::req(char* name, int units) {
	if (strcmp(currentProcess->pid, "init") == 0) {
		return false;
	}
	for (vector<Resource>::iterator it = resources.begin(); it != resources.end(); ++it) {
		if (strcmp((it)->rid, name) == 0) {
			if (units <= (it)->status) {
				OtherResources rs = OtherResources();
				rs.numUnits = units;
				rs.rid = name;
				currentProcess->otherResources.push_back(rs);
				(it)->status -= units;
				return true;
				//cout << "requested units successfully" << endl;
			}
			else {
				cout << "*Process " << currentProcess->pid << " is blocked. ";
				(it)->waitingProcesses.push_back(currentProcess);
				currentProcess->status = "blocked";
				currentProcess->numWaiting = units;
				find_nextp();
				return true;
			}
		}
	}
	return false;
};
bool Scheduler::rel(char* name, int units) {
	bool is_in = false;
	if (strcmp(currentProcess->pid, "init") == 0) {
		return false;
	}
	bool released = false;
	for (vector<Resource>::iterator it = resources.begin(); it != resources.end(); ++it) {
		if (strcmp((it)->rid, name) == 0) {
			//cout << "FOUND THE RESOURCE TO RELEASE" << endl;
			for (vector<OtherResources>::iterator rit = currentProcess->otherResources.begin(); rit != currentProcess->otherResources.end(); ++rit) {
				if (strcmp((rit)->rid, name) == 0){
					//cout << "CURR PROC HAS RESOURCE TO BE RELEASE" << endl;
					is_in = true;
					if (units < (rit)->numUnits) {
						rit->numUnits -= units;
						(it)->status += units;
						//cout << "released some of units" << endl;
						released = true;
						break;
					}
					else if (units == (rit)->numUnits) {
						rit->numUnits -= units;
						(it)->status += units;
						rit = currentProcess->otherResources.erase(rit);
						//cout << "released all units" << endl;
						released = true;
						break;
					}
					else {
						//cout << "cannot release that many units" << endl;
						break;
					}
				}
			}
			if (released) {
				//cout << "RELEASED UNITS" << endl;
				for (vector<ProcessNode*>::iterator pit = (it)->waitingProcesses.begin(); pit != (it)->waitingProcesses.end(); ++pit) {
					if (units == 0) {
						find_nextp();
						break;
					}
					else {
						if ((*pit)->numWaiting == units) {
							(*pit)->numWaiting = 0;
							(*pit)->status = "ready";
							req((*pit)->pid, units);
							//delete *pit;
							pit = (it)->waitingProcesses.erase(pit);
							units = 0;
						}
						else if ((*pit)->numWaiting > units) {
							(*pit)->numWaiting -= units;
							units = 0;
						}
						else if ((*pit)->numWaiting < units) {
							units = units - (*pit)->numWaiting;
							(*pit)->numWaiting = 0;
							(*pit)->status = "ready";
							req((*pit)->pid, units);
							pit = (it)->waitingProcesses.erase(pit);
						}
					}
				}
			}
		}
	}
	return true;
};
void Scheduler::to() {
	currentProcess->status = "ready";
	for (vector<ProcessNode*>::iterator it = processList.begin(); it != processList.end(); ++it) {
		if (strcmp((*it)->pid, currentProcess->pid) == 0) {
			processList.erase(it);
			processList.push_back(currentProcess);
			break;
		}
	}
	find_nextp();
};
Scheduler::~Scheduler() {};



void main() {
	string inputpathname, outputpathname, line;
	ifstream inputfile;
	ofstream outputfile;
	char *command[3];
	char *token;

	cout << "Enter test file pathname to open: ";
	cin >> inputpathname;
	cout << "Enter results output file pathname: ";
	cin >> outputpathname;
	inputfile.open(inputpathname);
	outputfile.open(outputpathname);
	Scheduler schedule = Scheduler();
	outputfile << "init ";
	if (inputfile.is_open()) {
		while (getline(inputfile, line)) {
			command[0] = "";
			command[1] = "";
			command[2] = "";
			char *linetp = new char[line.length() + 1];
			strcpy(linetp, line.c_str());
			token = strtok(linetp, " ");
			int i = 0;
			while (token != NULL) {
				command[i] = token;
				//cout << command[i] << endl;
				i++;
				token = strtok(NULL, " ");
			}
			if (strcmp(command[0],"cr") == 0) {
				bool success = schedule.cr(command[1], atoi(command[2]));
				if (success) {
					cout << "*Process " << schedule.currentProcess->pid << " is running." << endl;
					outputfile << schedule.currentProcess->pid << " ";
				}
				else {
					outputfile << "error ";
				}
			}
			if (strcmp(command[0], "de") == 0) {
				bool success_de = schedule.de(command[1]);
				if (success_de) {
					cout << "*Process " << schedule.currentProcess->pid << " is running." << endl;
					outputfile << schedule.currentProcess->pid << " ";
				}
				else {
					outputfile << "error ";
				}
			}
			if (strcmp(command[0], "req") == 0) {
				if (atoi(command[2]) < 0) {
					outputfile << "error ";
				}
				else if (strcmp(command[1], "R1") == 0 && atoi(command[2]) > 1) {
					outputfile << "error ";
				}
				else if (strcmp(command[1], "R2") == 0 && atoi(command[2]) > 2) {
					outputfile << "error ";
				}
				else if (strcmp(command[1], "R3") == 0 && atoi(command[2]) > 3) {
					outputfile << "error ";
				}
				else if (strcmp(command[1], "R4") == 0 && atoi(command[2]) > 4) {
					outputfile << "error ";
				}
				else {
					bool success_req = schedule.req(command[1], atoi(command[2]));
					if (success_req) {
						cout << "*Process " << schedule.currentProcess->pid << " is running." << endl;
						outputfile << schedule.currentProcess->pid << " ";
					}
					else {
						outputfile << "error ";
					}
				}
			}
			if (strcmp(command[0], "rel") == 0) {
				if (atoi(command[2]) < 0) {
					outputfile << "error ";
				}
				else if (strcmp(command[1], "R1") == 0 && atoi(command[2]) > 1) {
					outputfile << "error ";
				}
				else if (strcmp(command[1], "R2") == 0 && atoi(command[2]) > 2) {
					outputfile << "error ";
				}
				else if (strcmp(command[1], "R3") == 0 && atoi(command[2]) > 3) {
					outputfile << "error ";
				}
				else if (strcmp(command[1], "R4") == 0 && atoi(command[2]) > 4) {
					outputfile << "error ";
				}
				else {
					bool success_rel = schedule.rel(command[1], atoi(command[2]));
					if (success_rel) {
						cout << "*Process " << schedule.currentProcess->pid << " is running." << endl;
						outputfile << schedule.currentProcess->pid << " ";
					}
					else {
						outputfile << "error ";
					}
				}
			}
			if (strcmp(command[0], "to") == 0) {
				schedule.to();
				cout << "*Process " << schedule.currentProcess->pid << " is running." << endl;
				outputfile << schedule.currentProcess->pid << " ";
			}
			if (strcmp(command[0], "init") == 0) {
				outputfile << endl;
				schedule.init();
				outputfile << schedule.currentProcess->pid << " ";
			}
			else {
				continue;
			}
		}
	}
	
	inputfile.close();
	outputfile.close();
}