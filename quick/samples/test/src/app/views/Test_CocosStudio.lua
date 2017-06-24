local BaseLayer = require("app.scenes.BaseLayer")

local TestCase = class("Test_NodeTouchEvent", BaseLayer)

function TestCase:ctor()
	self.super.ctor(self)

	local node = cc.CSLoader:getInstance():createNodeWithFlatBuffersFile("css/NodeFullScreen.csb")
	node:addTo(self)
		:center()
	-- Multi-resolution adaptation
	ccui.Helper:seekWidgetByName(node, "Panel_top"):setPositionY(display.cy)
	ccui.Helper:seekWidgetByName(node, "Panel_bottom"):setPositionY(-display.cy)

	local list = ccui.Helper:seekWidgetByName(node, "ListView")
	for i = 1, 5 do
		local btn = cc.CSLoader:getInstance():createNodeWithFlatBuffersFile("css/NodeCell.csb")
			:getChildren()[1]
		btn:removeSelf()

		btn:setTitleText("Name" .. i)
		btn:setTitleFontSize(30)
		btn:addTouchEventListener(function(sender, eventType)
			if 2 == eventType then
				print("clicked " .. i)
			end
		end)
		list:pushBackCustomItem(btn)
	end

	ccui.Helper:seekWidgetByName(node, "Button_bottom")
		:addTouchEventListener(function(sender, eventType)
		if 2 == eventType then
			self:getParent():openScrollView()
			self:removeSelf()
		end
	end)
end

return TestCase
