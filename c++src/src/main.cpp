#include "kpmp_instance.h"
#include "kpmp_solution_writer.h"
#include "page.h"
#include "edge.h"
#include "node.h"
#include "deterministic.h"
#include "edgeswap.h"
#include "nodeswap.h"
#include "genetic.h"

#include <memory>
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <list>
#include <climits>

using namespace std;

#ifdef __linux__
# include <sys/times.h>
# include <unistd.h>
#endif

double getCPUtime()
{
#ifdef __linux__
	tms t;
	times(&t);
	double ct = sysconf(_SC_CLK_TCK);
	return t.tms_utime / ct;
#else
	return 0; //note: cluster runs on linux
#endif
}

void usage()
{
	std::cout << "USAGE:\t<program> -f filename -m mode\n";
	std::cout << "EXAMPLE:\t" << "./HOT -f instances/automatic-1.txt -m grasp\n\n";
} // usage

int runAlgorithm(int fileNum, int amount, int* best)
{
	// read instance
	std::string path("../../c++src/instances/automatic-");
	std::string file(path + /*std::to_string(fileNum)*/ + "1.txt");
	std::shared_ptr<KPMPInstance> instance(KPMPInstance::readInstance(file));

	unsigned int numVertices = instance->getNumVertices();
	unsigned int booksize = instance->getK();
	vector<vector<unsigned int>> adjList = instance->getAdjacencyList();
	vector<Node> spine;
	unsigned int totalCrossings = UINT_MAX;

	vector<Page> book;
	for (unsigned int k = 0; k < booksize; k++) {
		book.push_back(Page());
	}


	//init writer
	KPMPSolutionWriter *solution = new KPMPSolutionWriter(booksize);

	vector<Edge> edgeList;std:
	unsigned int edgeCount;

	//create initial spine
	for (unsigned int startNode = 0; startNode < adjList.size(); startNode++) {
		spine.push_back(Node(startNode, adjList[startNode].size(), startNode));
		// cout << "size: " << spine.size() << endl;
	}

	// populate edgeList and create node order

	Node* tmpItStart = nullptr;
	Node* tmpItEnd = nullptr;
	for (unsigned int startNode = 0; startNode < adjList.size(); startNode++) {
		for (unsigned int i = 0; i < adjList[startNode].size(); i++) {
			if (startNode < adjList[startNode][i]) {
				for (unsigned int j = 0; j < spine.size(); j++) {
					if (spine[j].getName() == startNode) {
						tmpItStart = &spine[j];
					}
					if (spine[j].getName() == adjList[startNode][i]) {
						tmpItEnd = &spine[j];
					}

				}
				edgeList.push_back(Edge(tmpItStart, tmpItEnd));
			}
		}
	}

	// create genetic...
	Genetic gen(&spine, &edgeList, &book);

	// create deterministic instance
	Deterministic deter(solution, &spine, &edgeList, &book, &totalCrossings);
	// sort spine in descending node order

	deter.sortSpineDFS();

	gen.createInitialSolutions(5);
	
	//deter.writeSpine();
	//deter.sortSpine(-1);

	// fill spineorder for sulution
	vector<unsigned int> finalSpineOrder(numVertices);
	for (unsigned int i = 0; i < spine.size(); i++) {
		finalSpineOrder[spine[i].getPosition()] = spine[i].getName();
	}
	// sort edges with descending length
	deter.sortEdges(-1);
	// deter.writeSpine();
	// deter.writeEdgeList();
	deter.h1();

	// for (unsigned int e=0; e < edgeList.size(); e++) {
	//     cout << "final edge state " << edgeList[e].getStartNode()->getName() << " - " << edgeList[e].getEndNode()->getName() << " crossings " << edgeList[e].getCrossings() << " on Page " << edgeList[e].getPage() <<endl;
	// }

	Edgeswap edgeswap(solution, &spine, &edgeList, &book, &totalCrossings);
	Nodeswap nodeswap(solution, &spine, &edgeList, &book, &totalCrossings, &adjList);
	solution->setSpineOrder(finalSpineOrder);
	//solution->write("solution");
	solution->clearSolution();
	unsigned int maxTotalCrossingsEdges = UINT_MAX;
	unsigned int maxTotalCrossingsNodes = UINT_MAX;
	unsigned int iteration = 0;
	// termination condition
	while (maxTotalCrossingsNodes > totalCrossings ) {
		while (maxTotalCrossingsEdges > totalCrossings) {
			maxTotalCrossingsEdges = totalCrossings;

			edgeswap.swap(3, 1);

		}
		/*for (int i = 0; i < edgeList.size(); i++) {
			std::cout << edgeList[i].getCrossings() << " ";
		}*/
		//std::cout << endl;

			iteration++;
		//
		// select
		// method 1 = bestImprovement look at all nodes and improve best
		// method 2 = firstImprovement take node with max edgecrossing
		// method 3 = randomImprovement take random node
		// mode
		// mode 1 = swapnodes
		// mode 2 = insert node somewhere
		maxTotalCrossingsNodes = maxTotalCrossingsEdges;
		//std::cout << "edgecossings " << maxTotalCrossingsEdges << endl;
		nodeswap.swap(3, 1);
		//std::cout << "nodecossings " << totalCrossings << endl;
/*
		for (int i = 0; i < spine.size(); i++) {
			std::cout << spine[i].getName() << " - " << spine[i].getCrossings() << endl;
		}*/

		// deter.writeEdgeList();
		// noEdgesToSwap, method
		// method 1 = bestImprovement
		// method 2 = firstImprovement
		// method 3 = randomImprovement
	}
	solution->clearSolution();

	for (unsigned int i = 0; i < spine.size(); i++) {
		finalSpineOrder[spine[i].getPosition()] = spine[i].getName();
	}

	for (unsigned int e = 0; e < edgeList.size(); e++) {
		solution->addEdgeOnPage(edgeList[e].getStartNode()->getName(), edgeList[e].getEndNode()->getName(), edgeList[e].getPage());
	}


	solution->setSpineOrder(finalSpineOrder);
	//cout << "total crossings " << totalCrossings << endl;
	//std::cout << "CPU Time: " << getCPUtime() << std::endl;
	if (totalCrossings < *best)
	{
		*best = totalCrossings;
		std::ostringstream tmp;
		tmp << "solutionswap" << fileNum << ".txt";
		solution->write(tmp.str());
	}
	return totalCrossings;
}

int main(int argc, char** argv)
{
	int problem = 0;
	// default values
	std::string mode("unset");
#ifdef __linux__
	std::string file("../instances/automatic-1.txt");
#else
	std::string file("../../c++src/instances/automatic-3.txt");
#endif

	//process input params
	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "-f") == 0) {
			if (i + 1 == argc) {
				std::cout << "Missing arg for: " << argv[i] << std::endl;
				return -1;
			}
			file.assign(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "-p") == 0) {
			if (i + 1 == argc) {
				std::cout << "Missing arg for: " << argv[i] << std::endl;
				return -1;
			}
			std::string str(argv[i + 1]);
			problem = 1;// std::stoi(str);
			++i;
		}
		else {
			std::cout << "Unrecognised command: " << argv[i] << std::endl;
			return -1;
		}
	}
	int numRuns = 10;
	int start = 1;
	int end = 5;
	if (problem != 0)
	{
		start = problem;
		end = problem;
	}
	for (int fileNum = start; fileNum <= end;fileNum++)
	{
		std::cout << "Starting problem " << fileNum << "-----------" << endl;
		int timee = 0;
		int best = INT_MAX;
		int solCount = 0;

		int* solutions = new int[numRuns];
		KPMPSolutionWriter* solPtr = nullptr;
		for (int i = 0; i < numRuns; i++)
		{
			int start = clock();
			int solution = runAlgorithm(fileNum, numRuns, &best);
			solutions[i] = solution;
			solCount += solution;
			timee += clock() - start;
			std::cout << "one: " << solution << endl;
		}
		double mean = (double)solCount / numRuns;
		double sd = 0;
		for (int i = 0; i < numRuns; i++) {
			sd += pow((solutions[i] - mean), 2);
		}
		sd = sqrt(sd / numRuns);

		std::cout << "total best: " << best << " average: " << mean << " sd: " << sd << endl;
		std::cout << "time needed average: " << timee / numRuns * 1000 / CLOCKS_PER_SEC << " ms" << endl;
	}

	//std::cin.get();
	return 0;
} // main
