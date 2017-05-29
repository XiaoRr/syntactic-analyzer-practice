
#include "stdafx.h"
#include "词法分析器.h"

//所有状态的名字
extern vector<string> mark = { "identifier","decimal","thread","features","flows","properties","end","none","in","out","data","port",
"event","parameter","flow","source","sink","path","constant","access","eg","aeg","sem","colon","dcolon",
"lb","rb","to","start","id1","dec0","dec1","dec2","dec3","eg1","aeg1","error" };

Lexical_Analysiser::Lexical_Analysiser(string filename) {
	ifstream t(filename);
	string tmp((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());	//读取整个文档内容
	this->all = tmp;
}

bool Lexical_Analysiser::analysis() {
	for (; p < all.length(); p++)
		parse(all[p]);	//逐字解析
	return bug == "";
}
vector<Node> Lexical_Analysiser::getResult() {
	return result;
}

//警告函数，发生了错误
void Lexical_Analysiser::alert(char c, string expect = "") {
	char tmp[100];
	sprintf_s(tmp, "错误行：%d 无法解析的字符：%c 字符的ascii: %d\n当前状态为%s;", line, c, (int)c, mark[state].c_str());
	bug += tmp;
	if (expect != "")bug+= "，期待输入为" + expect;
	bug += "\r\n";
	state = error;		//转为错误状态
}

//输出函数，输出已经匹配的单词
void  Lexical_Analysiser::print() {
	if (state == identifier) {		//对于已经匹配为id的单词，有可能是保留字，需要进一步筛选
		for (int i = 2; mark[i] != "eg"; i++) {
			if (s == mark[i])state = State(i);
		}
	}

	Node tmp = Node{ s,state,line };
	result.push_back(tmp);
	state = start;		//回归初始状态
	s = "";		//清空以存储下一个单词
}

//读取到了不属于当前单词的字符的撤销处理
void  Lexical_Analysiser::redo() {
	print();
	p--;
	flg = 0;	//标记
}

//解析函数，每次读取一个char
void  Lexical_Analysiser::parse(char c) {
	flg = 1;	//标记还原
	//过滤空字符
	if (isspace(c)) {
		if (state != start)print();
		if (c == '\n')line++;
		return;
	}

	//状态转移
	switch (state) {
	case start:
		if (isalpha(c))state = identifier;
		else if (c == '-')state = dec0;
		else if (c == '+')state = dec1;
		else if (isdigit(c))state = dec2;
		else if (c == '=')state = eg1;
		else if (c == '+')state = aeg1;
		else if (c == ';')state = sem;
		else if (c == ':')state = colon;
		else if (c == '{')state = lb;
		else if (c == '}')state = rb;
		else alert(c);
		break;
	case identifier:
		if (isalnum(c))break;
		else if (c == '_')state = id1;
		else redo();
		break;
	case id1:
		if (isalnum(c))state = identifier;
		else alert(c, "字母或数字");
		break;
	case dec0:
		if (isdigit(c))state = dec2;
		else if (c == '>')state = to;
		else alert(c, "小数或>");
		break;
	case dec1:
		if (isdigit(c))state = dec2;
		else if (c == '=')state = aeg1;
		else alert(c, "小数或=");
		break;
	case dec2:
		if (isdigit(c))break;
		else if (c == '.')state = dec3;
		else alert(c, "小数点或数字");
		break;
	case dec3:
		if (isdigit(c))state = decimal;
		else alert(c, "数字");
		break;
	case decimal:
		if (isdigit(c))break;
		else redo();
		break;
	case eg1:
		if (c == '>')state = eg;
		else alert(c, ">");
		break;
	case aeg1:
		if (c == '>')state = aeg;
		else alert(c, ">");
		break;
	case colon:
		if (c == ':')state = dcolon;
		else redo();
		break;
	case error:
		if (isspace(c))state = start;
		break;
	default:
		redo();
	}
	if (flg)s += c;	//逐字符记录当前匹配的单词
}

string Lexical_Analysiser::lexicalBugReport() {
	return bug;
}