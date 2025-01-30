#!/bin/bash
source ./ENV

rm -rf $ENV_EXPORT_INCLUDES_DIRPATH && mkdir -p $ENV_EXPORT_INCLUDES_DIRPATH
rm -rf $ENV_EXPORT_ENGINE_DIRPATH && mkdir -p $ENV_EXPORT_ENGINE_DIRPATH
rm -rf $ENV_EXPORT_EDITOR_STANDALONE_DIRPATH && mkdir -p $ENV_EXPORT_EDITOR_STANDALONE_DIRPATH

# Includes
cp -r ./include/coconuts2D "$ENV_EXPORT_INCLUDES_DIRPATH" 2>/dev/null
cp -r ./include/thirdparty/ "$ENV_EXPORT_INCLUDES_DIRPATH" 2>/dev/null

# Libraries and binaries
cp ./build/src/libccn2d.a "$ENV_EXPORT_ENGINE_DIRPATH/$ENV_EXPORT_ENGINE_OUTNAME" 2>/dev/null
cp ./build/src/Coconuts2D "$ENV_EXPORT_EDITOR_STANDALONE_DIRPATH/$ENV_EXPORT_EDITOR_STANDALONE_OUTNAME" 2>/dev/null
