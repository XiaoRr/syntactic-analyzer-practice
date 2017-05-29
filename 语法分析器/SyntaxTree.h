#pragma once
#include <string>
#include <vector>
#include "Node.cpp"
//using namespace std;

class SyntaxTree {
public:
	vector<TreeNode> tree;	//Ê÷
	SyntaxTree();
	int addChild(int father, string child);
	void printTree();
	bool rename(int num,string newname);
};