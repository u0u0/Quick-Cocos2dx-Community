local BaseLayer = require("app.scenes.BaseLayer")

local TestCase = class("Test_NodeFrameEvent", BaseLayer)

function TestCase:ctor()
	self.super.ctor(self)

	-- tips
	local label = display.newTTFLabel({
		text = "Check result in console log",
		size = 25,
		color = cc.c3b(0, 0, 0),
	})
	label:align(display.CENTER, display.cx, display.cy)
	self:addChild(label)

	-- real test
	self:addNodeEventListener(cc.NODE_ENTER_FRAME_EVENT, function(dt)
		print(dt)
	end)
	self:scheduleUpdate()

	self:performWithDelay(function()
		self:unscheduleUpdate()
		print("==unscheduleUpdate")
	end, 2)
end

return TestCase
