#!/usr/bin/env python
# -*- coding: UTF-8 -*-
"""
NAME
    PackageScripts --

SYNOPSIS
    PackageScripts [-h]

    -h show help
    -p project root dir
    -o output file name, like "game", will auto composed to game32.zip
    -b 32 or 64, luajit bytecode mode
"""

import os
import sys
import getopt
import shutil
import re
import platform
import subprocess
import zipfile

scriptRoot = os.path.split(os.path.realpath(__file__))[0]
engineRoot = os.environ.get('QUICK_V3_ROOT')
jitPath = ""
new_env = os.environ.copy()
luaFiles = []

def joinDir(root, *dirs):
    for item in dirs:
        root = os.path.join(root, item)
    return root

def initJitPath(mode):
    global jitPath
    global new_env
    sysstr = platform.system()
    if(sysstr =="Windows"):
        if "64" == mode:
            print "Error: mode 64 not support on win32"
            sys.exit(-1)
        jitPath = joinDir(scriptRoot, "win32", "luajit.exe")
    elif(sysstr == "Linux"):
        print "Liunux Support is coming sooooon"
        sys.exit(-1)
    elif(sysstr == "Darwin"):
        jitPath = joinDir(scriptRoot, "mac", "luajit")
        if "64" == mode:
            jitPath = jitPath + "64"
    else:
        print "Unsupport OS!"
        sys.exit(-1)

    # important, to find luajit lua
    new_env['LUA_PATH'] = joinDir(scriptRoot, "?.lua")

def doFile(path, luaRoot, zFile):
    tmp = path + ".tmp"
    jitcmd = '%s -bg "%s" "%s"' %(jitPath, path, tmp)
    # do shell cmd
    cmd = subprocess.Popen(jitcmd, shell = True, stdout = subprocess.PIPE, env = new_env)
    cmd.wait()
    if os.path.exists(tmp) == False:
        print "Error: Fail to compile:%s" %(path)
        sys.exit(-1)

    # remove perfix and suffix, replace / with .
    modulePath = path[len(luaRoot) + 1:-4]
    if os.sep == '\\':
        moduleName = re.sub('\\\\', '.', modulePath)
    else:
        moduleName = re.sub('/', '.', modulePath)
    print "== compiling: %s" %(moduleName)
    # fix the file modify time
    os.utime(tmp, (1330712280, 1330712280))
    # add to zip file
    zFile.write(tmp, moduleName)
    # remove temp file
    os.unlink(tmp)

def listDir(d):
    global luaFiles
    for item in os.listdir(d):
        nDir = joinDir(d, item)
        if os.path.isfile(nDir):
            if not nDir.endswith(".lua"): # only do lua file
                continue
            luaFiles.append(nDir)
        else:
            listDir(nDir)

def packageScript(projectDir, outName, mode):
    global luaFiles
    luaRoot = joinDir(projectDir, "src")
    if not os.path.exists(luaRoot):
        print "Error: %s is not exists" %(luaRoot)
        sys.exit(-2)

    print "====> init evn."
    initJitPath(mode)
    listDir(luaRoot) # Can't delete file in listdir

    print "====> Compile btyecode, and do ziping."
    outfile = joinDir(projectDir, "res", outName + mode + ".zip")
    zFile = zipfile.ZipFile(outfile, 'w')
    for item in luaFiles:
        doFile(item, luaRoot, zFile)
    zFile.close()
    print "====> Done, write to %s" %(outfile)

if __name__ == "__main__":
    # ===== parse args =====
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hp:o:b:")
    except getopt.GetoptError:
        # print help information and exit:
        print __doc__
        sys.exit(-2)

    # default value
    projectDir = ""
    name = "game"
    mode = "32"

    for o, a in opts:
        if o == "-h":
            # print help information and exit:
            print __doc__
            sys.exit(0)
        if o == "-p":
            projectDir = a
        if o == "-o":
            name = a
        if o == "-b":
            if "32" != a and "64" != a:
                print "Error: bytecode mode only support 32 or 64"
                sys.exit(-2)
            mode = a

    # check valid
    if 0 == len(projectDir):
        print "Error: -p must set"
        sys.exit(-2)
    if not os.path.exists(projectDir):
        print "Error: %s is not exists" %(projectDir)
        sys.exit(-2)

    # info printing
    print "== engineRoot: %s" %(engineRoot)
    packageScript(projectDir, name, mode)
