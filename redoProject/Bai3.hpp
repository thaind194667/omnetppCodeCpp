#ifndef _BAI3_HPP_
#define _BAI3_HPP_

#include "TimeExpandedNode.hpp"
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>

using namespace std;

// typedef struct{
//     int pos;
//     int min;
//     int max;
// }position;

//// cau b
TimeExpandedNode* isAvailable(std::vector<std::vector<TimeExpandedNode*>> graph, Point* origin, double time){
    for(auto& vec : graph) {
        for(auto& it : vec){
            if(it->origin->equals(origin)&& it->time==time)
                return it;
        }
    }
    return nullptr;
}

//// cau c
bool isavailable(std::vector<std::vector<TimeExpandedNode*>> graph, double time, int *index){
    int count = 0;
    for (vector<TimeExpandedNode *> it : graph) {
        for (TimeExpandedNode *TENode : it) {
            if (TENode->time != time) {
                *index = count;
                return false;
            }
        }
        count++;
    }
    *index = 0;
    return true;
}

//// cau d
void insert(std::vector<std::vector<TimeExpandedNode*>> &graph, TimeExpandedNode *ten)
{
    int count = 0;
    for(auto& it :graph) {
        if(ten->time == it.at(0)->time) {
            count++;
            it.push_back(ten);
        }
    }
    if(count  == 0){
        vector<TimeExpandedNode*> newvector;
        newvector.push_back(ten);
        graph.push_back(newvector);
    }
     
} 

//// cau f
void spread(std::vector<std::vector<TimeExpandedNode*>> &graph, int m, int n, double H) {
    TimeExpandedNode* node = graph[m][n];
    std::queue<TimeExpandedNode*> Q;
    Q.push(node);
    
    while (!Q.empty()) {
        TimeExpandedNode* temp = Q.front();
        Q.pop();
        
        for (auto& pair : temp->tgts) {
            Shape* s = pair.second;
            double time = temp->time + s->getTime();
            if (time < H) {
                Point* origin = s->end;  TimeExpandedNode* n = pair.first;
                TimeExpandedNode* foundItem = isAvailable(graph, origin, time);
                if (foundItem == nullptr) {
                    TimeExpandedNode* newNode = new TimeExpandedNode;
                    newNode->setTENode(origin);
                    newNode->time= time;
                    newNode->tgts = n->tgts;
                    pair.first = newNode;
                    foundItem = newNode;
                }
                int index = foundItem->indexInSources(s);
                if (index != -1) 
                    foundItem->srcs[index].first = temp;
                else {
                    foundItem->srcs.push_back(std::make_pair(temp, s));
                }
                insert(graph, foundItem); // Gọi hàm đã làm ở câu (d)
                Q.push(foundItem);
            }
        }
    }
}

//// cau e'
std::vector<std::pair<int, int>> filter(std::vector<std::vector<TimeExpandedNode*>> &graph){
    vector<std::pair<int,int>> a;
    int i,k;
    for(i = 0;i<graph.size();i++) {
        for(k=0;k<graph[i].size();k++) {
            int count  = 0;
            for(auto& it : graph.at(i).at(k)->srcs) {
                if(it.first->time+it.second->getTime() > graph.at(i).at(k)->time){
                    count++;
                }
            
            }
            if(count > 0)
                a.push_back(make_pair(i,k));
                  
        }
    }
    return a;
}  

//// cau f'
void remove(
    vector<pair<int, int>> filters,
    vector<vector<TimeExpandedNode *>> &graph
) {
    int length = filters.size();
    for (int i = length - 1; i >= 0; i--)
    {
        pair<int, int> p = filters.at(i);
        graph.at(0).erase(graph.at(0).begin() + p.second);
    }
}

//// cau g 
std::vector<int> getStartedNodes(std::vector<std::vector<TimeExpandedNode*>> graph)
{
    int count = 0;
    vector<int> result;

    for (TimeExpandedNode *TENode : graph.at(0)) {
        if (TENode->time == 0 && TENode->srcs.empty()) {
            result.push_back(count);
        }
        count++;
    }

    return result;
}
void assertTime(std::vector<TimeExpandedNode*> graph, double v){
    int stage  = 0;
    for(auto& it : graph)
    {
        int count = 0;
        for(auto& a : it->srcs)
        {
            if(a.first->time + a.second->getTime() !=  it->time) count++;
        }
        if(count != 0) { //cout<<count<<endl;
            stage =1;}
    }
    if(stage == 1) cout<<"Error assertTime"<<endl;
}

#endif