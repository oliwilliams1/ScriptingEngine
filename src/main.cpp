#include <iostream>
#include "angelscript.h"
#include "scriptstdstring.h"
#include "scriptbuilder.h"
#include <cassert>

// Message callback function
void MessageCallback(const asSMessageInfo* msg)
{
    switch (msg->type)
    {
    case asMSGTYPE_ERROR:
        std::cerr << "ERROR: ";
        break;
    case asMSGTYPE_WARNING:
        std::cerr << "WARNING: ";
        break;
    case asMSGTYPE_INFORMATION:
        std::cerr << "INFO: ";
        break;
    }
    std::cerr << msg->section << "(" << msg->row << ", " << msg->col << "): " << msg->message << std::endl;
}

// Print function
void print(const std::string& text)
{
    std::cout << text << std::endl;
}

int main()
{
    asIScriptEngine* engine = asCreateScriptEngine();

    // Set the message callback
    int r = engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);
    assert(r >= 0);

    // Register the standard string type
    RegisterStdString(engine);

    // Register the print function
    r = engine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(print), asCALL_CDECL);
    assert(r >= 0);

    // Create a script to execute
    const char* script = R"(
        void main() {
            print("Hello from AngelScript!");
        }
    )";

    // Compile the script
    asIScriptModule* mod = engine->GetModule("MyModule", asGM_ALWAYS_CREATE);
    r = mod->AddScriptSection("HelloWorld", script);
    assert(r >= 0);

    // Build the script to generate bytecode
    r = mod->Build();
    assert(r >= 0);

    // Get the function that we want to call
    asIScriptFunction* func = mod->GetFunctionByDecl("void main()");
    assert(func != nullptr);

    // Execute the script
    asIScriptContext* ctx = engine->CreateContext();
    r = ctx->Prepare(func);
    assert(r >= 0);
    r = ctx->Execute();
    assert(r >= 0);

    // Clean up
    ctx->Release();
    engine->ShutDownAndRelease();

    return 0;
}