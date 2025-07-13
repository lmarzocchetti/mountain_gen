#!/bin/sh

# Check Number of Arguments
if [[ "$#" -gt 2 || "$#" -lt 1 ]]; then
    echo "Usage: $0 {conf|conf-build|conf-run} {Debug|Release}"
    echo "Usage: $0 {build|run}"
    exit 1
fi

command=$1

if [[ "$#" -eq 2 ]]; then
    configuration=$2
fi

# Verify parameters
if [[ "$#" -eq 1 ]]; then
    if [[ "$command" != "build" && "$command" != "run" ]]; then
        echo "Error: The first parameter must be 'build' or 'run'."
        exit 1
    fi
else
    if [[ "$command" != "conf" && "$command" != "conf-build" && "$command" != "conf-run" ]]; then
        echo "Error: The first parameter must be 'conf', 'conf-build' or 'conf-run'."
        exit 1
    fi

    if [[ "$configuration" != "Debug" && "$configuration" != "Release" ]]; then
        echo "Error: The second parameter must be 'Debug' or 'Release'."
        exit 1
    fi
fi

project_name=$(grep "project(" CMakeLists.txt | cut -d'(' -f2 | cut -d' ' -f1)

# Command execution
if [[ "$#" -eq 1 ]]; then
    case "$command" in
        build)
            cmake --build build
            ;;
        run)
            cmake --build build
            ./build/$project_name
            ;;
    esac
else
    case "$command" in
        conf)
            cmake -S . -B build -DCMAKE_BUILD_TYPE=$configuration
            ;;
        conf-build)
            cmake -S . -B build -DCMAKE_BUILD_TYPE=$configuration
            cmake --build build
            ;;
        conf-run)
            cmake -S . -B build -DCMAKE_BUILD_TYPE=$configuration
            cmake --build build
            ./build/$project_name
            ;;
    esac
fi
