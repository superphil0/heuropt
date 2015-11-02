#ifndef PAGE_H_
#define PAGE_H_

#include <iostream>
#include <string>
#include <vector>
#include "edge.h"

using namespace std;

class Page {
private:
	unsigned int pageNummer;
	unsigned int pageCrossings;
	vector<Edge> edges;
	vector<Node> spineOrder;



public:

	Page(){

    }

	Page(vector<Node> spineOrder){
		this->spineOrder = spineOrder;
    }

	~Page() {

	}

	unsigned int getPageNumber() const {
		return pageNummer;
	}

	unsigned int getCrossings() const {
		return pageCrossings;

	}

	bool removeEdgeFromPage(Edge edge) {
		for (unsigned int e = 0; e < edges.size(); e++) {
			if((edges[e].getStartNode()->getName() == edge.getStartNode()->getName()) && (edges[e].getEndNode()->getName() == edge.getEndNode()->getName())) {
				// cout << "edge on pos " << e << " found" << endl;
				edges.erase(edges.begin()+e);
				return true;
			}
		}
		return false;
	}

	vector<Edge> getEdgesOnPage() {
		return edges;
	}


    unsigned int calculateCrossings(Edge edge) {
        unsigned int edgeCrossing = 0;
		unsigned int teststart;
		unsigned int testend;
		unsigned int start;
		unsigned int end;



		if (edge.getStartNode()->getPosition() < edge.getEndNode()->getPosition()) {
			start = edge.getStartNode()->getPosition();
			end = edge.getEndNode()->getPosition();
		} else {
			start = edge.getEndNode()->getPosition();
			end = edge.getStartNode()->getPosition();
		}

		for (unsigned int i = 0; i < edges.size(); i++) {
            //cout << " on " << edges[i].getStartNode() << "-" << edges[i].getEndNode() << endl;

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
        return edgeCrossing;
    }

    unsigned int addEdge(Edge edge) {
        edges.push_back(edge);
        return 1;
    }

    unsigned int writeEdges() {
		// cout << "number of edges " << edges.size() << endl;
		for (unsigned int i= 0; i < edges.size(); i++) {
            // cout << "edge from " << edges[i].getStartNode()->getName() << " to " << edges[i].getEndNode()->getName() << endl;
        }
        return 1;
    }

};

#endif /* PAGE_H_ */
