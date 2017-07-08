local BaseLayer = require("app.scenes.BaseLayer")

local TestCase = class("Test_NodeTouchEvent", BaseLayer)

function TestCase:ctor()
	self.super.ctor(self)

	-- tips
	local label = display.newTTFLabel({
		text = "New Audio engine test case",
		size = 25,
		color = cc.c3b(0, 0, 0),
	})
	label:align(display.CENTER, display.cx, display.top - 40)
	self:addChild(label)

	local channelCount = 0
	local loadedCB = function(fn, success)
		if not success then
			print("Fail to load audio:" .. fn)
			return
		end

		channelCount = channelCount + 1
		local playWay = audio.playBGM
		local text = "Play BGM:"
		if channelCount > 1 then
			playWay = audio.playEffect
			text = "Play Effect:"
		end

		local btn = ccui.Button:create()
			:pos(display.cx, display.height - 50 * channelCount - 100)
			:addTo(self)

		btn:setTitleText(text .. fn)
		btn:setTitleFontSize(30)
		btn:setTitleColor(cc.c3b(255, 255, 0))
		btn:addTouchEventListener(function(sender, eventType)
			if 2 == eventType then
				playWay(fn)
			end
		end)

		if channelCount == 2 then
			self:loadBtns()
		end
	end

	-- load audio sync
	local btn = ccui.Button:create()
		:pos(display.cx, display.height - 100)
		:addTo(self)

	btn:setTitleText("Preload oggs")
	btn:setTitleFontSize(30)
	btn:setTitleColor(cc.c3b(255, 255, 0))
	btn:addTouchEventListener(function(sender, eventType)
		if 2 == eventType then
			audio.loadFile("audio/bgm.ogg", loadedCB)
			audio.loadFile("audio/effect.ogg", loadedCB)
			btn:setVisible(false)
		end
	end)
end

function TestCase:loadBtns()
	local btns = {
		{"Down_BGMVolume", function()
			audio.setBGMVolume(0.3)
		end},
		{"Up_BGMVolume", function()
			audio.setBGMVolume(1)
		end},
		{"Down_EffectVolume", function()
			audio.setEffectVolume(0.3)
		end},
		{"Up_EffectVolume", function()
			audio.setEffectVolume(1)
		end},
		{"PauseAll", function()
			audio.pauseAll()
		end},
		{"SingleEffect_vol0.5", function()
			if not self._effect then
				self._effect = audio.playEffect("audio/effect.ogg", true)
				if self._effect then
					self._effect:setVolume(0.5) -- change volume just for this effect
					self:performWithDelay(function()
						self._effect:stop() -- stop looped effect
						self._effect = nil
					end, 3)
				end
			end
		end},
		{"ResumeAll", function()
			audio.resumeAll()
		end},
		{"StopAll", function()
			audio.stopAll()
		end},
		{"unloadFile", function()
			audio.unloadFile("audio/effect.ogg")
		end},
		{"unloadAllFile", function()
			audio.unloadAllFile()
		end},
	}

	for index, info in ipairs(btns) do
		local isOdd = index % 2 == 1
		local posX = display.cx + 150
		if isOdd then
			posX = display.cx - 150
		end
		local posY = math.floor((index - 1) / 2) * 50 + 50
		local btn = ccui.Button:create()
			:pos(posX, posY)
			:addTo(self)

		btn:setTitleText(info[1])
		btn:setTitleFontSize(30)
		btn:setTitleColor(cc.c3b(255, 255, 0))
		btn:addTouchEventListener(function(sender, eventType)
			if 2 == eventType then
				info[2]()
			end
		end)
	end
end

return TestCase
