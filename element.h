/*
 * ConvertToStructure.h
 *
 *  Created on: Jun 18, 2023
 *      Author: duckt
 */

#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <regex>
// #include "Point.cpp"
// #include "Shape.cpp"
// #include "TimeExpandedNode.cpp"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

FILE* out = fopen("out1.txt", "w");

class Point
{
public:
    double x;
    double y;

    void setPoint(double x, double y)
    {
        this->x = x;
        this->y = y;
    }

    double getX()
    {
        return x;
    }
    double getY()
    {
        return y;
    }

    void printPoint(int tabCount)
    {
        for(int i = 1; i <= tabCount; i++)
            fprintf(out, "\t");
        fprintf(out, "%lf %lf\n", this->getX(), this->getY());
        // cout << this->getX() << " " << this->getY() << endl;
    }

    bool equals(Point *p)
    {
        return (this->x == p->x) && (this->y == p->y);
    }
};

class Shape
{
public:
    Point *start;
    Point *end;
    double d;
    string name;
    double time;    /// add in bai3
    double getTime() {  /// add in bai3
        return this->time;
    }

    void setName(string name)
    {
        this->name = name;
        if(name.find("E") == std::string::npos
             && name.find("J") == std::string::npos){
            cout<<"DEBUG: "<<__FILE__<<":"<<__LINE__<<":"<<endl;
        }
    }

    void setPoint(Point *start, Point *end)
    {
        this->start = start;
        this->end = end;
    }


    bool equals(Shape *s)
    {
        return ((this->start)->equals(s->start) && (this->end)->equals(s->end));
    }

    // virtual double getWeight( ){
    //     if(time == 0){
    //         time = this->d/(Constant::v) ; //Với v là hằng số của lớp Constant
    //     }	
    //     return time;
    // }


    void printShape(int tabCount) {
        for(int i = 1; i <= tabCount; i++)
            fprintf(out, "\t");
        fprintf(out, " start: "); this->start->printPoint(tabCount + 1);
        for(int i = 1; i <= tabCount; i++)
            fprintf(out, "\t");
        fprintf(out, "end: ");this->end->printPoint(tabCount + 1);
        for(int i = 1; i <= tabCount; i++)
            fprintf(out, "\t");
        fprintf(out, "length: %lf, name: %s\n", this->d, this->name.c_str());
        // cout << this->d << "\n" << this->name<<"\n";
    }
};

class TimeExpandedNode
{
public:
    vector<pair<TimeExpandedNode *, Shape *>> srcs;
    Point *origin;
    double time;
    int layer;
    vector<pair<TimeExpandedNode *, Shape *>> tgts;

    TimeExpandedNode( ){ }  /// add in bai3

    TimeExpandedNode (Point* origin, double time,       /// add in bai3
        std::vector<std::pair<TimeExpandedNode*, Shape*>> tgts){
        this->origin = origin;
        this->time = time;
        for(auto it : tgts){
            this->tgts.push_back(make_pair(new TimeExpandedNode(), it.second));
        }                        
    }

    int indexInSources(Shape* s){   /// add in bai3
        int index = 0;
        bool found = false;
        for(auto it : tgts){
            Shape* shape = it.second;
            if(shape->equals(s)){
                        found = true;
                        return index;
            }
            index++;
        }
        if(!found)   return -1;
        return index;             
    }

    void printTENode(int tabCount) {
        for(int i = 1; i <= tabCount; i++)
            fprintf(out, "\t");
        fprintf(out, "Source: \n");
        for(int i = 0; i < this->srcs.size(); i++) {
            srcs[i].first->printOrigin(tabCount + 1);
            srcs[i].second->printShape(tabCount + 1);
        }

        for(int i = 1; i <= tabCount; i++)
            fprintf(out, "\t");
        fprintf(out, "My origin: ");
        this->origin->printPoint(tabCount + 1);

        for(int i = 1; i <= tabCount; i++)
            fprintf(out, "\t");
        fprintf(out, "time: %lf, and layer: %d\n", this->time, this->layer);

        for(int i = 1; i <= tabCount; i++)
            fprintf(out, "\t");
        fprintf(out, "Target: \n");
        for(int i = 0; i < this->tgts.size(); i++) {
            tgts[i].first->printOrigin(tabCount + 1);
            tgts[i].second->printShape(tabCount + 1);
        }
        fprintf(out, "\n");
    }

    void printOrigin(int tabCount) {
        for(int i = 1; i <= tabCount; i++)
            fprintf(out, "\t");
        fprintf(out, "origin point: ");
        this->origin->printPoint(0);
    }

    void setTENode(Point *point)
    {
        this->origin = point;
    }

    void insertSource(Shape *s)
    {
        // Nếu srcs có chứa một pair mà phần tử đầu của nó giống hệt s->start
        //     thì không thêm nullptr và s vào
        //         Ngược lại
        {
            pair<TimeExpandedNode *, Shape *> pair =
                make_pair(nullptr, s);
            this->srcs.push_back(pair);
        }
    }

    void insertTarget(Shape *s)
    {
        // Nếu tgts có chứa một pair mà phần tử đầu của nó giống hệt s->end
        //     thì không thêm cặp nullptr và s vào
        //         Ngược lại
        {
            pair<TimeExpandedNode *, Shape *> pair =
                make_pair(nullptr, s);
            this->tgts.push_back(pair);
        }
    }

    bool equals(TimeExpandedNode *n)
    {
        return this->origin->equals(n->origin);
    }

    void insertSourcesAndTargets(TimeExpandedNode *n, vector<pair<int, int>> fromN,
                                 vector<pair<int, int>> toN)
    {
        for (int i = 0; i < fromN.size(); i++)
        {
            int atN = fromN.at(i).first;
            int toThis = fromN.at(i).second;
            if (this->srcs.at(atN).first == nullptr)
            {
                this->srcs.at(atN).first = n;
            }
            if (n->tgts.at(toThis).first == nullptr)
            {
                n->tgts.at(toThis).first = this;
            }
        }

        for (int i = 0; i < toN.size(); i++)
        {
            int atN = toN.at(i).first;
            int fromThis = toN.at(i).second;
            if (this->tgts.at(fromThis).first == nullptr)
            {
                this->tgts.at(fromThis).first = n;
            }
            if (n->srcs.at(atN).first == nullptr)
            {
                n->srcs.at(atN).first = this;
            }
        }
    }
};

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

#endif /* _ELEMENT_H_ */
