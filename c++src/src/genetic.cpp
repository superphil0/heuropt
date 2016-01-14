#include <iostream>
#include <climits>

#include "genetic.h"
#include "edgeswap.h"
#include "deterministic.h"
#include "page.h"
#include "kpmp_solution_writer.h"

void Genetic::createInitialSolutions(unsigned int noOfSolutions) {
	KPMPSolutionWriter *solution = new KPMPSolutionWriter(3);
	unsigned int totalCrossings;
	for (unsigned int i = 0; i < noOfSolutions; i++) {
		Gensolution s;
		Deterministic d(solution, spine, edgeList, book, &totalCrossings);
		d.sortSpineDFS();
		s.setSpine(*spine);
		//std::cout << "in" << endl;
		createRandomPageAssignment(s);
	}
	std::cout << "finished" << endl;
}

void Genetic::createRandomPageAssignment(Gensolution s) {
	vector<Edge> rndSolution;

	int page;
	int crossings;
	unsigned int booksize = book->size();

	srand(clock());

	//find best pages for inserting edges
	for (unsigned int e = 0; e < edgeList->size(); e++) {
		page = 0 + (rand() % (int)(booksize - 0));
		//std::cout << e << "-" << page << "; ";
		Edge tmpEdge = (*edgeList)[e];
		tmpEdge.setPage(page);
		rndSolution.push_back(tmpEdge);
	}
	crossings = Utilities::calculateEdgeCrossing(&rndSolution);
	s.setEdgeList(rndSolution);
	s.setCrossings(crossings);
	std::cout << "crs " << crossings << endl;
	/*int insertAt = 0;
	for (insertAt; insertAt < geneticSolution.size(); insertAt++) {
		if (geneticSolution[insertAt].getCrossings() > crossings) {
			break;
		}
	}*/
	geneticSolution.push_back(s);
	//std::cout << "insert at: " << insertAt << endl;



	//this->geneticSolution.push_back(s);
	rndSolution.clear();
}

// takes k solutions runs the algorithm and returns all solutions
Gensolution Genetic::iterateOnce()
{
	std::cout << "start Iteration" << endl;
	int minCrossing = INT_MAX;
	Gensolution bestSolution;
	int numSolutions = geneticSolution.size();
	int m = 50; // wieviel recombinen
	unsigned int booksize = book->size();
	// select best m solutions
	vector<Gensolution> selectedSolutions = select(geneticSolution, m);
	// recombine them will produce m-1 children
	for (int i = 0; i < m - 1; i++)
	{
		Gensolution child = merge(selectedSolutions[i], selectedSolutions[i + 1]);
		// Gensolution child = mergePivot(selectedSolutions[i], selectedSolutions[i+1]);
		// mutation
		for (unsigned int e = 0; e < child.getEdgeList().size(); e++) {
			if (rand() % 100 < 1)
			{
				int page = 0 + (rand() % (int)(booksize - 0));
				child.getEdgeList()[e].setPage(page);
			}
		}
		//Edgeswap edgeswap(&spine, &rndSolution, &book, &totalCrossings);
		vector<Edge> el = child.getEdgeList();
		int crossings = Utilities::calculateEdgeCrossing(&el);
		// int crossings = Utilities::calculateEdgeCrossing(&child.getEdgeList());
		child.setCrossings(crossings);
		if (crossings < minCrossing) {
			minCrossing = crossings;
			bestSolution = child;
		}
		//std::cout << "crs " << crossings << endl;
		// replacement
		int pos = (int)rand() % numSolutions;
		geneticSolution[pos] = child;
	}
	return bestSolution;
}
