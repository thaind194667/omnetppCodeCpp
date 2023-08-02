#ifndef _BAI7_HPP_
#define _BAI7_HPP_

#include "Bai56.hpp"

using namespace std;

int keySize;
int TENodesSize;

//// Bai7-b
void assignKey(TimeExpandedNode *element, int* autoIncreament) {
    if(element == NULL)
        return;
    if(element->key == -1){
        element->key = *autoIncreament;
        *autoIncreament = *autoIncreament + 1;
    }
    // vector<pair<TimeExpandedNode *, Shape *>> * v = &element->tgts;
    if(&element->tgts == nullptr)
        return;                                                                                                                                                                                                                                                                                                                              
    if(element->tgts.size() == 0)
        return;
    for(std::pair<TimeExpandedNode *, Shape *> e : element->tgts){
        assignKey(e.first, autoIncreament);
    }
}

//// Bai7-c
void assertCheckKey(std::vector<std::vector<TimeExpandedNode*>> graph) {
    cout << "TENodes size: " << TENodesSize << "\n";
    cout << "KeySize: " << keySize << "\n";
    assert(keySize == TENodesSize + 1);
    int check[keySize];
    memset(check, 0, sizeof(check));
    for(std::vector<TimeExpandedNode *> it : graph){
        for(TimeExpandedNode* element : it){
            int x = element->key;
            assert(check[x] == 0);
            check[x]++;
        }
    }
}

//// Bai7-e
void modify(vector<vector<TimeExpandedNode*>> graph, string shapeName, int number, double minMiddleMax[]) {
    double expectedValue;
    if(!minMiddleMax[2]) {
        expectedValue = (minMiddleMax[0] + minMiddleMax[1]) / (2.0 * number);
    }
    else {
        expectedValue = (minMiddleMax[0] + 2 * minMiddleMax[1] + minMiddleMax[2]) / (4.0 * number);
    }
    for(std::vector<TimeExpandedNode *> it : graph){
        for(TimeExpandedNode* element : it){

            //// Kiểm tra trong srcs và tgts của từng TENodes
            for(pair<TimeExpandedNode *, Shape *> p : element->srcs) {
                Shape* shape = p.second;
                if(shape->name == shapeName) {
                    shape->time = expectedValue; /// trường time (của shape) sẽ bằng expectedValue
                }
            }
            for(pair<TimeExpandedNode *, Shape *> p : element->tgts) {
                Shape* shape = p.second;
                if(shape->name == shapeName) {
                    shape->time = expectedValue; /// trường time (của shape) sẽ bằng expectedValue
                }
            }
        }
    }
}


//// Bai7-e
void writeFile(vector<vector<TimeExpandedNode*>> &graph, string* fileName) {
    ofstream outFile;
    outFile.open(*fileName);
}


#endif