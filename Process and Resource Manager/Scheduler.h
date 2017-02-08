#pragma once
#include <vector>



struct OtherResources {
	char *rid;
	int numUnits;
};

struct ProcessNode {
	char *pid;
	char *status;
	int priority;
	int numWaiting;
	char *parent;
	int pos;
	std::vector<ProcessNode*> children;
	std::vector<OtherResources> otherResources;
};

struct Resource {
	char *rid;
	int status;		//number of units available
	int max;
	std::vector<ProcessNode*> waitingProcesses;
};

class Scheduler {
public:
	Resource R1, R2, R3, R4;
	char *status;
	std::vector<ProcessNode*> processList;
	ProcessNode* currentProcess;
	std::vector<Resource> resources;
	bool curr_del;

	Scheduler();
	void init();
	void find_nextp();
	bool cr(char* name, int priority);
	bool de(char* name);
	bool req(char* name, int units);
	bool rel(char* name, int units);
	void to();
	~Scheduler();
};