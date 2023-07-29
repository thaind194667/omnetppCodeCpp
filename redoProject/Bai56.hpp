#ifndef _BAI56_HPP_
#define _BAI56_HPP_

#include "Bai4.hpp"
#include <fstream>

using namespace std;

/////// Bai5 - a
TimeExpandedNode* isAvailable(
    std::vector<std::vector<TimeExpandedNode*>> graph, 
    Point* origin
) {
    TimeExpandedNode* res;// = (TimeExpandedNode*)malloc(sizeof(TimeExpandedNode));
    const double MINTIME = 999999;
    double minTime = MINTIME;
    // res->time = 9999;
    for(int i = 0; i < graph.size(); i++){
        vector<TimeExpandedNode*> v = graph.at(i);
        for(int j = 0; j < v.size(); j++){
            TimeExpandedNode* temp = v.at(j);
            if(temp->origin->equals(origin) && temp->time < minTime) {
                res = temp;
                res->origin = temp->origin;
                res->name = temp->name;
                res->layer = temp->layer;
                res->tgts = temp->tgts;
                res->srcs = temp->srcs;
                minTime = temp->time;
            }
        }
    }
    // cout << "time when done: " << res->time << "\n";
    if(minTime < MINTIME)
        return res;
    else return nullptr;
}

//// Bai5 - b
std::vector<std::vector<TimeExpandedNode*>> connectAllChains(
    std::vector<std::vector<TimeExpandedNode*>> graph,
    std::vector<Point*> points,
    double H, double v
) {
    printf("points 's size: %ld\n", points.size());
    for(int i = 0; i < points.size(); i++){
        int j = 0;
        Point* origin = points[i];
        // origin->printPoint();
        TimeExpandedNode* temp = isAvailable(graph, origin);
        // printf("%d ", j);j++;
        if(temp == nullptr) {
            // cout << "Not avaible\n";
            continue;
        }
        std::vector<std::tuple<int, int, double>> chains = getChains(graph, origin);
        // printf("%d %d\n", j, chains.size());j++;
        std::vector<std::pair<int, double>> newChains = createNewChains(chains, graph, H, v);
        // printf("%d %d\n", j, newChains.size());j++;
        std::vector<std::pair<int, int>> newPos = insert(graph, newChains, temp);
        // printf("%d %d\n", j, newPos.size());j++;
        assert(checkInsertion(graph, newChains, temp));
        assert(checkResult(graph, newChains, newPos, temp));
        for(auto& elem : newPos) {
            spread(graph, elem.first, elem.second, H);
        }
        // printf("%d\n", j);j++;
        std::vector<std::pair<int, int>> newOrder = merge(chains, newPos);
        // printf("%d %d\n", j, newOrder.size());j++;
        std::vector<std::vector<TimeExpandedNode*>>conchains = connectChains(graph, newOrder);
        // printf("%d\n", j);j++;
    }
    return graph;
}

/************************************************************************************************/
///// Bai6 - c
class Station : public TimeExpandedNode {
public:
    Station( ) : TimeExpandedNode( ) { }

    Station(TimeExpandedNode* temp, std::string name) : TimeExpandedNode( ){
        this->srcs = temp->srcs;
        this->name = name;
        this->tgts = temp->tgts;
        this->origin = temp->origin;
        this->time = temp->time;
        for(auto pr: this->srcs){
            for(auto pt: pr.first->tgts){
                if(pt.first->equals(temp)){ 
                    pt.first = this;
                }
            }
        }
        for(auto pr: this->tgts){
            for(auto pt: pr.first->srcs){
                if(pt.first->equals(temp)){ 
                    pt.first = this;
                }
            }
        }
    }

};
void replaceStation (std::vector<std::vector<TimeExpandedNode*>> graph, std::string stations) {
    
    for(int i = 0; i < graph.size(); i++){
        auto v = graph.at(i);
        for(int j = 0; j < v.size(); j++){
            auto temp = v.at(j);
            if(temp->endOfLane( )){
                auto name = temp->isStation(stations);
                if(!name.empty()){
                        graph.at(i).at(j) = new Station(temp, name);
                }    
            }
        }
    }
}

//// Bai6 - e1 ?
class ArtificialStation: public Station {
public:
    double bestTime;
    double amplitude;
    double earliness, tardiness;
    ArtificialStation(std::string name, double bestTime, double amplitude) : Station( ){
        this->name = name;
        this->time = bestTime;
        this->amplitude = amplitude;
        this->earliness = bestTime - amplitude;
        this->tardiness = bestTime + amplitude;
    }

};

///// Bai6 - d
//  std::map<std::string, std::vector<ArtificialStation*>> getTimeWindows(
//     std::string fileName, double H, std::string *stations
// ) {
//     ifstream inFile;
//     // open the file stream
//     inFile.open("./intinerary.txt");
//     std::map<std::string, std::vector<ArtificialStation*>> result; 
//     string line;
//     while(getline(inFile, line)){
//         // line = nội dung của một dòng
//         bestTime = getBestTime(line);
//         amplitude = getAmplitude(line);
//         name = getName(line);
//                 std::vector<ArtificialStation*> values ;
//         period = getPeriod(line);
//         for(int i = 0; i < H; i += period){
//             values.push_back(new ArtificialStation(name, bestTime + i, amplitude));
//         }
//                 if(values.size( ) > 0){
//                             result.insert({name, values});
//                             stations->append("$" + name + "$");
//                 }
//     }
//     return result;

// }


// void runBai5() {
//     double H = 6;
//     double v = 1;
//     //// Buoc1 (doc Allpart.txt)
//     TENodeAlgorithm* a = new TENodeAlgorithm();
//     a->runRead();
//     vector<Point*> points;
//     for(TimeExpandedNode* node: allTENs.at(0)) {
//         points.push_back(node->origin);
//     }
// 	cout<< "Done step 1\n";

//     //// Buoc2
//     a->assertCheck();
// 	cout<< "Done step 2\n";

//     //// Buoc3
// 	vector<int> initializations = getStartedNodes(allTENs);
// 	cout<< "Done step 3\n";

//     cout << initializations.size();
// 	for(auto index : initializations){
// 		// cout<< 
// 		spread(allTENs, index, H, v);
// 	}
// 	cout<< "Done step 4\n";

// 	//// Buoc5
// 	std::vector<std::pair<int, int>> redundants = filter(allTENs, v);
// 	remove(redundants, allTENs);
// 	cout<< "Done step 5\n";

// 	//// Buoc6
// 	a->assertCheck();
// 	cout<< "Done step 6\n";

// 	//// Buoc7
// 	assertTime(allTENs, 0);
// 	cout<< "Done step 7\n";

//     //// Buoc8
//     connectAllChains(allTENs, points, H, v);
//     cout << "Done step 8\n";

//     //// Buoc9
// 	redundants = filter(allTENs, v);
// 	remove(redundants, allTENs);
// 	cout<< "Done step 9\n";

// 	//// Buoc10
// 	a->assertCheck();
// 	cout<< "Done step 10\n";

// 	//// Buoc11
// 	assertTime(allTENs, 0);
// 	cout<< "Done step 11\n";

// 	//// Buoc12
// 	int count_TEN = 0;
//     for (vector<TimeExpandedNode *> it : allTENs)
//     {
//         for (TimeExpandedNode *TENode : it)
//         {
//             count_TEN++;
//         }
//     }
//     cout << "Number of TENode in allTENS: " << count_TEN << endl;
// 	cout<< "Done step 12\n";

// 	cout<< "Done Bai 5\n";
// }

// void runBai6() {

// }

#endif