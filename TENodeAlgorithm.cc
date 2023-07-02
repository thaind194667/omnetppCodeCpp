/*
 * TENodeAlgorithm.cc
 *
 *  Created on: Jun 18, 2023
 *      Author: duckt
 */

#include "TENodeAlgorithm.h"

//using namespace std;

// vector<Point *> P;
// vector<Shape *> S;
// vector<vector<TimeExpandedNode *>> allTENs;
// vector<TimeExpandedNode *> tempTENs;
//TENodeAlgorithm::TENodeAlgorithm() {
//    // TODO Auto-generated constructor stub
//
//}
//
//TENodeAlgorithm::~TENodeAlgorithm() {
//    // TODO Auto-generated destructor stub
//}

void convertToStruct() {

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

    int counter = 0;
    for (TimeExpandedNode *nA : allTENs.at(0))
    {
        for (TimeExpandedNode *nB : allTENs.at(0))
        {
            vector<pair<int, int>> nA_to_nB;
            vector<pair<int, int>> nB_to_nA;
            if (!nA->equals(nB))
            {
                counter++;
                if (counter % (10 * 1000 * 1000) == 0)                 {
                    cout << counter << endl;
                }
                nB_to_nA = getConcidences(&(nA->srcs), &(nB->tgts));
                nA_to_nB = getConcidences(&(nB->srcs), &(nA->tgts));
                nA->insertSourcesAndTargets(nB, nB_to_nA, nA_to_nB);
            }
        }
    }
    //cout<<"End of second nested-for loop"<<endl;


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
        n->printTENode(0);
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
    //cout<<"End of fifth nested-for loop"<<endl;
    cout<<"Done converting."<<endl;
}

void addToStruct(string str) {
    if (strSmatch(str, "[_A-Z0-9]+(?= )") == "")
        return;
    Shape *shape = new Shape();

    shape->setName(strSmatch(str, "[_A-Z0-9]+(?= )"));

    string point_1 = strSmatch(str, "[_][-0-9.,]+(?=_)");
    string point_2 = strSmatch(str, "[^_A-Z ][-0-9.,]+(?=$)");

    string s1 = "[^_ ][-0-9.,]+(?=,)";
    string s2 = "[^_,][-0-9.]+(?=$)";
    string s3 = "[^_,][-0-9.]+(?=$)";

    Point *firstPoint = new Point();
    firstPoint->setPoint(stof(strSmatch(point_1, s1)),
                         stof(strSmatch(point_1, s2)));
    Point *lastPoint = new Point();
    lastPoint->setPoint(stof(strSmatch(point_2, s1)),
                        stof(strSmatch(point_2, s3)));
    if(firstPoint->equals(lastPoint)){
        //cout<<"DEBUG "<<__FILE__<<":"<<__LINE__<<endl;
        return;
    }
    shape->setPoint(firstPoint, lastPoint);

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

void TENodeAlgorithm::runRead() {

    vector<EdgeForRead> edges;
    vector<Junc> juncs;
    vector<EdgeForRead> starts;
    vector<EdgeForRead> ends;
    vector<string> dead_end;
    const char *filename = "vd013.net.xml";
    outFile.open("AllParts.txt");
    pugi::xml_document doc;
    if (!doc.load_file(filename))
    {
        cout << "Can't read" << endl;
        return;
    }
    cout<< "Reading ... \n";
    pugi::xml_node panels = doc.child("net");
    pugi::xml_attribute attr;
    for (pugi::xml_node panel = panels.child("junction"); panel; panel = panel.next_sibling("junction"))
    {
        attr = panel.first_attribute();
        if (strcmp(attr.next_attribute().name(), "type") == 0 && strcmp(attr.next_attribute().value(), "dead_end") == 0)
        {
            dead_end.push_back(attr.value());
        }
    }
    for (pugi::xml_node panel = panels.child("edge"); panel; panel = panel.next_sibling("edge"))
    {
        attr = panel.first_attribute();
        if (strcmp(attr.next_attribute().name(), "from") == 0 && strcmp(attr.next_attribute().next_attribute().name(), "to") == 0 && strcmp(attr.next_attribute().value(), attr.next_attribute().next_attribute().value()) != 0)
        {
            int check1 = 0;
            int check2 = 0;
            for (string s : dead_end)
            {
                if (strcmp(attr.next_attribute().value(), s.c_str()) == 0)
                {
                    check1 = 1;
                }
                if (strcmp(attr.next_attribute().next_attribute().value(), s.c_str()) == 0)
                {
                    check2 = 1;
                }
                if (check1 == 1 && check2 == 1)
                {
                    break;
                }
            }
            EdgeForRead e;
            get<0>(e) = attr.value();
            string shape;
            double length;
            int i = 0;
            for (pugi::xml_node child = panel.first_child(); child; child = child.next_sibling())
            {
                attr = child.first_attribute().next_attribute().next_attribute();
                if (strcmp(attr.name(), "disallow") == 0 && strcmp(attr.value(), "pedestrian") == 0)
                {
                    shape = attr.next_attribute().next_attribute().next_attribute().value();
                    length = atof(attr.next_attribute().next_attribute().value());
                    i++;
                }
                if (i > 1)
                    break;
            }
            if (i == 1)
            {
                vector<double> nums;
                stringstream ss(shape);
                double num;
                while (ss >> num)
                {
                    nums.push_back(num);
                    if (ss.peek() == ',' || ss.peek() == ' ')
                    {
                        ss.ignore();
                    }
                }
                ShapeElement s = ShapeElement({PointElement({nums[0], nums[1]}), PointElement({nums[2], nums[3]}), length});
                get<1>(e) = s;
                if (check1 == 1)
                {
                    starts.push_back(e);
                }
                if (check2 == 1)
                {
                    ends.push_back(e);
                }

                edges.push_back(e);
            }
        }
        else if (strcmp(attr.next_attribute().name(), "function") == 0 && strcmp(attr.next_attribute().value(), "internal") == 0)
        {
            Junc j;
            get<0>(j) = attr.value();
            get<0>(j).erase(0, 1);
            string shape;
            double length;
            int i = 0;
            for (pugi::xml_node child = panel.first_child(); child; child = child.next_sibling())
            {
                attr = child.first_attribute().next_attribute().next_attribute();
                if (strcmp(attr.name(), "disallow") == 0 && strcmp(attr.value(), "pedestrian") == 0)
                {
                    shape = attr.next_attribute().next_attribute().next_attribute().value();
                    length = atof(attr.next_attribute().next_attribute().value());
                    i++;
                }
                if (i > 1)
                    break;
            }
            if (i == 1)
            {
                vector<double> nums;
                stringstream ss(shape);
                double num;
                while (ss >> num)
                {
                    nums.push_back(num);
                    if (ss.peek() == ',' || ss.peek() == ' ')
                    {
                        ss.ignore();
                    }
                }

                std::vector<PointElement> points;

                for (int i = 0; i < nums.size() - 1; i += 2)
                {
                    points.push_back(PointElement({nums[i], nums[i + 1]}));
                }
                get<1>(j) = points;
                juncs.push_back(j);
            }
        }
    }


    allTENs.push_back(tempTENs);

    for (EdgeForRead e : edges)
    {
        double x = 1.56;
        vector<std::pair<PointElement, PointElement>> list = listPointElement(get<0>(get<1>(e)), get<1>(get<1>(e)), x);
        int i = 0;
        for (std::pair<PointElement, PointElement> point : list)
        {
            std::stringstream printString;
            printString << get<0>(e) << " " << i << "_" << fixed << setprecision(2)
                    << point.first.first << "," << point.first.second << "_"
                    << point.second.first << "," << point.second.second;
            outFile << get<0>(e) << " " << i << "_" << fixed << setprecision(2)
                    << point.first.first << "," << point.first.second << "_"
                    << point.second.first << "," << point.second.second <<"\n";
//            cout << printString.str() <<"\n";
            addToStruct(printString.str());
            i++;
        }
    }

    for (Junc j : juncs)
    {
        double x = 1.56;
        vector<PointElement> points = get<1>(j);
        int n = 0;
        for (int i = 0; i < points.size() - 1; i++)
        {
            vector<std::pair<PointElement, PointElement>> list = listPointElement(points[i], points[i + 1], x);
            for (std::pair<PointElement, PointElement> point : list)
            {
                std::stringstream printString;
                printString << get<0>(j) << " " << n << "_" << fixed << setprecision(2)
                        << point.first.first << "," << point.first.second << "_"
                        << point.second.first << "," << point.second.second;
                outFile << get<0>(j) << " " << n << "_" << fixed << setprecision(2)
                        << point.first.first << "," << point.first.second << "_"
                        << point.second.first << "," << point.second.second << "\n";
//                cout << printString.str() <<"\n";
                addToStruct(printString.str());
                n++;
            }
        }
    }

    outFile.close();
    cout<< "Done reading\n";
    convertToStruct();
    return;
}

