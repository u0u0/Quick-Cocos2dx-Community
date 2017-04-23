require("config")
require("cocos.init")
require("framework.init")

local AppBase = require("framework.AppBase")
local WelcomeApp = class("WelcomeApp", AppBase)

function WelcomeApp:run()
    cc.FileUtils:getInstance():addSearchPath("res/")
    display.addSpriteFrames("Images_hd.plist", "Images_hd.png")
	
    self:enterScene("WelcomeScene")
end

return WelcomeApp
