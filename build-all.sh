#!/bin/bash
# Build script for building everything

set echo on

echo "Building everything..."

pushd engine
source build.sh
popd

ERRORLEVEL=$?
if [ $ERRORLEVEL -ne 0 ]
then
    echo "Error:"$ERRORLEVEL &&N exit
fi

pushd game
source build.sh
popd
ERRORLEVEL=$?
if [ $ERRORLEVEL -ne 0 ]
then
    echo "Error:"$ERRORLEVEL &&N exit
fi

echo "All assemblies built successfully"
