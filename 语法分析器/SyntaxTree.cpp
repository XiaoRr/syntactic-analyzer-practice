#include "stdafx.h"
#include "SyntaxTree.h"

//#include "Node.h"
using namespace std;
SyntaxTree::SyntaxTree() {
	tree.push_back(TreeNode{"root"});
}

bool SyntaxTree::rename(int num,string newname) {
	if (num > tree.size())return false;
	tree[num].name = newname;
	return true;
}

int SyntaxTree::addChild(int father, string child) {
	tree.push_back(TreeNode{ child });
	tree[father].child.push_back(tree.size() - 1);
	return tree.size() - 1;
}

void SyntaxTree::printTree() {
	cout << "结果已经输出到out.dot中" << endl;
	freopen("out.dot", "w", stdout);
	//文件头
	cout << "digraph ast{\r\nfontname = \"Microsoft YaHei\";\r\nfontsize = 10;\r\n"
		<< " node [shape = circle, fontname = \"Microsoft YaHei\", fontsize = 10];"
		<< "edge [fontname = \"Microsoft YaHei\", fontsize = 10];"
		<< "node [shape=\"plaintext\"];\r\n";
	//文件内容
	for (auto i = 0; i < tree.size(); i++) {
		string dotname = "dot" + to_string(i);
		cout << dotname << " [label=\"" << tree[i].name << "\"];" << endl;
	}

	for (auto i = 0; i < tree.size(); i++) {\
		string dotname = "dot" + to_string(i);
		for (auto j = 0; j < tree[i].child.size(); j++) {
			cout << dotname << "->" << " dot" << tree[i].child[j] << ";"<<endl;
		}
	}

	cout << "}" << endl;
}



