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

############################################################
#http://www.coolcode.org/archives/?article-307.html
############################################################

import struct

_DELTA = 0x9E3779B9

def _long2str(v, w):
    n = (len(v) - 1) << 2
    if w:
        m = v[-1]
        if (m < n - 3) or (m > n): return ''
        n = m
    s = struct.pack('<%iL' % len(v), *v)
    return s[0:n] if w else s

def _str2long(s, w):
    n = len(s)
    m = (4 - (n & 3) & 3) + n
    s = s.ljust(m, "\0")
    v = list(struct.unpack('<%iL' % (m >> 2), s))
    if w: v.append(n)
    return v

def xxteaEncrypt(str, key):
    if str == '': return str
    v = _str2long(str, True)
    k = _str2long(key.ljust(16, "\0"), False)
    n = len(v) - 1
    z = v[n]
    y = v[0]
    sum = 0
    q = 6 + 52 // (n + 1)
    while q > 0:
        sum = (sum + _DELTA) & 0xffffffff
        e = sum >> 2 & 3
        for p in xrange(n):
            y = v[p + 1]
            v[p] = (v[p] + ((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + (k[p & 3 ^ e] ^ z))) & 0xffffffff
            z = v[p]
        y = v[0]
        v[n] = (v[n] + ((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + (k[n & 3 ^ e] ^ z))) & 0xffffffff
        z = v[n]
        q -= 1
    return _long2str(v, False)

def xxteaDecrypt(str, key):
    if str == '': return str
    v = _str2long(str, False)
    k = _str2long(key.ljust(16, "\0"), False)
    n = len(v) - 1
    z = v[n]
    y = v[0]
    q = 6 + 52 // (n + 1)
    sum = (q * _DELTA) & 0xffffffff
    while (sum != 0):
        e = sum >> 2 & 3
        for p in xrange(n, 0, -1):
            z = v[p - 1]
            v[p] = (v[p] - ((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + (k[p & 3 ^ e] ^ z))) & 0xffffffff
            y = v[p]
        z = v[n]
        v[0] = (v[0] - ((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + (k[0 & 3 ^ e] ^ z))) & 0xffffffff
        y = v[0]
        sum = (sum - _DELTA) & 0xffffffff
    return _long2str(v, True)

##########################################

scriptRoot = os.path.split(os.path.realpath(__file__))[0]
engineRoot = os.environ.get('QUICK_V3_ROOT')

# ----- function define --------
def joinDir(root, *dirs):
    for item in dirs:
        root = os.path.join(root, item)
    return root

def checkFileExt(path):
    binExt = [".ogg", ".zip", ".jpg", ".jpeg", ".png", ".pvr", ".ccz", ".bmp", ".tmx", ".plist"]
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
        buff = encodeSign + xxteaEncrypt(buff, encodeKey)

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
