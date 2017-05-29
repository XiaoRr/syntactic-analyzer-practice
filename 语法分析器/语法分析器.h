#pragma once
#include <string>
#include <vector>
#include "Node.cpp"
#include "SyntaxTree.h"
#include "syntaxError.h"

class Syntax_Analysiser {
public:
	enum Synstate {
		Thread = 1,
		Feature,
		Port_spec,
		Port_type,
		Parameter,
		IOtype,
		Flow_spec,
		Flow_source_spec,
		Flow_sink_spec,
		Flow_path_spec,
		Association,
		Splitter,
		Reference,
		//Package_name,
		Associations
	};
	SyntaxTree tree;
	Syntax_Analysiser(vector<Node> nodelist);	//构造函数
	string getBugReport();	//获取错误报告
	string getDotResult();	//获取语法树
	int myfind(int p,int q,int num1,int num2);
	void print(int p, int q,int state);
	bool trymatch(State state);
	bool analysis(Synstate state,int father);
	State getState(int p);
	int getLine(int p);
	string getName(int p);
	bool match(State state,bool isthrow);
private:
	string bug = "";
	string dot = "";
	vector<Node> nodelist;

	int child;	//tmp
	int pointer;	//读取指针
};