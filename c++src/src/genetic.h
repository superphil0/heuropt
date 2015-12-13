#ifndef GENETIC_H_
#define GENETIC_H_

#include "kpmp_solution_writer.h"
#include "node.h"
#include "edge.h"
#include "page.h"
#include "gensolution.h"
#include "utilities.h"
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

	void iterateOnce();
    void createInitialSolutions(unsigned int noOfSolutions);

    void createRandomPageAssignment();

    Gensolution merge(Gensolution s1, Gensolution s2) {
        vector<Edge> s1Edges = s1.getEdgeList();
        vector<Edge> s2Edges = s2.getEdgeList();
        vector<Edge> finalEdgeList;
        //unsigned edgeCount = s1.getEdgeList().size();
        srand (clock());
        for (unsigned int i = 0; i < s1Edges.size(); i++) {
            if (s1Edges[i].getPage() == s2Edges[i].getPage()) {
                finalEdgeList.push_back(s1Edges[i]);
            } else {

                // int r = 0 + (rand() % (int)(1 - 0));
                int r = 0 + (rand() % (int)(1 - 0 + 1));
                //cout << "r " << r << " ";
                if (r == 0) {
                    finalEdgeList.push_back(s1Edges[i]);
                } else {
                    finalEdgeList.push_back(s2Edges[i]);
                }
            }
        }
		Gensolution s;
        int crossings = Utilities::calculateEdgeCrossing(&finalEdgeList);
		s.setEdgeList(finalEdgeList);
        s.setCrossings(crossings);
        for (int i=0; i < finalEdgeList.size(); i++) {
            //cout << i << "-" << finalEdgeList[i].getPage() << "; ";
        }
		return s;
    }

    Gensolution mergePivot(Gensolution s1, Gensolution s2) {
        vector<Edge> s1Edges = s1.getEdgeList();
        vector<Edge> s2Edges = s2.getEdgeList();
        vector<Edge> finalEdgeList;

        int pivot = 0 + (rand() % (int)(s1Edges.size() - 0 + 1));

        for (int i=0; i < s1Edges.size(); i++ ) {
            if (i<pivot) {
                finalEdgeList.push_back(s1Edges[i]);
            }
            else {
                finalEdgeList.push_back(s2Edges[i]);
            }
        }

        Gensolution s;
        int crossings = Utilities::calculateEdgeCrossing(&finalEdgeList);
		s.setEdgeList(finalEdgeList);
        s.setCrossings(crossings);

        return s;

    }

    vector<Gensolution> select(vector<Gensolution> solutions, int k) {
			srand(clock());
			int numSolutions = solutions.size();
			double sum = 0;
			for(int i = 0; i < numSolutions; i++)
			{
				if (solutions[i].getCrossings() != 0)
				{
					sum += 1 / (double)solutions[i].getCrossings();
				}
			}
			vector<Gensolution> returns;
			for(int j = 0; j<k; j++)
			{
				double which = fmod((double) rand(), sum); //this is now index;
				int i = 0;
				while(which > 0)
				{
					if (solutions[i].getCrossings() != 0)
					{
						which -= 1 / (double) solutions[i].getCrossings();
					}
					i++;
				}
				i--;
				returns.push_back(solutions[i]);
			}
			return returns;
	}
};


#endif /* DETERMINISTIC_H_ */
