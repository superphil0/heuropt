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
		//d.sortSpine(0);
		s.setSpine(*spine);
		//std::cout << "in" << endl;
		createRandomPageAssignment(s);
	}

	/*for (int i = 0; i < geneticSolution.size(); i++) {
		vector<Node> sp =  geneticSolution[i].getSpine();
		for (int j = 0; j < spine->size(); j++) {
			cout << &sp[j] << " " << sp[j].getName() <<":"<<sp[j].getPosition()<< " - ";
		}
		cout << endl;
	}
	std::cout << "finished" << endl;*/
}

void Genetic::createRandomPageAssignment(Gensolution s) {
	vector<Edge> rndSolution;
	int page;
	int crossings;
	unsigned int booksize = book->size();
	vector<Page> tmpBook;
	for (unsigned int k = 0; k < (booksize); k++) {
		tmpBook.push_back(Page());
	}

	srand(clock());

	//find best pages for inserting edges
	for (unsigned int e = 0; e < edgeList->size(); e++) {
		page = 0 + (rand() % (int)(booksize - 0));
		//std::cout << e << "-" << page << "; ";
		Edge tmpEdge = (*edgeList)[e];
		tmpEdge.setPage(page);
		tmpBook[page].addEdge(tmpEdge);
		rndSolution.push_back(tmpEdge);
	}
	crossings = Utilities::calculateEdgeCrossing(&rndSolution);
	s.setEdgeList(rndSolution);
	s.setCrossings(crossings);
	s.setBook(tmpBook);
	//std::cout << "initial crossings " << crossings << endl;
	/*int insertAt = 0;
	for (insertAt; insertAt < geneticSolution.size(); insertAt++) {
		if (geneticSolution[insertAt].getCrossings() > crossings) {
			break;
		}
	}*/

	 /*for (int i = 0; i < s.getSpine().size(); i++) {
	 	cout << s.getSpine()[i].getName() << ":" << s.getSpine()[i].getPosition() << "   ";
	 }
	 cout << endl;*/

	 vector<Edge> tmpEdgeList;
	 tmpEdgeList = s.getEdgeList();
	 vector<Node> tmpSpine;
	 tmpSpine = s.getSpine();
	 unsigned int tmpCrossings;
	 tmpCrossings = s.getCrossings();
	 unsigned int maxCrossing = UINT_MAX;
	 //cout << "spine address: " << &tmpSpine << endl;
	 Edgeswap e(&tmpSpine, &tmpEdgeList, &tmpBook, &tmpCrossings);
	 while (maxCrossing > tmpCrossings){
	 	//cout << "swappe: " << maxCrossing << " -> " << tmpCrossings << endl;
	 	maxCrossing = tmpCrossings;
	 	e.swap(2, 1);
	 	/*for (int i = 0; i < tmpEdgeList.size(); i++) {
	 		cout << tmpEdgeList[i].getStartNode()->getName() << "-" << tmpEdgeList[i].getEndNode()->getName() <<
	 		":" << tmpEdgeList[i].getPage() << "  ";
	 	}
	 	cout << endl;*/
	 	tmpCrossings = Utilities::calculateEdgeCrossing(&tmpEdgeList);
	 }

	 s.setSpine(tmpSpine);
	 s.setEdgeList(tmpEdgeList);
	 s.setCrossings(tmpCrossings);
	 /*for (int i = 0; i < s.getEdgeList().size(); i++ ) {
	 	cout << s.getEdgeList()[i].getStartNode()->getName() << "-" << s.getEdgeList()[i].getEndNode()->getName() <<
	 	":" << s.getEdgeList()[i].getPage() << endl;
	 }

	 cout << "Crossings after Edgeswap: " << s.getCrossings() << endl;*/
	geneticSolution.push_back(s);

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
