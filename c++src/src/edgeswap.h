#ifndef EDGESWAP_H_
#define EDGESWAP_H_

#include "kpmp_solution_writer.h"
#include "node.h"
#include "edge.h"
#include "page.h"

#include <iostream>
#include <algorithm>
#include <list>
#include <utility>

using namespace std;


class Edgeswap {

private:
	KPMPSolutionWriter* solution;
	vector<Node>* spine;
	vector<Edge>* edgeList;
	vector<Page>* book;
	unsigned int* totalCrossings;
public:
	Edgeswap(KPMPSolutionWriter* solution, vector<Node>* spine, vector<Edge>* edgeList, vector<Page>* book, unsigned int* totalCrossings) {
		this->solution = solution;
		this->spine = spine;
		this->edgeList = edgeList;
		this->book = book;
		this->totalCrossings = totalCrossings;
	}

	~Edgeswap() {

	}

	void swap(unsigned int noEdgesToSwap);


};


#endif /* EDGESWAP_H_ */
