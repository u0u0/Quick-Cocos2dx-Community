#!/usr/bin/env python
# -*- coding: UTF-8 -*-
"""
NAME
    CreateProject --

SYNOPSIS
    CreateProject [-h]

    -h show help
    -p package name
    -o output dir
    -l set screen orientation to landscape, default if portrait.
    --deep-copy will copy cocos2d sources, default will use QUICK_V3_ROOT env
"""

import os
import sys
import getopt
import shutil
import re
import glob

scriptRoot = os.path.split(os.path.realpath(__file__))[0]
engineRoot = os.environ.get('QUICK_V3_ROOT')
replacementDict = {}

def joinDir(root, *dirs):
    for item in dirs:
        root = os.path.join(root, item)
    return root

def checkStringValid(string):
    if re.match('^[0-9a-z]+$', string):
        return True
    else:
        return False

def initDict(names, isLandscape):
    packageName = (".").join(names)
    lastName = names[2]

    replacementDict["__PROJECT_PACKAGE_FULL_NAME__"] = packageName
    replacementDict["__PROJECT_PACKAGE_LAST_NAME__"] = lastName
    replacementDict["__PROJECT_COCOS_NAME__"] = lastName
    if isLandscape:
        replacementDict["__SCREEN_ORIENTATION_L__"] = "landscape"
        replacementDict["__SCREEN_WIDTH__"] = "960"
        replacementDict["__SCREEN_HEIGHT__"] = "640"
        replacementDict["__SCREEN_ORIENTATION_QUICK__"] = "FIXED_HEIGHT"
        replacementDict["__SCREEN_ORIENTATION_IOS__"] = "<string>UIInterfaceOrientationLandscapeRight</string>\n<string>UIInterfaceOrientationLandscapeLeft</string>"
        replacementDict["__SCREEN_ORIENTATION_IOS5__"] = "UIInterfaceOrientationIsLandscape"
        replacementDict["__SCREEN_ORIENTATION_IOS6__"] = "UIInterfaceOrientationMaskLandscape"
        replacementDict["__SCREEN_ORIENTATION_SHOULDAUTOROTATE__"] = "YES"
        replacementDict["__SCREEN_ORIENTATION_ANDROID__"] = "sensorLandscape"
    else:
        replacementDict["__SCREEN_ORIENTATION_L__"] = "portrait"
        replacementDict["__SCREEN_WIDTH__"] = "640"
        replacementDict["__SCREEN_HEIGHT__"] = "960"
        replacementDict["__SCREEN_ORIENTATION_QUICK__"] = "FIXED_WIDTH"
        replacementDict["__SCREEN_ORIENTATION_IOS__"] = "<string>UIInterfaceOrientationPortrait</string>"
        replacementDict["__SCREEN_ORIENTATION_IOS5__"] = "UIInterfaceOrientationIsPortrait"
        replacementDict["__SCREEN_ORIENTATION_IOS6__"] = "UIInterfaceOrientationMaskPortraitUpsideDown"
        replacementDict["__SCREEN_ORIENTATION_SHOULDAUTOROTATE__"] = "NO"
        replacementDict["__SCREEN_ORIENTATION_ANDROID__"] = "portrait"

def stringReplace(s):
    for key in replacementDict:
        s = re.sub(key, replacementDict[key], s)
    return s

def checkPackageName(packageName):
    print "====> Checking packageName validity."
    packageName = packageName.lower()
    arr =  packageName.split(".")
    if len(arr) != 3:
        print "Error: packageName must be composed of three sections connected by points"
        sys.exit(-2)
    for item in arr:
        if not checkStringValid(item):
            print "Error: %s must be composed of English characters(a~z) and number(0~9)" %(item)
            sys.exit(-2)
    print "====> PackageName is valid."
    return arr

def createRootPath(outputDir):
    if os.path.exists(outputDir):
        print "Error: %s is already exist" %(outputDir)
        sys.exit(-2)
    os.mkdir(outputDir)
    if not os.path.exists(outputDir):
        print "Error: create %s fail" %(outputDir)
        sys.exit(-2)

def copyFile(src, dest):
    buff = ""
    inFp = open(src, 'rb')
    for line in inFp.readlines(): 
        line = stringReplace(line)
        buff += line
    inFp.close()

    outFp = open(dest, 'wb')
    outFp.write(buff)
    outFp.close()

def copyDir(src, dest):
    for item in os.listdir(src):
        if "." == item[0]: # ignore hidden files
            continue

        nSrc = joinDir(src, item)
        item = stringReplace(item)
        nDest = joinDir(dest, item)
        if os.path.isfile(nSrc):
            copyFile(nSrc, nDest)
        else:
            if not os.path.exists(nDest):
                os.mkdir(nDest)
            copyDir(nSrc, nDest)

def replaceFileContent(filePath, replaceItems):
    fileContent = None
    with open(filePath) as f:
        fileContent = f.read()

    for old, new in replaceItems.items():
        fileContent = fileContent.replace(old, new)

    with open(filePath, "w") as f:
        f.write(fileContent)


def createProject(packageName, outputDir, isLandscape, needCopyCocos2d):
    names = checkPackageName(packageName)
    lastName = names[2]
    initDict(names, isLandscape)

    templateRoot = joinDir(engineRoot, "quick", "template")
    outputDir = joinDir(outputDir, lastName)

    print "====> Copying template."
    createRootPath(outputDir)
    copyDir(templateRoot, outputDir)
    print "====> Copying framework."
    shutil.copytree(joinDir(engineRoot, "quick", "framework"), joinDir(outputDir, "src", "framework"))
    print "====> Copying cocos."
    shutil.copytree(joinDir(engineRoot, "quick", "cocos"), joinDir(outputDir, "src", "cocos"))
    if needCopyCocos2d:
        print "====> Copying cocos sources."
        os.mkdir(joinDir(outputDir, "frameworks", "cocos2d-x"))
        for dirName in ["cocos", "external", "build"]:
            shutil.copytree(joinDir(engineRoot, dirName), joinDir(outputDir, "frameworks", "cocos2d-x", dirName))
        os.mkdir(joinDir(outputDir, "frameworks", "cocos2d-x", "quick"))
        shutil.copytree(joinDir(engineRoot, "quick", "lib"), joinDir(outputDir, "frameworks", "cocos2d-x", "quick", "lib"))
        print "====> Removing QUICK_V3_ROOT env."
        # ios/mac: 
        xcodeprojFilePath = joinDir(glob.glob(joinDir(outputDir, "frameworks", "runtime-src", "proj.ios_mac", "*.xcodeproj"))[0], "project.pbxproj")
        replaceFileContent(xcodeprojFilePath, {"path = \"quick/lib/quick-src": "path = \"../../cocos2d-x/quick/lib/quick-src",
                                               "sourceTree = QUICK_V3_ROOT;": "sourceTree = \"<group>\";",
                                               "$(QUICK_V3_ROOT)": "$(PROJECT_DIR)/../../cocos2d-x",
                                               "path = build/cocos2d_libs.xcodeproj": "path = \"../../cocos2d-x/build/cocos2d_libs.xcodeproj\"",
                                               "path = \"cocos/scripting/lua-bindings/proj.ios_mac/cocos2d_lua_bindings.xcodeproj": "path = \"../../cocos2d-x/cocos/scripting/lua-bindings/proj.ios_mac/cocos2d_lua_bindings.xcodeproj"})
        # android:
        androidBuildFilePath = joinDir(outputDir, "frameworks", "runtime-src", "proj.android", "build_native.py")
        replaceFileContent(androidBuildFilePath, {"engineRoot = os.environ.get('QUICK_V3_ROOT')": "engineRoot = os.path.join(frameworksRoot, 'cocos2d-x')"})
        # sln: %QUICK_V3_ROOT% -> ..\..\cocos2d-x
        replaceFileContent(glob.glob(joinDir(outputDir, "frameworks", "runtime-src", "proj.win32", "*.sln"))[0], {"%QUICK_V3_ROOT%":"..\..\cocos2d-x"})
        # vcxproj: $(QUICK_V3_ROOT) -> $(ProjectDir)..\..\cocos2d-x
        replaceFileContent(glob.glob(joinDir(outputDir, "frameworks", "runtime-src", "proj.win32", "*.vcxproj"))[0], {"$(QUICK_V3_ROOT)":"$(ProjectDir)..\..\cocos2d-x"})
        # vcxproj.filters: $(QUICK_V3_ROOT) -> $(ProjectDir)..\..\cocos2d-x
        replaceFileContent(glob.glob(joinDir(outputDir, "frameworks", "runtime-src", "proj.win32", "*.vcxproj.filters"))[0], {"$(QUICK_V3_ROOT)":"$(ProjectDir)..\..\cocos2d-x"})
    print "====> Done."

if __name__ == "__main__":
    # ===== parse args =====
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hp:o:l", ["deep-copy"])
    except getopt.GetoptError:
        # print help information and exit:
        print __doc__
        sys.exit(-2)

    packageName = ""
    outputDir = ""
    isLandscape = False
    needCopyCocos2d = False
    for o, a in opts:
        if o == "-h":
            # print help information and exit:
            print __doc__
            sys.exit(0)
        if o == "-p":
            packageName = a
        if o == "-o":
            outputDir = a
        if o == "-l":
            isLandscape = True
        if o == "--deep-copy":
            needCopyCocos2d = True

    if len(packageName) == 0:
        print "Error: use -p xxx.xxx.xxx to set package name"
        sys.exit(-2)
    if len(outputDir) == 0:
        print "Error: use -o /path/path/ to set output dir"
        sys.exit(-2)

    # info printing
    print "== engineRoot: %s" %(engineRoot)
    createProject(packageName, outputDir, isLandscape, needCopyCocos2d)
