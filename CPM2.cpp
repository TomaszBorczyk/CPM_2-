#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <utility>
#include <vector>


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
	Node() {
	}

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

class Graph {
	int nNodes, nEdges;
	std::vector<Node> nodes;
	int* idToIndex;	//tablica translacji id -> indeks w celu znalezienia wierzcho³ka po sortowaniu topologicznym
	std::vector<std::vector<int>> edges;
	Node finish; //sztuczny wierzcho³ek koñcowy, który przechowuje informacje o d³ugoœci harmonogramu i LS/LF dla koñcu grafu

public:

	Graph(int _nodes, int _edges) {
		nNodes = _nodes;
		nEdges = _edges;
		edges.resize(nEdges);
		idToIndex = new int[nNodes];
	}

	void setnNodes(int n) { nNodes = n; }
	void setnEdges(int n) { nEdges = n; }


	void readNodes(std::ifstream& file) {
		int nodeTime;
		for (int i = 0; i < nNodes; i++) {
			file >> nodeTime;
			Node node(nodeTime, i + 1);
			idToIndex[i] = i;
			nodes.push_back(node);
		}
	}

	void readEdges(std::ifstream& file) {
		std::pair<int, int> nPair;
		int a, b;
		for (int i = 0; i < nEdges; i++) {
			file >> a >> b;

			nodes[idToIndex[a - 1]].increaseOutdegree();
			nodes[idToIndex[b - 1]].increaseIndegree();
			edges[a - 1].push_back(b - 1);
		}
	}

	void setIdToIndex(int id, int index) {
		idToIndex[id] = index;
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


	void setESEF_for_starters() { //ustawienie ES/EF dla wierzcho³ków pocz¹tkowych
		for (std::vector<Node>::iterator it = nodes.begin(); it != nodes.end(); it++) {
			if ((*it).getIndegree() == 0) {
				int a = (*it).getProcTime();
				(*it).setEF(a);
			}
		}
	}

	void setLSLF_for_enders() { //ustawienie LF/LF dla wierzcho³ków koñcowych, po przejœciu ca³ego grafu
		for (std::vector<Node>::iterator it = nodes.begin(); it != nodes.end(); it++) {
			if ((*it).getOutdegree() == 0) {
				(*it).setLSLFBasedOnSuccessor(&finish);
			}
		}
	}

	void setESEF_for_graph() { // ustawienie ES/EF dla reszty grafu, po ustawieniu ES/EF dla wierzcho³ków pocz¹tkowych
		std::vector<std::vector<int>>::iterator it = edges.begin();
		for (; it != edges.end(); it++) {
			std::vector<int>::iterator jt = (*it).begin();
			for (; jt != (*it).end(); jt++) {
				nodes.at((idToIndex[*jt])).setESEFBasedOnPredecessor(&nodes.at(idToIndex[it - edges.begin()]));
			}
		}
	}


	void setLSLF_for_graph() {// ustawienie LS/LF dla reszty grafu, po ustawieniu LS/LF dla wierzcho³ków koñcowych
		for (unsigned i = edges.size(); i-- > 0; ) {
			for (unsigned j = edges[i].size(); j-- > 0; ) {
				nodes[idToIndex[i]].setLSLFBasedOnSuccessor(&nodes[idToIndex[edges.at(i).at(j)]]);
			}
		}
	}

	void displayEdges() {
		for (int i = 0; i < edges.size(); i++) {
			std::cout << "id: " << i + 1 << "   ";
			for (auto e : edges[i]) {
				std::cout << e + 1 << ", ";
			}
			std::cout << "\n";
		}
	}

	void displayNodes() {
		for (Node e : nodes) {
			std::cout << e.getES() << " " << e.getEF() << " " << e.getLS() << " " << e.getLF() << " id: " << e.getId() << " indegree: " << e.getIndegree() << " out: " << e.getOutdegree() << std::endl;
		}
		std::cout << finish.getES();
	}

	void topologicalSort() { 	//na podstawie wikipedii
		std::deque<Node> Q;
		std::vector<Node> result;
		Node tempNode;
		int nodeId;
		for (auto node : nodes) {
			if (node.getIndegree() == 0) {
				Q.push_back(node);
			}
		}

		while (!Q.empty()) {
			tempNode = Q.front();
			result.push_back(tempNode);
			setIdToIndex(tempNode.getId() - 1, result.size() - 1);
			Q.pop_front();

			for (auto e : edges[tempNode.getId() - 1]) {
				Node& currNode = nodes[idToIndex[e]];
				currNode.decreaseIndTopoSort();
				if (currNode.getIndegreeTopoSort() == 0) {
					Q.push_front(currNode);
				}
			}
		}
		nodes = result;
	}


	void sortById() {
		std::sort(nodes.begin(), nodes.end(), [](Node n1, Node n2) {
			return n1.getId() < n2.getId();
		});
	}

};


int main()
{
	int nNodes, nEdges;
	FILE *ofile;
	char decision;

	std::string fileName;

	std::ifstream file("data20.txt");
	std::cout << "Posortowaæ topologicznie? y/n ";
	std::cin >> decision;

	file >> nNodes >> nEdges;

	Graph graph(nNodes, nEdges);

	graph.readNodes(file);
	graph.readEdges(file);

	if (decision == 'y')
		graph.topologicalSort();

	graph.setESEF_for_starters();
	graph.setESEF_for_graph();

	graph.setFinish();
	graph.setLSLF_for_enders();
	graph.setLSLF_for_graph();

	graph.sortById();
	graph.displayNodes();

	std::cin.ignore();
	std::cin.ignore();

	return 0;
}