/*
 * TENodeAlgorithm.h
 *
 *  Created on: Jun 18, 2023
 *      Author: duckt
 */

#ifndef VEINS_INET_TENODEALGORITHM_H_
#define VEINS_INET_TENODEALGORITHM_H_

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
#include "pugixml.h"

typedef std::pair<double, double> PointElement;
typedef std::tuple<PointElement, PointElement, double> ShapeElement;
typedef std::tuple<std::string, ShapeElement> EdgeForRead;
typedef std::tuple<std::string, std::vector<PointElement>> Junc;
typedef std::tuple<std::string, int, ShapeElement> Part;
using namespace std;

inline ofstream outFile;

class TENodeAlgorithm {
public:
//    TENodeAlgorithm();
//    virtual ~TENodeAlgorithm();


    vector<std::pair<PointElement, PointElement>> listPointElement(PointElement A, PointElement B, double x)
    {
        double d = sqrt(pow(B.first - A.first, 2) + pow(B.second - A.second, 2));
        int n = int(d / x);
        float x1, y1, x2, y2;
        vector<std::pair<PointElement, PointElement>> list;
        double ratio = x / d;
        if (n == 0)
        {
            list.push_back({A, B});
            return list;
        }

        for (int i = 0; i < n; i++)
        {
            x1 = A.first + i * (B.first - A.first) * ratio;
            y1 = A.second + i * (B.second - A.second) * ratio;
            x2 = A.first + (i + 1) * (B.first - A.first) * ratio;
            y2 = A.second + (i + 1) * (B.second - A.second) * ratio;
            list.push_back({PointElement({x1, y1}), PointElement({x2, y2})});
        }
        if (d / x != int(d / x))
        {
            list.push_back({PointElement({x2, y2}), B});
        }

        return list;
    }

    void splitEdge(double x, EdgeForRead *e)
    {
        vector<std::pair<PointElement, PointElement>> list = listPointElement(get<0>(get<1>(*e)), get<1>(get<1>(*e)), x);
        int i = 0;
        for (std::pair<PointElement, PointElement> PointElement : list)
        {
            outFile << get<0>(*e) << " " << i << "_" << fixed << setprecision(2) << PointElement.first.first << "," << PointElement.first.second << "_" << PointElement.second.first << "," << PointElement.second.second << "\n";
            i++;
        }
    }

    void splitJunc(double x, Junc *j)
    {
        vector<PointElement> PointForReads = get<1>(*j);
        int n = 0;
        for (int i = 0; i < PointForReads.size() - 1; i++)
        {
            vector<std::pair<PointElement, PointElement>> list = listPointElement(PointForReads[i], PointForReads[i + 1], x);
            for (std::pair<PointElement, PointElement> PointElement : list)
            {
                outFile << get<0>(*j) << " " << n << "_" << PointElement.first.first << "," << PointElement.first.second << "_" << PointElement.second.first << "," << PointElement.second.second << "\n";
                n++;
            }
        }
    }

    vector<EdgeForRead> getSucc(vector<EdgeForRead> *edges, EdgeForRead *e)
    {
        vector<EdgeForRead> list;
        for (EdgeForRead edge : *edges)
        {
            if (get<0>(get<1>(edge)) == get<1>(get<1>(*e)))
            {
                list.push_back(edge);
            }
        }
        return list;
    }

    void runRead();
};

#endif /* VEINS_INET_TENODEALGORITHM_H_ */
