local BaseLayer = class("BaseLayer", function()
    return ccui.Layout:create()
end)

function BaseLayer:ctor()
	self:setContentSize(cc.size(display.width, display.height))
	self:setTouchEnabled(false)
	self:setBackGroundColor(cc.c3b(0x3c, 0x3c, 0x3c))
	self:setBackGroundColorType(1)

	-- back button
	local btn = ccui.Button:create()
	btn:pos(display.width - 100, 40)
	btn:addTo(self)
	btn:setTitleText("Back")
	btn:setTitleFontSize(26)
	btn:addTouchEventListener(function(sender, eventType)
		if 2 == eventType then
			self:getParent():openScrollView()
			self:removeSelf()
			audio.stopAll()
		end
	end)
end

return BaseLayer
