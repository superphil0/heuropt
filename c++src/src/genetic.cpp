#include <iostream>

#include "genetic.h"
#include "deterministic.h"
#include "page.h"
#include "kpmp_solution_writer.h"
#include "utilities.h"

void Genetic::createInitialSolutions(unsigned int noOfSolutions) {
    //Deterministic(KPMPSolutionWriter* solution, vector<Node>* spine, vector<Edge>* edgeList, vector<Page>* book, unsigned int * totalCrossings)

    KPMPSolutionWriter *solution = new KPMPSolutionWriter(3);
    unsigned int totalCrossings;
    Deterministic d(solution, spine, edgeList, book, &totalCrossings);
    d.sortSpineDFS();
    for (unsigned int i = 0; i < noOfSolutions; i++ ) {
        cout << "in" << endl;
        createRandomPageAssignment();
    }
    cout << "finished" << endl;
    merge((this->geneticSolution[0]).getEdgeList(), this->geneticSolution[1].getEdgeList());
}

void Genetic::createRandomPageAssignment() {
    vector<Edge> rndSolution;

    int page;
    int crossings;
    unsigned int booksize = book->size();

    srand (clock());

    //find best pages for inserting edges
    for(unsigned int e = 0; e < edgeList->size(); e++) {
        page = 0 + (rand() % (int)(booksize - 0));
        cout << e << "-" << page << "; ";
        Edge tmpEdge = (*edgeList)[e];
        tmpEdge.setPage(page);
        rndSolution.push_back(tmpEdge);
    }
    Gensolution s;
    crossings = Utilities::calculateEdgeCrossing(&rndSolution);
    s.setEdgeList(rndSolution);
    s.setCrossings(crossings);
    cout << "crs " << crossings << endl; 

    this->geneticSolution.push_back(s);
    rndSolution.clear();


}
