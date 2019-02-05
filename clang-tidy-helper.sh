#!/bin/bash

rm -rf ct-build
mkdir ct-build
cd ct-build
cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_CXX_CLANG_TIDY:STRING="clang-tidy;-fix;-checks=*,-google-readability-braces-around-statements,-readability-braces-around-statements,-llvm-header-guard,-google-runtime-references,-google-runtime-int;-header-filter=.*" ..
make
