#ifndef UTILITIES_H_
#define UTILITIES_H_

#include "node.h"
#include "edge.h"
#include "page.h"

#include <iostream>
#include <stdlib.h>

using namespace std;

class Utilities {

public:

	static unsigned int calculateNodeCrossings(vector<Edge>* edgeList) {
		unsigned int edgeCrossings;
		for (unsigned int e=0; e < edgeList->size(); e++) {
			edgeCrossings = (*edgeList)[e].getCrossings();
			(*edgeList)[e].getStartNode()->setCrossings((*edgeList)[e].getStartNode()->getCrossings() + edgeCrossings);
			(*edgeList)[e].getEndNode()->setCrossings((*edgeList)[e].getEndNode()->getCrossings() + edgeCrossings);
		}
		return edgeCrossings;
	}

	static unsigned int calculateEdgeCrossing(vector<Edge>* edgeList) {
		unsigned int teststart;
		unsigned int testend;
		unsigned int start;
		unsigned int end;

		unsigned int edgeCrossing = 0;
		unsigned int totalCrossing = 0;



		for (unsigned int i = 0; i < (*edgeList).size(); i++) {
			// set start and end node in correct order for easier testing
			if ((*edgeList)[i].getStartNode()->getPosition() < (*edgeList)[i].getEndNode()->getPosition()) {
				start = (*edgeList)[i].getStartNode()->getPosition();
				end = (*edgeList)[i].getEndNode()->getPosition();
			} else {
				start = (*edgeList)[i].getEndNode()->getPosition();
				end = (*edgeList)[i].getStartNode()->getPosition();
			}
			for (unsigned int j = 0; j < (*edgeList).size(); j++) {
				if (i != j && (*edgeList)[i].getPage() == (*edgeList)[j].getPage()) {
					// set start and end node in correct order for easier testing
					if ((*edgeList)[j].getStartNode()->getPosition() < (*edgeList)[j].getEndNode()->getPosition()) {
						teststart = (*edgeList)[j].getStartNode()->getPosition();
						testend = (*edgeList)[j].getEndNode()->getPosition();
					} else {
						teststart = (*edgeList)[j].getEndNode()->getPosition();
						testend = (*edgeList)[j].getStartNode()->getPosition();
					}

					if (start < teststart) { //startNode outside edge
						if ( (end > teststart) && (end < testend) ) { // endNode inside edge
							edgeCrossing++;
						}
					} else if ( (start > teststart) && (start < testend) ) { // startNode inside edge
						if (end > testend) { // endNode outside edge
							edgeCrossing++;
						}
					}
				}
			}
			(*edgeList)[i].setCrossings(edgeCrossing);
			totalCrossing+=edgeCrossing;
			edgeCrossing = 0;
		}

		return totalCrossing/2;

	}

	static unsigned int calculateEdgeCrossing(Edge* edge, vector<Page>* book) {
		// tmpValues for nodes of edges to test
		unsigned int teststart;
		unsigned int testend;
		unsigned int start;
		unsigned int end;

		vector<Edge> edges;

		unsigned int edgeCrossing = 0;
		unsigned int edgeOnPage;

		edgeOnPage = edge->getPage();
		edges = (*book)[edgeOnPage].getEdgesOnPage();

		// set start and end node in correct order for easier testing
		if (edge->getStartNode()->getPosition() < edge->getEndNode()->getPosition()) {
			start = edge->getStartNode()->getPosition();
			end = edge->getEndNode()->getPosition();
		} else {
			start = edge->getEndNode()->getPosition();
			end = edge->getStartNode()->getPosition();
		}

		for (unsigned int i = 0; i < edges.size(); i++) {
			// set start and end node in correct order for easier testing
			if (edges[i].getStartNode()->getPosition() < edges[i].getEndNode()->getPosition()) {
				teststart = edges[i].getStartNode()->getPosition();
				testend = edges[i].getEndNode()->getPosition();
			} else {
				teststart = edges[i].getEndNode()->getPosition();
				testend = edges[i].getStartNode()->getPosition();
			}

			if (start < teststart) { //startNode outside edge
				if ( (end > teststart) && (end < testend) ) { // endNode inside edge
					edgeCrossing++;
				}
			} else if ( (start > teststart) && (start < testend) ) { // startNode inside edge
				if (end > testend) { // endNode outside edge
					edgeCrossing++;
				}
			}
		}
		edge->setCrossings(edgeCrossing);
		return edgeCrossing;

	}

	static unsigned int calcuteAllEdgeCrossing(vector<Edge>* edgeList,	vector<Page>* book) {
		unsigned int edgeCrossing;
		unsigned int totalCrossing = 0;
		// calcute the crossing number for each edge -> total number of crossings
		// and write solution
		for (unsigned int e=0; e < edgeList->size(); e++) {
			edgeCrossing = calculateEdgeCrossing(&((*edgeList)[e]), book);

			// set crossings for edge
			//(*edgeList)[e].setCrossings(edgeCrossing);
			totalCrossing += edgeCrossing;
		}
		// crossings are counted per edge -> each crossing is counted twice
		//cout << "crossings new method: " << totalCrossing/2 << endl;
		return totalCrossing /= 2;

	}





};

#endif /* UTILITIES_H_ */
