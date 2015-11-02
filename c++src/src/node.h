#ifndef NODE_H_
#define NODE_H_

#include <iostream>

using namespace std;

class Node {
private:
	unsigned int name;
	unsigned int edgeCount;
	int pos;


public:
	Node() {

	}

	Node(unsigned int name, unsigned int edgeCount, int pos){
        this->name = name;
		this->edgeCount = edgeCount;
		this->pos = pos;
    }

	~Node() {

	}

	void setNode(unsigned int name, unsigned int edgeCount){
        this->name = name;
		this->edgeCount = edgeCount;
	}

	unsigned int getEdgeCount() const {
		return edgeCount;
	}

	unsigned int getName() const {
		return name;
	}

	int getPosition() const {
		return pos;
	}

	void setPosition(int pos) {
		this->pos = pos;
	}

	static bool dscEdgeCount(Node n1, Node n2) {
	    return n1.getEdgeCount() > n2.getEdgeCount();
	}

	static bool ascEdgeCount(Node n1, Node n2) {
	    return n1.getEdgeCount() < n2.getEdgeCount();
	}

};

#endif /* NODE_H_ */
