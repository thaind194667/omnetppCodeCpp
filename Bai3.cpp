#ifndef _BAI3_H_
#define _BAI3_H_

#include "Bai3.h"
// #include "element.h"

using namespace std;

//// cau i
int main() {
    TENodeAlgorithm* a = new TENodeAlgorithm();
    a->runRead();
    // vector<Point *> P;
    // vector<Shape *> S;
    // vector<vector<TimeExpandedNode *>> allTENs;
    // vector<TimeExpandedNode *> tempTENs;
    // allTENs.push_back(tempTENs);

    // ifstream input("AllParts.txt");
    // input.is_open();
    // int counter = 0;

    // while (!input.eof())
    // {
    //     string str;
    //     getline(input, str);
    //     if (strSmatch(str, "[_A-Z0-9]+(?= )") == "")
    //         break;
    //     Shape *shape = new Shape();

    //     shape->setName(strSmatch(str, "[_A-Z0-9]+(?= )"));

    //     string point_1 = strSmatch(str, "[_][-0-9.,]+(?=_)");
    //     string point_2 = strSmatch(str, "[^_A-Z ][-0-9.,]+(?=$)");

    //     Point *firstPoint = new Point();
    //     firstPoint->setPoint(stof(strSmatch(point_1, "[^_ ][-0-9.,]+(?=,)")),
    //                          stof(strSmatch(point_1, "[^_,][-0-9.]+(?=$)")));
    //     Point *lastPoint = new Point();
    //     lastPoint->setPoint(stof(strSmatch(point_2, "[^_ ][-0-9.,]+(?=,)")),
    //                         stof(strSmatch(point_2, "[^_,][-0-9.]+(?=$)")));

    //     shape->setPoint(firstPoint, lastPoint);

    //     if (!checkPoint(firstPoint, P))
    //     {
    //         P.push_back(firstPoint);
    //         TimeExpandedNode *n1 = new TimeExpandedNode();
    //         n1->setTENode(firstPoint);
    //         allTENs.at(0).push_back(n1);
    //     }

    //     if (!checkPoint(lastPoint, P))
    //     {
    //         P.push_back(lastPoint);
    //         TimeExpandedNode *n2 = new TimeExpandedNode();
    //         n2->setTENode(lastPoint);
    //         allTENs.at(0).push_back(n2);
    //     }

    //     if (!checkShape(shape, S))
    //     {
    //         S.push_back(shape);
    //     }
    // }

    // for (TimeExpandedNode *n : allTENs.at(0))
    // {
    //     for (Shape *s : S)
    //     {
    //         if (s->start->equals(n->origin))
    //         {
    //             n->insertTarget(s);
    //         }
    //         else if (s->end->equals(n->origin))
    //         {
    //             n->insertSource(s);
    //         }
    //     }
    // }
    // cout << "End of first nested-for loops" << endl;
    // // cout << "allTENs.at(0).size( ) = " << allTENs.at(0).size() << endl;

    // for (TimeExpandedNode *nA : allTENs.at(0))
    // {
    //     for (TimeExpandedNode *nB : allTENs.at(0))
    //     {
    //         vector<pair<int, int>> nA_to_nB;
    //         vector<pair<int, int>> nB_to_nA;
    //         if (!nA->equals(nB))
    //         {
    //             counter++;
    //             // cout<<//"A("<<nA->origin->x<<", "<<nA->origin->y<<") to ("<<nB->origin->x<<", "<<nB->origin->y<<") "<<
    //             //         counter<<endl;
    //             if ( // counter == 105875 ||
    //                  // counter == 220553 //||
    //                 counter % (10 * 1000 * 1000) == 0
    //                 // counter == 67116
    //             )
    //             {
    //                 // cout<<"BDEUG"<<endl;
    //                 cout << counter << endl;
    //             }
    //             nB_to_nA = getConcidences(&(nA->srcs), &(nB->tgts));
    //             nA_to_nB = getConcidences(&(nB->srcs), &(nA->tgts));
    //             nA->insertSourcesAndTargets(nB, nB_to_nA, nA_to_nB);
    //         }
    //     }
    // }
    // cout << "End of second nested-for loop" << endl;

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

#endif

