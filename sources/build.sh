#!/bin/bash
if [[ ${PWD##/*/} != "sources" ]]; then
    echo "This script must be executed from the 'sources' directory."
    exit -1
else
    if [[ ! -d "../build" ]]; then
        mkdir ../build
    fi
    gcc -o ../build/crud crud.c -Wall -Werror
fi
