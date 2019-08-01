--[[
Design for Quick-Cocos2dx-Community
First Write at 2017.2.17 by u0u0
]]

local socket = require "socket"
if not socket then return end

local scheduler = require("framework.scheduler")
local SimpleTCP = class("SimpleTCP")

local string = string
local pairs = pairs
local print = print
local assert = assert

--------- class var and method --------------
SimpleTCP._VERSION = socket._VERSION
SimpleTCP._DEBUG = socket._DEBUG
SimpleTCP.CONNECT_TIMEOUT = 15 -- second

SimpleTCP.STAT_CONNECTING = 1
SimpleTCP.STAT_FAILED = 2
SimpleTCP.STAT_CONNECTED = 3
SimpleTCP.STAT_CLOSED = 4

SimpleTCP.EVENT_CONNECTING = "Connecting"
SimpleTCP.EVENT_FAILED = "Failed"
SimpleTCP.EVENT_CONNECTED = "Connected"
SimpleTCP.EVENT_CLOSED = "Closed"
SimpleTCP.EVENT_DATA = "Data"

function SimpleTCP.getTime()
	return socket.gettime()
end

-------- instant var and public method -------------
function SimpleTCP:ctor(host, port, callback)
	if not host then print("Worning SimpleTCP:ctor() host is nil") end
	if not port then print("Worning SimpleTCP:ctor() port is nil") end

	self.host = host
	self.port = port
	self.tcp = nil
	self.callback = callback
end

--[[
start connect by user
]]
function SimpleTCP:connect()
	if (self.stat == SimpleTCP.STAT_CONNECTING or self.stat == SimpleTCP.STAT_CONNECTED) then
		print("Error: SimpleTCP:connect() call at wrong stat:", self.stat)
		return
	end

	self.stat = SimpleTCP.STAT_CONNECTING
	self.callback(SimpleTCP.EVENT_CONNECTING)
	self.connectingTime = 0

	if self.tcp then
		self:_connectAndCheck()
		-- start global scheduler
		assert(not self.globalUpdateHandler, "SimpleTCP:connect status wrong, need reviewing!")
		self.globalUpdateHandler = scheduler.scheduleUpdateGlobal(handler(self, self._update))
	else
		-- if "closed", create a new LuaSocket
		socket.dns.isIpv6(self.host, function(err, isIpv6)
			assert(err == nil, "Error in socket.dns.isIpv6")
			-- get a master socket
			if isIpv6 then
				self.tcp = socket.tcp6()
			else
				self.tcp = socket.tcp()
			end
			-- make LuaSocket work like Asynchronously
			self.tcp:settimeout(0)

			self:_connectAndCheck()
			-- start global scheduler
			assert(not self.globalUpdateHandler, "SimpleTCP:connect status wrong, need reviewing!")
			self.globalUpdateHandler = scheduler.scheduleUpdateGlobal(handler(self, self._update))
		end)
	end
end

--[[
send data to server by user
]]
function SimpleTCP:send(data)
	if self.stat ~= SimpleTCP.STAT_CONNECTED then
		print("Error: SimpleTCP is not connected.")
		return
	end
	self.tcp:send(data)
end

--[[
close by user, but SimpleTCP.EVENT_CLOSED will waiting for server's response
]]
function SimpleTCP:close()
	if self.stat == SimpleTCP.STAT_CONNECTING then
		print("Error: SimpleTCP is connecting, wait it end then you can call close()")
		return
	end

	-- set self.tcp = nil in _update()
	self.tcp:close()
end

------- private methods ------------

--[[
In asynchronous LuaSocket useage, use connect() return for stat checking 
Return true for SimpleTCP.STAT_CONNECTED
]]
function SimpleTCP:_connectAndCheck()
	local rtn, err = self.tcp:connectAsyn(self.host, self.port)
	-- err in case of "already connected" is special fix for LuaSocket working on windows
	-- refer to: http://lua-users.org/lists/lua-l/2009-10/msg00584.html
	return rtn == 1 or err == "already connected"
end

function SimpleTCP:_update(dt)
	if self.stat == SimpleTCP.STAT_CONNECTED then
		local body, status, partial = self.tcp:receive("*a")	-- receive mode: get all data
		-- 1. If receive successful
		if body and string.len(body) > 0 then
			self.callback(SimpleTCP.EVENT_DATA, body)
			return
		end

		-- 2. If got an error. Firstly, transfer partial data.
		if partial and string.len(partial) > 0 then
			self.callback(SimpleTCP.EVENT_DATA, partial)
			-- Not return here, continue to check the error type
		end

		-- 3. Error type "timeout" will be ignored; but "closed" need handling.
		if status == "closed" or status == "Socket is not connected" then
			-- if close from server, safty free LuaSocket resource
			self.tcp:close()
			self.tcp = nil
			-- stop scheduler
			scheduler.unscheduleGlobal(self.globalUpdateHandler)
			self.globalUpdateHandler = nil
			-- notification
			self.stat = SimpleTCP.STAT_CLOSED
			self.callback(SimpleTCP.EVENT_CLOSED)
		end
		return
	end

	if self.stat == SimpleTCP.STAT_CONNECTING then
		if self:_connectAndCheck() then
			self.stat = SimpleTCP.STAT_CONNECTED
			self.callback(SimpleTCP.EVENT_CONNECTED)
			return
		else
			self.connectingTime = self.connectingTime + dt
			if self.connectingTime >= SimpleTCP.CONNECT_TIMEOUT then
				-- stop scheduler
				scheduler.unscheduleGlobal(self.globalUpdateHandler)
				self.globalUpdateHandler = nil
				-- notification
				self.stat = SimpleTCP.STAT_FAILED
				self.callback(SimpleTCP.EVENT_FAILED)
			end
			return
		end
	end
end

return SimpleTCP
