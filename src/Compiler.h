#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include "VM.h"

class Parser
{
public:
	void CleanUpFile(const std::string& path);
	void Tokenize();

	void Compile();

	std::vector<Function> functions;
private:
	void compileFuncBody(int& i);

	// std::unoredered_+map<std::string, Function> functionsMap;

	std::string currentExpression;

	Type currentType;
	Function currentFunction;
	bool inFunc = false;
	int latestFuncAddress = 0;

	uint16_t resultRegister;
	uint16_t minRegister;

	std::string lastToken;
	std::string currentFileText;
	std::vector<std::string> tokens;

	std::unordered_map<std::string, std::pair<uint16_t, std::string>> variableMap;

	void trimLines(std::stringstream& ss, std::vector<std::string>& lines);
	void variableRealization(std::vector<std::string>& lines);
	void tokenizeFuncBody(std::stringstream& stream, int& i);
	void compileLine(const std::string& line);
	void stripString(std::string& str);
	void removeVariableType(std::string& str);
	void evaluateExpression(std::string& exp);
	void padOperators(std::string& input);
};