#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

SRCDIR=${DIR}/src
cd "${SRCDIR}"

orgPath=${PATH}

# ubuntu cross build 32 bit, `sudo apt-get install libc6-dev-i386`

toolchainPath=/home/u0u0/works/android/android-ndk-r20b/build/tools/make-standalone-toolchain.sh
installPrefix=/home/u0u0/works/android

archs=("arm" "x86" "arm64")
dests=("armeabi-v7a" "x86" "arm64-v8a")
hostBits=("32" "32" "64")
apis=("16" "16" "21")
crossPrefixs=("arm-linux-androideabi-" "i686-linux-android-" "aarch64-linux-android-")

for ((i=0;i<${#archs[@]};i++)) do
    arch=${archs[i]}
    dest=${dests[i]}
    hostBit=${hostBits[i]}
    api=${apis[i]}
    crossPrefix=${crossPrefixs[i]}
    installRoot=${installPrefix}/standalone-toolchain-${arch}

    if [ ! -d "${installRoot}" ];then
        echo "Creating standalone: ${installRoot}"
        ${toolchainPath} --arch=${arch} --platform=${api} --stl=libc++ --install-dir=${installRoot}
    fi

    # standalone setting
    export PATH="${installRoot}/bin:${orgPath}"
    # armv7 special fix
    TFLAGS="--sysroot ${installRoot}/sysroot"
    if [ ${dest} == "armeabi-v7a" ]; then
        TFLAGS+=" -march=armv7-a -mfloat-abi=softfp"
    fi;
    # start building
    make clean
    make CC=clang HOST_CC="gcc -m${hostBit}" CROSS=${crossPrefix} TARGET_SYS=Linux TARGET_FLAGS="$TFLAGS"

    if [ -f ${SRCDIR}/src/libluajit.a ]; then
        DESTDIR=${DIR}/prebuilt/android/${dest}
        rm ${DESTDIR}/*.a
        mv ${SRCDIR}/src/libluajit.a ${DESTDIR}/libluajit.a
    fi;
done;

make clean
