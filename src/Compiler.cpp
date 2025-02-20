#include "Compiler.h"
#include "exprtk.hpp"

void Parser::CleanUpFile(const std::string& path)
{
	std::ifstream file(path);
	std::string line, res;

	if (!file.is_open()) {
		std::cerr << "Could not open the file: " << path << std::endl;
		return;
	}

	while (std::getline(file, line)) {
		res += line;
	}
	
	file.close();

	std::vector<std::string> symbols = { "=", "+", "-", "*", "/", "{", "}", ";", "("};
	std::ostringstream clean1;

	for (const char& character : res) {
		std::string charStr(1, character);
		
		if (std::find(symbols.begin(), symbols.end(), charStr) != symbols.end()) 
		{
			clean1 << " " << charStr << " ";
		} 
		else 
		{
			clean1 << character;
		}
	}

	std::string clean1str = clean1.str();
	char lastChar = ' ';
	std::ostringstream clean2;

	for (const char& character : clean1str) 
	{
		if (lastChar != ' ' || character != ' ') 
		{
			clean2 << character;
		}
		lastChar = character;
	}

	currentFileText = clean2.str();
}

void Parser::Tokenize()
{
	std::istringstream stream(currentFileText);
	std::string token;

	while (stream >> token)
	{
		tokens.push_back(token);
	}
}

void Parser::CompileFuncBody(int& i)
{
	std::stringstream stream;
	int maxI = 0;

	// temp, skip till first brace
	i += 3;
	while (true)
	{
		std::string token = tokens[i];
		if (token == "}")
		{
			maxI = i;
			break;
		}

		stream << token << " ";
		i++;
	}

	i = maxI + 1;

	std::vector<std::string> lines;

	for (int j = 0; j < stream.str().length(); j++)
	{
		if (stream.str()[j] == ';')
		{
			lines.push_back(stream.str().substr(0, j));
			stream.str(stream.str().substr(j + 1));
			j = 0;
		}
	}

	// remove whitepace at start and end of line
	for (int j = 0; j < lines.size(); j++)
	{
		lines[j] = lines[j].substr(lines[j].find_first_not_of(' '));
		lines[j] = lines[j].substr(0, lines[j].find_last_not_of(' ') + 1);
	}

	std::vector<std::string> variableDeclarations;

	for (int j = 0; j < lines.size(); j++)
	{
		std::string& line = lines[j];

		if (line.find("int") != std::string::npos || line.find("float") != std::string::npos)
		{
			variableDeclarations.push_back(line);
		}
	}

	std::map<std::string, std::pair<uint8_t, std::string>> variableMap;

	for (int j = 0; j < variableDeclarations.size(); j++)
	{
		std::string& line = variableDeclarations[j];

		size_t varNameStart = line.find(' ') + 1;
		size_t varNameEnd = line.find('=', varNameStart);

		std::string variableName = line.substr(varNameStart, varNameEnd - varNameStart);
		variableName.erase(variableName.find_last_not_of(' ') + 1);
		variableName.erase(0, variableName.find_first_not_of(' '));


		size_t valueStart = line.find('=', varNameEnd) + 1;
		size_t valueEnd = line.find(';', valueStart);
		std::string variableValue = line.substr(valueStart, valueEnd - valueStart);
		variableValue.erase(variableValue.find_last_not_of(' ') + 1);

		variableMap[variableName] = {static_cast<uint8_t>(currentType), variableValue};
	}

	for (const auto& pair : variableMap)
	{
		const std::string& varName = pair.first;
		uint8_t varIndex = pair.second.first;
		const std::string& varValue = pair.second.second;

		std::cout << "Variable Name: " << varName
			<< ", Index: " << static_cast<int>(varIndex)
			<< ", Value: " << varValue << std::endl;

		bool containsAlphabetic = false;
		for (char c : varValue) {
			if (std::isalpha(c)) {
				containsAlphabetic = true;
				break;
			}
		}

		if (containsAlphabetic) {
			std::cout << "varValue contains alphabetic characters." << std::endl;
		}
		else {
			std::string expression = varValue;
			exprtk::parser<double> parser;
			exprtk::expression<double> expr;

			if (parser.compile(expression, expr))
			{
				double result = expr.value();
				std::cout << "Result: " << result << std::endl;
			}
			else
			{
				std::cerr << "Error: " << parser.error() << std::endl;
			}
		}
	}
}

void Parser::Compile()
{
	for (int i = 0; i < tokens.size(); i++)
	{
		std::string& token = tokens[i];

		if (token == "int") currentType = Type::Int;
		if (token == "float") currentType = Type::Float;

		if (token == "(" && lastToken != "if" && lastToken != "while")
		{
			// must be func
			currentFunction.returnType = currentType;
			if (lastToken == "main")
			{
				currentFunction.returnAddress = 0;
				currentFunction.funcAddress   = 0;
			} 
			else
			{
				currentFunction.returnAddress = (latestFuncAddress == 0 ? 1 : latestFuncAddress);
				currentFunction.funcAddress   = (latestFuncAddress == 0 ? 1 : latestFuncAddress);

				latestFuncAddress++;
			}

			CompileFuncBody(i);
			functions.push_back(currentFunction);
		}
		lastToken = token;
	}
}