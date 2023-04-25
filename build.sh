#!/bin/sh                                                                                                                                                                                                     
# Copyright (c) 2013-2016 Americancoin 2013 developers

export CFLAGS="-march=native -mtune=native -O3"
export CXXFLAGS="-march=native -mtune=native -O3"
export DEBUG_CXXFLAGS=""

./autogen.sh

./configure  --without-gui --with-incompatible-bdb --without-miniupnpc --disable-tests

echo ""
echo ""
echo ""
echo "Type make command"
