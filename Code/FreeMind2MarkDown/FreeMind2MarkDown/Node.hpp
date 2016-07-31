//
//  Node.hpp
//  FM2MD
//
//  Created by Ming on 7/18/16.
//  Copyright © 2016 mitake. All rights reserved.
//

#ifndef Node_hpp
#define Node_hpp

#include <stdio.h>
#include <string>
#include <iostream>

#endif /* Node_hpp */


using namespace std;

class Node
{
public:
    string name ;
    int level ;
    int number ;
    bool isClosed ;
    Node * nodes[100];
    int nodeNum = 0;
    string chapter ;
    bool isRoot;
    void setNode(Node* nNode){
        int nlevel = nNode->level;
        if ( nlevel == level + 1) {
            nodes[nodeNum++] = nNode ;
            nNode->number = nodeNum;
            nNode->chapter = chapter;
            nNode->chapter.append(std::to_string(nNode->number));
            nNode->chapter.append(".");        
        }
        else {
            for (int i = 0; i< nodeNum; i++) {
                Node *node = nodes[i];
                if(!node->isClosed) {
                    node->setNode(nNode);
                    break;                    
                }
            }
        }
        
    };
    void setNodeClose(int nlevel) {        
        if (nlevel == level) {
            isClosed = true;
        }
        else {
            for (int i = 0; i< nodeNum; i++) {
                Node *node = nodes[i];
                if(!node->isClosed) {
                    node->setNodeClose(nlevel);
                    break;
                }
            }
        }
    }

};

