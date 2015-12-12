#ifndef GENETIC_H_
#define GENETIC_H_

#include "kpmp_solution_writer.h"
#include "node.h"
#include "edge.h"
#include "page.h"
#include "gensolution.h"

#include <iostream>
#include <algorithm>
#include <list>
#include <time.h>
#include <utility>

using namespace std;

class Genetic {

private:
    vector<Gensolution> geneticSolution;
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
        srand (clock());
        for (unsigned int i = 0; i < s1.size(); i++) {
            if (s1[i].getPage() == s2[i].getPage()) {
                finalEdgeList.push_back(s1[i]);
            } else {

                // int r = 0 + (rand() % (int)(1 - 0));
                int r = 0 + (rand() % (int)(1 - 0 + 1));
                cout << "r " << r << " ";
                if (r == 0) {
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
    /*vector<solution> select(vector<solution> solutions, int k) {
			srand(clock());
			long sum = 0; 
			for(int i = 0; i < numSolutions; i++)
			{
				sum += solution[i]->fitness;
			}
			vector<solution> returns;
			for(int j = 0; j<k; j++)
			{
				long which = (long) 20*rand() % sum; //this is now index;
				int i = 0;
				while(which > 0)
				{
					r -= solution[i]->fitness;
					i++;
				}
				i--;
				returns.pushback(solution[i]);
			}
			return returns;
	}
	*/
};


#endif /* DETERMINISTIC_H_ */
