#!/bin/sh
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
SRCDIR=$DIR/src
DESTDIR=$DIR/prebuilt/mac
#MACOSX_DEPLOYMENT_TARGET="10.6"
LIPO="lipo"
STRIP="strip"

rm "$DESTDIR"/*.a

cd $SRCDIR
make clean
make CC="gcc -arch x86_64" all
mv "$SRCDIR"/src/libluajit.a "$DESTDIR"/libluajit.a

make clean
