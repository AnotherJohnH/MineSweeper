#!/bin/bash
source ../../Platform/scripts/BUILD_EMSCRIPTEN.sh
cmake ../.. -DPLT_TARGET=Emscripten
make
rm -rf CMake* CPack* Makefile cmake* libPLT.a
