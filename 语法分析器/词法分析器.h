#pragma once
#include <string>
#include <vector>
#include "Node.cpp"
using namespace std;

class Lexical_Analysiser {
public:
	Lexical_Analysiser(string filename);	//��ʼ��
	string lexicalBugReport();	//��ȡbug��Ϣ
	vector<Node> getResult();	//��ȡ�ʷ��������
	bool analysis();	//���� ����ֵΪtrue���޴ʷ�����

private:
	string s = "";		//�洢���ڽ����ĵ���
	State state = start;		//��ʼ״̬��start
	int p = 0;	//�ļ���ȡָ��
	bool flg;	//��ǣ���ǰ�ַ����ڵ�ǰ����
	int line = 1;	//�к�
	string all;	//�ļ���������
	string bug = "";	//��ȡbug
	vector<Node> result;	//�������


	void alert(char c, string expect);
	void print();
	void redo();
	void parse(char c);
};