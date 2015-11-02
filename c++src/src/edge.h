#ifndef EDGE_H_
#define EDGE_H_

#include "node.h"

#include <iostream>
#include <stdlib.h>

using namespace std;

class Edge {
private:
	Node* startNode;
	Node* endNode;
	unsigned int page;
	unsigned int crossings;



public:
    Edge(Node* startNode, Node* endNode){
        this->startNode = startNode;
        this->endNode = endNode;
		this->crossings = 0;
    }

	~Edge() {

	}

	Node* getStartNode() const {
		return startNode;
	}

	Node* getEndNode() const {
		return endNode;
	}

	void setPage(unsigned int page) {
		this->page = page;
	}

	unsigned int getPage() {
		return page;
	}

	void setCrossings(unsigned int crossings) {
		this->crossings = crossings;
	}

	unsigned int getCrossings() {
		return crossings;
	}

	int getLength() const {
		return abs(endNode->getPosition() - startNode->getPosition());
	}

	static bool ascLength(Edge e1, Edge e2) {
	    return (abs(e1.getEndNode()->getPosition() - e1.getStartNode()->getPosition())) < abs((e2.getEndNode()->getPosition() - e2.getStartNode()->getPosition()));
	}

	static bool dscLength(Edge e1, Edge e2) {
	    return (abs(e1.getEndNode()->getPosition() - e1.getStartNode()->getPosition())) > abs((e2.getEndNode()->getPosition() - e2.getStartNode()->getPosition()));
	}

	void writeEdge() {
		cout << startNode->getName() << ' ' << endNode->getName() << ' ' << startNode->getPosition() << ' ' << endNode->getPosition() <<  ' ' << getLength() << endl;
	}


};

#endif /* EDGE_H_ */
