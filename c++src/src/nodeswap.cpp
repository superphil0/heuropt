#include <iostream>

#include "nodeswap.h"
#include <time.h>
#include <climits>
#include <algorithm>
#include "edgeswap.h"
#include <ctime>

bool sortNodesByEdgeCrossings(Node* first, Node* second) {
	return first->getCrossings() > second->getCrossings();
}
void swapNodes(Node* node, int position, vector<Node>* spine)
{
	int oldPos = node->getPosition();
	auto it = find_if(spine->begin(), spine->end(), [&position](const Node& obj) {return obj.getPosition() == position;});
	Node* otherNode = &(*it);
	node->setPosition(position);
	otherNode->setPosition(oldPos);
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
void Nodeswap::swap(int select, int mode) {

	// init variables for page checking
	vector<unsigned int> pages;
	unsigned int c = 0;
	srand(clock());
	*totalCrossings = 0;
	int edgesPage = 0;
	for (unsigned int node = 0; node < spine->size(); node++) {
		spine->at(node).setCrossings(0);
	}
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
	int crossingsBest = oldBest;
	int bestNode = 0;
	int endValue = 1;
	int startValue = 0;
	if (select == 1)
	{
		endValue = nodes;
		startValue = 0;
	}
	else if (select == 2)
	{
		endValue = 1;
		startValue = 0;
	}
	else if (select == 3)
	{
		// select which node to swap before
		startValue = rand() % nodes;
		endValue = startValue + 1;
	}
	// because they are sorted after quality
	for (int node = startValue; node < endValue; node++)
	{
		for (int i = 0; i < nodes; i++)
		{
			if (i == node) continue;
			unsigned int result = crossingsWithNodeOnPos(spineRef.at(node), i, mode);
			if (result < crossingsBest)
			{
				crossingsBest = result;
				bestPos = i;
				bestNode = node;
				break;
			}
		}
	}
	if (crossingsBest == oldBest) return;
	if (mode == 1)
	{
		swapNodes(spineRef.at(bestNode), bestPos, spine);
	}
	else {
		changeOrder(spineRef.at(bestNode), bestPos, spine);
	}
	//cout << "crossings before: " << oldBest << " new best: " << crossingsBest << endl;
}

// starts at 0 and ends at len - -1
unsigned int Nodeswap::crossingsWithNodeOnPos(Node* node, int position, int mode)
{
	if (position >= spine->size()) return UINT_MAX;
	int edgesPage = 0;
	int oldPos = node->getPosition();
	int c = 0;
	if (mode == 1)
	{
		// swap
		swapNodes(node, position, spine);
	}
	else {
		// or insert
		changeOrder(node, position, spine);
	}
	// calc  new crossing value
	*totalCrossings = 0;
	for (unsigned int e = 0; e < edgeList->size(); e++) {
		edgesPage = (*edgeList)[e].getPage();
		c = (*book)[edgesPage].calculateCrossings((*edgeList)[e]);
		*totalCrossings += c;
	}
	*totalCrossings /= 2;
	if (mode == 1)
	{
		// swap
		swapNodes(node, oldPos, spine);
	}
	else {
		// or insert
		changeOrder(node, oldPos, spine);
	}
	return *totalCrossings;
}
