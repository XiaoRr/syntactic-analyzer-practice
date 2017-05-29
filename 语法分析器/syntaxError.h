#pragma once
#include<exception>
#include<string>
class syntaxError : public std::runtime_error {
public:
	explicit syntaxError(const std::string &s, const int line);
	int line;

};