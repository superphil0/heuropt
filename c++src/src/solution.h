#ifndef SOLUTION_H_
#define SOLUTION_H_

#include "node.h"
#include "edge.h"
#include "page.h"

#include <iostream>
#include <algorithm>
#include <list>
#include <utility>

using namespace std;

class Solution {
private:
    vector<Page> book;

public:
    Solution(vector<Page> book) {
        this->book = book;
    }

    ~Solution() {

    }



};


#endif /* SOLUTION_H_ */
