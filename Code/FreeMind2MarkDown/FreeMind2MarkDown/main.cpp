//
//  main.cpp
//  FM2MD
//
//  Created by Ming on 7/17/16.
//  Copyright © 2016 mitake. All rights reserved.
//

#include <iostream>
#include <regex>
#include <string>
#include <stdio.h>
#include <fstream>
#include "Node.hpp"

using namespace std;

string getTEXT(smatch matches) {
    string key = "";
    string retrunText = "";
    for (size_t i = 1; i < matches.size(); i++) {
        string match(matches[i].first, matches[i].second);
        if (i== (matches.size()-1)) {
            retrunText = match;
        }
    }
    return retrunText;
}


string  getTableOfContent(Node* node,int level) {
    string tableOfContent = "";
    if (level == 0) {
        return "";
    }
    
    if (node->isRoot) {
        tableOfContent.append("# "+node->name+"\n");
        tableOfContent.append("## 目录 \n");
    }
    else {
        for (int i = 1; i < node->level; i++) {
            tableOfContent.append("\t");
        }
        if (node->level==1) {
            tableOfContent.append(node->chapter);
        }
        else {
            tableOfContent.append("* ");
        }
        
        tableOfContent.append(" [" + node->name + "](#" + node->name + ")" + "\n");
    }
    
    
    level-- ;
    int nodeCount = sizeof(node->nodes) / sizeof(node);
    for (int i = 0; i < nodeCount; i++) {
        if (node->nodes[i]) {
            tableOfContent.append(getTableOfContent(node->nodes[i], level));
        }
        else {
            break;
        }
    }
    return tableOfContent;
}

string  getContent(Node* node,int level) {
    
    string context = "";
    bool nochapter = false;
    if (level <= 0) {
        nochapter = true;
    }
    
    if (!node->isRoot) {
        context.append("<a name=" + node->name +"></a>\n");
        for (int i = 0; i < node->level; i++) {
            context.append("#");
        }
        context.append("#");
        context.append("\t");
        if (nochapter) {
            context.append(node->name + "\n\n");
        }
        else {
            context.append(node->chapter + "" + node->name + "\n\n");
        }
    }
    
    level--;
    int nodeCount = sizeof(node->nodes) / sizeof(node);
    for (int i = 0; i < nodeCount; i++) {
        if (node->nodes[i]) {
            context.append(getContent(node->nodes[i], level));
        }
        else {
            break;
        }
    }
    return context;
}

Node* createNode(string nodeName,int level,int number) {
    Node* nNode = new Node();
    nNode->name = nodeName;
    nNode->level = level;
    nNode->number = number;
    return nNode;
}

int main(int argc, const char * argv[]) {
    
    string inputfile = "";
    string outputfile = "";
    
    cout << "Please enter input file:";
    getline(cin, inputfile);
    cout << "Please enter output file:";
    getline(cin, outputfile);
    
    ifstream input;
    ifstream ouput;
    string readLine;
    ifstream mmfile(inputfile);
    ofstream mdfile;
    mdfile.open (outputfile);
    
    int currentLevel = 0 ;
    int currentNum = 1;
    
    Node * root = NULL;
    while ( getline (mmfile,readLine) ) // read line by line
    {
        string text(readLine);
        regex expr0("<node (.*?)=\"(.*?)\" (.*?)=\"(.*?)\" (.*?)=\"(.*?)\" (.*?)=\"(.*?)\\\"/>");
        regex expr1("<node (.*?)=\"(.*?)\" (.*?)=\"(.*?)\" (.*?)=\"(.*?)\" (.*?)=\"(.*?)\\\">");
        regex expr2("</node>");
        regex expr4("<node (.*?)=\"(.*?)\" (.*?)=\"(.*?)\" (.*?)=\"(.*?)\" (.*?)=\"(.*?)\" (.*?)=\"(.*?)\\\">");
        regex expr5("<node (.*?)=\"(.*?)\" (.*?)=\"(.*?)\" (.*?)=\"(.*?)\" (.*?)=\"(.*?)\" (.*?)=\"(.*?)\" (.*?)=\"(.*?)\\\">");
        
        smatch matches;
        
        string nodeName = "" ;
        Node * nNode = NULL;
        
        if (regex_match(text, matches, expr0)) {
            nodeName = getTEXT(matches);
            nNode = createNode(nodeName, currentLevel, currentNum);
            nNode->setNodeClose(currentLevel);
        }
        else if (regex_match(text, matches, expr5)) {
            nodeName = getTEXT(matches);
            nNode = createNode(nodeName, currentLevel, currentNum);
            currentLevel++;
        }
        else if (regex_match(text, matches, expr4)) {
            nodeName = getTEXT(matches);
            nNode = createNode(nodeName, currentLevel, currentNum);
            currentLevel++;
        }
        else if (regex_match(text, matches, expr1)) {
            nodeName = getTEXT(matches);
            nNode = createNode(nodeName, currentLevel, currentNum);
            currentLevel++;
        }
        else if (regex_match(text, matches, expr2)) {
            nodeName = getTEXT(matches);
            currentLevel--;
            root->setNodeClose(currentLevel);
            
        }
        
        if (nNode) {
            if (!root) {
                root = nNode;
                nNode->isRoot = true;
            }
            else {
                root->setNode(nNode);
            }
        }
        
    }
    input.close();
    
    string contextAll = getTableOfContent(root, 3);
    contextAll.append("\n");
    contextAll.append(getContent(root, 3));
    mdfile << contextAll ;
    mdfile.close();
    
    
    return 0;
}

