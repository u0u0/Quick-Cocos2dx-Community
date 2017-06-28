--[[--

针对 ccui.Widget 的扩展

]]

local Widget = ccui.Widget

function Widget:setTouchEnabled(enable)
	if self._LuaListeners and self._LuaListeners[cc.NODE_TOUCH_EVENT] then
		cc.Node.setTouchEnabled(self, enable)
	else
		local cfunc = tolua.getcfunction(self, "setTouchEnabled")
		cfunc(self, enable)
	end
end
