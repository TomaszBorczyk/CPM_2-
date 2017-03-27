#ifndef _GRAPH_
#define _GRAPH_

#include <vector>

#include "Node.hpp"


class Graph {
	int nNodes, nEdges;
	std::vector<Node> nodes;
	int* idToIndex;	//tablica translacji id -> indeks w celu znalezienia wierzcho³ka po sortowaniu topologicznym
	std::vector<std::vector<int>> edges;
	Node finish; //sztuczny wierzcho³ek koñcowy, który przechowuje informacje o d³ugoœci harmonogramu i LS/LF dla koñcu grafu

public:

	Graph(int _nodes, int _edges) : nNodes(_nodes), nEdges(_edges) {
		edges.resize(nEdges);
		idToIndex = new int[nNodes];
	}

	void setnNodes(int n) { nNodes = n; }
	void setnEdges(int n) { nEdges = n; }

  void readNodes(std::ifstream& file);
  void readEdges(std::ifstream& file);
  void setIdToIndex(int id, int index);
  void setFinish();
  void setESEF_for_starters();
  void setLSLF_for_enders();
  void setESEF_for_graph();
  void setLSLF_for_graph();
  void displayEdges();
  void displayNodes();
  void topologicalSort();
  void sortById();
};

#endif //_GRAPH_
