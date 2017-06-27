--[[--

针对 ccui.Widget 的扩展

]]

local Widget = ccui.Widget

function Widget:setTouchEnabled(enable)
	if self._LuaListeners and self._LuaListeners[cc.NODE_TOUCH_EVENT] then
		local mt = getmetatable(self)
		repeat -- find parent's setTouchEnabled method
			if "class cc.Node" == tolua.type(mt) then
				break
			end
			mt = getmetatable(mt)
		until not mt
		
		local nodeFunc = mt["setTouchEnabled"]
		assert(nodeFunc, "Error while finding Node:setTouchEnabled!")
		nodeFunc(self, enable)
	else
		local cfunc = tolua.getcfunction(self, "setTouchEnabled")
		cfunc(self, enable)
	end
end
