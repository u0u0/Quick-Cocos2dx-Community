#!/bin/sh
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
SRCDIR=$DIR/src
DESTDIR=$DIR/prebuilt/linux

rm "$DESTDIR"/*.a

cd $SRCDIR
make clean
make CC="clang -m64 -DLUAJIT_ENABLE_GC64" all
# make CC="clang -m32" all
mv "$SRCDIR"/src/libluajit.a "$DESTDIR"/libluajit.a

make clean
