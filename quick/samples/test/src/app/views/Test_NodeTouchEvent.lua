local BaseLayer = require("app.scenes.BaseLayer")

local TestCase = class("Test_NodeTouchEvent", BaseLayer)

function TestCase:ctor()
	self.super.ctor(self)

	-- tips
	local label = display.newTTFLabel({
		text = "Left is single touch, right is mulit touchs",
		size = 25,
		color = cc.c3b(0, 0, 0),
	})
	label:align(display.CENTER, display.cx, display.cy + 200)
	self:addChild(label)

	-- real test case 1 : single touch
	local sp = display.newSprite("button.png")
	sp:addNodeEventListener(cc.NODE_TOUCH_EVENT, function(event)
		dump(event)
		if event.name == "began" then
			return true
		end
	end)
	sp:setTouchMode(cc.TOUCH_MODE_ONE_BY_ONE) -- default mode
	sp:setTouchEnabled(true)
	sp:pos(150, display.cy):addTo(self)
	sp:setScale(0.5)
	sp:setRotation(45)

	-- real test case 2 : mulit touch
	--[[
		mulit touch always after single touch, so can NOT get touch here,
		because BaseLayer get full screen touch.
		And mulit touch's began has no "return true" way
	]]--
	local multi = display.newSprite("button.png")
	multi:addNodeEventListener(cc.NODE_TOUCH_EVENT, function(event)
		dump(event)
	end)
	multi:setTouchMode(cc.TOUCH_MODE_ALL_AT_ONCE)
	multi:setTouchEnabled(true)
	multi:pos(display.width - 150, display.cy):addTo(self)
	multi:setScale(0.5)
	multi:setRotation(45)
end

return TestCase
