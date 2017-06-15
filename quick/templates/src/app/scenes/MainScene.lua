
local MainScene = class("MainScene", function()
    return display.newScene("MainScene")
end)

function MainScene:ctor()
    display.newTTFLabel({text = "Hello, World", size = 64})
        :align(display.CENTER, display.cx, display.cy)
        :addTo(self)
end

function MainScene:onEnter()
end

function MainScene:onExit()
end

return MainScene
