#ifndef GENSOLUTION_H_
#define GENSOLUTION_H_

#include "node.h"
#include "edge.h"
#include "page.h"

#include <iostream>
#include <algorithm>
#include <list>
#include <utility>

using namespace std;

class Gensolution {
private:
    vector<Edge> edgeList;
    int crossings;

public:
    Gensolution() {

    }

    ~Gensolution() {

    }

    void setEdgeList(vector<Edge> edgeList) {
        this->edgeList = edgeList;
    }

    vector<Edge> getEdgeList() {
        return this->edgeList;
    }

    void setCrossings(int crossings) {
        this->crossings = crossings;
    }

    int getCrossings() {
        return this->crossings;
    }

};


#endif /* GENSOLUTION_H_ */
