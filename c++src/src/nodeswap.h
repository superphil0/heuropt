#ifndef NODESWAP_H_
#define NODESWAP_H_

#include "kpmp_solution_writer.h"
#include "node.h"
#include "edge.h"
#include "page.h"

#include <iostream>
#include <algorithm>
#include <list>
#include <utility>

using namespace std;


class Nodeswap {

private:
    KPMPSolutionWriter* solution;
    vector<Node>* spine;
    vector<Edge>* edgeList;
    vector<Page>* book;
    unsigned int* totalCrossings;
	vector<vector<unsigned int>>* adjList;
public:
	Nodeswap(KPMPSolutionWriter* solution, vector<Node>* spine, vector<Edge>* edgeList, vector<Page>* book, unsigned int* totalCrossings,vector<vector<unsigned int>>* adjList ) {
        this->solution = solution;
        this->spine = spine;
        this->edgeList = edgeList;
        this->book = book;
        this->totalCrossings = totalCrossings;
		this->adjList = adjList;
	}

	~Nodeswap() {

	}

	unsigned int crossingsWithNodeOnPos(Node* node, int position, int mode);
    void swap(int nodeToSelect, int mode);


};


#endif /* EDGESWAP_H_ */
