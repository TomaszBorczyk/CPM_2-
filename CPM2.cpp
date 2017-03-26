// CPM.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <utility>
#include <vector>
#include <algorithm>



//std::pair<int, int> takePair
class Node {

private:
	int id;
	int procTime;
	int earlyStart;
	int earlyFinish;
	int lateStart;
	int lateFinish;
	int indegree;
	int outdegree;


public:
	Node() {
	}

	Node(int procTime, int id) {
		this->id = id;
		this->procTime = procTime;
		indegree = 0;
		outdegree = 0;
		earlyStart = 0;
		lateFinish = 9999;
		lateStart = 9999;
	}

	

	void increaseIndegree() { indegree++; }

	void increaseOutdegree() { outdegree++; }

	void setES(int es) { this->earlyStart = es; }

	void setEF(int ef) { this->earlyFinish = ef; }

	void setLS(int ls) { lateStart = ls; }

	void setLF(int lf) { lateFinish = lf; }

	int getES() { return earlyStart; }

	int getEF() { return earlyFinish; }

	int getLS() { return lateStart; }

	int getLF() { return lateFinish; }

	int getIndegree() { return indegree; }

	int getOutdegree() { return outdegree; }

	int getProcTime() { return procTime; }

	void setESEFBasedOnPredecessor(Node* node) {
		this->setES(std::max(this->getES(), node->getEF()));
		this->setEF(this->getES() + this->getProcTime());

	}

	void setLSLFBasedOnSuccessor(Node* node) {
		this->setLF(std::min(this->getLF(), node->getLS()));
		this->setLS(this->getLF() - this->getProcTime());
	}
};

class Graph {
	int nNodes, nEdges;
	std::vector <std::pair<int, int >> edges;
	Node finish;

public:
	std::vector<Node> nodes;


	void setnNodes(int n) {
		nNodes = n;
	}

	void setnEdges(int n) {
		nEdges = n;
	}

	void readNodes(std::ifstream& file) {
		int nodeTime;
		for (int i = 0; i < nNodes; i++) {
			file >> nodeTime;
			Node node(nodeTime, i+1);
			//if (!i) { node.setES(0); node.setEF(nodeTime); }
			nodes.push_back(node);
		}
	}

	void readEdges(std::ifstream& file) {
		std::pair<int, int> nPair;
		int a, b;
		for (int i = 0; i < nEdges; i++) {
			file >> a >> b;
			edges.push_back(std::make_pair(a, b));
		}
	}


	void setIndegreeAndOutdegree() {
		for (int i = 0; i < nNodes; i++) {
			for (std::pair<int, int> e : edges) {
				if (e.second == i + 1) {
					nodes.at(i).increaseIndegree();
				}
				else if (e.first == i + 1) {
					nodes.at(i).increaseOutdegree();
				}
			}
		}
	}

	void setFinish() {
		for (Node e : nodes) {
			if (e.getOutdegree() == 0) {
				finish.setES(std::max(finish.getES(), e.getEF()));
				finish.setEF(finish.getES());
				finish.setLS(finish.getES());
				finish.setLF(finish.getES());
			}
		}
	}


	void setESEF_for_starters() {
		for (std::vector<Node>::iterator it = nodes.begin(); it != nodes.end(); it++) {
			if ((*it).getIndegree() == 0) {
				int a = (*it).getProcTime();
				(*it).setEF(a);
			}
		}
	}

	void setLSLF_for_enders() {
		for (std::vector<Node>::iterator it = nodes.begin(); it != nodes.end(); it++) {
			if ((*it).getOutdegree() == 0) {
				(*it).setLSLFBasedOnSuccessor(&finish);
			}
		}
	}

	void setESFF_for_graph() {
		for (std::pair<int, int> e : edges) {
			nodes.at(e.second - 1).setESEFBasedOnPredecessor(&nodes.at(e.first - 1));
		}
	}


	void setLSLF_for_graph() {
		for (int i = edges.size() - 1; i >= 0; i--) {
			nodes.at(edges.at(i).first - 1).setLSLFBasedOnSuccessor(&nodes.at(edges.at(i).second - 1));
		}
	}

	std::queue<Node> returnZeroIndegree() {
		std::queue<Node> zeros;

		for (Node e : nodes) {
			if (e.getIndegree())
				zeros.push(e);
		}
		return zeros;
		
	}

	void displayNodes() {
		for (Node e : nodes) {
			std::cout << e.getES() << " " << e.getEF() << " " << e.getLS() << " " << e.getLF() <<" " << e.getIndegree()<< std::endl;
		}
		std::cout << finish.getES();
	}
};


std::vector<Node> sortGraph(Graph& graph) {
	//std::queue<Node> Q;
	std::sort(graph.nodes.begin(), graph.nodes.end(), ([](Node arg1, Node arg2)->bool {return arg1.getIndegree() < arg2.getIndegree(); }));
	return graph.nodes;
}

void calcTotalETime(int nNodes, std::vector<Node> nodes) {
	for (Node e : nodes) {

	}
}



int main()
{
	Graph graph;
	int nNodes, nEdges;
	FILE *ofile;

	std::string fileName;

	/*std::cout << "Enter filename\n";
	std::cin >> fileName;*/

	std::ifstream file("dataSort10.txt");

	file >> nNodes >> nEdges;
	
	//graph.setnNodes(nNodes);
	//graph.setnEdges(nEdges);

	//graph.readNodes(file);
	//graph.readEdges(file);
	//graph.setIndegreeAndOutdegree();
	//graph.setESEF_for_starters();

	//graph.setESFF_for_graph();
	//graph.setFinish();
	//graph.setLSLF_for_enders();
	//graph.setLSLF_for_graph();
	//graph.displayNodes();


	graph.setnNodes(nNodes);
	graph.setnEdges(nEdges);

	graph.readNodes(file);
	graph.readEdges(file);
	graph.setIndegreeAndOutdegree();

	sortGraph(graph);
	graph.displayNodes();



	std::cin >> fileName;

	return 0;
}

