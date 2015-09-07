#!/bin/bash
# 
# Usage:
#   export NDK_ROOT=/path/to/NDK-r9b
#   ./test.sh

# exit this script if any commmand fails
set -e

# read user.cfg if it exists and is readable

_CFG_FILE=$(dirname "$0")"/user.cfg"
if [ -f "$_CFG_FILE" ];
then
    if [ ! -r "$_CFG_FILE" ]; then
       echo "Fatal Error: $_CFG_FILE exists but is unreadable"
       exit 1
    fi
fi

# paths

if [ -z "${NDK_ROOT+aaa}" ]; then
# ... if NDK_ROOT is not set, use "$HOME/bin/android-ndk"
    NDK_ROOT="$HOME/bin/android-ndk"
fi

if [ -z "${PYTHON_BIN+aaa}" ]; then
# ... if PYTHON_BIN is not set, use "/usr/bin/python2.7"
    PYTHON_BIN="/usr/bin/python2.7"
fi

# find current dir
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# paths with defaults hardcoded to relative paths

if [ -z "${CXX_GENERATOR_ROOT+aaa}" ]; then
    CXX_GENERATOR_ROOT="$DIR/.."
fi


echo "Paths"
echo "    NDK_ROOT: $NDK_ROOT"
echo "    PYTHON_BIN: $PYTHON_BIN"
echo "    CXX_GENERATOR_ROOT: $CXX_GENERATOR_ROOT"
echo "    TO_JS_ROOT: $TO_JS_ROOT"

# check NDK version, must be r9b
if ! grep -q r9b $NDK_ROOT/RELEASE.TXT
then
    echo " Fatal Error: NDK r9b must be required!"
    exit 1
fi

# check clang include path
OS_NAME=$('uname')
NDK_LLVM_ROOT=$NDK_ROOT/toolchains/llvm-3.3/prebuilt
case "$OS_NAME" in
    Darwin | darwin)
        echo "in darwin"
        if [ -d "$NDK_LLVM_ROOT/darwin-x86_64" ]; then
            NDK_LLVM_ROOT=$NDK_LLVM_ROOT/darwin-x86_64
        elif [ -d "$NDK_LLVM_ROOT/darwin-x86" ]; then
            NDK_LLVM_ROOT=$NDK_LLVM_ROOT/darwin-x86
        else
            echo $NDK_LLVM_ROOT
            echo " Fatal Error: $NDK_LLVM_ROOT doesn't contains prebuilt llvm 3.3"
            exit 1
        fi
        ;;
    Linux | linux)
        echo "in linux"
        if [ -d "$NDK_LLVM_ROOT/linux-x86_64" ]; then
            NDK_LLVM_ROOT=$NDK_LLVM_ROOT/linux-x86_64
        elif [ -d "$NDK_LLVM_ROOT/linux-x86" ]; then
            NDK_LLVM_ROOT=$NDK_LLVM_ROOT/linux-x86
        else
            echo " Fatal Error: $NDK_LLVM_ROOT doesn't contains prebuilt llvm 3.3"
            exit 1
        fi
        ;;
    *)
        echo " Fatal Error: Please run this script in linux or mac osx."
        exit 1
        ;;
esac


# write userconf.ini

_CONF_INI_FILE="$PWD/userconf.ini"
if [ -f "$_CONF_INI_FILE" ]
then
    rm "$_CONF_INI_FILE"
fi

_CONTENTS=""
_CONTENTS+="[DEFAULT]"'\n'
_CONTENTS+="androidndkdir=$NDK_ROOT"'\n'
_CONTENTS+="clangllvmdir=$NDK_LLVM_ROOT"'\n'
_CONTENTS+="cxxgeneratordir=$CXX_GENERATOR_ROOT"'\n'
_CONTENTS+="extra_flags="'\n'

echo 
echo "generating userconf.ini..."
echo ---
echo -e "$_CONTENTS"
echo -e "$_CONTENTS" > "$_CONF_INI_FILE"
echo ---

# Generate bindings for cocos2dx
echo "Generating bindings for cocos2dx..."
set -x

LD_LIBRARY_PATH=${CXX_GENERATOR_ROOT}/libclang $PYTHON_BIN ${CXX_GENERATOR_ROOT}/generator.py ${CXX_GENERATOR_ROOT}/test/test.ini -t spidermonkey -s testandroid -o ./simple_test_bindings
