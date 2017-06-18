#!/usr/bin/env python
# -*- coding: UTF-8 -*-
"""
NAME
    EncodeRes --

SYNOPSIS
    EncodeRes [-h]

    -h show help
    -p project root
    -s sign
    -k key file
"""

import os
import sys
import getopt
import shutil
import re
import platform
# import subprocess

scriptRoot = os.path.split(os.path.realpath(__file__))[0]
engineRoot = os.environ.get('QUICK_V3_ROOT')

# load xxtea
sysstr = platform.system()
if(sysstr =="Windows"):
    from win32 import xxtea
elif(sysstr == "Linux"):
    print "Liunux Support is coming sooooon"
    sys.exit(-1)
elif(sysstr == "Darwin"):
    from mac import xxtea
else:
    print "Unsupport OS!"
    sys.exit(-1)

# ----- function define --------
def joinDir(root, *dirs):
    for item in dirs:
        root = os.path.join(root, item)
    return root

def checkFileExt(path):
    binExt = [".zip", ".jpg", ".jpeg", ".png", ".pvr", ".ccz", ".bmp"]
    ext = os.path.splitext(path)[1]
    ext = ext.lower()
    return ext in binExt

def copyFile(src, dest, encodeSign, encodeKey):
    buff = ""
    inFp = open(src, 'rb')
    for line in inFp.readlines(): 
        buff += line
    inFp.close()

    if checkFileExt(src):
        # xxtea encoder
        buff = encodeSign + xxtea.encrypt(buff, encodeKey)

    outFp = open(dest, 'wb')
    outFp.write(buff)
    outFp.close()

def copyDir(src, dest, encodeSign, encodeKey):
    for item in os.listdir(src):
        if "." == item[0]: # ignore hidden files
            continue

        nSrc = joinDir(src, item)
        nDest = joinDir(dest, item)
        if os.path.isfile(nSrc):
            copyFile(nSrc, nDest, encodeSign, encodeKey)
        else:
            if not os.path.exists(nDest):
                os.mkdir(nDest)
            copyDir(nSrc, nDest, encodeSign, encodeKey)

def encodeRes(projectDir, encodeSign, encodeKey):
    print "====> Checking dir."
    resPath = joinDir(projectDir, "res")
    resBKPath = joinDir(projectDir, "res_bk")
    if os.path.exists(resBKPath):
        print "Error, res has encodered!!"
        sys.exit(-2)
    
    print "====> Start encoding res."
    # 1. backup res, removd res
    shutil.copytree(resPath, resBKPath)
    shutil.rmtree(resPath)
    os.mkdir(resPath)
    # 2. work backup dir, encode bin file.
    copyDir(resBKPath, resPath, encodeSign, encodeKey)
    print "====> old res:%s." %(resBKPath)
    print "====> encodered res:%s" %(resPath)
    print "====> Done."

if __name__ == "__main__":
    # ===== parse args =====
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hp:s:k:")
    except getopt.GetoptError:
        # print help information and exit:
        print __doc__
        sys.exit(-2)

    # default value
    projectDir = ""
    encodeKey = ""
    encodeSign = ""
    keyFile = ""

    for o, a in opts:
        if o == "-h":
            # print help information and exit:
            print __doc__
            sys.exit(0)
        if o == "-p":
            projectDir = a
        if o == "-s":
            encodeSign = a
        if o == "-k":
            keyFile = a

    # check valid
    if 0 == len(projectDir):
        print "Error: use -p to set project root dir."
        sys.exit(-2)
    if not os.path.exists(projectDir):
        print "Error: %s not exists." %(projectDir)
        sys.exit(-2)

    if 0 == len(encodeSign):
        print "Error: use -s to set sign."
        sys.exit(-2)
    # get key
    if 0 == len(keyFile):
        print "Error: use -k to key file path."
        sys.exit(-2)
    if not os.path.exists(keyFile):
        print "Error: %s not exists." %(keyFile)
        sys.exit(-2)
    inFp = open(keyFile, 'rb')
    encodeKey = inFp.read()
    inFp.close()

    # info printing
    print "== engineRoot: %s" %(engineRoot)
    encodeRes(projectDir, encodeSign, encodeKey)
