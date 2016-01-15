#ifndef GENSOLUTION_H_
#define GENSOLUTION_H_

#include "node.h"
#include "edge.h"
#include "node.h"
#include "page.h"

#include <iostream>
#include <algorithm>
#include <list>
#include <utility>

using namespace std;

class Gensolution {
private:
    vector<Edge> edgeList;
    vector<Node> spine;
    vector<Page> book;
    int crossings;

public:
    Gensolution() {

    }

    ~Gensolution() {

    }

    void setSpine(vector<Node> spine) {
        this->spine = spine;
    }
    vector<Node> getSpine() {
        return this->spine;
    }

    void setEdgeList(vector<Edge> edgeList) {
        this->edgeList = edgeList;
    }
    vector<Edge> getEdgeList() {
        return this->edgeList;
    }

    void setBook(vector<Page> book) {
        this->book = book;
    }
    vector<Page> getBook() {
        return this->book;
    }

    void setCrossings(int crossings) {
        this->crossings = crossings;
    }

    int getCrossings() {
        return this->crossings;
    }

};


#endif /* GENSOLUTION_H_ */
