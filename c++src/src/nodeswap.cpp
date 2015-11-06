#include <iostream>

#include "nodeswap.h"
#include <time.h>
#include <climits>
#include <algorithm>
#include "edgeswap.h"

bool sortNodesByEdgeCrossings(Node first, Node second) {
    return first.getCrossings() > second.getCrossings();
}
void Nodeswap::swap(unsigned int noNodesToSwap) {

    // init variables for page checking
    vector<unsigned int> pages;
    unsigned int currMinCrossing = UINT_MAX;
    unsigned int c = 0;
    srand (time(NULL));
    *totalCrossings = 0;
    int edgesPage = 0;
	// calculate crossings per node
    for (unsigned int e=0; e < edgeList->size(); e++) {
        edgesPage = (*edgeList)[e].getPage();
        c = (*book)[edgesPage].calculateCrossings((*edgeList)[e]);
		Node* a = (*edgeList)[e].getStartNode();
		a->setCrossings(a->getCrossings() + c);
		a = (*edgeList)[e].getEndNode();
		a->setCrossings(a->getCrossings() + c);
        *totalCrossings += c;
    }

    *totalCrossings /= 2;

	// sort nodes
	sort(spine->begin(), spine->end(), sortNodesByEdgeCrossings);

	// swap nodes shifty 1,2,3 => 2,3,1
	unsigned int last = spine->begin()->getPosition();
	unsigned int tmp = spine->begin()->getPosition();
	for (unsigned int n = 1; n < noNodesToSwap; n++)
	{
		tmp = spine->at(n).getPosition();
		spine->at(n).setPosition(last);
		last = tmp;
	}
	spine->begin()->setPosition(last);
	int oldVal = *totalCrossings;
    // calc  new crossing value
    for (unsigned int e=0; e < edgeList->size(); e++) {
        edgesPage = (*edgeList)[e].getPage();
        c = (*book)[edgesPage].calculateCrossings((*edgeList)[e]);
        *totalCrossings += c;
    }
    *totalCrossings /= 2;
	if (*totalCrossings > oldVal)
	{
		// should use the old form
	}
	cout << "old: " << oldVal << " newval: " << *totalCrossings << endl;
}
