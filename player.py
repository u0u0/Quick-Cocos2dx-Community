#!/usr/bin/python
# coding=utf-8

import sys, getopt
import os, os.path
#import shutil

QUICK_V3_ROOT = 'QUICK_V3_ROOT'

class Player(object):
	MAC_CHECK_FILES = ['.bash_profile', '.bash_login', '.profile']
	ZSH_CHECK_FILES = ['.zshrc']
	RE_FORMAT = r'^export[ \t]+%s=(.+)'

	def __init__(self):
		self.need_backup = True
		self.backup_file = None
		self.file_used_for_setup = ''

	def isWindows(self):
		return sys.platform == 'win32'

	def isMac(self):
		return sys.platform == 'darwin'

	def isZsh(self):
		shellItem = os.environ.get('SHELL')
		if shellItem is not None:
			if len(shellItem) >= 3:
				return shellItem[-3:] == "zsh"
		return False

	def getUnixFileList(self):
		file_list = None
		if self.isZsh():
			file_list = Player.ZSH_CHECK_FILES
		elif self.isMac():
			file_list = Player.MAC_CHECK_FILES
		return file_list

	def searchUnixVariable(self,var_name, file_name):
		if not os.path.isfile(file_name):
			return None
		import re
		str_re = self.RE_FORMAT % var_name
		patten = re.compile(str_re)
		ret = None
		for line in open(file_name):
			str1 = line.lstrip(' \t')
			match = patten.match(str1)
			if match is not None:
				ret = match.group(1)
		return ret

	def setEnvironmentVariableUnix(self,key, value):

		if self.need_backup:
			# backup the environment file
			self.backup_file = self.genBackupFile()
			#shutil.copy(self.file_used_for_setup, self.backup_file)
			self.need_backup = False

		file = open(self.file_used_for_setup, 'a')
		file.write('\n# Add environment variable %s for quick-cocos2dx-community\n' % key)
		file.write('export %s=%s\n' % (key, value))
		file.close()
		return True

	def genBackupFile(self):
		file_name = os.path.basename(self.file_used_for_setup)
		file_path = os.path.dirname(self.file_used_for_setup)
		backup_file_name = file_name + ".backup"
		path = os.path.join(file_path, backup_file_name)
		i = 1
		while os.path.exists(path):
			backup_file_name = file_name + ".backup%d" % i
			path = os.path.join(file_path, backup_file_name)
			i += 1
		return path

	def findEnvironmentVariable(self,var):
		print("  ->Search for environment variable %s..." % var)
		ret = None
		try:
			ret = os.environ[var]
		except Exception:
			if not self.isWindows():
				file_list = self.getUnixFileList()

				if file_list is not None:
					home = os.path.expanduser('~')
					for name in file_list:
						path = os.path.join(home, name)
						ret = self.searchUnixVariable(var, path)
						if ret is not None:
							break
			else:
				import _winreg
				try:
					env = None
					env = _winreg.OpenKeyEx(_winreg.HKEY_CURRENT_USER,
											'Environment',
											0,
											_winreg.KEY_READ)
					ret = _winreg.QueryValueEx(env, var)[0]
					_winreg.CloseKey(env)
				except Exception:
					if env:
						_winreg.CloseKey(env)
					ret = None
		if ret is None:
			print("    ->%s not found\n" % var)
		else:
			print("    ->%s is found : %s\n" % (var, ret))
		return ret

	def setWindowsPath(self,add_dir):
		ret = False
		import _winreg
		try:
			env = None
			path = None
			env = _winreg.OpenKeyEx(_winreg.HKEY_CURRENT_USER,
									'Environment',
									0,
									_winreg.KEY_SET_VALUE | _winreg.KEY_READ)
			path = _winreg.QueryValueEx(env, 'Path')[0]

			# add variable if can't find it in PATH
			path_lower = path.lower()
			add_dir_lower = add_dir.lower()
			if (path_lower.find(add_dir_lower) == -1):
				path = add_dir + ';' + path
				_winreg.SetValueEx(env, 'Path', 0, _winreg.REG_SZ, path)
				_winreg.FlushKey(env)

			_winreg.CloseKey(env)
			ret = True
		except Exception:
			if not path:
				path = add_dir
				_winreg.SetValueEx(env, 'Path', 0, _winreg.REG_SZ, path)
				_winreg.FlushKey(env)
				ret = True
			else:
				_winreg.SetValueEx(env, 'Path', 0, _winreg.REG_SZ, path)
				_winreg.FlushKey(env)
				ret = False

			if env:
				_winreg.CloseKey(env)

		if ret:
			print("  ->Add directory \"%s\" into PATH succeed!\n" % add_dir)
		else:
			print("  ->Add directory \"%s\" into PATH failed!\n" % add_dir)

	def setEnvironmentVariable(self,key, value):
		print("  -> Add %s environment variable..." % key)
		ret = False
		if self.isWindows():
			ret = self.setEnvironmentVariableWin32(key, value)
		else:
			ret = self.setEnvironmentVariableUnix(key, value)

		if ret:
			print("    ->Added %s=%s\n" % (key, value))
		else:
			print("    ->Add failed\n")

		return ret

	def removeDirFromWinPath(self,remove_dir):
		import _winreg
		try:
			env = None
			path = None
			env = _winreg.OpenKeyEx(_winreg.HKEY_CURRENT_USER,
									'Environment',
									0,
									_winreg.KEY_SET_VALUE | _winreg.KEY_READ)
			path = _winreg.QueryValueEx(env, 'Path')[0]

			path_lower = path.lower()
			remove_dir = remove_dir.replace('/', '\\')
			remove_dir_lower = remove_dir.lower()
			start_pos = path_lower.find(remove_dir_lower)
			if (start_pos >= 0):
				length = len(remove_dir_lower)
				need_remove = path[start_pos:(start_pos + length)]
				path = path.replace(need_remove, '')
				path = path.replace(';;', ';')
				_winreg.SetValueEx(env, 'Path', 0, _winreg.REG_SZ, path)
				_winreg.FlushKey(env)
			_winreg.CloseKey(env)

			print('  ->Remove directory \"%s\" from PATH!\n' % remove_dir)
		except Exception:
			print('  ->Remove directory \"%s\" from PATH failed!\n' %remove_dir)

	def setWindowsPath(self,add_dir):
		ret = False
		import _winreg
		try:
			env = None
			path = None
			env = _winreg.OpenKeyEx(_winreg.HKEY_CURRENT_USER,
									'Environment',
									0,
									_winreg.KEY_SET_VALUE | _winreg.KEY_READ)
			path = _winreg.QueryValueEx(env, 'Path')[0]

			# add variable if can't find it in PATH
			path_lower = path.lower()
			add_dir_lower = add_dir.lower()
			if (path_lower.find(add_dir_lower) == -1):
				path = add_dir + ';' + path
				_winreg.SetValueEx(env, 'Path', 0, _winreg.REG_SZ, path)
				_winreg.FlushKey(env)

			_winreg.CloseKey(env)
			ret = True
		except Exception:
			if not path:
				path = add_dir
				_winreg.SetValueEx(env, 'Path', 0, _winreg.REG_SZ, path)
				_winreg.FlushKey(env)
				ret = True
			else:
				_winreg.SetValueEx(env, 'Path', 0, _winreg.REG_SZ, path)
				_winreg.FlushKey(env)
				ret = False

			if env:
				_winreg.CloseKey(env)

		if ret:
			print("  ->Add directory \"%s\" into PATH succeed!\n" % add_dir)
		else:
			print("  ->Add directory \"%s\" into PATH failed!\n" % add_dir)

	def forceUpdateEnv(self,var_name, value):
		ret = False
		if self.isWindows():
			print("  ->Force update environment variable %s" % var_name)
			ret = self.setEnvironmentVariableWin32(var_name, value)
			if not ret:
				print("    ->Failed!")
			else:
				print("    ->Succeed : %s=%s" % (var_name, value))
		else:
			ret = self.forceUpdateUnixEnv(var_name, value)
		return ret

	def getFilepathForSetup(self):
		file_list = self.getUnixFileList()

		file_to_write = None
		if file_list is None:
			return ''

		home = os.path.expanduser('~')
		for file_name in file_list:
	 		file_path = os.path.join(home, file_name)
			if os.path.exists(file_path):
				file_to_write = file_path
				break

		if file_to_write is None:
			self.need_backup = False
			file_to_write = os.path.join(home, file_list[0])
			file_obj = open(file_to_write, 'w')
			file_obj.close()

		return file_to_write

	def setQuickRoot(self):
		print('\nSetting up quick-cocos2dx-community...')
		self.file_used_for_setup = self.getFilepathForSetup()


		print("->Check environment variable %s" % QUICK_V3_ROOT)
		quick_v3_root = None
		if self.isWindows():
			quick_v3_root = os.path.join(os.path.abspath('.'))
		else:
			quick_v3_root = os.path.join("`cat ~/.QUICK_V3_ROOT`")
			file = open(os.path.expanduser('~')+'/.'+QUICK_V3_ROOT,'wt')
			file.truncate()
			file.write(os.path.abspath('.').strip()+"\n")
			file.close()

		old_dir = self.findEnvironmentVariable(QUICK_V3_ROOT)
		if old_dir is None:
			if self.isWindows():
				setWindowsPath(quick_v3_root)
			self.setEnvironmentVariable(QUICK_V3_ROOT, quick_v3_root)
		else:
			if old_dir == quick_v3_root:
				return
			if self.isWindows():
				self.removeDirFromWinPath(old_dir)
				self.setWindowsPath(quick_v3_root)
			self.forceUpdateEnv(QUICK_V3_ROOT, quick_v3_root)
		if self.isWindows():
			print('\nPlease restart the terminal or restart computer to make added system variables take effect\n')
		else:
			print('\nPlease execute command: "source %s" to make added system variables take effect\n' %self.file_used_for_setup)

	def launchPlayer(self):
		if self.isMac():
			os.system("open ./quick/player/player3.app")
		elif self.isWindows():
			os.system(os.path.abspath('.')+"/quick/player/player3.exe")

if __name__ == '__main__':
	player = Player()
	opts,args = getopt.getopt(sys.argv[1:], "er")
	for op, value in opts:
		if op == "-e":
			player.setQuickRoot()
			sys.exit()
		elif op == "-r":
			player.launchPlayer()
			sys.exit()

	print("set environment variable:\n    python player.py -e \nrun palyer:\n    python player.py -r")

	if player.isWindows():
		import ctypes
		HWND_BROADCAST = 0xFFFF
		WM_SETTINGCHANGE = 0x1A
		SMTO_ABORTIFHUNG = 0x0002
		result = ctypes.c_long()
		SendMessageTimeoutW = ctypes.windll.user32.SendMessageTimeoutW
		SendMessageTimeoutW(HWND_BROADCAST, WM_SETTINGCHANGE, 0,
							u'Environment', SMTO_ABORTIFHUNG, 5000, ctypes.byref(result))
