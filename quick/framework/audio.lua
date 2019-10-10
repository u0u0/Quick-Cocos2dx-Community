--[[
Copyright 2017 KeNan Liu

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
]]

-- Singleton class
local audio = {}
audio._buffers = {}
audio._sources = {}
audio._scheduler = nil -- global schedule hander
-- pos 1 is for BGM
audio._sources[1] = Rapid2D_CAudio.newSource()
if not (audio._sources[1]) then
	print("Error: init BGM source fail, check if have OpenAL init error above!")
	-- fake function, disable audio output when init failed
	audio.loadFile = function(path, callback)
		callback(path, true)
	end
	audio.unloadFile = function(path) end
	audio.unloadAllFile = function() end
	audio.playBGMSync = function(path, isLoop) end
	audio.playBGM = function(path, isLoop) end
	audio.stopBGM = function() end
	audio.setBGMVolume = function(vol) end
	audio.playEffectSync = function(path, isLoop) end
	audio.playEffect = function(path, isLoop) end
	audio.setEffectVolume = function(vol) end
	audio.stopEffect = function() end
	audio.stopAll = function() end
	audio.pauseAll = function() end
	audio.resumeAll = function() end
	return audio
end

audio._BGMVolume = 1.0
audio._effectVolume = 1.0

local scheduler = require("framework.scheduler")

-- INTERNAL API, recircle source from effects, call by director
local function update(dt)
	local sources = audio._sources
	local total = #sources
	local index = 2
	while index <= total do
		local stat = sources[index]:getStat()
		if 4 == stat then
			sources[index]:__gc() -- free OpenAL resource
			table.remove(sources, index)
			total = total - 1
		else
			index = index + 1
		end
	end

	if 1 == total then
		scheduler.unscheduleGlobal(audio._scheduler)
		audio._scheduler = nil
	end
end

--------------- buffer -------------------
function audio.loadFile(path, callback)
	if audio._buffers[path] then
		callback(path, true)
	else
		assert(callback, "ONLY support asyn load file, please set callback!")
		Rapid2D_CAudio.newBuffer(path, function(buffID)
			if buffID then
				audio._buffers[path] = buffID
				callback(path, true)
			else
				callback(path, false)
			end
		end)
	end
end

function audio.unloadFile(path)
	local buffer = audio._buffers[path]
	if buffer then
		buffer:__gc()
	end
	audio._buffers[path] = nil
end

function audio.unloadAllFile()
	for path, buffer in pairs(audio._buffers) do
		buffer:__gc()
	end
	audio._buffers = {}
end

--[[
function for CSource
	play2d(buffer, isLoop)
    pause()
    resume()
    stop()
	setVolume(vol)
    getStat()
]]--

--------------- BGM 2D API -------------------
-- no need preload file
function audio.playBGMSync(path, isLoop)
	audio.loadFile(path, function(pn, isSuccess)
		if isSuccess then
			audio.playBGM(pn, isLoop)
		end
	end)
end

-- need preload file
function audio.playBGM(path, isLoop)
	local buffer = audio._buffers[path]
	if not buffer then
		print(path .. " have not loaded!!")
		return
	end

	isLoop = isLoop ~= false and true or false
	audio._sources[1]:stop()
	audio._sources[1]:play2d(buffer, isLoop)
	audio._sources[1]:setVolume(audio._BGMVolume)
end

function audio.stopBGM()
	audio._sources[1]:stop()
end

function audio.setBGMVolume(vol)
	if vol > 1.0 then
		vol = 1.0
	end
	if vol < 0.0 then
		vol = 0.0
	end
	audio._sources[1]:setVolume(vol)
	audio._BGMVolume = vol
end

--------------- Effect 2D API -------------------
-- no need preload file
function audio.playEffectSync(path, isLoop)
	audio.loadFile(path, function(pn, isSuccess)
		if isSuccess then
			audio.playEffect(pn, isLoop)
		end
	end)
end

-- need preload file
function audio.playEffect(path, isLoop)
	local buffer = audio._buffers[path]
	if not buffer then
		print(path .. " have not loaded!!")
		return
	end
	
	local source = Rapid2D_CAudio.newSource()
	if source then
		isLoop = isLoop == true and true or false
		table.insert(audio._sources, source)
		source:setVolume(audio._effectVolume)
		source:play2d(buffer, isLoop)

		-- start recircle scheduler
		if not audio._scheduler then
			audio._scheduler = scheduler.scheduleGlobal(update, 0.1)
		end
	end
	return source
end

function audio.setEffectVolume(vol)
	if vol > 1.0 then
		vol = 1.0
	end
	if vol < 0.0 then
		vol = 0.0
	end
	audio._effectVolume = vol

	for i = 2, #audio._sources do
		audio._sources[i]:setVolume(vol)
	end
end

function audio.stopEffect()
	for i = 2, #audio._sources do
		audio._sources[i]:stop()
	end
end

--------------- work both on BGM and Effects -------------------
function audio.stopAll()
	for i = 1, #audio._sources do
		audio._sources[i]:stop()
	end
end

function audio.pauseAll()
	for i = 1, #audio._sources do
		audio._sources[i]:pause()
	end
end

function audio.resumeAll()
	for i = 1, #audio._sources do
		audio._sources[i]:resume()
	end
end

return audio
