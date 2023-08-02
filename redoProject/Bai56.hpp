#ifndef _BAI56_HPP_
#define _BAI56_HPP_

#include "Bai4.hpp"
#include <fstream>

using namespace std;

////// Bai6-e
template<typename Base, typename T>
inline bool instanceof(const T *ptr) { 
    return dynamic_cast<const Base*>(ptr) != nullptr;
}


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
                // res->layer = temp->layer;
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

///// Bai6-f
class ArtificialShape : public PausingShape 
{
public:
//Phương thức getTime được ghi đè như sau
    ArtificialShape(double time) {this->time = time;}
    double getTime( ){
        return this->time;
    }
};

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

    void createConnection(TimeExpandedNode* node){   /// add in bai6-g
        ArtificialShape* aShape;
        if(instanceof<Station>(node) && !instanceof<ArtificialStation>(node)){
            if(node->name == this->name){
                //để gọi được hàm max cần dùng thư viện cmath
                double penaltyT = max(this->earliness - node->time, 0.0);
                penaltyT = max(penaltyT, node->time - this->tardiness); 
                aShape = new ArtificialShape(penaltyT);
                node->tgts.push_back(std::make_pair(this, aShape));
                this->srcs.push_back(std::make_pair(node, aShape));
            }
        }
    }


};

///// Bai6 - d
vector<string> splitString(string S, char delimiter) {
    string tmp = "";
    vector<string> result;
    for(int i = 0; i < S.size(); i++) {
        if(S[i] == delimiter) {
            if(tmp != "") {
                result.push_back(tmp);
                tmp = "";
            }
        }
        else tmp += S[i];
    }
    if(tmp != "") result.push_back(tmp);
    return result;
}

std::map<std::string, std::vector<ArtificialStation*>> getTimeWindows(
    std::string fileName, double H, std::string *stations
) {
    ifstream inFile;
    string now = "";
    // open the file stream
    inFile.open(fileName);
    std::map<std::string, std::vector<ArtificialStation*>> result; 
    string line;
    while(getline(inFile, line)){
        // line = nội dung của một dòng
        vector<string> partsOfLine = splitString(line, ' ');
        printf("parts of line :  ");
        for(int i = 0; i < partsOfLine.size(); i++) {
            cout<< partsOfLine[i]<< " ";
        }cout<< "\n";
        string name = partsOfLine[0];
        int period = std::stoi(partsOfLine[5]);
        double bestTime = std::stod(partsOfLine[6]);
        double amplitude = std::stod(partsOfLine[7]);
        printf("name: %s, period: %d, bestTime: %lf, amplitude: %lf\n", name.c_str(), period, bestTime, amplitude);
        std::vector<ArtificialStation*> values;
        for(int i = 0; i < H; i += period){
            values.push_back(new ArtificialStation(name, bestTime + i, amplitude));
        }
        // cout << values.size() << "\n";
        if(values.size() > 0){
            result.insert({name, values});
            now += "$" + name + "$";
        }
    }
    *stations = now;
    cout << "done read file intinerary.txt\n";
    return result;

}


#endif