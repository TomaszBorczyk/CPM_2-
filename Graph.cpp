#include <queue>
#include <fstream>
#include <iostream>
#include <functional>

#include "Graph.hpp"

void Graph::readNodes(std::ifstream& file) {
  int nodeTime;
  for (int i = 0; i < nNodes; i++) {
    file >> nodeTime;
    Node node(nodeTime, i + 1);
    idToIndex[i] = i;
    nodes.push_back(node);
  }
}

void Graph::readEdges(std::ifstream& file) {
  std::pair<int, int> nPair;
  int a, b;
  for (int i = 0; i < nEdges; i++) {
    file >> a >> b;

    nodes[idToIndex[a - 1]].increaseOutdegree();
    nodes[idToIndex[b - 1]].increaseIndegree();
    edges[a - 1].push_back(b - 1);
  }
}

void Graph::setIdToIndex(int id, int index) {
  idToIndex[id] = index;
}


void Graph::setFinish() {
  for (Node e : nodes) {
    if (e.getOutdegree() == 0) {
      finish.setES(std::max(finish.getES(), e.getEF()));
      finish.setEF(finish.getES());
      finish.setLS(finish.getES());
      finish.setLF(finish.getES());
    }
  }
}


void Graph::setESEF_for_starters() { //ustawienie ES/EF dla wierzcho³ków pocz¹tkowych
  for (std::vector<Node>::iterator it = nodes.begin(); it != nodes.end(); it++) {
    if ((*it).getIndegree() == 0) {
      int a = (*it).getProcTime();
      (*it).setEF(a);
    }
  }
}

void Graph::setLSLF_for_enders() { //ustawienie LF/LF dla wierzcho³ków koñcowych, po przejœciu ca³ego grafu
  for (std::vector<Node>::iterator it = nodes.begin(); it != nodes.end(); it++) {
    if ((*it).getOutdegree() == 0) {
      (*it).setLSLFBasedOnSuccessor(&finish);
    }
  }
}

void Graph::setESEF_for_graph() { // ustawienie ES/EF dla reszty grafu, po ustawieniu ES/EF dla wierzcho³ków pocz¹tkowych
  std::vector<std::vector<int>>::iterator it = edges.begin();
  for (; it != edges.end(); it++) {
    std::vector<int>::iterator jt = (*it).begin();
    for (; jt != (*it).end(); jt++) {
      nodes.at((idToIndex[*jt])).setESEFBasedOnPredecessor(&nodes.at(idToIndex[it - edges.begin()]));
    }
  }
}


void Graph::setLSLF_for_graph() {// ustawienie LS/LF dla reszty grafu, po ustawieniu LS/LF dla wierzcho³ków koñcowych
  for (unsigned i = edges.size(); i-- > 0; ) {
    for (unsigned j = edges[i].size(); j-- > 0; ) {
      nodes[idToIndex[i]].setLSLFBasedOnSuccessor(&nodes[idToIndex[edges.at(i).at(j)]]);
    }
  }
}

void Graph::displayEdges() {
  for (int i = 0; i < edges.size(); i++) {
    std::cout << "id: " << i + 1 << "   ";
    for (auto e : edges[i]) {
      std::cout << e + 1 << ", ";
    }
    std::cout << "\n";
  }
}

void Graph::displayNodes() {
  for (Node e : nodes) {
    std::cout << e.getES() << " " << e.getEF() << " " << e.getLS() << " " << e.getLF() << " id: " << e.getId() << " indegree: " << e.getIndegree() << " out: " << e.getOutdegree() << std::endl;
  }
  std::cout << finish.getES();
}

void Graph::topologicalSort() { 	//na podstawie wikipedii
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


void Graph::sortById() {
  std::sort(nodes.begin(), nodes.end(), [](Node n1, Node n2) {  return n1.getId() < n2.getId();  }  );
}
