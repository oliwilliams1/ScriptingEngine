#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include "VM.h"

std::string cleanUpFile(const std::string& path);

class Parser
{
public:
	void CleanUpFile(const std::string& path);
	void Tokenize();

	void Compile();

private:
	void CompileFuncBody(int& i);

	std::vector<Function> functions;

	// std::map<std::string, Function> functionsMap;

	std::string currentExpression;

	Type currentType;
	Function currentFunction;
	bool inFunc = false;
	int latestFuncAddress = 0;

	std::string lastToken;
	std::string currentFileText;
	std::vector<std::string> tokens;

};