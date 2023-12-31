#ifndef _BAI4_HPP_
#define _BAI4_HPP_

#include "Bai3.hpp"
#include <assert.h>

using namespace std;

//// cau a
bool compare(std::tuple<int, int, double>a ,std::tuple<int, int, double> b)
{
    return (get<2>(a) < get<2>(b));
}


std::vector<std::tuple<int, int, double>> getChains(
    std::vector<std::vector<TimeExpandedNode*>> graph, 
    Point* origin
) {  
    std::vector<std::tuple<int, int, double>> a;
    int i,k;
    for(i = 0;i<graph.size();i++) {
        for(k= 0;k<graph[i].size();k++) {
            if(graph.at(i).at(k)->origin->equals(origin))
                a.push_back(make_tuple(i,k,graph[i][k]->time));
        }
    }
    sort(a.begin(),a.end(),compare);
    return a;
}

//// cau b
std::vector<std::pair<int, double>> createNewChains(
    std::vector<std::tuple<int, int, double>> oldChains,
    std::vector<std::vector<TimeExpandedNode*>> graph, 
    double H, double deltaT
) {
    int count = 0,steps = 0;
    int oldSize = oldChains.size();
    int fixedindex = get<1>(oldChains.at(0));
    std::vector<std::pair<int, double>> newChains;
    while (count < oldSize -1)
    {
        auto& prev = oldChains.at(count);
        auto& next = oldChains.at(count+1);
        steps = 0;
        if(get<2>(prev)<get<2>(next)-deltaT)
        {
            steps = floor((get<2>(next)-get<2>(prev))/deltaT);
            int size = newChains.size( );
            for(int i = 0; i < steps; i++){
                newChains.push_back(make_pair(i+size,get<2>(prev)+deltaT*(i+1)));
            }
        }
        count++;
    }
    auto& last = oldChains.at(oldSize-1);
    steps =floor(H-get<2>(last));
    int size = newChains.size()+oldChains.size();
    for(int i = 0; i < steps; i++){
		newChains.push_back(std::make_pair(i + size, get<2>(last) + deltaT*(1 + i)));
	}
	return newChains;

}

//// cau c 
std::vector<std::pair<int, int>>insert(std::vector<std::vector<TimeExpandedNode*>> &graph, std::vector<std::pair<int, double>> newChains, TimeExpandedNode* p)
{
    std::vector<std::pair<int, int>> result;
    for(auto elem : newChains)
    {
        if(isAvailable(graph,p->origin,elem.second)!=nullptr) continue;
        TimeExpandedNode * newNode = new TimeExpandedNode();
        newNode->setTENode(p->origin);
        newNode->time = elem.second;
        newNode->tgts=p->tgts;
        if(graph.size() <= elem.first)
        {
            std::vector<TimeExpandedNode*> v;
            v.push_back(newNode);
            graph.push_back(v);
            result.push_back(make_pair(graph.size() - 1, 0));
        } 
        else{
            std::vector<TimeExpandedNode *> currVector = graph.at(elem.first);
            if(currVector.size( ) == 0 || currVector.at(0)->time == elem.second ){
                graph.at(elem.first).push_back(newNode);
                result.push_back(make_pair(elem.first,graph.at(elem.first).size() -1));
            }
            else{
                std::vector<TimeExpandedNode*> v;
                v.push_back(newNode);
                graph.insert(graph.begin( ) + elem.first,1, v);
                result.push_back(std::make_pair(elem.first, 0));
            }

        }

    }
    return result;
}
bool checkInsertion(std::vector<std::vector<TimeExpandedNode*>> graph, 
	std::vector<std::pair<int, double>> newChains, 
		TimeExpandedNode* p){
	for(auto& elem : newChains){
		if(isAvailable(graph, p->origin, elem.second)!=nullptr){
			return true;
		}
	}
	return false;
}
bool checkResult(std::vector<std::vector<TimeExpandedNode*>> graph, 
	std::vector<std::pair<int, double>> newChains, 
    std::vector<std::pair<int, int>> newPositions,
    TimeExpandedNode* p
){
    int i = 0;
	for(auto elem : newChains){
		auto& pos = newPositions.at(i);
		if(!graph.at(pos.first).at(pos.second)->origin->equals(p->origin) || 
			graph.at(pos.first).at(pos.second)->time != elem.second
			){
			return false;
		}
		i++;
	}
	return true;
}

//// cau d
void assertNewOrder(std::vector<std::pair<int, int>> newOrder, std::vector<std::vector<TimeExpandedNode*>> graph)
{
    for(auto& e1 : newOrder){
	   for(auto& e2 : newOrder){
		if(e1.first < e2.first){			
           assert(graph.at(e1.first).at(e1.second)->time < graph.at(e2.first).at(e2.second)->time);
		}
	  }
    }
}

std::vector<std::pair<int, int>>  merge(std::vector<std::tuple<int, int, double>> oldChains,
						std::vector<std::pair<int, int>> newChains){
	int count = 0, i = 0, j = 0;
	std::vector<std::pair<int, int>> result ;
	int sizeOld = oldChains.size( ), sizeNew = newChains.size( );
	int size = oldChains.size( ) + newChains.size( );
	while(count < size && i < sizeOld && j < sizeNew){
		if( get<0>(oldChains.at(i)) < newChains.at(j).first){
			result.push_back(std::make_pair(i, get<1>(oldChains.at(i))));
			i++;
		}
		else if(get<0>(oldChains.at(i)) > newChains.at(j).first){
			result.push_back(std::make_pair(j, newChains.at(j).second));
			j++;
		}
		else{ //trường hợp này nghĩa là get<0>(oldChains.at(i)) == newChains.at(j).first
			//tuy vậy điều này gần như không được - và không thể xảy ra
			result.push_back(std::make_pair(i, get<1>(oldChains.at(i))));
			i++; j++;
		}
		count++;
	}
	return result;
}


//// cau e
class PausingShape : public Shape
{
public:
    PausingShape( ){ d = 0; name = "artificial"; time = 0;}
    PausingShape(double time) {
        this->time = time;
                name = "artificial"; 
    }
    PausingShape(Shape *s, std::string name){
        this->name = name;
        this->start = s->start;
        this->end = s->end;
        this->d = s->d;
        this->time = s->getTime();
    }
    //Phương thức getTime được ghi đè như sau
    double getTime( ){
        return this->time;
    }

};


//// cau f
std::vector<std::vector<TimeExpandedNode*>> connectChains(
std::vector<std::vector<TimeExpandedNode*>> graph,
    std::vector<std::pair<int, int>> newOrder
) {
    // printf("graph size before connect: %ld\n", graph.size());
    // printf("new order size: %ld\n", newOrder.size());
    for(int i = 0; i < newOrder.size() - 1; i++){
        int i1Prev = newOrder.at(i).first;
        int i2Prev = newOrder.at(i).second;
        int j1Next = newOrder.at(i+1).first;
        int j2Next = newOrder.at(i+1).second;
        // printf("%d %d %d %d\n", i1Prev, i2Prev, j1Next, j2Next);
        // printf("graph at j1Prev 's size: %ld\n", graph.at(i1Prev).size());
        // printf("graph at j1Next 's size: %ld\n\n", graph.at(j1Next).size());
        if(i1Prev >= graph.size() || j1Next >= graph.size()) {
            continue;
        }
        if(i2Prev >= graph.at(i1Prev).size() || j2Next >= graph.at(j1Next).size()) {
            continue;
        }
        TimeExpandedNode* prev = graph.at(i1Prev).at(i2Prev);
        TimeExpandedNode* next = graph.at(j1Next).at(j2Next);
        auto s0 = new PausingShape(next->time - prev->time);
        s0->start = prev->origin; 
        s0->end = next->origin;
        prev->tgts.push_back(std::make_pair(next, s0));
        next->srcs.push_back(std::make_pair(prev, s0));
    }
    // printf("graph size after connect: %ld\n", graph.size());
    return graph;

}

#endif