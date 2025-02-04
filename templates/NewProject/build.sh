#!/bin/bash

source ./ENV
export ENGINE="${ENV_COCONUTS2D_EXPORT_ENGINE_DIRPATH}${ENV_COCONUTS2D_EXPORT_ENGINE_OUTNAME}"

rm -rf build
mkdir -p build

cd build
cmake ..
make
cd ..
