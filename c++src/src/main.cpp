#include "kpmp_instance.h"
#include "kpmp_solution_writer.h"
#include "page.h"
#include "edge.h"
#include "node.h"
#include "deterministic.h"
#include "edgeswap.h"
#include "nodeswap.h"

#include <memory>
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <list>
#include <climits>

using namespace std;

#ifdef __linux__
# include <sys/times.h>
# include <unistd.h>
#endif

double getCPUtime()
{
#ifdef __linux__
    tms t;
    times( &t );
    double ct = sysconf( _SC_CLK_TCK );
    return t.tms_utime / ct;
#else
    return 0; //note: cluster runs on linux
#endif
}

void usage()
{
    cout << "USAGE:\t<program> -f filename -m mode\n";
    cout << "EXAMPLE:\t" << "./HOT -f instances/automatic-1.txt -m grasp\n\n";
} // usage


int main(int argc, char** argv)
{
    // default values
    std::string mode("unset");
#ifdef __linux__
    std::string file("../instances/automatic-1.txt");
#else
    std::string file("../../c++src/instances/automatic-3.txt");
#endif

    //process input params
    for(int i = 1; i < argc; ++i) {
        if(strcmp(argv[i], "-m") == 0) {
            if(i+1 == argc) {
                std::cout << "Missing arg for: " << argv[i] << std::endl;
                return -1;
            }
            mode.assign(argv[i+1]);
            ++i;
        } else if(strcmp(argv[i], "-f") == 0) {
            if(i+1 == argc) {
                std::cout << "Missing arg for: " << argv[i] << std::endl;
                return -1;
            }
            file.assign(argv[i+1]);
            ++i;
        } else {
            std::cout << "Unrecognised command: " << argv[i] << std::endl;
            return -1;
        }
    }

    // read instance
    std::shared_ptr<KPMPInstance> instance(KPMPInstance::readInstance(file));
    // transform mode to lower string
    std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);


    unsigned int numVertices = instance->getNumVertices();
    unsigned int booksize = instance->getK();
    vector<vector<unsigned int>> adjList = instance->getAdjacencyList();
    vector<Node> spine;
    unsigned int totalCrossings = UINT_MAX;

    vector<Page> book;
    for (unsigned int k = 0; k < booksize; k++) {
        book.push_back(Page());
    }


    //init writer
    KPMPSolutionWriter *solution = new KPMPSolutionWriter(booksize);


    //solve instance
    if(mode == "deterministic") {
        //TODO: DET

        //Page bookpage = Page();
        vector<Edge> edgeList;
        unsigned int edgeCount;

        //create initial spine
        for (unsigned int startNode = 0; startNode < adjList.size(); startNode++) {
            spine.push_back(Node(startNode, adjList[startNode].size(), startNode));
            // cout << "size: " << spine.size() << endl;
        }

        // populate edgeList and create node order

        Node* tmpItStart = nullptr;
		Node* tmpItEnd = nullptr;
        for (unsigned int startNode = 0; startNode < adjList.size(); startNode++) {
            for (unsigned int i = 0; i < adjList[startNode].size(); i++) {
                if (startNode < adjList[startNode][i]) {
                    for (unsigned int j = 0; j < spine.size(); j++) {
                        if (spine[j].getName() == startNode) {
                            tmpItStart = &spine[j];
                        }
                        if (spine[j].getName() == adjList[startNode][i]) {
                            tmpItEnd = &spine[j];
                        }

                    }
                    edgeList.push_back(Edge(tmpItStart, tmpItEnd));
                }
            }
        }

        // create deterministic instance
        Deterministic deter(solution, &spine, &edgeList, &book, &totalCrossings);
        // sort spine in descending node order

        deter.sortSpineDFS();

        deter.writeSpine();
        //deter.sortSpine(-1);

        // fill spineorder for sulution
        vector<unsigned int> finalSpineOrder(numVertices);
        for (unsigned int i = 0; i <  spine.size(); i++) {
            finalSpineOrder[spine[i].getPosition()] = spine[i].getName();
        }
        // sort edges with descending length
        deter.sortEdges(-1);
        // deter.writeSpine();
        // deter.writeEdgeList();
        deter.h1();

        // for (unsigned int e=0; e < edgeList.size(); e++) {
        //     cout << "final edge state " << edgeList[e].getStartNode()->getName() << " - " << edgeList[e].getEndNode()->getName() << " crossings " << edgeList[e].getCrossings() << " on Page " << edgeList[e].getPage() <<endl;
        // }

        Edgeswap edgeswap(solution, &spine, &edgeList, &book, &totalCrossings);
        Nodeswap nodeswap(solution, &spine, &edgeList, &book, &totalCrossings, &adjList);
        solution->setSpineOrder(finalSpineOrder);
        solution->write("solution");
        solution->clearSolution();
        unsigned int maxTotalCrossings = UINT_MAX;
        unsigned int iteration = 0;
		// termination condition
        while (maxTotalCrossings > totalCrossings || iteration < 10) {
            // mySolution.str("");
			// if (maxTotalCrossings > totalCrossings) {
			// }
			 	iteration++;
            maxTotalCrossings = totalCrossings;
            solution->clearSolution();

			nodeswap.swap(3);
            // deter.writeSpine();
            // deter.writeEdgeList();
            // noEdgesToSwap, method
            // method 1 = bestImprovement
            // method 2 = firstImprovement
            // method 3 = randomImprovement
            edgeswap.swap(3, 1);
			// vector<unsigned int> finalSpineOrder(numVertices);
			// for (unsigned int i = 0; i <  spine.size(); i++) {
			// 	finalSpineOrder[spine[i].getPosition()] = spine[i].getName();
			// }
			// solution->setSpineOrder(finalSpineOrder);
			// cout << "iterations " << iteration << endl;
            // mySolution << "solution_" << iteration++;
            // solution->write(mySolution.str());
            // solution->clearSolution();

        }



        solution->setSpineOrder(finalSpineOrder);

        solution->write("solutionswap.txt");



    } else if(mode == "grasp") {
        //TODO: GRASP
    } else {
        std::cout << "Mode: " << mode << " is not implemented" << std::endl;
        usage();
        return -1;
    }

    std::cout << "CPU Time: " << getCPUtime() << std::endl;
	std::cin.get();
    return 0;
} // main
