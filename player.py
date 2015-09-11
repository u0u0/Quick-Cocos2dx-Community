#!/usr/bin/python
# coding=utf-8

import sys
import os

platform = sys.platform

if platform == "darwin":
    os.system("open ./quick/player/player3.app")
elif platform == "win32":
	currPath = os.path.abspath('.')
	os.system(currPath+"/quick/player/player3.exe")