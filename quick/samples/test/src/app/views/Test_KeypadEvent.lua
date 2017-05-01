local BaseLayer = require("app.scenes.BaseLayer")

local TestCase = class("Test_KeypadEvent", BaseLayer)

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
	self:addNodeEventListener(cc.KEYPAD_EVENT, function(event)
		-- event.code is number
		-- event.key is string
		print(event.code, event.key, event.type)
	end)
	self:setKeypadEnabled(true)

	print("==is keypad enable:", self:isKeypadEnabled())
end

return TestCase
