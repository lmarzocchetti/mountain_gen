# c-template
Simple CMake template for C projects using CPM

# Already included libraries
I have created this template to provide at least a minimal set of functionality to the C language.
These libraries are included (and easily swappable or removable):
- mlib: Library for Containers and more (like strings etc)
- sds: Simple library to support dynamic strings
- mimalloc: Replacement to standard malloc. I have set this to NOT replace the standard malloc, but simply import and use it
- raylib: Fairly simple OpenGL library for 2d/3d game development or graphics
- GLib: Container (and much more) library. (Use if you need more abstraction like Objects, Async, etc)
- Interface99: Macro library to have static and dynamic interfaces in C
- Datatype99: Macro library to have sum types in C
- CException: Library that implement a simple Exception mechanism (setjmp/longjmp)

For now these are the only included libraries, but it can change in the future (suggestions are appreciated)

Default Active Libraries -> mlib

# Usage
## Setup
First of all clone the template:
```
$ git clone https://github.com/lmarzocchetti/c-template
```

Rename the folder with your project name:
```
$ mv c-template your-project-name
```

Set the project name in the CMakeLists.txt file:
```
project(c-template C) -> project(your-project-name C)
```

### Vscode
If vscode complains about true and false (C23 standard) add these lines to your `.vscode/c_cpp_properties.json`
```
{
    "configurations": [
        {
            "name": "c-template",
            "defines": [
                "true=1",
                "false=0"
            ],
            "compilerPath": "path-to-your-c-compiler",
            "cStandard": "c23",
            "includePath": ["${workspaceFolder}/**"],
            "intelliSenseMode": "type-of-intellisense-based-on-compiler" 
        }
    ],
    "version": 4
}
```

## Build and Run
You can use normal CMake commands or use my script `run.sh` to build and run your project!

### With Configuration
Use conf to configure your cmake project (in Release or Debug mode): 
```
$ ./run.sh conf [Release|Debug]
```
Use conf-build to configure and build the final executable
```
$ ./run.sh conf-build [Release|Debug]
```

Use conf-run to configure, build and run the final executable
```
$ ./run.sh conf-run [Release|Debug]
```

### Without Configuration
If you already run the configuration step of cmake and want only to build or run the executable:
```
$ ./run.sh build
```

```
$ ./run.sh run
```

# Credits
- mlib: https://github.com/P-p-H-d/mlib
- sds: https://github.com/antirez/sds
- mimalloc: https://github.com/microsoft/mimalloc
- CException: https://github.com/ThrowTheSwitch/CException
- Raylib: https://github.com/raysan5/raylib
- Datatype99: https://github.com/Hirrolot/datatype99
- Interface99: https://github.com/hirrolot/interface99