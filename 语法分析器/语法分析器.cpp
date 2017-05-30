#include "stdafx.h"
#include "语法分析器.h"


Syntax_Analysiser::Syntax_Analysiser(vector<Node> nodelist) :nodelist(nodelist),pointer(0){

};

vector<string> SSName = { "null","Thread","Feature","Port_spec","Port_type","Parameter","IOtype","Flow_spec",
"Flow_source_spec","Flow_sink_spec","Flow_path_spec","Association","Splitter","Reference","Associations" };

State Syntax_Analysiser::getState(int p) {
	return nodelist[p].state;
}

int Syntax_Analysiser::getLine(int p = -1) {
	if (p < 0)p = pointer;
	return nodelist[p].line;
}

string Syntax_Analysiser::getName(int p = -1) {
	if (p < 0)p = pointer;
	return nodelist[p].content;
}

void Syntax_Analysiser::print(int p, int q,int state) {
	cout << p << ' ' << q << ' '<<state<<endl;
	for (int i = p; i < q; i++) {
		cout << mark[getState(i)] << ' ';

	}
	cout << endl;
}

//match 匹配成功后会移动指针至下一个字符
bool Syntax_Analysiser::match(State state,bool isthrow = true) {
	//if(isthrow)cout << "匹配" << mark[state] << (getState(pointer) == state ? " 成功" : " 失败") << endl;
	//else cout << "实验匹配" << mark[state] << (getState(pointer) == state ? " 成功" : " 失败") << endl;
	if (getState(pointer) == state) {
		pointer++;
		return true;
	}
	if(isthrow)throw syntaxError(string("can't match ")+mark[state], getLine());
	return false;
}

//trymatch 纯粹的尝试匹配
bool Syntax_Analysiser::trymatch(State state) {
	//cout << "try匹配" << mark[state] << (getState(pointer) == state ? " 成功" : " 失败") << endl;

	return getState(pointer) == state;
}

bool Syntax_Analysiser::analysis(Synstate state,int father) {
	//cout << "State=" << SSName[state] << endl;
	if (pointer >= nodelist.size())return false;
	try{
		string tmp;
		int child;
		switch (state) {
		case Thread:	//Thread -->thread identifier [ features feature ] [ flows flow_spec ] [ properties association; ] end identifier ;
			if(!match(thread,false))return false;
			match(identifier);
			child = tree.addChild(father, string("Thread(") + getName(pointer - 1) + ')');
			if (match(features, false))analysis(Feature, child);
			if (match(flows, false)) analysis(Flow_spec, child);
			if (match(properties, false)) {
				if (!analysis(Association, child))throw syntaxError("can't find association", getLine());
				match(sem);
			}
			match(State::end);
			match(identifier);
			match(sem);
			analysis(Thread, father);
			break;
		case Feature:	//feature-->identifier : IOtype (port_spec | Parameter)| none;
			if (match(none, false)) {
				tree.addChild(father, "Feature(none)");
				match(sem);
			}
			else {
				child = tree.addChild(father, "Feature("+getName()+')');
				match(identifier);
				match(colon);
				analysis(IOtype, child);
				if (trymatch(parameter)) {
					analysis(Parameter, child);
				}
				else if (trymatch(State::data) || trymatch(event)) {
					analysis(Port_spec, child);
				}
				else {
					throw syntaxError("can't match parameter|data|event", getLine());
				}
			}
			break;
		case Port_spec:	//port_spec --> port_type Associations ;
			child = tree.addChild(father, "Port_spec("+getName()+")");
			analysis(Port_type, child);
			analysis(Associations, child);
			match(sem);
			break;
		case Port_type: //port_type-->data port Reference | event data port Reference | event port
			if (match(State::data, false)) {
				child = tree.addChild(father, "Port_type(data port)");
				match(State::port);
				analysis(Reference, child);
				break;
			}
			match(event);
			if (match(State::data, false)) {
				child = tree.addChild(father, "Port_type(event data port)");
				match(State::port);
				analysis(Reference, child);
				break;
			}
			match(port);
			break;
		case Parameter:	//Parameter-->parameter Reference Associations;
			child = tree.addChild(father, "Parameter");
			match(parameter);
			analysis(Reference, child);
			analysis(Associations, child);
			match(sem);
			break;
		case IOtype:	//IOtype-->in | out | in out
			if (match(in, false)) {
				if (match(out, false)) {
					child = tree.addChild(father, "IOtype(in out)");
				}
				else
					child = tree.addChild(father, "IOtype(in)");
			}
			else if (match(out, false)) {
				child = tree.addChild(father, "IOtype(out)");
			}
			else {
				throw syntaxError("can't match IOtype", getLine());
			}
			break;
		case Flow_spec:	//flow_spec-->identifier : flow (flow_source_spec | flow_sink_spec | flow_path_spec) | none;
			if (match(none, false)) {
				child = tree.addChild(father, string("Flow_spec(none)"));
				match(sem);
				break;
			}
			child = tree.addChild(father, string("Flow_spec(") + getName() + ')');
			match(identifier);
			match(colon);
			match(flow);
			if (trymatch(source))analysis(Flow_source_spec, child);
			else if (trymatch(sink))analysis(Flow_sink_spec, child);
			else if (trymatch(path))analysis(Flow_path_spec, child);
			else throw("can't match source|sink|path", getLine());
			break;
		case Flow_source_spec:	//flow_source_spec -->source identifier Associations ;
			match(source);
			child = tree.addChild(father, string("Flow_source_spec(") + getName() + ')');
			match(identifier);
			analysis(Associations, child);
			match(sem);
			break;
		case Flow_sink_spec:	//flow_sink_spec -->sink identifier Associations;
			match(sink);
			child = tree.addChild(father, string("Flow_sink_spec(") + getName() + ')');
			match(identifier);
			analysis(Associations, child);
			match(sem);
			break;
		case Flow_path_spec:	//flow_path_spec -->path identifier ->identifier;
			match(path);
			tmp = getName() + "->";
			match(identifier);
			match(to);
			tmp += getName();
			match(identifier);
			tree.addChild(father, string("Flow_path_spec(") + tmp + ')');
			match(sem);
			break;
		case Association:	//association --> [[ identifier :: ] identifier splitter [ constant ] access decimal | none]
			if (match(none, false)) {
				child = tree.addChild(father, string("Association(none)"));
				break;
			}
			tmp = getName();
			if(!match(identifier,false))return false;
			if (match(dcolon,false)) {
				tmp += "::" + getName();
				match(identifier);
			}
			child = tree.addChild(father, string("Association(")+tmp+")");
			analysis(Splitter, child);
			match(constant, false);
			match(access);
			tree.addChild(child, string("Decimal(" + getName() + ")"));
			match(decimal);
			break;
		case Splitter:	//splitter--> => | +=>
			if (match(eg,false)) {
				tree.addChild(father, string("Splitter(=>)"));
				break;
			}
			match(aeg);
			tree.addChild(father, string("Splitter(+=>)"));
			break;
		case Reference:	//reference-->[identifier {:: reference}]
			if (!trymatch(identifier))return false;
			child = tree.addChild(father, string("Reference("+getName()+")"));
			match(identifier);
			if(match(dcolon,false))
				analysis(Reference, child);
			break;
		case Associations:	//Associations --> [ { { association } } ] 
			if (!match(lb,false))return false;
			child = tree.addChild(father, string("Associations"));
			while (analysis(Association, child));
			match(rb);
			break;
		default:
			throw syntaxError("unknown SynState", getLine());
		}
	}
	catch (syntaxError se) {
		cout << "Error:" << se.what() << " Line:"<< se.line << " but is"<< getName()<<endl;
	}
	return true;
}
/*
Thread -->thread identifier [ features feature ] [ flows flow_spec ] [ properties association; ] end identifier ;

feature-->port_spec|Parameter| none ;
port_spec --> identifier : IOtype port_type [ { { association } } ] ;
port_type -->data port [ reference ] | event data port [ reference ]| event port
Parameter -->identifier : IOtype parameter [ reference ][ { { association } } ] ;
IOtype-->in | out | in out
flow_spec -->flow_source_spec| flow_sink_spec| flow_path_spec| none;
flow_source_spec -->identifier : flow source identifier [ { { association } } ] ;
flow_sink_spec -->identifier : flow sink identifier[ { { association } } ] ;
flow_path_spec -->identifier : flow path identifier ->identifier;
association -->[ identifier :: ] identifier splitter [ constant ] access decimal | none
splitter-->  => | +=>
reference -->[ package_name :: ]  identifier
package_name --> { identifier :: }  identifier

*/