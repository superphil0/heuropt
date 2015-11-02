#include <iostream>

#include "deterministic.h"
#include <time.h>
#include <climits>

bool sortNodeDegreeAsc(pair<unsigned int, unsigned int> left, pair<unsigned int, unsigned int> right) {
    return left.second < right.second;
}

bool sortNodeDegreeDsc(pair<unsigned int, unsigned int> left, pair<unsigned int, unsigned int> right) {
    return left.second > right.second;
}

void Deterministic::writeSpine() {
    cout << "Spine: " << endl;
    for(unsigned int i = 0; i < spine->size(); i++) {
        cout << "name " << (*spine)[i].getName() << " pos " << (*spine)[i].getPosition() << endl;
    }
}

void Deterministic::writeEdgeList() {
    for (unsigned int i=0; i < (*edgeList).size(); i++) {
        // cout << (*edgeList)[i].getStartNode()->getName() << "-" << (*edgeList)[i].getEndNode()->getName() << (*edgeList)[i].getStartNode()->getPosition() << "-" << (*edgeList)[i].getEndNode()->getPosition() << endl;
        cout << (*edgeList)[i].getStartNode()->getName() << "-" << (*edgeList)[i].getEndNode()->getName() << (*edgeList)[i].getCrossings() << endl;
    }
}


void Deterministic::sortSpine(int order) {
    vector<pair<unsigned int, unsigned int>> sortVector;
    for(int i = 0; i < spine->size(); i++) {
        sortVector.push_back( make_pair((*spine)[i].getName(), (*spine)[i].getEdgeCount()) );
    }
    // sort spine on node degree
    //sort(sortVector.begin(), sortVector.end(), sortNodeDegreeAsc);

    // shuffle spine order
    random_shuffle(sortVector.begin(), sortVector.end());

    // set the position of the nodes in the spine
    for(int i = 0; i < sortVector.size(); i++) {
        (*spine)[sortVector[i].first].setPosition(i);

    }

}

void Deterministic::sortEdges(int order) {
    if (order > 0) {
        sort(edgeList->begin(), edgeList->end(), Edge::ascLength);
    } else if ( order < 0){
        sort(edgeList->begin(), edgeList->end(), Edge::dscLength);
    } else {
        //random
    }
}

void Deterministic::sortSpineDFS() {

    vector<unsigned int> nodes;
    vector<pair<unsigned int, unsigned int>> spineOrder;
    // randomly chose start node
    srand (time(NULL));
    for (unsigned int i = 0; i < spine->size(); i++) {
        nodes.push_back(i);
    }
    random_shuffle(nodes.begin(), nodes.end());
    unsigned int start;
    bool success;
    unsigned int pos = 0;
    start = nodes[0];
    spineOrder.push_back(make_pair(pos++, start));
    nodes.erase(nodes.begin());
    while(nodes.size() != 0) {
        success = false;
        for (unsigned int e = 0; e < edgeList->size(); e++) {
            if ( ((*edgeList)[e].getStartNode()->getName() == start) &&
                 (find(nodes.begin(), nodes.end(), (*edgeList)[e].getEndNode()->getName()) != nodes.end()) ) {
                start = (*edgeList)[e].getEndNode()->getName();
                nodes.erase(find(nodes.begin(), nodes.end(), start));
                spineOrder.push_back(make_pair(pos++, start));
                success = true;
                break;
            }
        }
        // if no successor was found take next in order
        if (!success) {
            start = nodes[0];
            spineOrder.push_back(make_pair(pos++, start));
            nodes.erase(nodes.begin());
        }
    }

    for (unsigned int n = 0; n < spine->size(); n++) {

        (*spine)[spineOrder[n].second].setPosition(n);
    }




}


void Deterministic::h1() {

    // init variables
    vector<unsigned int> pages;
    unsigned int currMinCrossing = UINT_MAX;
    unsigned int c = 0;
    *totalCrossings = 0;
    srand (time(NULL));

    //find best pages for inserting edges
    for(unsigned int e = 0; e < edgeList->size(); e++) {
        for (unsigned int p = 0; p < book->size(); p++) {
            c = (*book)[p].calculateCrossings((*edgeList)[e]);
            if ( c < currMinCrossing) {
                pages.clear();
                pages.push_back(p);
                currMinCrossing = c;
            } else if (c == currMinCrossing) {
                pages.push_back(p);
            }
        }

        // for multiple equal good solutions choose randomly
        random_shuffle(pages.begin(), pages.end());

        (*book)[pages[0]].addEdge((*edgeList)[e]);
        (*edgeList)[e].setPage(pages[0]);
        solution->addEdgeOnPage((*edgeList)[e].getStartNode()->getName(), (*edgeList)[e].getEndNode()->getName(),  pages[0]);
        *totalCrossings += currMinCrossing;
        currMinCrossing = UINT_MAX;
        pages.clear();
    }

    // calcultate crossings for each edge
    int edgesPage = 0;
    for (unsigned int e=0; e < edgeList->size(); e++) {
        edgesPage = (*edgeList)[e].getPage();
        (*edgeList)[e].setCrossings((*book)[edgesPage].calculateCrossings((*edgeList)[e]));
    }

    cout << "total crossings " << *totalCrossings << endl;
}
