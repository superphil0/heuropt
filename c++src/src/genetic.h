#ifndef GENETIC_H_
#define GENETIC_H_

#include "kpmp_solution_writer.h"
#include "node.h"
#include "edge.h"
#include "page.h"
#include "solution.h"

#include <iostream>
#include <algorithm>
#include <list>
#include <time.h>
#include <utility>

using namespace std;

class Genetic {

private:
    vector<vector<Edge>> solution;
    vector<Node>* spine;
    vector<Edge>* edgeList;
    vector<Page>* book;

public:
	Genetic(vector<Node>* spine, vector<Edge>* edgeList, vector<Page>* book) {
        this->spine = spine;
        this->edgeList = edgeList;
        this->book = book;
	}

	~Genetic() {

	}

    void createInitialSolutions(unsigned int noOfSolutions);

    void createRandomPageAssignment();

    void merge(vector<Edge> s1, vector<Edge> s2) {
        //vector<Edge> s1Edges = s1.getEdgeList();
        //vector<Edge> s2Edges = s2.getEdgeList();
        vector<Edge> finalEdgeList;
        //unsigned edgeCount = s1.getEdgeList().size();

        for (unsigned int i = 0; i < s1.size(); i++) {
            if (s1[i].getPage() == s2[i].getPage()) {
                finalEdgeList.push_back(s1[i]);
            } else {
                srand (clock());
                double r = ((double) rand() / (1)) + 1;
                if (r < .5) {
                    finalEdgeList.push_back(s1[i]);
                } else {
                    finalEdgeList.push_back(s2[i]);
                }
            }
        }
        for (int i=0; i < finalEdgeList.size(); i++) {
            cout << i << "-" << finalEdgeList[i].getPage() << "; ";
        }
    }
};


#endif /* DETERMINISTIC_H_ */
