#include "Compiler.h"
#include "parserWrapper.h"

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

void Parser::trimLines(std::stringstream& ss, std::vector<std::string>& lines)
{
	for (int j = 0; j < ss.str().length(); j++)
	{
		if (ss.str()[j] == ';')
		{
			lines.push_back(ss.str().substr(0, j));
			ss.str(ss.str().substr(j + 1));
			j = 0;
		}
	}

	// remove whitepace at start and end of line
	for (int j = 0; j < lines.size(); j++)
	{
		lines[j] = lines[j].substr(lines[j].find_first_not_of(' '));
		lines[j] = lines[j].substr(0, lines[j].find_last_not_of(' ') + 1);
	}
}

void Parser::variableRealization(std::vector<std::string>& lines, std::unordered_map<std::string, std::pair<uint16_t, std::string>>& variableMap)
{
	std::vector<std::string> variableDeclarations;

	for (int j = 0; j < lines.size(); j++)
	{
		std::string& line = lines[j];

		if (line.find("int") != std::string::npos || line.find("float") != std::string::npos)
		{
			variableDeclarations.push_back(line);
		}
	}

	int index = 0;

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

		variableMap[variableName] = { index, variableValue };
		index++;
	}

	currentFunction.functionScope.resize(variableMap.size());

	// preset const variables
	for (const auto& pair : variableMap)
	{
		const std::string& varName = pair.first;
		uint16_t varIndex = pair.second.first;
		const std::string& varValue = pair.second.second;

		bool containsAlpha = false;
		for (char c : varValue)
		{
			if (std::isalpha(c))
			{
				containsAlpha = true;
				break;
			}
		}

		if (!containsAlpha)
		{
			float res = parseExpression(varValue);

			if (currentType == Type::Int)
			{
				int ires = (int)res;
				currentFunction.functionScope[varIndex] = *(uint32_t*)&ires;
			}
			else // float
			{
				currentFunction.functionScope[varIndex] = *(uint32_t*)&res;
			}
		}
	}
}

void Parser::tokenizeFuncBody(std::stringstream& stream, int& i)
{
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
}

void compileLine(const std::string& line, std::unordered_map<std::string, std::pair<uint16_t, std::string>>& variableMap)
{

}

void Parser::CompileFuncBody(int& i)
{
	std::stringstream stream;
	tokenizeFuncBody(stream, i);

	std::vector<std::string> lines;
	trimLines(stream, lines);

	std::unordered_map<std::string, std::pair<uint16_t, std::string>> variableMap;
	variableRealization(lines, variableMap);

	for (const std::string& line : lines)
	{
		// handle speical things
		if (line.substr(0, 6) == "return")
		{
			std::string variableName = line.substr(6);

			variableName.erase(0, variableName.find_first_not_of(' '));

			variableName.erase(variableName.find_last_not_of(' ') + 1);

			currentFunction.code.push_back((uint16_t)Inst::Return);

			if (variableMap.find(variableName) != variableMap.end())
			{
				currentFunction.code.push_back(variableMap[variableName].first);
			} 
			else
			{
				std::cout << "unknown return variable name: " << variableName << std::endl;
			}

			break;
		}

		compileLine(line, variableMap);
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
			currentFunction = Function();
			inFunc = false;
		}
		lastToken = token;
	}
}