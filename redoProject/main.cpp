
#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <regex>
#include <assert.h>
// #include "Point.hpp"
// #include "Shape.hpp"
// #include "TimeExpandedNode.hpp"
#include "Bai3.hpp"
#include "Bai4.hpp"
#include "Bai56.hpp"
#include "readxml.hpp"

using namespace std;
std::vector<part> allpart;

vector<Point *> P;
vector<Shape *> S;
vector<vector<TimeExpandedNode *>> allTENs;
vector<TimeExpandedNode *> tempTENs;

// Hàm getCoincedence như sau
vector<pair<int, int>> getConcidences(vector<pair<TimeExpandedNode *, Shape *>> *srcs,
                                      vector<pair<TimeExpandedNode *, Shape *>> *tgts)
{
    vector<pair<int, int>> result;
    for (int i = 0; i < srcs->size(); i++)
    {
        for (int j = 0; j < tgts->size(); j++)
        {
            if (srcs->at(i).second->equals(tgts->at(j).second))
            {
                result.push_back(make_pair(i, j));
            }
        }
    }
    return result;
}

// Hàm dùng Regular Expression để bắt chuỗi
string strSmatch(string str, string W_regex)
{
    regex word_regex(W_regex);
    smatch match;
    regex_search(str, match, word_regex);
    return match.str();
}

// Hàm kiểm tra xem điểm point có trong vector P hay không
bool checkPoint(Point *point, vector<Point *> P)
{
    if (P.empty())
        return false;
    for (Point *p : P)
    {
        if (point->equals(p))
            return true;
    }
    return false;
}

// Hàm kiểm tra xem shape có trong vector S hay không
bool checkShape(Shape *shape, vector<Shape *> S)
{
    if (S.empty())
        return false;
    for (Shape *s : S)
    {
        if (shape->equals(s))
            return true;
    }
    return false;
}

void assertCheck() {
    for (Point *p : P)
    {
        int i = 0;
        for (TimeExpandedNode *n : allTENs.at(0))
        {
            if (p->equals(n->origin))
                i++;
            assert(n->time == 0);
            // Tất cả các trường time của tất cả các TENode đều bằng 0
        }
        assert(i = 1);
        // mọi point chỉ xuất hiện một lần trong tất cả các trường origin của tất cả các TENode
    }
    //cout<<"End of third nested-for loop"<<endl;

    for (Shape *s : S)
    {
        int i = 0;
        for (TimeExpandedNode *n : allTENs.at(0))
        {
            for (pair<TimeExpandedNode *, Shape *> x : n->srcs)
            {
                if (s->equals(x.second))
                    i++;

                assert(x.first != nullptr && x.second != nullptr);
                // Trong các vector srcs và tgts của các TENode, nếu vector nào khác rỗng thì tất cả các trường của tất
                // cả các phần tử trong vector đều khác null.
            }

            for (pair<TimeExpandedNode *, Shape *> x : n->tgts)
            {
                if (s->equals(x.second))
                    i++;
		if(x.first == nullptr || x.second == nullptr){
			cout<<"DEBUYG"<<endl;
		}
                assert(x.first != nullptr && x.second != nullptr);
                // Trong các vector srcs và tgts của các TENode, nếu vector nào khác rỗng thì tất cả các trường của tất
                // cả các phần tử trong vector đều khác null.
            }
        }
        assert(i = 2);
        // Mọi shape chỉ xuất hiện hai lần trong tất cả các trường srcs và tgts của tất cả các TENode.
    }
    //cout<<"End of forth nested-for loop"<<endl;

    for (TimeExpandedNode *n : allTENs.at(0))
    {
        int sum = n->srcs.size() + n->tgts.size();
        int i = 0;
        for (TimeExpandedNode *m : allTENs.at(0))
        {
            for (pair<TimeExpandedNode *, Shape *> x : m->srcs)
            {
                if (n == x.first)
                    i++;
            }

            for (pair<TimeExpandedNode *, Shape *> x : m->tgts)
            {
                if (n == x.first)
                    i++;
            }
        }
        assert(sum == i);
        // Nếu phần tử n có tổng độ dài của srcs và tgts của nó là X. Thì số lần xuất hiện của n trong các vector
        // srcs và tgts của các TENodes khác cũng sẽ bằng đúng X
    }
}

void insertToStruct() {
    allTENs.push_back(tempTENs);

    //ifstream input("./AllParts.txt");
    //input.is_open();
    int counter = 0;
    makeallpart();
    for(auto& a : allpart)
    {
        Shape *shape = new Shape();
        //shape->setName(a.name);
        Point *firstPoint = new Point();
        firstPoint->setPoint(a.Point1.first,a.Point1.second);
        Point *lastPoint = new Point();
        lastPoint->setPoint(a.Point2.first,a.Point2.second);
        if(!firstPoint->equals(lastPoint)){
        shape->setPoint(firstPoint, lastPoint);
        shape->setName(a.name);
         //cout<<shape->name<<endl;
            if (!checkPoint(firstPoint, P))
            {
                P.push_back(firstPoint);
                TimeExpandedNode *n1 = new TimeExpandedNode();
                n1->setTENode(firstPoint);
                allTENs.at(0).push_back(n1);
            }

            if (!checkPoint(lastPoint, P))
            {
                P.push_back(lastPoint);
                TimeExpandedNode *n2 = new TimeExpandedNode();
                n2->setTENode(lastPoint);
                allTENs.at(0).push_back(n2);
            }

            if (!checkShape(shape, S))
            {
                S.push_back(shape);
            }
        }
        //
    }

    for (TimeExpandedNode *n : allTENs.at(0))
    {
        for (Shape *s : S)
        {
            if (s->start->equals(n->origin))
            {
                n->insertTarget(s);
            }
            else if (s->end->equals(n->origin))
            {
                n->insertSource(s);
            }
        }
    }
    //cout<<"End of first nested-for loops"<<endl;
    //cout<<"allTENs.at(0).size( ) = "<<allTENs.at(0).size()<<endl;

    for (TimeExpandedNode *nA : allTENs.at(0))
    {
        for (TimeExpandedNode *nB : allTENs.at(0))
        {
            vector<pair<int, int>> nA_to_nB;
            vector<pair<int, int>> nB_to_nA;
            if (!nA->equals(nB))
            {
                nB_to_nA = getConcidences(&(nA->srcs), &(nB->tgts));
                nA_to_nB = getConcidences(&(nB->srcs), &(nA->tgts));
                nA->insertSourcesAndTargets(nB, nB_to_nA, nA_to_nB);
            }
        }
    }
    for (TimeExpandedNode *n : allTENs.at(0)) {
        if(n->time != 0) cout << "Has none zero\n";
    }
    //cout<<"End of second nested-for loop"<<endl;
    assertCheck();
}

void runBai3() {
    cout << "Before bai3: \n";
    for(auto& it : allTENs){
        cout<<it.size()<<endl;
    }
    cout<< "Done step 1\n";

    //// Buoc2
	assertCheck();
	cout<< "Done step 2\n";

    //cout<<"DONE."<<endl;
    //// Buoc3
    vector<int>  initializations = getStartedNodes(allTENs);
	cout << "initializations.size() : " << initializations.size()<< "\n";
	cout<< "Done step 3\n";

    //// Buoc4 
    for(auto& index : initializations){
		spread(allTENs, 0, index, 6);
    }
	cout<< "Done step 4\n";

    //// Buoc5
    std::vector<std::pair<int, int>> redundants = filter(allTENs);
    cout << "redundants.size():" << redundants.size() << endl;
    remove(redundants,allTENs);
    cout<< "Done step 5\n";

    //// Buoc6
	// assertCheck();
	cout<< "Done step 6\n";

    //// Buoc7
    for(auto& it: allTENs)
        assertTime(it,1);
	cout<< "Done step 7\n";

    //// Buoc 8
    cout << "TENODES's size after run bai3: \n";
    for(auto& it : allTENs) {
        cout<<it.size()<<endl;
    }
    cout<< "Done step 8\n";

	cout<< "Done Bai 4\n";
    exit(0);

}

void runBai4() {
    cout << "Before bai4: \n";
    for(auto& it : allTENs){
        cout<<it.size()<<endl;
    }
    cout<< "Done step 1\n";
    
    //// Buoc2
	assertCheck();
	cout<< "Done step 2\n";

    //cout<<"DONE."<<endl;
    //// Buoc3
    vector<int>  initializations = getStartedNodes(allTENs);
	cout << initializations.size()<<"\n";
	cout<< "Done step 3\n";

    //// Buoc4 
    for(auto& index : initializations){
		spread(allTENs, 0, index, 6);
    }
	cout<< "Done step 4\n";
    
    //// Buoc5
    std::vector<std::pair<int, int>> redundants = filter(allTENs);
    cout<<redundants.size()<<endl;
    remove(redundants,allTENs);
    cout<< "Done step 5\n";

	//// Buoc6
	// assertCheck();
	cout<< "Done step 6\n";
    
    //// Buoc7
    for(auto& it: allTENs)
        assertTime(it,1);
	cout<< "Done step 7\n";

    //// Buoc8
    initializations = getStartedNodes(allTENs);
    for(auto index : initializations)
    {
        auto& temp = allTENs.at(0).at(index);
        std::vector<std::tuple<int, int, double>> chains = getChains(allTENs, temp->origin);
        std::vector<std::pair<int, double>> newChains = createNewChains(chains,allTENs, 6, 1);
        std::vector<std::pair<int, int>>newPos = insert(allTENs, newChains, temp);
        assert(checkInsertion(allTENs, newChains, temp)); 
	    assert(checkResult(allTENs, newChains, newPos, temp));
        for(auto& elem : newPos){
            spread(allTENs, elem.first, elem.second, 6);
	    }
        std::vector<std::pair<int, int>> newOrder = merge(chains, newPos);
        std::vector<std::vector<TimeExpandedNode*>>conchains =  connectChains(allTENs, newOrder);
    }
    cout<< "Done step 8\n";

    //// Buoc9
    redundants = filter(allTENs);
    remove(redundants,allTENs);
    cout<< "Done step 9\n";

    //// Buoc10
	// assertCheck();
	cout<< "Done step 10\n";

	//// Buoc11
    for(auto& it: allTENs)
        assertTime(it,1); 
	cout<< "Done step 11\n";

    cout << "TENODES's size after run bai4: \n";
    for(auto& it : allTENs) {
        cout<<it.size()<<endl;
    }
    cout<< "Done step 12\n";

	cout<< "Done Bai 4\n";
    exit(0);
}

void runBai5() {
    double H = 6;
    double v = 1;
    //// Buoc1 (doc Allpart.txt)
    cout << "Before bai5: \n";
    for(auto& it : allTENs){
        cout<<it.size()<<endl;
    }
    // vector<Point*> points;
    // for(TimeExpandedNode* node: allTENs.at(0)) {
    //     points.push_back(node->origin);
    // }
	cout<< "Done step 1\n";

    //// Buoc2
    assertCheck();
	cout<< "Done step 2\n";

    //// Buoc3
	vector<int> initializations = getStartedNodes(allTENs);
	cout << "initializations.size() : " << initializations.size()<< "\n";
	cout<< "Done step 3\n";

    // cout << initializations.size();
    //// Buoc4
	for(auto index : initializations){
		// cout<< 
		spread(allTENs, 0, index, H);
	}
	cout<< "Done step 4\n";

	//// Buoc5
	std::vector<std::pair<int, int>> redundants = filter(allTENs);
    cout << "redundants.size(): " << redundants.size() << endl;
	remove(redundants, allTENs);
	cout<< "Done step 5\n";

	//// Buoc6
	// assertCheck();
	cout<< "Done step 6\n";

	//// Buoc7
	for(auto& it: allTENs)
        assertTime(it,1);
	cout<< "Done step 7\n";

    //// Buoc8
    connectAllChains(allTENs, P, H, v);
    cout << "Done step 8\n";

    //// Buoc9
	redundants = filter(allTENs);
    cout << "redundants.size(): " << redundants.size() << endl;
	remove(redundants, allTENs);
	cout<< "Done step 9\n";

	//// Buoc10
	// assertCheck();
	cout<< "Done step 10\n";

	//// Buoc11
	for(auto& it: allTENs)
        assertTime(it,1);
	cout<< "Done step 11\n";

	//// Buoc12
	cout << "TENODES's size after run bai5: \n";
    for(auto& it : allTENs) {
        cout<<it.size()<<endl;
    }
	cout<< "Done step 12\n";

	cout<< "Done Bai 5\n";
}

void runBai6() {
    double H = 5;
    double v = 1;
    //// Buoc1 (doc Allpart.txt)
    cout << "Before bai6: \n";
    for(auto& it : allTENs){
        cout<<it.size()<<endl;
    }
    // vector<Point*> points;
    // for(TimeExpandedNode* node: allTENs.at(0)) {
    //     points.push_back(node->origin);
    // }
	cout<< "Done step 1\n";

    //// Buoc2
    assertCheck();
	cout<< "Done step 2\n";

    //// Buoc3
	vector<int> initializations = getStartedNodes(allTENs);
	cout << "initializations.size() : " << initializations.size()<< "\n";
	cout<< "Done step 3\n";

    // cout << initializations.size();
    //// Buoc4
	for(auto index : initializations){
		// cout<< 
		spread(allTENs, 0, index, H);
	}
	cout<< "Done step 4\n";

	//// Buoc5
	std::vector<std::pair<int, int>> redundants = filter(allTENs);
    cout << "redundants.size(): " << redundants.size() << endl;
	remove(redundants, allTENs);
	cout<< "Done step 5\n";

	//// Buoc6
	// assertCheck();
	cout<< "Done step 6\n";

	//// Buoc7
	for(auto& it: allTENs)
        assertTime(it,1);
	cout<< "Done step 7\n";

    //// Buoc8
    connectAllChains(allTENs, P, H, v);
    cout << "Done step 8\n";

    //// Buoc9
    string* stations = new string();
    std::map<std::string, std::vector<ArtificialStation*>>
        mapArtificialStations = getTimeWindows("./intinerary.txt", H, stations);
    cout << "mapArtificialStations.size : " << mapArtificialStations.size() << "\n";
    cout << "stations string: " << *stations << "\n";
    cout << "Done step 9\n";

    //// Buoc10
    for(int i = 0; i < allTENs.size() ; i++){
        auto v = allTENs.at(i);
        for(int j = 0; j < v.size( ); j++){
            auto temp = v.at(j);
            if(instanceof<Station>(temp)){
                std::string name = temp->name;
                auto foundit = mapArtificialStations.find(name);
                if(foundit != mapArtificialStations.end() ){
                    auto allArStations = mapArtificialStations[name];
                    for(auto elem : allArStations){
                        elem->createConnection(temp);
                    }
                }
            }
        }
    }
    cout<< "Done step 10\n";

    //// Buoc11
    for (const auto& el : mapArtificialStations){ 
    auto v = el.second;
        for(auto elem : v){
            insert(allTENs, elem);
        }
    }

    cout<< "Done step 11\n";


    //// Buoc12
	redundants = filter(allTENs);
    cout << "redundants.size(): " << redundants.size() << endl;
	remove(redundants, allTENs);
	cout<< "Done step 12\n";

	//// Buoc13
	// assertCheck();
	cout<< "Done step 13\n";

	//// Buoc14
	for(auto& it: allTENs)
        assertTime(it,1);
	cout<< "Done step 14\n";

	//// Buoc15
	cout << "TENODES's size after run bai6: \n";
    for(auto& it : allTENs) {
        cout<<it.size()<<endl;
    }
	cout<< "Done step 15\n";

	cout<< "Done Bai 6\n";
}

int main(int argc, char *argv[])
{
    insertToStruct();
    if(strcmp(argv[1], "3") == 0) {
        cout << "Running ex3\n";
        runBai3();
    }
    else if(strcmp(argv[1], "4") == 0) {
        cout << "Running ex4\n";
        runBai4();
    }
    else if(strcmp(argv[1], "5") == 0) {
        cout << "Running ex5\n";
        runBai5();
    }
    else if(strcmp(argv[1], "6") == 0) {
        cout << "Running ex6\n";
        runBai6();
    }
}