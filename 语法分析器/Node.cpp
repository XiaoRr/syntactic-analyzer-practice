#include "stdafx.h"
#pragma once
using namespace std;

//ö���࣬ö������״̬
enum State {
	identifier = 0,		//id
	decimal,		//����
	/*����Ϊ������*/
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
	/*����Ϊ����*/
	eg,		//=>
	aeg,	//+=>
	sem,	//;
	colon,	//:
	dcolon,	//::
	lb,		//{
	rb,		//}
	to,		//->
	/*����Ϊ����״̬*/
	start,
	id1,
	dec0,
	dec1,
	dec2,
	dec3,
	eg1,
	aeg1,
	/*����״̬*/
	error
};

//ö������ַ�����
extern vector<string> mark;

//SynName���ַ���Ӧ
extern vector<string> SSName;
typedef struct {
	string content;	//����
	State state;	//״̬
	int line;		//��������
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

