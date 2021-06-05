#!/bin/sh
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
SRCDIR=$DIR/src
DESTDIR=$DIR/prebuilt/linux

rm "$DESTDIR"/*.a

cd $SRCDIR
make clean
make CC="clang -m64" all
# make CC="clang -m32 -DLUAJIT_DISABLE_GC64" all
mv "$SRCDIR"/src/libluajit.a "$DESTDIR"/libluajit.a

make clean
