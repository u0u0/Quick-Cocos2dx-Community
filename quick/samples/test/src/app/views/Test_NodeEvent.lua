local BaseLayer = require("app.scenes.BaseLayer")

local TestCase = class("Test_NodeEvent", BaseLayer)

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
	-- case 1, default handler
	self:setNodeEventEnabled(true)

	-- case 2, customer handler
	self:performWithDelay(function()
		local node = display.newNode()
		node:addNodeEventListener(cc.NODE_EVENT, function(event)
			print("** node event:", event.name)
		end)
		node:addTo(self)
	end, 1)
end

function TestCase:onEnter()
	print("==onEnter")
end

function TestCase:onExit()
	print("==onExit")
end

function TestCase:onEnterTransitionFinish()
	print("==onEnterTransitionFinish")
end

function TestCase:onExitTransitionStart()
	print("==onExitTransitionStart")
end

function TestCase:onCleanup()
	print("==onCleanup")
end

return TestCase
