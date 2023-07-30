
#ifndef _TIMEEXPANDEDNODE_HPP_
#define _TIMEEXPANDEDNODE_HPP_

#include <iostream>
#include <string>
#include <string.h>
#include "Point.hpp"
#include "Shape.hpp"
#include <vector>
using namespace std;

class TimeExpandedNode
{
public:
    vector<pair<TimeExpandedNode *, Shape *>> srcs;
    Point *origin;
    double time;
    int layer;
    vector<pair<TimeExpandedNode *, Shape *>> tgts;
    string name = "";  /// add in bai6

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

    // TimeExpandedNode(){this->time = 0;}
    // TimeExpandedNode (Point* origin, double time, 
    //                 std::vector<std::pair<TimeExpandedNode*, Shape*>> tgts){
    //     this->origin = origin;
    //     this->time = time;
    //     for(auto& it : tgts){
    //         this->tgts.push_back(std::make_pair(nullptr, it.second));
    //     }                        
    // }

    void ExpandedNode (Point* origin, double time, 
                           std::vector<std::pair<TimeExpandedNode*, Shape*>> tgts){
                        this->origin = origin;
                        this->time = time;
                        for(auto& it : tgts){
                                  this->tgts.push_back(std::make_pair(nullptr, it.second));
                        }                        
	}


    int indexInSources(Shape* s){
           int index = 0;
           bool found = false;
           for(auto& it : srcs){
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

    bool endOfLane(){ /// add in bai6-a
        if(this->tgts.size() <= 1) return true;
        for(int i = 0; i < this->srcs.size(); i++){
            Shape* shape = this->srcs.at(i).second;
            auto name = shape->name;
            if(name.compare("artificial") != 0){
                for(int j = 0; j < this->tgts.size( ); j++){
                    auto nextShape = this->tgts.at(j).second;
                    auto nextName = nextShape->name;
                    if(nextName.compare("artificial") != 0){
                        if(nextName.compare(name) != 0){
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    std::string isStation(std::string stations) { /// add in bai6-b
        for(int i = 0; i < this->srcs.size(); i++){
            auto shape = this->srcs.at(i).second;
            auto name = shape->name;
            if(name.compare("artificial") != 0){
                if(stations.find("$" + name + "$") != std::string::npos){
                    return name;
                }
            }	
        }
        return "";

    }

    virtual TimeExpandedNode* transform(TimeExpandedNode* node) {      /// add in bai6-d
        return this;
    }

    virtual void connect2ArtificialStation(TimeExpandedNode* node){ //// add in bai6-d

    } 

};

#endif