#include "serra.h"
#include <iostream>

int main()
{
    Serra serra;

    size_t script = serra.LoadScript("../scripts/script.serra", "script");

    serra.RunScript(script);

    return 0;
}