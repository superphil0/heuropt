#include <iostream>

#include "edgeswap.h"
#include <time.h>
#include <climits>

bool sortByCrossingAsc(Edge first, Edge second) {
    return first.getCrossings() < second.getCrossings();
}

bool sortByCrossingDsc(Edge first, Edge second) {
    return first.getCrossings() > second.getCrossings();
}


void Edgeswap::swap(unsigned int noEdgesToSwap) {

    // sort edges s.t. the edges with the most crossings are at the beginning of the vector
    sort(edgeList->begin(), edgeList->end(), sortByCrossingDsc);

    // remove the first e edges from the solution
    for (int e=0; e < noEdgesToSwap; e++) {
        for (int p=0; p < book->size(); p++) {
            if ((*book)[p].removeEdgeFromPage((*edgeList)[e])) {
                break;
            }
        }
    }

    // init variables for page checking
    vector<unsigned int> pages;
    unsigned int currMinCrossing = UINT_MAX;
    unsigned int c = 0;
    *totalCrossings = 0;
    srand (time(NULL));

    //find best pages for removed edges
    // if possible edges should be added to different page
    for (int e=0; e < noEdgesToSwap; e++) {
        for (unsigned int p = 0; p < book->size(); p++) {
            c = (*book)[p].calculateCrossings((*edgeList)[e]);
            if ( (c < currMinCrossing) && (p != (*edgeList)[e].getPage()) && (c < (*edgeList)[e].getCrossings()) ) {
                pages.clear();
                pages.push_back(p);
                currMinCrossing = c;
            } else if ( (c == currMinCrossing) && (p != (*edgeList)[e].getPage()) && (c < (*edgeList)[e].getCrossings()) ) {
                pages.push_back(p);
            }
        }

        // better solution for edge found
        if (pages.size() != 0) {

            //random_shuffle(pages.begin(), pages.end());
            // pages[0] - first element in shuffled vector
            (*book)[pages[0]].addEdge((*edgeList)[e]);
            (*edgeList)[e].setPage(pages[0]);
            // cout <<"insert " << (*edgeList)[e].getStartNode()->getName() << " - " << (*edgeList)[e].getEndNode()->getName() << " to " << pages[0] << endl;
            pages.clear();
        }
        // the origial page is the best solution
        else {
            (*book)[(*edgeList)[e].getPage()].addEdge((*edgeList)[e]);
            (*edgeList)[e].setPage((*edgeList)[e].getPage());
        }
        currMinCrossing = UINT_MAX;
    }



    int edgesPage = 0;
    // calcute the crossing number for each edge -> total number of crossings
    // and write solution
    for (unsigned int e=0; e < edgeList->size(); e++) {
        edgesPage = (*edgeList)[e].getPage();
        solution->addEdgeOnPage((*edgeList)[e].getStartNode()->getName(), (*edgeList)[e].getEndNode()->getName(),  edgesPage);
        c = (*book)[edgesPage].calculateCrossings((*edgeList)[e]);
        (*edgeList)[e].setCrossings(c);
        *totalCrossings += c;
    }
    // crossings are counted per edge -> each crossing is counted twice
    *totalCrossings /= 2;
    cout << "total crossings " << *totalCrossings << endl;


}