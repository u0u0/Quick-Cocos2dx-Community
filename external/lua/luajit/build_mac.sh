#!/bin/sh
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
SRCDIR=$DIR/src
DESTDIR=$DIR/prebuilt/mac
LIPO="lipo"
STRIP="strip"

rm "$DESTDIR"/*.a

cd $SRCDIR
export MACOSX_DEPLOYMENT_TARGET=10.9
make clean
make CC="clang -m64 -arch x86_64 -DLUAJIT_ENABLE_GC64" all
mv "$SRCDIR"/src/libluajit.a "$DESTDIR"/libluajit.a

make clean
