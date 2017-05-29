
#include "stdafx.h"
#include "�ʷ�������.h"

//����״̬������
extern vector<string> mark = { "identifier","decimal","thread","features","flows","properties","end","none","in","out","data","port",
"event","parameter","flow","source","sink","path","constant","access","eg","aeg","sem","colon","dcolon",
"lb","rb","to","start","id1","dec0","dec1","dec2","dec3","eg1","aeg1","error" };

Lexical_Analysiser::Lexical_Analysiser(string filename) {
	ifstream t(filename);
	string tmp((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());	//��ȡ�����ĵ�����
	this->all = tmp;
}

bool Lexical_Analysiser::analysis() {
	for (; p < all.length(); p++)
		parse(all[p]);	//���ֽ���
	return bug == "";
}
vector<Node> Lexical_Analysiser::getResult() {
	return result;
}

//���溯���������˴���
void Lexical_Analysiser::alert(char c, string expect = "") {
	char tmp[100];
	sprintf_s(tmp, "�����У�%d �޷��������ַ���%c �ַ���ascii: %d\n��ǰ״̬Ϊ%s;", line, c, (int)c, mark[state].c_str());
	bug += tmp;
	if (expect != "")bug+= "���ڴ�����Ϊ" + expect;
	bug += "\r\n";
	state = error;		//תΪ����״̬
}

//�������������Ѿ�ƥ��ĵ���
void  Lexical_Analysiser::print() {
	if (state == identifier) {		//�����Ѿ�ƥ��Ϊid�ĵ��ʣ��п����Ǳ����֣���Ҫ��һ��ɸѡ
		for (int i = 2; mark[i] != "eg"; i++) {
			if (s == mark[i])state = State(i);
		}
	}

	Node tmp = Node{ s,state,line };
	result.push_back(tmp);
	state = start;		//�ع��ʼ״̬
	s = "";		//����Դ洢��һ������
}

//��ȡ���˲����ڵ�ǰ���ʵ��ַ��ĳ�������
void  Lexical_Analysiser::redo() {
	print();
	p--;
	flg = 0;	//���
}

//����������ÿ�ζ�ȡһ��char
void  Lexical_Analysiser::parse(char c) {
	flg = 1;	//��ǻ�ԭ
	//���˿��ַ�
	if (isspace(c)) {
		if (state != start)print();
		if (c == '\n')line++;
		return;
	}

	//״̬ת��
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
		else alert(c, "��ĸ������");
		break;
	case dec0:
		if (isdigit(c))state = dec2;
		else if (c == '>')state = to;
		else alert(c, "С����>");
		break;
	case dec1:
		if (isdigit(c))state = dec2;
		else if (c == '=')state = aeg1;
		else alert(c, "С����=");
		break;
	case dec2:
		if (isdigit(c))break;
		else if (c == '.')state = dec3;
		else alert(c, "С���������");
		break;
	case dec3:
		if (isdigit(c))state = decimal;
		else alert(c, "����");
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
	if (flg)s += c;	//���ַ���¼��ǰƥ��ĵ���
}

string Lexical_Analysiser::lexicalBugReport() {
	return bug;
}