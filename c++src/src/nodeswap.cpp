#include <iostream>

#include "nodeswap.h"
#include <time.h>
#include <climits>
#include <algorithm>
#include "edgeswap.h"

bool sortNodesByEdgeCrossings(Node* first, Node* second) {
	return first->getCrossings() > second->getCrossings();
}
void changeOrder(Node* node, int position, vector<Node>* spine)
{
	int oldPos = node->getPosition();
	int lower = min(position, oldPos);
	int upper = max(position, oldPos);
	// only have to modify position of nodes between old pos and new pos
	for (unsigned int i = 0; i < spine->size(); i++)
	{
		int pos = spine->at(i).getPosition();
		if (pos >= lower && pos <= upper)
		{
			if (oldPos < pos)
			{
				spine->at(i).setPosition(pos - 1);
			}
			else {
				spine->at(i).setPosition(pos + 1);
			}
		}
	}
	node->setPosition(position);
}
void Nodeswap::swap(unsigned int noNodesToSwap) {

	// init variables for page checking
	vector<unsigned int> pages;
	unsigned int c = 0;
	srand(time(NULL));
	*totalCrossings = 0;
	int edgesPage = 0;
	// calculate crossings per node
	for (unsigned int e = 0; e < edgeList->size(); e++) {
		edgesPage = (*edgeList)[e].getPage();
		c = (*book)[edgesPage].calculateCrossings((*edgeList)[e]);
		Node* a = (*edgeList)[e].getStartNode();
		a->setCrossings(a->getCrossings() + c);
		a = (*edgeList)[e].getEndNode();
		a->setCrossings(a->getCrossings() + c);
		*totalCrossings += c;
	}

	*totalCrossings /= 2;
	int oldBest = *totalCrossings;
	// sort nodes
	int nodes = spine->size();
	vector<Node*> spineRef;
	for (int i = 0; i < nodes; i++)
	{
		spineRef.push_back(&spine->at(i));
	}
	sort(spineRef.begin(), spineRef.end(), sortNodesByEdgeCrossings);
	// always only try for node that has the most edgecrossings
	// alternative: has the most edges that have crossings
	int bestPos = 0;
	int crossingsBest = INT_MAX;
	int bestNode = 0;
	for (int i = 0; i < nodes; i++)
	{
		unsigned int result = crossingsWithNodeOnPos(spineRef.at(0), i);
		if (result < crossingsBest)
		{
			crossingsBest = result;
			bestPos = i;
		}
	}
	changeOrder(spineRef.at(0), bestPos, spine);
	cout << "crossings before: " << oldBest << " new best: " << crossingsBest << endl;
}

// starts at 0 and ends at len - -1
unsigned int Nodeswap::crossingsWithNodeOnPos(Node* node, int position)
{
	if (position >= spine->size()) return UINT_MAX;
	int edgesPage = 0;
	int c = 0;
	int oldPos = node->getPosition();
	Node* otherNode = &spine->at(position);
	// swap
	node->setPosition(position);
	otherNode->setPosition(oldPos);
	// or insert
	//changeOrder(node, position, spine);
	// calc  new crossing value
	*totalCrossings = 0;
	for (unsigned int e = 0; e < edgeList->size(); e++) {
		edgesPage = (*edgeList)[e].getPage();
		c = (*book)[edgesPage].calculateCrossings((*edgeList)[e]);
		*totalCrossings += c;
	}
	*totalCrossings /= 2;
	// insert
	//changeOrder(node, oldPos, spine);
	// swap
	node->setPosition(oldPos);
	otherNode->setPosition(position);
	return *totalCrossings;
}
