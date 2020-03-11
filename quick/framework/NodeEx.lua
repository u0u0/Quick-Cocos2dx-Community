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
c.ACCELEROMETER_EVENT        = 5

-- touch
c.TOUCH_MODE_ALL_AT_ONCE              = cc.TOUCHES_ALL_AT_ONCE
c.TOUCH_MODE_ONE_BY_ONE               = cc.TOUCHES_ONE_BY_ONE

local function isPointIn( rc, pt )
    local rect = c.rect(rc.x, rc.y, rc.width, rc.height)
    return c.rectContainsPoint(rect, pt)
end

function Node:align(anchorPoint, x, y)
    self:setAnchorPoint(display.ANCHOR_POINTS[anchorPoint])
    if x and y then self:setPosition(x, y) end
    return self
end

function Node:schedule(callback, interval)
    local seq = c.Sequence:create(
        c.DelayTime:create(interval),
        c.CallFunc:create(callback)
    )
    local action = c.RepeatForever:create(seq)
    self:runAction(action)
    return action
end

function Node:performWithDelay(callback, delay)
    local action = c.Sequence:create(
        c.DelayTime:create(delay),
        c.CallFunc:create(callback)
    )
    self:runAction(action)
    return action
end

--[[--

测试一个点是否在当前结点区域中

@param tabel point cc.p的点位置,世界坐标
@return boolean 是否在结点区域中

]]
function Node:hitTest(point)
    local nsp = self:convertToNodeSpace(point)
    local rect = self:getContentSize()
    rect.x = 0
    rect.y = 0
    if c.rectContainsPoint(rect, nsp) then
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

function Node:setAccelerometerEnabled(enabled)
	cc.Device:setAccelerometerEnabled(enabled)
	if not enabled then
		return
	end

	local listener = cc.EventListenerAcceleration:create(function(event, x, y, z, timestamp)
		-- call listener
		self._LuaListeners[c.ACCELEROMETER_EVENT]{
			x = x,
			y = y,
			z = z,
			timestamp = timestamp
		}
	end)
	self:getEventDispatcher():addEventListenerWithSceneGraphPriority(listener, self)
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
			-- call listener
			self._LuaListeners[c.KEYPAD_EVENT]{
				code = keycode,
				key = KeypadEventCodeConvert(keycode),
				type = "Pressed"
			}
        end

        local onKeyReleased = function(keycode, event)
			-- call listener
			self._LuaListeners[c.KEYPAD_EVENT]{
				code = keycode,
				key = KeypadEventCodeConvert(keycode),
				type = "Released"
			}
        end

        local listener = c.EventListenerKeyboard:create()
        listener:registerScriptHandler(onKeyPressed, c.Handler.EVENT_KEYBOARD_PRESSED)
        listener:registerScriptHandler(onKeyReleased, c.Handler.EVENT_KEYBOARD_RELEASED)
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
		self._LuaListeners[c.NODE_ENTER_FRAME_EVENT](dt)
    end

    self:scheduleUpdateWithPriorityLua(listener, 0)
    return self
end

function Node:setTouchMode(mode)
	if mode ~= c.TOUCH_MODE_ALL_AT_ONCE and mode ~= c.TOUCHES_ONE_BY_ONE then
		print("== wrong mode", mode)
		return
	end
	self._luaTouchMode = mode
end

function Node:setTouchEnabled(enable)
	-- remove old
	local eventDispatcher = self:getEventDispatcher()
	if self._luaTouchListener then
		eventDispatcher:removeEventListener(self._luaTouchListener)
		self._luaTouchListener = nil
	end

	if not enable then
		return self
	end

	assert(self._LuaListeners, "Error: addNodeEventListener(cc.NODE_TOUCH_EVENT, func) first!")
	assert(self._LuaListeners[c.NODE_TOUCH_EVENT], "Error: addNodeEventListener(cc.NODE_TOUCH_EVENT, func) first!")

	local isSingle = true
	if self._luaTouchMode and self._luaTouchMode == c.TOUCH_MODE_ALL_AT_ONCE then
		isSingle = false
	end

	-- add new
	if isSingle then
		self._luaTouchListener = c.EventListenerTouchOneByOne:create()
		self._luaTouchListener:setSwallowTouches(true)
		local dealFunc = function(touch, name)
			local tp = touch:getLocation()
			local sp = touch:getStartLocation()
			local pp = touch:getPreviousLocation()

			if name == "began" then
				if not self:isVisible(true) or not self:hitTest(tp) then
					return false
				end
			elseif name == "ended" then
				if not self:hitTest(tp) then -- out of touch area
					name = "cancelled"
				end
			end

			-- call listener
			return self._LuaListeners[c.NODE_TOUCH_EVENT]{
				name = name,
				x = tp.x,
				y = tp.y,
				startX = sp.x,
				startY = sp.y,
				prevX = pp.x,
				prevY = pp.y,
			}
		end
		self._luaTouchListener:registerScriptHandler(function(touch, event)
			return dealFunc(touch, "began")
		end, c.Handler.EVENT_TOUCH_BEGAN)
		self._luaTouchListener:registerScriptHandler(function(touch, event)
			dealFunc(touch, "moved")
		end, c.Handler.EVENT_TOUCH_MOVED)
		self._luaTouchListener:registerScriptHandler(function(touch, event)
			dealFunc(touch, "ended")
		end, c.Handler.EVENT_TOUCH_ENDED)
		self._luaTouchListener:registerScriptHandler(function(touch, event)
			dealFunc(touch, "cancelled")
		end, c.Handler.EVENT_TOUCH_CANCELLED)
	else
		self._luaTouchListener = c.EventListenerTouchAllAtOnce:create()
		local dealFunc = function(touchs, name)
			local points = {}
			for _, touch in pairs(touchs) do
				local tp = touch:getLocation()
				local sp = touch:getStartLocation()
				local pp = touch:getPreviousLocation()
				points[touch:getId()] = {
					x = tp.x,
					y = tp.y,
					startX = sp.x,
					startY = sp.y,
					prevX = pp.x,
					prevY = pp.y,
				}
			end

			-- call listener
			self._LuaListeners[c.NODE_TOUCH_EVENT]{
				name = name,
				points = points,
			}
		end
		self._luaTouchListener:registerScriptHandler(function(touchs, event)
			dealFunc(touchs, "began")
		end, c.Handler.EVENT_TOUCHES_BEGAN)
		self._luaTouchListener:registerScriptHandler(function(touchs, event)
			dealFunc(touchs, "moved")
		end, c.Handler.EVENT_TOUCHES_MOVED)
		self._luaTouchListener:registerScriptHandler(function(touchs, event)
			dealFunc(touchs, "ended")
		end, c.Handler.EVENT_TOUCHES_ENDED)
		self._luaTouchListener:registerScriptHandler(function(touchs, event)
			dealFunc(touchs, "cancelled")
		end, c.Handler.EVENT_TOUCHES_CANCELLED)
	end
	eventDispatcher:addEventListenerWithSceneGraphPriority(self._luaTouchListener, self)
	return self
end

function Node:setTouchSwallowEnabled(enable)
	if self._luaTouchListener then
		self._luaTouchListener:setSwallowTouches(enable)
	end
end

function Node:addNodeEventListener(evt, hdl)
	self._LuaListeners = self._LuaListeners or {}
	if evt == c.NODE_EVENT then
		self:registerScriptHandler(function(evt)
			-- call listener
			self._LuaListeners[c.NODE_EVENT]{name = evt}
		end)
	end

	self._LuaListeners[evt] = hdl
end

function Node:removeNodeEventListener(evt)
    if not self._LuaListeners then return end

	if evt == c.KEYPAD_EVENT then
		self:setKeypadEnabled(false)
	elseif evt == c.NODE_EVENT then
		self:unregisterScriptHandler()
	elseif evt == c.NODE_ENTER_FRAME_EVENT then
		self:unscheduleUpdate()
	elseif evt == c.NODE_TOUCH_EVENT then
		self:setTouchEnabled(false)
	elseif evt == c.ACCELEROMETER_EVENT then
		cc.Device:setAccelerometerEnabled(false)
	end

	self._LuaListeners[evt] = nil
end

function Node:removeAllNodeEventListeners()
    self:removeNodeEventListener(c.NODE_EVENT)
    self:removeNodeEventListener(c.NODE_ENTER_FRAME_EVENT)
    self:removeNodeEventListener(c.NODE_TOUCH_EVENT)
    self:removeNodeEventListener(c.KEYPAD_EVENT)
    self:removeNodeEventListener(c.ACCELEROMETER_EVENT)
end
