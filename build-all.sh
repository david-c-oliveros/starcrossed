#!/bin/bash
# Build script for building everything

set echo on

echo "Building everything..."

source build_engine.sh

ERRORLEVEL=$?
if [ $ERRORLEVEL -ne 0 ]
then
    echo "Error:"$ERRORLEVEL &&N exit
fi

echo "All assemblies built successfully"
