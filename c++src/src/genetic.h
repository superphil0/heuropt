#ifndef GENETIC_H_
#define GENETIC_H_

#include "kpmp_solution_writer.h"
#include "node.h"
#include "edge.h"
#include "page.h"
#include "gensolution.h"
#include "utilities.h"
#include <iostream>
#include <algorithm>
#include <list>
#include <time.h>
#include <utility>

using namespace std;

class Genetic {

private:
	vector<Gensolution> geneticSolution;
	vector<Node>* spine;
	vector<Edge>* edgeList;
	vector<Page>* book;

public:
	Genetic(vector<Node>* spine, vector<Edge>* edgeList, vector<Page>* book) {
		this->spine = spine;
		this->edgeList = edgeList;
		this->book = book;
	}

	~Genetic() {

	}

	Gensolution iterateOnce();
	void createInitialSolutions(unsigned int noOfSolutions);

	void createRandomPageAssignment(Gensolution s);

	Gensolution merge(Gensolution s1, Gensolution s2) {
		/*cout << "Before merge " << endl;
		for (int i = 0; i < s1.getSpine().size(); i++) {
			cout << s1.getSpine()[i].getName() << "-" << s1.getSpine()[i].getPosition() << endl;
		}*/
		vector<Edge> s1Edges = s1.getEdgeList();
		vector<Edge> s2Edges = s2.getEdgeList();
		vector<Edge> finalEdgeList;
		srand(clock());
		vector<Node> spine = mergeSpine(s1.getSpine(), s2.getSpine());
		Gensolution s;
		s.setSpine(spine);
		for (unsigned int i = 0; i < s1Edges.size(); i++) {
			if (s1Edges[i].getPage() == s2Edges[i].getPage()) {
				finalEdgeList.push_back(s1Edges[i]);
			}
			else {
				int r = 0 + (rand() % (int)(1 - 0 + 1));
				if (r == 0) {
					finalEdgeList.push_back(s1Edges[i]);
				}
				else {
					finalEdgeList.push_back(s2Edges[i]);
				}
			}
		}
		int crossings = Utilities::calculateEdgeCrossing(&finalEdgeList);
		s.setEdgeList(finalEdgeList);
		s.setCrossings(crossings);
		/*cout << "After node merge " << endl;
		for (int i = 0; i < s.getSpine().size(); i++) {
			cout << s.getSpine()[i].getName() << "-" << s.getSpine()[i].getPosition() << endl;
		}*/
		return s;
	}

	vector<Node> mergeSpine(vector<Node> spine1, vector<Node> spine2)
	{
		vector<Node> newSpine;
		int sizeSpine = spine1.size();
		vector<int> numberCheck(sizeSpine, 0); // check if alle numbers from 0-n-1 have appeared int solution all fields need to be 1
		vector<bool> positionSet(sizeSpine, false); // check if alle numbers from 0-n-1 have appeared int solution all fields need to be 1

		// fill everything somehow
		for (unsigned int i = 0; i < spine1.size(); i++)
		{
			int r = 0 + (rand() % (int)(1 - 0 + 1));
			if (r == 0)
			{
				if (numberCheck.at(spine1[i].getName()) == 0)
				{
					newSpine.push_back(spine1[i]);
					positionSet.at(i) = true;
					numberCheck.at(spine1[i].getName())++;
				}
			}
			else
			{
				if (numberCheck.at(spine1[i].getName()) == 0)
				{
					newSpine.push_back(spine1[i]);
					positionSet.at(i) = true;
					numberCheck.at(spine1[i].getName())++;
				}
			}
		}
		// fix double entries
		int j = 0;
		for (; j < numberCheck.size(); j++)
		{
			if (numberCheck.at(j) == 0)
			{
				numberCheck.at(j) = 1;
				for (unsigned int k = 0; k < newSpine.size(); k++)
				{
					if (spine1.at(k).getName() == j)
					{
						for (int i = 0; i < positionSet.size(); i++)
						{
							if (positionSet.at(i) == false)
							{
								spine1.at(k).setPosition(i);
								newSpine.push_back(spine1.at(k));
								break;
							}
						}
						break;
					}
				}
			}
		}
		return newSpine;

	}

	Gensolution mergePivot(Gensolution s1, Gensolution s2) {
		vector<Edge> s1Edges = s1.getEdgeList();
		vector<Edge> s2Edges = s2.getEdgeList();
		vector<Edge> finalEdgeList;

		int pivot = 0 + (rand() % (int)(s1Edges.size() - 0 + 1));

		for (int i = 0; i < s1Edges.size(); i++) {
			if (i < pivot) {
				finalEdgeList.push_back(s1Edges[i]);
			}
			else {
				finalEdgeList.push_back(s2Edges[i]);
			}
		}

		Gensolution s;
		int crossings = Utilities::calculateEdgeCrossing(&finalEdgeList);
		s.setEdgeList(finalEdgeList);
		s.setCrossings(crossings);

		return s;

	}

	vector<Gensolution> select(vector<Gensolution> solutions, int k) {
		srand(clock());
		int numSolutions = solutions.size();
		double sum = 0;
		for (int i = 0; i < numSolutions; i++)
		{
			if (solutions[i].getCrossings() != 0)
			{
				sum += 1 / (double)solutions[i].getCrossings();
			}
		}
		vector<Gensolution> returns;
		for (int j = 0; j < k; j++)
		{
			double which = fmod((double)rand(), sum); //this is now index;
			int i = 0;
			while (which > 0)
			{
				if (solutions[i].getCrossings() != 0)
				{
					which -= 1 / (double)solutions[i].getCrossings();
				}
				i++;
			}
			if (i > 0) i--;
			returns.push_back(solutions[i]);
		}
		random_shuffle(returns.begin(), returns.end());
		return returns;
	}
};


#endif /* DETERMINISTIC_H_ */
