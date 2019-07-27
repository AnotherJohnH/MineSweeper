#!/bin/bash
source ../../Platform/scripts/BUILD_WEBASM.sh
cmake ../.. -DPLT_TARGET=WebAsm
make
rm -rf CMake* CPack* Makefile cmake* libPLT.a
