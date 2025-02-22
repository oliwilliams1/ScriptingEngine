#include "Compiler.h"
#include "parserWrapper.h"

std::string Compiler::addCharsUpToComment(const std::string& line)
{
	size_t commentPos = line.find("//");
	if (commentPos != std::string::npos) 
	{
		return line.substr(0, commentPos);
	}
	return line;
}

void Compiler::CleanUpFile(const std::string& path)
{
	std::ifstream file(path);
	std::string line, res;

	if (!file.is_open()) {
		std::cerr << "Could not open the file: " << path << std::endl;
		return;
	}

	while (std::getline(file, line)) {
		res += addCharsUpToComment(line);
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

void Compiler::Tokenize()
{
	std::istringstream stream(currentFileText);
	std::string token;

	while (stream >> token)
	{
		tokens.push_back(token);
	}
}

void Compiler::trimLines(std::stringstream& ss, std::vector<std::string>& lines)
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

void Compiler::stripString(std::string& str)
{
	str.erase(str.find_last_not_of(' ') + 1);
	str.erase(0, str.find_first_not_of(' '));
}

void Compiler::removeVariableType(std::string& str)
{
	std::string types[] = { "int", "float" };

	for (const std::string& type : types) 
	{
		size_t pos;
		while ((pos = str.find(type)) != std::string::npos) 
		{
			str.erase(pos, type.length());
		}
	}
}

void Compiler::padOperators(std::string& input)
{
	std::vector<std::string> operators = { "+", "-", "*", "/", "(", ")" };

	for (const auto& op : operators)
	{
		std::string replacement = " " + op + " ";
		size_t pos = 0;

		while ((pos = input.find(op, pos)) != std::string::npos) {
			input.replace(pos, op.length(), replacement);
			pos += replacement.length();
		}
	}

	input.erase(std::unique(input.begin(), input.end(), [](char a, char b) {
		return a == ' ' && b == ' ';
		}), input.end());

	stripString(input);
}

void Compiler::variableRealization(std::vector<std::string>& lines)
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

        Type variableType;
        if (line.find("int") != std::string::npos) {
            variableType = Type::Int;
        } else if (line.find("float") != std::string::npos) {
            variableType = Type::Float;
        }

        variableMap[variableName] = { static_cast<uint16_t>(index), variableValue, variableType };
        index++;
    }

    currentFunction.functionScope.resize(variableMap.size());

    // preset const variables
    for (const auto& pair : variableMap)
    {
        const std::string& varName = pair.first;
        const VariableInfo& varInfo = pair.second;
        uint16_t varIndex = varInfo.index;
        const std::string& varValue = varInfo.value;
        Type variableType = varInfo.variableType;

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
            if (variableType == Type::Int)
            {
                int res = parseExpression(varValue);
                currentFunction.functionScope[varIndex] = *(uint32_t*)&res;
            }
            else // float
            {
                float res = parseExpression(varValue);
                currentFunction.functionScope[varIndex] = *(uint32_t*)&res;
            }
        }
    }
}

void Compiler::tokenizeFuncBody(std::stringstream& stream, int& i)
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

std::vector<std::string> Compiler::preProcessExpression(std::string& exp)
{
	std::string expression = exp;
	padOperators(expression);

	// Split by ' '
	std::vector<std::string> tokens;
	std::stringstream stream(expression);
	std::string token;

	while (std::getline(stream, token, ' '))
	{
		tokens.push_back(token);
	}

	std::vector<std::string> operators = { "+", "-", "*", "/", "(", ")" };

	for (std::string& token : tokens)
	{
		// std::cout << token << " ";
		bool isVar = false;

		if (std::find(operators.begin(), operators.end(), token) != operators.end())
		{
			continue;
		}

		for (const char c : token)
		{
			if (std::isalpha(c))
			{
				isVar = true;
				break;
			}
		}

		if (!isVar) // token is a constant, upload to reg
		{
			uint32_t value;
			if (currentExpressionType == Type::Int)
			{
				int res = std::stoi(token);
				value = *(uint32_t*)&res;
			}
			else // float
			{
				float res = std::stof(token);
				value = *(uint32_t*)&res;
			}
			std::cout << value << std::endl;

			uint16_t reg = minRegister++;
			uint16_t part1 = (value >> 16) & 0xFFFF;
			uint16_t part2 = value & 0xFFFF;

			currentFunction.code.push_back((uint16_t)Inst::SetRegister);
			currentFunction.code.push_back(reg);
			currentFunction.code.push_back(part1);
			currentFunction.code.push_back(part2);

			token = "$" + std::to_string((uint16_t)65535 - reg);
		}
		else // token is a variable
		{
			std::string varName = token;
			varName.erase(varName.find_last_not_of(' ') + 1);
			varName.erase(0, varName.find_first_not_of(' '));

			if (variableMap.find(varName) == variableMap.end())
			{
				std::cout << "Variable not found: " << varName << " line: " << __LINE__ << std::endl;
				exit(1);
			}

			uint16_t varIndex = variableMap[varName].index;
			token = "$" + std::to_string(varIndex);
		}
	}

	/*std::cout << " => ";

	for (const std::string& token : tokens)
	{
		std::cout << token << " ";
	}*/

	return tokens;
}

void Compiler::evaluateExpression(std::string& exp)
{
	std::vector<std::string> tokens = preProcessExpression(exp);
}

void Compiler::compileLine(const std::string& line)
{
	if (line.find('=') == std::string::npos)
	{
		std::cout << "not compiling your stupid line: " << line << std::endl;
		return;
	}

	std::string LHS = line.substr(0, line.find('='));
	
	if (LHS.substr(0, 3) == "int")
	{
		currentExpressionType = Type::Int;
	}
	else if (LHS.substr(0, 5) == "float")
	{
		currentExpressionType = Type::Float;
	}
	else
	{
		std::cout << "boiy what kinda line are you tryna compiole?!, no expression type found on line: " << line << "error on compliler line:" << __LINE__ << std::endl;
	}
	removeVariableType(LHS);
	stripString(LHS);

	if (variableMap.find(LHS) == variableMap.end())
	{
		std::cout << "LHS of line not a variable: " << line << std::endl;
		return;
	}

	std::string RHS = line.substr(line.find('=') + 1);

	bool containsVar = false;
	for (const char& c : RHS)
	{
		if (std::isalpha(c))
		{
			std::vector<char> operators = { '+', '-', '*', '/', '(', ')' };
			
			bool isOp = false;
			for (const char& op : operators)
			{
				if (c == op)
				{
					isOp = true;
				}
			}
			
			if (isOp == false)
			{
				containsVar = true;
				break;
			}
		}
	}
	
	if (containsVar)
	{
		evaluateExpression(RHS);
	}
}

void Compiler::compileFuncBody(int& i)
{
	std::stringstream stream;
	tokenizeFuncBody(stream, i);

	std::vector<std::string> lines;
	trimLines(stream, lines);

	variableMap.clear();
	variableRealization(lines);

	minRegister = 0;
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
				currentFunction.code.push_back(variableMap[variableName].index);
			} 
			else
			{
				std::cout << "unknown return variable name: " << variableName << std::endl;
			}

			break;
		}

		compileLine(line);
	}
}

void Compiler::Compile()
{
	for (int i = 0; i < tokens.size(); i++)
	{
		std::string& token = tokens[i];

		if (token == "int") currentFuncType = Type::Int;
		if (token == "float") currentFuncType = Type::Float;

		if (token == "(" && lastToken != "if" && lastToken != "while")
		{
			// must be func
			currentFunction.returnType = currentFuncType;
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

			compileFuncBody(i);
			functions.push_back(currentFunction);
			currentFunction = Function();
			inFunc = false;
		}
		lastToken = token;
	}
}