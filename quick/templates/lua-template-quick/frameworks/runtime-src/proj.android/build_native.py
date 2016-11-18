#!/usr/bin/env python
# -*- coding: UTF-8 -*-
"""
NAME
    build_native --

SYNOPSIS
    build_native [-h] [-r] [-c]

    -h show help
    -r release mode
    -c clean binary files
"""

import os
import sys
import getopt
import subprocess
import shutil
import platform

projectRoot = os.path.split(os.path.realpath(__file__))[0]
runtimeRoot = os.path.split(projectRoot)[0]
frameworksRoot = os.path.split(runtimeRoot)[0]
appRoot = os.path.split(frameworksRoot)[0]
engineRoot = os.environ.get('QUICK_V3_ROOT')
new_env = os.environ.copy()
new_env['COCOS2DX_ROOT'] = engineRoot

ndkPathDepart = ":"
if(platform.system() =="Windows"):
    ndkPathDepart = ";"
ndkModulePath = engineRoot + ndkPathDepart + engineRoot + "/cocos" + ndkPathDepart + engineRoot + "/quick/lib" + ndkPathDepart + engineRoot + "/external" + ndkPathDepart + engineRoot + "/cocos/scripting" + ndkPathDepart + runtimeRoot + "/Classes"

def joinDir(root, *dirs):
    for item in dirs:
        root = os.path.join(root, item)
    return root

def buildNative(isRelease):
    # check ndk env
    print "====> checking for ndk-build commond\n"
    cmd = subprocess.Popen('ndk-build -v', shell=True, stdout=subprocess.PIPE)
    cmd.wait()
    if cmd.returncode != 0:
        print "Error: ndk-build commond not found!! \
                \n** Please set you Android NDK root to your system Path **"
        return

    # nkd-build
    print "====> start building library\n"
    cmd = subprocess.Popen('ndk-build NDK_DEBUG=%s NDK_MODULE_PATH=%s' \
            %((0 if isRelease else 1), ndkModulePath), shell=True, env=new_env)
    cmd.wait()
    if cmd.returncode != 0:
        print "Error while building, check error above!"
        return

    # mv *.so to 'src/main/libcocos2dx'
    libsDir = joinDir(projectRoot, "libcocos2dx", "libs")
    jniLibsDir = joinDir(projectRoot, "libcocos2dx", "src", "main", "libcocos2dx")

    print "====> Moveing *.so to jniLibs\n"
    if os.path.exists(jniLibsDir):
        shutil.rmtree(jniLibsDir)
    shutil.move(libsDir, jniLibsDir)

    print "====> Copying src,res to Android asserts\n"
    androidAssertsDir = joinDir(projectRoot, "app", "src", "main", "assets")
    resDir = joinDir(appRoot, "res")
    resTargetDir = joinDir(androidAssertsDir, "res")
    if os.path.exists(resTargetDir):
        shutil.rmtree(resTargetDir)
    shutil.copytree(resDir, resTargetDir)

    srcDir = joinDir(appRoot, "src")
    resTargetDir = joinDir(androidAssertsDir, "src")
    if os.path.exists(resTargetDir):
        shutil.rmtree(resTargetDir)
    shutil.copytree(srcDir, resTargetDir)

    print "====> Build Done\n"

def cleanNative():
    print "====> start cleaning\n"
    cmd = subprocess.Popen('ndk-build clean NDK_MODULE_PATH=%s' \
            %(ndkModulePath), shell=True, env=new_env)
    cmd.wait()
    if cmd.returncode != 0:
        print "Error while cleaning, check error above!"
        return

    objDir = joinDir(projectRoot, "libcocos2dx", "obj")
    if os.path.exists(objDir):
        shutil.rmtree(objDir)

    print "====> clean done\n"

if __name__ == "__main__":
    isRelease = False
    isClean = False

    # ===== parse args =====
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hrc")
    except getopt.GetoptError:
        # print help information and exit:
        print __doc__
        sys.exit(-2)

    for o, a in opts:
        if o == "-h":
            # print help information and exit:
            print __doc__
            sys.exit(0)
        if o == "-r":
            isRelease = True
        if o == "-c":
            isClean = True

    jniDir = joinDir(projectRoot, "libcocos2dx", "jni")
    if False == os.path.exists(jniDir):
        print "Error: %s is not a project directory" %(projectRoot)
        sys.exit(-2)

    # info printing
    print "engineRoot: %s" %(engineRoot)
    print "appRoot: %s" %(appRoot)

    # run build
    os.chdir(jniDir) # change work dir to jni root
    if isClean:
        cleanNative()
    else:
        buildNative(isRelease)
