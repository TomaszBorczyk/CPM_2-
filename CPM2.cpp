//#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <utility>

#include "Graph.hpp"

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
