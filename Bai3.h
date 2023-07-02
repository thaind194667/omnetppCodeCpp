#ifndef _BAI3_H_
#define _BAI3_H_

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
                    assert(elem.first->time + elem.second->getTime() == TENode->time);
                }
            }
        }
    }
}

#endif