// 语法分析器.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "词法分析器.h"
#include "语法分析器.h"

int main()
{
	Lexical_Analysiser la =  Lexical_Analysiser("test1.txt");
	if(!la.analysis())	//分析失败
		cout << la.lexicalBugReport() <<endl;
	else {	//分析成功，进一步进行语法分析
		Syntax_Analysiser sa = Syntax_Analysiser(la.getResult());
		sa.analysis(Syntax_Analysiser::Synstate::Thread, 0);
		sa.tree.printTree();
	}
	getchar();
    return 0;
}

