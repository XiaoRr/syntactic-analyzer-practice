// �﷨������.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "�ʷ�������.h"
#include "�﷨������.h"

int main()
{
	Lexical_Analysiser la =  Lexical_Analysiser("test1.txt");
	if(!la.analysis())	//����ʧ��
		cout << la.lexicalBugReport() <<endl;
	else {	//�����ɹ�����һ�������﷨����
		Syntax_Analysiser sa = Syntax_Analysiser(la.getResult());
		sa.analysis(Syntax_Analysiser::Synstate::Thread, 0);
		sa.tree.printTree();
	}
	getchar();
    return 0;
}

