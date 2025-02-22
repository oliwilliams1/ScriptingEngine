#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include "VM.h"

// chargpt
struct VariableInfo {
    uint16_t index;
    std::string value;
    Type variableType;

    VariableInfo() : index(0), value(""), variableType(Type::Int) {}

    VariableInfo(uint16_t idx, const std::string& val, Type type)
        : index(idx), value(val), variableType(type) {}

    VariableInfo& operator=(const VariableInfo& other) {
        if (this != &other) {
            index = other.index;
            value = other.value;
            variableType = other.variableType;
        }
        return *this;
    }

    VariableInfo& operator=(VariableInfo&& other) noexcept {
        if (this != &other) {
            index = other.index;
            value = std::move(other.value);
            variableType = other.variableType;
        }
        return *this;
    }

    VariableInfo(const VariableInfo& other)
        : index(other.index), value(other.value), variableType(other.variableType) {}

    VariableInfo(VariableInfo&& other) noexcept
        : index(other.index), value(std::move(other.value)), variableType(other.variableType) {}
};


class Compiler
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
	Type currentExpressionType;

	Type currentFuncType;
	Function currentFunction;
	bool inFunc = false;
	int latestFuncAddress = 0;

	uint16_t resultRegister;
	uint16_t minRegister;

	std::string lastToken;
	std::string currentFileText;
	std::vector<std::string> tokens;

	std::unordered_map<std::string, VariableInfo> variableMap;

	std::string addCharsUpToComment(const std::string& line);
	void trimLines(std::stringstream& ss, std::vector<std::string>& lines);
	void variableRealization(std::vector<std::string>& lines);
	void tokenizeFuncBody(std::stringstream& stream, int& i);
	void compileLine(const std::string& line);
	void stripString(std::string& str);
	void removeVariableType(std::string& str);
	std::vector<std::string> preProcessExpression(std::string& exp);
	void evaluateExpression(std::string& exp);
	void padOperators(std::string& input);
};