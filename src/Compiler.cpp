#include "Compiler.h"

std::string cleanUpFile(const std::string& path)
{
    std::ifstream file(path);
    std::string line, res;

    if (!file.is_open()) {
        std::cerr << "Could not open the file: " << path << std::endl;
        return "";
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

    return clean2.str();
}