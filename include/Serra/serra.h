#pragma once
#include <string>
#include <vector>
#include <angelscript.h>

class Serra
{
public:
	Serra();
	~Serra();

	size_t LoadScript(const std::string& path, const std::string& name);
	void RunScript(const size_t& index);

private:
	std::vector<asIScriptContext*> contexts;
	void initAngelScript();
};