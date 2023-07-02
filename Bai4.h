#ifndef _BAI4_H_
#define _BAI4_H_

#include "Bai3.h"
#include <assert.h>

using namespace std;

//// cau a
std::vector<std::tuple<int, int, double>> getChains(
    std::vector<std::vector<TimeExpandedNode*>> graph, 
    Point* origin ) {

	std::vector<std::tuple<int, int, double>> res;

	for(int i = 0; i < graph.size(); i++) {
		vector<TimeExpandedNode*> v = graph.at(i);

		for(int j = 0; j < v.size(); j++) {
			TimeExpandedNode* curr = v.at(j);
			if(curr->origin->equals(origin)) {
				res.push_back(std::make_tuple(i, j, curr->time));
			}
		}
	}
	return res;
}

//// cau b
std::vector<std::pair<int, double>> createNewChains(
    std::vector<std::tuple<int, int, double>> oldChains,
    std::vector<std::vector<TimeExpandedNode*>> graph, 
    double H, double deltaT ) {

    int count = 0, steps = 0;
	int oldSize = oldChains.size( );
	int fixedIndex = std::get<1>(oldChains.at(0));
    int size;
	std::vector<std::pair<int, double>> newChains;

	while(count < oldSize - 1){
		auto prev = oldChains.at(count);
		auto next = oldChains.at(count + 1);
		steps = 0;
		if( std::get<2>(prev) < std::get<2>(next) - deltaT){
			steps = floor((std::get<2>(next)  - std::get<2>(prev))/deltaT);
			size = newChains.size( );
			for(int i = 0; i < steps; i++){
				newChains.push_back(std::make_pair(i + size, 
									std::get<2>(prev) + deltaT*(i+1)));
			}
		}
		count++;
	}
	auto last = oldChains.at(oldSize - 1);
	steps = floor((H - std::get<2>(last ))/deltaT);
	size = newChains.size( ) + oldChains.size( );
	for(int i = 0; i < steps; i++){
		newChains.push_back(std::make_pair(i + size, std::get<2>(last) + deltaT*(1 + i)));
	}
	return newChains;

}


//// cau c 
std::vector<std::pair<int, int>> insert(
    std::vector<std::vector<TimeExpandedNode*>> graph, 
    std::vector<std::pair<int, double>> newChains, 
    TimeExpandedNode* p
) {

    std::vector<std::pair<int, int>> result;
	for(auto elem : newChains){
		if(isAvailable(graph, p->origin, elem.second)){
			continue;
		}
		TimeExpandedNode* newNode = new TimeExpandedNode(p->origin,
								elem.second,
								p->tgts);

		if(graph.size( ) <= elem.first){
			std::vector<TimeExpandedNode*> v{newNode};
			graph.push_back(v);
			result.push_back(std::make_pair(graph.size( ) - 1, 0));
		}
		else{
			std::vector<TimeExpandedNode*> currVector = graph.at(elem.first);
			if(currVector.size( ) == 0){
				graph.at(elem.first).push_back(newNode);
				result.push_back(std::make_pair(elem.first, 
						  		       graph.at(elem.first).size( ) -1));
			}
			else if(currVector.at(0)->time == elem.second){
				graph.at(elem.first).push_back(newNode);
				result.push_back(std::make_pair(elem.first, 
						  		       graph.at(elem.first).size( ) -1));
			}
			else{
				std::vector<TimeExpandedNode*> v{newNode};
				graph.insert(graph.begin( ) + elem.first,
						1, v);
				result.push_back(std::make_pair(elem.first, 0));
			}
		}
	}
	return result;

}


bool checkInsertion(std::vector<std::vector<TimeExpandedNode*>> graph, 
	std::vector<std::pair<int, double>> newChains, 
	TimeExpandedNode* p){

	for(auto elem : newChains){
		if(!isAvailable(graph, p->origin, elem.second)){
			return false;
		}
	}
	return true;
}

bool checkResult(std::vector<std::vector<TimeExpandedNode*>> graph, 
	std::vector<std::pair<int, double>> newChains, 
		std::vector<std::pair<int, int>> newPositions,
		TimeExpandedNode* p){
    int i = 0;
	for(auto elem : newChains){
		std::pair<int, int> pos = newPositions.at(i);
        TimeExpandedNode* a = graph.at(pos.first).at(pos.second);
		if( !a->origin->equals(p->origin) || 
			graph.at(pos.first).at(pos.second)->time != elem.second
			){
			return false;
		}
		i++;
	}
	return true;
}


//// cau d
void assertNewOrder( std::vector<std::pair<int, int>> newOrder, std::vector<std::vector<TimeExpandedNode*>> graph ) {
    for(auto e1 : newOrder){
        for(auto e2 : newOrder){
            if(e1.first < e2.first){			
                assert( graph.at(e1.first).at(e1.second)->time < graph.at(e2.first).at(e2.second)->time);
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
        this->time = s->getTime( );
    }
    //Phương thức getTime được ghi đè như sau
    double getTime( ){
        return this->time;
    }

};


//// cau f
std::vector<std::vector<TimeExpandedNode*>> connectChains(
    std::vector<std::vector<TimeExpandedNode*>> graph,
    std::vector<std::pair<int, int>> newOrder ) {

    for(int i = 0; i < newOrder.size() - 1; i++){
        int i1Prev = newOrder.at(i).first;
        int i2Prev = newOrder.at(i).second;
        int j1Next = newOrder.at(i+1).first;
        int j2Next = newOrder.at(i+1).second;
        TimeExpandedNode* prev = graph.at(i1Prev).at(i2Prev);
        TimeExpandedNode* next = graph.at(j1Next).at(j2Next);
        auto s0 = new PausingShape(next->time - prev->time);
        s0->start = prev->origin; 
        s0->end = next->origin;
        prev->tgts.push_back(std::make_pair(next, s0));
        next->srcs.push_back(std::make_pair(prev, s0));
    }
    return graph;

}


//// cau g
void runBai4(int H, double v){

	//// Buoc1 (doc Allpart.txt)
    TENodeAlgorithm* a = new TENodeAlgorithm();
    a->runRead();
	cout<< "Done step 1\n";

	//// Buoc2
	cout<< "Done step 2\n";

	//// Buoc3
	vector<int> initializations = getStartedNodes(allTENs);
	cout<< "Done step 3\n";

	//// Buoc4 
	for(auto index : initializations){
		spread(allTENs, index, H, v);
	}
	cout<< "Done step 4\n";

	//// Buoc5
	std::vector<std::pair<int, int>> redundants = filter(allTENs, v);
	remove(redundants, allTENs);
	cout<< "Done step 5\n";

	//// Buoc6
	cout<< "Done step 6\n";

	//// Buoc7
	assertTime(allTENs, 0);
	cout<< "Done step 7\n";


	//// Buoc8
	for(auto index : initializations){
		TimeExpandedNode* temp = allTENs.at(0).at(index);
		std::vector<std::tuple<int, int, double>> chains = getChains(allTENs, temp->origin);
		std::vector<std::pair<int, double>> newChains = createNewChains(chains, allTENs, H, v);
		std::vector<std::pair<int, int>> newPos = insert(allTENs, newChains, temp);
		assert(checkInsertion(allTENs, newChains, temp)); 
		assert(checkResult(allTENs, newChains, newPos, temp));
		for(auto elem : newPos){
			spread(allTENs, elem.first, elem.second, v);
		}
		std::vector<std::pair<int, int>> newOrder = merge(chains, newPos);
		connectChains(allTENs, newOrder);
	}
	cout<< "Done step 8\n";

	//// Buoc9
	redundants = filter(allTENs, v);
	remove(redundants, allTENs);
	cout<< "Done step 9\n";

	//// Buoc10
	cout<< "Done step 10\n";

	//// Buoc11
	assertTime(allTENs, 0);
	cout<< "Done step 11\n";

	//// Buoc12
	int count_TEN = 0;
    for (vector<TimeExpandedNode *> it : allTENs)
    {
        for (TimeExpandedNode *TENode : it)
        {
            count_TEN++;
        }
    }
    cout << "Number of TENode in allTENS: " << count_TEN << endl;
	cout<< "Done step 12\n";

	cout<< "Done Bai 4\n";


}



#endif