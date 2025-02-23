# SerraScript
SerraScript is a wrapper of angelScript, adding features to tightly intergrate it with my other projects and a simpler API.

# Future
Need to implement a JIT to compete with c# and luaJIT, which I dont want to interface with due to bad syntax or bad API.

# Building
To build executable, follow these steps:
## Prerequisted
- Ensure you have CMake installed
- For Windows: Visual Studio (MSVC)
- For unix-based systems: make and a c++ compiler like g++

```bash
git clone <repourl>
cd <repourl>
mkdir build
cd build
cmake ..
```

If on windows, open SerraScript.sln in the /build directory, set SerraScript as startup project in solution explorer, then building should work fine.

--- 

If on unix-based systems (linux/macos)
```bash
make
./SerraScript
```