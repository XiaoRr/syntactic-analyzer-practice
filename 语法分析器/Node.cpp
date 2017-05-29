#include "stdafx.h"
#pragma once
using namespace std;

//枚举类，枚举所有状态
enum State {
	identifier = 0,		//id
	decimal,		//数字
	/*以下为保留字*/
	thread,
	features,
	flows,
	properties,
	end,
	none,
	in,
	out,
	data,
	port,
	event,
	parameter,
	flow,
	source,
	sink,
	path,
	constant,
	access,
	/*以下为符号*/
	eg,		//=>
	aeg,	//+=>
	sem,	//;
	colon,	//:
	dcolon,	//::
	lb,		//{
	rb,		//}
	to,		//->
	/*以下为过渡状态*/
	start,
	id1,
	dec0,
	dec1,
	dec2,
	dec3,
	eg1,
	aeg1,
	/*错误状态*/
	error
};

//枚举类的字符名字
extern vector<string> mark;

//SynName的字符对应
extern vector<string> SSName;
typedef struct {
	string content;	//内容
	State state;	//状态
	int line;		//所在行数
}Node;

typedef struct {
	string name;
	vector<int> child;
}TreeNode;

typedef struct {
	vector<State> firstmark;
	vector<State> endmark;
	int type;
}synRule;

