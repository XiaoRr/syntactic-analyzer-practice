#include "stdafx.h"
#include "syntaxError.h"

syntaxError::syntaxError(const std::string & s, const int line):
	std::runtime_error(s), line(line) {}

