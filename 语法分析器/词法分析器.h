#pragma once
#include <string>
#include <vector>
#include "Node.cpp"
using namespace std;

class Lexical_Analysiser {
public:
	Lexical_Analysiser(string filename);	//初始化
	string lexicalBugReport();	//获取bug信息
	vector<Node> getResult();	//获取词法分析结果
	bool analysis();	//分析 返回值为true则无词法错误

private:
	string s = "";		//存储正在解析的单词
	State state = start;		//初始状态：start
	int p = 0;	//文件读取指针
	bool flg;	//标记：当前字符属于当前单词
	int line = 1;	//行号
	string all;	//文件所有内容
	string bug = "";	//获取bug
	vector<Node> result;	//解析结果


	void alert(char c, string expect);
	void print();
	void redo();
	void parse(char c);
};