
require("config")
require("cocos.init")
require("framework.init")

local MyApp = class("MyApp", cc.mvc.AppBase)

function MyApp:ctor()
    MyApp.super.ctor(self)
end

function MyApp:run()
	local writePath = cc.FileUtils:getInstance():getWritablePath() 
	cc.FileUtils:getInstance():addSearchPath(writePath .. "hotupdate/res/")
	cc.FileUtils:getInstance():addSearchPath("res/")

	print("== listing engine search path")
	local searchPaths = cc.FileUtils:getInstance():getSearchPaths()
	for k,v in pairs(searchPaths) do
		print(k,v)
	end

	self:enterScene("MainScene")
end

return MyApp
