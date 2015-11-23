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
make CC="gcc -m64 -arch x86_64" all
mv "$SRCDIR"/src/libluajit.a "$DESTDIR"/libluajit-x86_64.a

make clean
make CC="gcc -m32 -arch i386" all
mv "$SRCDIR"/src/libluajit.a "$DESTDIR"/libluajit-i386.a

make clean
$LIPO -create "$DESTDIR"/libluajit-*.a -output "$DESTDIR"/libluajit.a
$STRIP -S "$DESTDIR"/libluajit.a
$LIPO -info "$DESTDIR"/libluajit.a

rm "$DESTDIR"/libluajit-*.a

make clean
