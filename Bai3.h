// #ifndef _BAI3_H_
// #define _BAI3_H_

#include "TENodeAlgorithm.h"
// #include "element.h"

using namespace std;

void sortGraph(vector<vector<TimeExpandedNode *>> &graph)
{
    sort(graph.begin(), graph.end(),
         [](vector<TimeExpandedNode *> a, vector<TimeExpandedNode *> b)
         {
             return a.at(0)->time < b.at(0)->time;
         });
}

//// cau b
TimeExpandedNode* isAvailable(std::vector<std::vector<TimeExpandedNode*>> graph, Point* origin, double time) {
    
    for (vector<TimeExpandedNode *> it : graph) {
        for (TimeExpandedNode *TENode : it) {
            if (TENode->origin->equals(origin) 
                && TENode->time == time){
                
                return TENode;
            }
        }
    }
    return nullptr;
}

//// cau c
bool isAvailable(std::vector<std::vector<TimeExpandedNode*>> graph, double time, int *index) {
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
    // int count = 0;
    // for(auto& it : graph.at(*index))
    // {
    //     if(it->time == time) count++;
    // }     
    // if(count == graph.at(*index).size()) {return true;}
    // else{
    //     index = index + 1;
    //     return false;
    // }
}
//// cau d
void insert(std::vector<std::vector<TimeExpandedNode*>> graph, TimeExpandedNode *ten) {
    for (vector<TimeExpandedNode *> it : graph) {
        if (it.at(0)->time == ten->time) {
            it.push_back(ten);
            return;
        }
    }
    vector<TimeExpandedNode *> tempTEN;
    tempTEN.push_back(ten);
    graph.push_back(tempTEN);
    // int count = 0;
     
    // for(auto& it :graph) {
    //     if(ten->time == it.at(0)->time) {
    //         count++;
    //         it.push_back(ten);
    //     }
    // }
    // if(count  == 0){
    //     vector<TimeExpandedNode*> newvector;
    //     newvector.push_back(ten);
    //     graph.push_back(newvector);
    // }
}

//// cau f
void spread(vector<vector<TimeExpandedNode *>> &graph, int m, int n, double H)
{
    int count = 0;
    TimeExpandedNode *node = graph.at(m).at(n);
    // node->origin->printPoint();
    queue<TimeExpandedNode *> Q;
    Q.push(node);

    while (!Q.empty())
    {
        TimeExpandedNode *temp = Q.front();
        Q.pop();
        // cout << temp->origin->x << " - " << temp->origin->y << endl;
        for (pair<TimeExpandedNode *, Shape *> pair : temp->tgts)
        {
            Shape *s = pair.second;
            double time = temp->time + s->getTime();
            if (time < H)
            {
                count++;
                Point *origin = s->end;
                TimeExpandedNode *TENNode = pair.first;
                TimeExpandedNode *foundItem = isAvailable(graph, origin, time);
                if (foundItem == nullptr)
                {
                    TimeExpandedNode *newNode =
                        new TimeExpandedNode(origin, time, TENNode->tgts);
                    pair.first = newNode;
                    foundItem = newNode;
                }
                int index = foundItem->indexInSources(s);
                if (index != -1)
                {
                    foundItem->srcs.at(index).first = temp;
                }
                else
                {
                    foundItem->srcs.push_back(make_pair(temp, s));
                }

                insert(graph, foundItem); // gọi hàm đã làm ở câu (d)
                Q.push(foundItem);
            }
            cout << "Spread_1" << endl;
        }
        cout << "Spread_2" << endl;
    }
    cout << "count: " << count << endl;
    cout << "Spread_3" << endl;
    /*
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
                    TimeExpandedNode* newNode = new TimeExpandedNode();
                    newNode->setTENode(origin);
                    newNode->time= time;
                    newNode->tgts = n->tgts;
                    pair.first = newNode;
                    foundItem = newNode;
                }
                int index = foundItem->indexInSources(s);
                if (index != -1) {
                    foundItem->srcs[index].first = temp;
                }
                else {
                    foundItem->srcs.push_back(std::make_pair(temp, s));
                }
                insert(graph, foundItem); // Gọi hàm đã làm ở câu (d)
                Q.push(foundItem);
            }
        }
    }
    */
}

//// cau e'
std::vector<std::pair<int, int>> filter(std::vector<std::vector<TimeExpandedNode*>> graph, double v) {
    
    vector<pair<int, int>> filters;
    int count = 0;
    for (TimeExpandedNode *TENode : graph.at(v)) {
        for (auto pair : TENode->srcs) {
            if (pair.first->time + pair.second->getTime() > TENode->time) {
                filters.push_back(make_pair(v, count));
                break;
            }
        }
        count++;
    }
    return filters;
    // vector<std::pair<int,int>> a;
    // int i,k;
    // for(i = 0;i<graph.size();i++)
    // {
    //     for(k=0;k<graph[i].size();k++)
    //     {
    //        int count  = 0;
    //        for(auto& it : graph.at(i).at(k)->srcs)
    //        {
    //         if(it.first->time+it.second->getTime() > graph.at(i).at(k)->time){
    //           count++;
    //           //a.push_back(make_pair(i,k));
    //         }
            
    //        }
    //        if(count > 0)
    //         {
    //          a.push_back(make_pair(i,k));
    //         }  
    //     }
    // }
    // return a;
}


//// cau f'
void remove(std::vector<std::pair<int, int>> filters, std::vector<std::vector<TimeExpandedNode*>> graph) {
    int length = filters.size();
    for (int i = length - 1; i >= 0; i--)
    {
        pair<int, int> p = filters.at(i);
        // cout << graph.at(0).at(p.second)->origin->x << " - "
        //      << graph.at(0).at(p.second)->origin->y << " --- " << p.second << endl;

        graph.at(0).erase(graph.at(0).begin() + p.second);
    }
}


//// cau g 
std::vector<int> getStartedNodes(std::vector<std::vector<TimeExpandedNode*>> graph) {
    int count = 0;
    vector<int> result;

    for (TimeExpandedNode *TENode : graph.at(0)) {
        if (TENode->time == 0 && TENode->srcs.empty()) {
            result.push_back(count);
        }
        count++;
    }

    return result;
    // std::vector<int> a;
    // int k ;
    // for(auto& it : graph)
    // {
    //     for(k = 0;k<it.size();k++)
    //     {
    //         if(it[k]->srcs.empty())
    //         {
    //             if(it[k]->time==0)
    //             {
    //                 a.push_back(k);
    //             }
    //         }
    //     }
    // }
    // return a;
}


/// cau h 
void assertTime(vector<vector<TimeExpandedNode*>> graph, double v) {
    for (vector<TimeExpandedNode *> it : graph) {
        for (TimeExpandedNode *TENode : it)
        {
            if (!TENode->srcs.empty())
            {
                for (auto elem : TENode->srcs)
                {
                    assert(elem.first->time + elem.second->getTime() != TENode->time);
                }
            }
        }
    }
    // int stage  = 0;
    // for(auto& it : graph)
    // {
    //     int count = 0;
    //     for(auto& a : it->srcs)
    //     {
    //         if(a.first->time + a.second->getTime() !=  it->time) count++;
    //     }
    //     if(count != 0) { //cout<<count<<endl;
    //         stage =1;}
    // }
    // if(stage == 1) cout<<"Error assertTime"<<endl;
}

void runBai3() {
    TENodeAlgorithm* a = new TENodeAlgorithm();
    a->runRead();
    vector<int> initializations = getStartedNodes(allTENs);
    cout << "End of 3 nested-for loop" << endl;

    for (auto index : initializations)
    {
        spread(allTENs, 0, index, 1000);
    }
    sortGraph(allTENs);
    cout << "End of 4 nested-for loop" << endl;

    vector<pair<int, int>> redundants = filter(allTENs, 0);
    cout << "End of 5 nested-for loop" << endl;

    remove(redundants, allTENs);
    cout << "End of 6 nested-for loop" << endl;

    assertTime(allTENs, 0);
    cout << "End of 7 nested-for loop" << endl;

    int count_TEN = 0;
    for (vector<TimeExpandedNode *> it : allTENs)
    {
        for (TimeExpandedNode *TENode : it)
        {
            count_TEN++;
        }
    }
    cout << "Number of TENode in allTENS: " << count_TEN << endl;
}

// #endif