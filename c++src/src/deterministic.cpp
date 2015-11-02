#include <iostream>

#include "deterministic.h"
#include <time.h>
#include <climits>

bool so(pair<unsigned int, unsigned int> left, pair<unsigned int, unsigned int> right) {
    return left.second < right.second;
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
    //sort(sortVector.begin(), sortVector.end(), so);
    random_shuffle(sortVector.begin(), sortVector.end());

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


void Deterministic::h1() {
    vector<unsigned int> pages;
    unsigned int currMinCrossing = UINT_MAX;
    unsigned int c = 0;
    *totalCrossings = 0;
    srand (time(NULL));
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

        random_shuffle(pages.begin(), pages.end());
        // pages[0] - first element in shuffled vector
        (*book)[pages[0]].addEdge((*edgeList)[e]);
        (*edgeList)[e].setPage(pages[0]);
        solution->addEdgeOnPage((*edgeList)[e].getStartNode()->getName(), (*edgeList)[e].getEndNode()->getName(),  pages[0]);
        // cout <<"insert " << (*edgeList)[e].getStartNode()->getName() << " - " << (*edgeList)[e].getEndNode()->getName() << " to " << pages[0] << endl;
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


    // vector<Edge> edgesOnPage;
    // for (unsigned int p=0; p < book->size(); p++) {
    //     edgesOnPage = (*book)[p].getEdgesOnPage();
    //     for (unsigned int e=0; e < edgesOnPage.size(); e++) {
    //         cout << "page " << p << " edge " << edgesOnPage[e].getStartNode()->getName() << " - " << edgesOnPage[e].getEndNode()->getName() << endl;
    //         c = (*book)[p].calculateCrossings(edgesOnPage[e]);
    //         cout << "edgecrossings " << c << endl;
    //         edgesOnPage[e].setCrossings(c);
    //     }
    // }
    cout << "total crossings " << *totalCrossings << endl;
}
