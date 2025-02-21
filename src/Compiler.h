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

	std::vector<Function> functions;
private:
	void CompileFuncBody(int& i);

	// std::map<std::string, Function> functionsMap;

	std::string currentExpression;

	Type currentType;
	Function currentFunction;
	bool inFunc = false;
	int latestFuncAddress = 0;

	uint16_t resultRegister;

	std::string lastToken;
	std::string currentFileText;
	std::vector<std::string> tokens;

};