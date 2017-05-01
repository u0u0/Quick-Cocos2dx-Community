--[[

Copyright (c) 2011-2014 chukong-inc.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

]]

--[[--

针对 cc.Node 的扩展

]]

local c = cc
local Node = c.Node

-- cocos2dx events
c.NODE_EVENT                 = 1
c.NODE_ENTER_FRAME_EVENT     = 2
c.NODE_TOUCH_EVENT           = 3
c.KEYPAD_EVENT               = 4

-- touch
c.TOUCH_MODE_ALL_AT_ONCE              = cc.TOUCHES_ALL_AT_ONCE
c.TOUCH_MODE_ONE_BY_ONE               = cc.TOUCHES_ONE_BY_ONE

local function isPointIn( rc, pt )
    local rect = cc.rect(rc.x, rc.y, rc.width, rc.height)
    return cc.rectContainsPoint(rect, pt)
end

function Node:align(anchorPoint, x, y)
    self:setAnchorPoint(display.ANCHOR_POINTS[anchorPoint])
    if x and y then self:setPosition(x, y) end
    return self
end

function Node:schedule(callback, interval)
    local seq = transition.sequence({
        cc.DelayTime:create(interval),
        cc.CallFunc:create(callback),
    })
    local action = cc.RepeatForever:create(seq)
    self:runAction(action)
    return action
end

function Node:performWithDelay(callback, delay)
    local action = transition.sequence({
        cc.DelayTime:create(delay),
        cc.CallFunc:create(callback),
    })
    self:runAction(action)
    return action
end

function Node:getCascadeBoundingBox()
    local rc
    local func = tolua.getcfunction(self, "getCascadeBoundingBox")
    if func then
        rc = func(self)
    end

    rc.origin = {x=rc.x, y=rc.y}
    rc.size = {width=rc.width, height=rc.height}
    rc.containsPoint = isPointIn
    return rc
end

--[[--

测试一个点是否在当前结点区域中

@param tabel point cc.p的点位置,世界坐标
@param boolean isCascade 是否用结点的所有子结点共同区域计算还是只用本身的区域

@return boolean 是否在结点区域中

]]
function Node:hitTest(point, isCascade)
    local nsp = self:convertToNodeSpace(point)
    local rect
    if isCascade then
        rect = self:getCascadeBoundingBox()
    else
        rect = self:getBoundingBox()
    end
    rect.x = 0
    rect.y = 0

    if cc.rectContainsPoint(rect, nsp) then
        return true
    end
    return false
end

function Node:removeSelf()
    self:removeFromParent(true)
end

-- override me for setNodeEventEnabled(true)
function Node:onEnter()
end

-- override me for setNodeEventEnabled(true)
function Node:onExit()
end

-- override me for setNodeEventEnabled(true)
function Node:onEnterTransitionFinish()
end

-- override me for setNodeEventEnabled(true)
function Node:onExitTransitionStart()
end

-- override me for setNodeEventEnabled(true)
function Node:onCleanup()
end

function Node:setNodeEventEnabled(enabled)
	if enabled then
		local listener = function(event)
			local name = event.name
			if name == "enter" then
				self:onEnter()
			elseif name == "exit" then
				self:onExit()
			elseif name == "enterTransitionFinish" then
				self:onEnterTransitionFinish()
			elseif name == "exitTransitionStart" then
				self:onExitTransitionStart()
			elseif name == "cleanup" then
				self:onCleanup()
			end
		end
		self:addNodeEventListener(c.NODE_EVENT, listener)
	else
		self:removeNodeEventListener(c.NODE_EVENT)
	end
	return self
end

local function KeypadEventCodeConvert(code)
    local key
    if code == 6 then
        key = "back"
    elseif code == 16 then
        key = "menu"
    else
        key = tostring(code)
    end
    return key
end

function Node:setKeypadEnabled(enable)
    if enable == self:isKeypadEnabled() then
        return self
    end

	local eventDispatcher = self:getEventDispatcher()
    if enable then
        local onKeyPressed = function(keycode, event)
			if self._LuaListeners[c.KEYPAD_EVENT] then
				-- call listener
				self._LuaListeners[c.KEYPAD_EVENT]{
					code = keycode,
					key = KeypadEventCodeConvert(keycode),
					type = "Pressed"
				}
			end
        end

        local onKeyReleased = function(keycode, event)
			if self._LuaListeners[c.KEYPAD_EVENT] then
				-- call listener
				self._LuaListeners[c.KEYPAD_EVENT]{
					code = keycode,
					key = KeypadEventCodeConvert(keycode),
					type = "Released"
				}
			end
        end

        local listener = cc.EventListenerKeyboard:create()
        listener:registerScriptHandler(onKeyPressed, cc.Handler.EVENT_KEYBOARD_PRESSED )
        listener:registerScriptHandler(onKeyReleased, cc.Handler.EVENT_KEYBOARD_RELEASED )
        eventDispatcher:addEventListenerWithSceneGraphPriority(listener, self)
        self.__key_event_handle__ = listener
	else
        eventDispatcher:removeEventListener(self.__key_event_handle__)
        self.__key_event_handle__ = nil
	end

    return self
end

function Node:isKeypadEnabled()
    if self.__key_event_handle__ then
		return true
	end
	return false
end

function Node:scheduleUpdate()
    local listener = function (dt)
		-- call listener
		if self._LuaListeners[c.NODE_ENTER_FRAME_EVENT] then
			self._LuaListeners[c.NODE_ENTER_FRAME_EVENT](dt)
		end
    end

    self:scheduleUpdateWithPriorityLua(listener, 0)
    return self
end

function Node:addNodeEventListener(evt, hdl)
    if not self._LuaListeners then
        self._LuaListeners = {}
		if evt == c.NODE_EVENT then
			self._baseNodeEventListener = function(evt)
				-- call listener
				if self._LuaListeners[c.NODE_EVENT] then
					self._LuaListeners[c.NODE_EVENT]{name = evt}
				end
			end
			self:registerScriptHandler(self._baseNodeEventListener)
		end
    end

	self._LuaListeners[evt] = hdl
end

function Node:removeNodeEventListener(evt)
    if not self._LuaListeners then return end

	if evt == c.KEYPAD_EVENT then
		self:setKeypadEnabled(false)
	elseif evt == c.NODE_EVENT then
		self:unregisterScriptHandler(self._baseNodeEventListener)
	elseif evt == c.NODE_ENTER_FRAME_EVENT then
		self:unscheduleUpdate()
	elseif evt == c.NODE_TOUCH_EVENT then
		self:setTouchEnabled(false)
	end

	self._LuaListeners[evt] = nil
end

function Node:removeAllNodeEventListeners()
    self:removeNodeEventListener(c.NODE_EVENT)
    self:removeNodeEventListener(c.NODE_ENTER_FRAME_EVENT)
    self:removeNodeEventListener(c.NODE_TOUCH_EVENT)
    self:removeNodeEventListener(c.KEYPAD_EVENT)
end

-- clone related
function Node:clone()
    local cloneNode = self:createCloneInstance_()

    cloneNode:copyProperties_(self)
    cloneNode:copySpecialPeerVal_(self)
    cloneNode:copyClonedWidgetChildren_(self)

    return cloneNode
end

function Node:createCloneInstance_()
    local nodeType = tolua.type(self)
    local cloneNode

    if "cc.Sprite" == nodeType then
        cloneNode = cc.Sprite:create()
    elseif "ccui.Scale9Sprite" == nodeType then
        cloneNode = ccui.Scale9Sprite:create()
    elseif "cc.LayerColor" == nodeType then
        local clr = self:getColor()
        clr.a = self:getOpacity()
        cloneNode = cc.LayerColor:create(clr)
    else
        cloneNode = display.newNode()
        if "cc.Node" ~= nodeType then
            print("WARING! treat " .. nodeType .. " as cc.Node")
        end
    end

    return cloneNode
end

function Node:copyClonedWidgetChildren_(node)
    local children = node:getChildren()
    if not children or 0 == #children then
        return
    end

    for i, child in ipairs(children) do
        local cloneChild = child:clone()
        if cloneChild then
            self:addChild(cloneChild)
        end
		if child.name and self.subChildren then
            self.subChildren[child.name] = cloneChild
        end
    end
end

function Node:copySpecialProperties_(node)
    local nodeType = tolua.type(self)

    if "cc.Sprite" == nodeType then
        self:setSpriteFrame(node:getSpriteFrame())
    elseif "ccui.Scale9Sprite" == nodeType then
        self:setSpriteFrame(node:getSprite():getSpriteFrame())
    elseif "cc.LayerColor" == nodeType then
        self:setTouchEnabled(false)
    end

    -- copy peer
    local peer = tolua.getpeer(node)
    if peer then
        local clonePeer = clone(peer)
        tolua.setpeer(self, clonePeer)
    end
    
end

function Node:copyProperties_(node)
    self:setVisible(node:isVisible())
    self:setTouchEnabled(node:isTouchEnabled())
    self:setLocalZOrder(node:getLocalZOrder())
    self:setTag(node:getTag())
    self:setName(node:getName())
    self:setContentSize(node:getContentSize())
    self:setPosition(node:getPosition())
    self:setAnchorPoint(node:getAnchorPoint())
    self:setScaleX(node:getScaleX())
    self:setScaleY(node:getScaleY())
    self:setRotation(node:getRotation())
    self:setRotationSkewX(node:getRotationSkewX())
    self:setRotationSkewY(node:getRotationSkewY())
    if self.isFlippedX and node.isFlippedX then
        self:setFlippedX(node:isFlippedX())
        self:setFlippedY(node:isFlippedY())
    end
    self:setColor(node:getColor())
    self:setOpacity(node:getOpacity())

    self:copySpecialProperties_(node)
end

-- 拷贝特殊的peer值
function Node:copySpecialPeerVal_(node)
    if node.name then
        self.name = node.name
    end
end
