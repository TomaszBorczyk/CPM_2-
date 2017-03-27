#ifndef _NODE_
#define _NODE_

class Node {

private:
	int procTime;
	int id;
	
	int earlyStart;
	int earlyFinish;
	int lateStart;
	int lateFinish;
	int indegree;
	int outdegree;

	int indegreeTopoSort;

public:
	Node() {}

	Node(int procTime, int id) {
		this->procTime = procTime;
		this->id = id;
		indegree = indegreeTopoSort = 0; //iloœæ po³¹czeñ wchodz¹cych i TopoSort, czyli to samo na potrzeby sortowania topologicznego
		outdegree = 0;   //iloœæ po³¹czeñ wychodz¹cych
		earlyStart = 0;
		lateFinish = 9999;
		lateStart = 9999;
	}

	void increaseIndegree() { indegree++; indegreeTopoSort++; }
	void decreaseIndTopoSort() { indegreeTopoSort--; }
	void decreaseIndegree() { indegree--; }
	void increaseOutdegree() { outdegree++; }

	void setId(int id) { this->id = id; }
	void setES(int es) { this->earlyStart = es; }
	void setEF(int ef) { this->earlyFinish = ef; }
	void setLS(int ls) { lateStart = ls; }
	void setLF(int lf) { lateFinish = lf; }
	void setIndegree(int _indegree) { indegree = _indegree; }
	void setOutdegree(int _outdegree) { outdegree = _outdegree; }

	int getES() { return earlyStart; }
	int getEF() { return earlyFinish; }
	int getLS() { return lateStart; }
	int getLF() { return lateFinish; }
	int getIndegree() { return indegree; }
	int getIndegreeTopoSort() { return indegreeTopoSort; }
	int getOutdegree() { return outdegree; }
	int getProcTime() { return procTime; }
	int getId() { return id; }

	void setESEFBasedOnPredecessor(Node* node) {
		this->setES(std::max(this->getES(), node->getEF()));
		this->setEF(this->getES() + this->getProcTime());
	}

	void setLSLFBasedOnSuccessor(Node* node) {
		this->setLF(std::min(this->getLF(), node->getLS()));
		this->setLS(this->getLF() - this->getProcTime());

	}
};


#endif //_NODE_
