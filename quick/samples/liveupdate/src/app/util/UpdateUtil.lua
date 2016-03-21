--
-- Author: rsma
-- Date: 2016-03-17 15:39:48
--
-- require("config")
-- require("cocos.init")
-- require("framework.init")
require "lfs"
--保存在本地的版本文件名称
UpdateResName="ResList"
--从服务端检查的版本变更日志
UpdateResConfig=nil
--更新资源存放目录
UpdatePath = cc.FileUtils:getInstance():getWritablePath().."upd/"

function hex(s)
	s=string.gsub(s,"(.)",function (x) return string.format("%02X",string.byte(x)) end)
	return s
end

function readFile(path)
	local _file = io.open(path, "rb")
	if _file then
		local content = _file:read("*all")
		io.close(_file)
		return content
	end
	return nil
end

function removeFile(path)
	print("removeFile: "..path)
	os.remove(path)
end

function checkFile(fileName, cryptoCode)
	if not io.exists(fileName) then
		return false
	end

	local data=readFile(fileName)
	if data==nil then
		return false
	end

	if cryptoCode==nil then
		return true
	end

	local ms = crypto.md5(hex(data))
	if ms==cryptoCode then
		return true
	end
	return false
end

function checkDirOK( basepath, filepath)
	local filepath = filepath or ""
	local basepath = basepath or ""

	local function __mkdir(filepath, basepath)
		local oldpath = lfs.currentdir()

		if lfs.chdir(basepath..filepath) then
	        lfs.chdir(oldpath)
	        return true
	    end

	    local path = string.split(filepath, "/")
	    local newbasepath = basepath .. path[1] .."/"

		if lfs.chdir(newbasepath) then
			lfs.chdir(oldpath)
		else
			if not lfs.mkdir(newbasepath) then
				return false
			end
		end

		local newfilepath = ""
		if path[2] then
			table.remove(path,1)
			newfilepath = table.concat(path,"/") .. "/"
		end
		return __mkdir(newfilepath, newbasepath)
	end

	return __mkdir(filepath, basepath)
end

function os.exists(path)
    return cc.FileUtils:getInstance():isFileExist(path)
end

function os.mkdir(path)
    if not os.exists(path) then
        return lfs.mkdir(path)
    end
    return true
end

function os.rmdir(path)
    print("os.rmdir:", path)
    if os.exists(path) then
        local function _rmdir(path)
            local iter, dir_obj = lfs.dir(path)
            while true do
                local dir = iter(dir_obj)
                if dir == nil then break end
                if dir ~= "." and dir ~= ".." then
                    local curDir = path..dir
                    local mode = lfs.attributes(curDir, "mode") 
                    if mode == "directory" then
                        _rmdir(curDir.."/")
                    elseif mode == "file" then
                        os.remove(curDir)
                    end
                end
            end
            local succ, des = os.remove(path)
            if des then print(des) end
            return succ
        end
        _rmdir(path)
    end
    return true
end
--进入游戏优先检查UPD下的更新文件并加载
function checkUpdateFile()
	if os.exists(UpdatePath .. UpdateResName) then
		UpdateResConfig = dofile(UpdatePath .. UpdateResName)
		if GAME_VERSION<UpdateResConfig.ver then
           for _,v in pairs(UpdateResConfig.stage) do
           		if v.act=="load" then
                    cc.LuaLoadChunksFromZIP(UpdatePath..v.path..v.name)
                end
           end
           GAME_VERSION = UpdateResConfig.ver
        end
	end
end