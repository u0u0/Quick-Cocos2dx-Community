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
		end
	end)

	local btn = ccui.Button:create()
		:pos(display.cx - 100, 60)
		:addTo(self)

	btn:setTitleText("PauseAll")
	btn:setTitleFontSize(30)
	btn:setTitleColor(cc.c3b(255, 255, 0))
	btn:addTouchEventListener(function(sender, eventType)
		if 2 == eventType then
			audio.pauseAll()
		end
	end)

	local btn = ccui.Button:create()
		:pos(display.cx + 100, 60)
		:addTo(self)

	btn:setTitleText("ResumeAll")
	btn:setTitleFontSize(30)
	btn:setTitleColor(cc.c3b(255, 255, 0))
	btn:addTouchEventListener(function(sender, eventType)
		if 2 == eventType then
			audio.resumeAll()
		end
	end)
end

return TestCase
