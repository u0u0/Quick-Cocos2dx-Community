#!/bin/sh
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
LIPO="xcrun -sdk iphoneos lipo"
STRIP="xcrun -sdk iphoneos strip"

SRCDIR=$DIR/src
DESTDIR=$DIR/prebuilt/mac

IXCODE=`xcode-select -print-path`
ISDK=$IXCODE/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk

rm "$DESTDIR"/*.a

cd $SRCDIR
export MACOSX_DEPLOYMENT_TARGET=10.9

make clean
make CC="clang -m64 -arch x86_64" all
mv "$SRCDIR"/src/libluajit.a "$DESTDIR"/libluajit-x86_64.a

make clean
make HOST_CC="clang" TARGET_FLAGS="-arch arm64 -isysroot $ISDK" TARGET=arm64 TARGET_SYS=Darwin
mv "$SRCDIR"/src/libluajit.a "$DESTDIR"/libluajit-arm64.a

make clean
$LIPO -create "$DESTDIR"/libluajit-*.a -output "$DESTDIR"/libluajit.a
$STRIP -S "$DESTDIR"/libluajit.a
$LIPO -info "$DESTDIR"/libluajit.a

rm "$DESTDIR"/libluajit-*.a

make clean
