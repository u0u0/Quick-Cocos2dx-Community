local BaseLayer = require("app.scenes.BaseLayer")

local TestCase = class("Test_AccelerometerEvent", BaseLayer)

function TestCase:ctor()
	self.super.ctor(self)

	-- tips
	local label = display.newTTFLabel({
		text = "Test Accelerometer On iOS or Android",
		size = 25,
		color = cc.c3b(0, 0, 0),
	})
	label:align(display.CENTER, display.cx, display.cy + 200)
	self:addChild(label)

	-- show
	local result = display.newTTFLabel({
		text = "0, 0, 0",
		size = 25,
		color = cc.c3b(255, 0, 0),
	})
	result:center()
	self:addChild(result)

	self:addNodeEventListener(cc.ACCELEROMETER_EVENT, function(event)
		result:setString("x=" .. event.x)
		print("x", event.x)
		print("y", event.y)
		print("z", event.z)
		print("time", event.timestamp)
	end)
	self:setAccelerometerEnabled(true)
end

return TestCase
