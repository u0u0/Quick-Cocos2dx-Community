--[[--

针对 ccui.Widget 的扩展

]]

local Widget = ccui.Widget

--[[
wapper ccui.Widget:setTouchEnabled to make it support Node:addNodeEventListener(cc.NODE_TOUCH_EVENT)
Normally, ccui.Widget have it own touch dealing in cpp code.
This wapper help you have chance to deal touch in lua code.
]]--
function Widget:setTouchEnabled(enable)
	local cfunc = tolua.getcfunction(self, "setTouchEnabled")
	if self._LuaListeners and self._LuaListeners[cc.NODE_TOUCH_EVENT] then
		cc.Node.setTouchEnabled(self, enable)
		-- widget's cpp listener may enable from csb setting.
		-- In some cases, dispatch may not send to lua created listener
		-- Remove cpp listener to avoid uncertain TouchEvent dispatch
		cfunc(self, false)
	else
		cfunc(self, enable)
	end
end

-- The easiest way to support addClickEventListener
function Widget:addClickEventListener(callback)
	self:addTouchEventListener(function(ref, eventType)
		if cc.EventCode.ENDED == eventType then
			callback()
		end
	end)
end
