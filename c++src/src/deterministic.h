#ifndef DETERMINISTIC_H_
#define DETERMINISTIC_H_

#include "kpmp_solution_writer.h"
#include "node.h"
#include "edge.h"
#include "page.h"

#include <iostream>
#include <algorithm>
#include <list>
#include <utility>

using namespace std;


class Deterministic {

private:
    KPMPSolutionWriter* solution;
    vector<Node>* spine;
    vector<Edge>* edgeList;
    vector<Page>* book;
    unsigned int* totalCrossings;

public:
	Deterministic(KPMPSolutionWriter* solution, vector<Node>* spine, vector<Edge>* edgeList, vector<Page>* book, unsigned int * totalCrossings) {
        this->solution = solution;
        this->spine = spine;
        this->edgeList = edgeList;
        this->book = book;
        this->totalCrossings = totalCrossings;
	}

	~Deterministic() {

	}

    void writeSpine();

    void writeEdgeList();

    void sortSpine(int order);

    void DFS(unsigned int start, vector<unsigned int>* nodes, vector<pair<unsigned int, unsigned int>>* spineOrder);

    void sortSpineDFS();

    void sortEdges(int order);

    void h1();
};


#endif /* DETERMINISTIC_H_ */
