#include "serra.h"

#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>

#include <scriptstdstring.h>
#include <scriptbuilder.h>

static asIScriptEngine* engine = nullptr;

static void MessageCallback(const asSMessageInfo* msg)
{
    switch (msg->type)
    {
    case asMSGTYPE_ERROR:
        std::cerr << "ERROR:\t";
        break;
    case asMSGTYPE_WARNING:
        std::cerr << "WARNING:\t";
        break;
    case asMSGTYPE_INFORMATION:
        std::cerr << "INFO:\t";
        break;
    }
    std::cerr << msg->section << "(" << msg->row << ", " << msg->col << "): " << msg->message << std::endl;
}

static void print(const std::string& text)
{
    std::cout << "Script IO:\t" << text << std::endl;
}

Serra::Serra()
{
    if (engine == nullptr)
    {
        initAngelScript();
    }
}

void Serra::initAngelScript()
{
    engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
    
    // set callbacks
    int r = engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);
    assert(r >= 0);

    RegisterStdString(engine);

    r = engine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(print), asCALL_CDECL);
    assert(r >= 0);
}

Serra::~Serra()
{
    if (engine == nullptr)
    {
        std::cerr << "ERROR: AngelScript engine is null when destructing" << std::endl;
        return;
    }

    for (asIScriptContext* ctx : contexts)
	{
		ctx->Release();
	}

    engine->ShutDownAndRelease();
}

size_t Serra::LoadScript(const std::string& path, const std::string& name)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << path << std::endl;
        return -1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    asIScriptModule* mod = engine->GetModule("SerraScripts", asGM_ALWAYS_CREATE);
    int r = mod->AddScriptSection(name.c_str(), buffer.str().c_str(), buffer.str().length(), 0);
    if (r < 0)
    {
        std::cerr << "Error: Failed to add script section. Code: " << r << std::endl;
        return -1;
    }

    r = mod->Build();
    if (r < 0)
    {
        std::cerr << "Error: Failed to build module. Code: " << r << std::endl;
        return -1;
    }

    asIScriptFunction* func = mod->GetFunctionByDecl("void main()");
    if (func == nullptr)
    {
        std::cerr << "Error: Function 'void main()' not found in the script." << std::endl;
        return -1;
    }

    asIScriptContext* ctx = engine->CreateContext();
    if (!ctx)
    {
        std::cerr << "Error: Could not create script context." << std::endl;
        return -1;
    }

    r = ctx->Prepare(func);
    if (r < 0)
    {
        std::cerr << "Error: Failed to prepare script. Code: " << r << std::endl;
        ctx->Release();
        return -1;
    }
    
    contexts.push_back(ctx);
    return contexts.size() - 1;
}

void Serra::RunScript(const size_t& index)
{
    if (index == -1)
    {
        std::cerr << "Error: Running script that had errors during loading" << std::endl;
        return;
    }

    int r = contexts[index]->Execute();
    if (r != asEXECUTION_FINISHED)
    {
        std::cerr << "Error: Script execution failed with code " << r << std::endl;
    }
}