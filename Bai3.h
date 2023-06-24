#ifndef _BAI3_H_
#define _BAI3_H_

#include "TENodeAlgorithm.h"
#include "element.h"

using namespace std;

//// cau b
TimeExpandedNode* isAvailable(std::vector<std::vector<TimeExpandedNode*>> graph, Point* origin, double time) {
    for(auto node: graph) {
        for(auto element: node) {
            if(element->origin->equals(origin) && element->time == time) {
                return element;
            }
        }
    }
    return NULL;
}

//// cau c
bool isAvailable(std::vector<std::vector<TimeExpandedNode*>> graph, double time, int *index) {
    for(auto node: graph) {
        for(auto element: node) {
            if(element->time == time) {
                return element;
            }
        }
    }
}
//// cau d
void insert(std::vector<std::vector<TimeExpandedNode*>> graph, TimeExpandedNode *ten) {

}

//// cau f
void spread(std::vector<std::vector<TimeExpandedNode*>> graph, int m, int n, double H){
    TimeExpandedNode* node = graph.at(m).at(n);
    std:queue<TimeExpandedNode*> Q;
    Q.push(node );
    while(!Q.empty()){
        TimeExpandedNode* temp = Q.front();
        Q.pop();
        for(auto pair: temp->tgts){
            auto s = pair.second; 
            double time = temp->time + s->getTime();
            if(time < H){
                auto origin = s->end;  auto n = pair.first;
                auto foundItem = isAvailable(graph, origin, time);
                if(foundItem == NULL){
                    TimeExpandedNode* newNode = 
                                    new TimeExpandedNode(origin, time, n->tgts);
                    pair.first = newNode;
                    foundItem = newNode;
                }
                int index = foundItem->indexInSources(s);
                if(index != -1){
                    foundItem->srcs.at(index).first = temp;
                }
                else{
                    foundItem->srcs.push_back(std::make_pair(temp, s));
                }
                insert(graph, foundItem); //gọi hàm đã làm ở câu (d)
                Q.push(foundItem);
            }
        }
    }
}

//// cau e'
std::vector<std::pair<int, int>> filter(std::vector<std::vector<TimeExpandedNode*>> graph, double v) {

}


//// cau f'
void remove(std::vector<std::pair<int, int>> filters, std::vector<std::vector<TimeExpandedNode*>> graph) {

}


//// cau g 
std::vector<int> getStartedNodes(std::vector<std::vector<TimeExpandedNode*>> graph) {

}


/// cau h 
void assertTime(std::vector<TimeExpandedNode*> graph, double v) {

}


//// cau i
void runBai3() {

}

#endif

