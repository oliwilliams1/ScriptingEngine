#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
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

	void trimLines(std::stringstream& ss, std::vector<std::string>& lines);
	void variableRealization(std::vector<std::string>& lines, std::unordered_map<std::string, std::pair<uint16_t, std::string>>& variableMap);
	void tokenizeFuncBody(std::stringstream& stream, int& i);
	void compileLine(const std::string& line, std::unordered_map<std::string, std::pair<uint16_t, std::string>>& variableMap);
};