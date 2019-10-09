local eventDispatcher = cc.Director:getInstance():getEventDispatcher()

local OpenProjectUI = class("OpenProjectUI", function()
	return display.newColorLayer(cc.c4b(56, 56, 56, 250))
end)

function OpenProjectUI:ctor()
	self:addNodeEventListener(cc.NODE_EVENT, function(e)
		if e.name == "enter" then
			self:onEnter()
		elseif e.name == "exit" then
			eventDispatcher:removeEventListener(self.eventListenerCustom_)
		end
	end)
end

function OpenProjectUI:onEnter()
    -- project location:
    display.newTTFLabel({
        text = "Choose Project Location:",
        size = 25,
        color = display.COLOR_WHITE
	})
    :align(display.LEFT_CENTER, 40, display.top - 55)
    :addTo(self)

	local locationEditbox = ccui.EditBox:create(cc.size(display.width-250, 40), "ButtonNormal.png", 1)
    locationEditbox:setAnchorPoint(0,0)
	locationEditbox:pos(40, display.top - 120)
    self:addChild(locationEditbox)
	
	local selectButton = ccui.Button:create("ButtonNormal.png",
			"ButtonPressed.png", "ButtonDisabled.png", 1)
	selectButton:setScale9Enabled(true)
    selectButton:setAnchorPoint(0,0)
    selectButton:setContentSize(cc.size(150, 40))
	selectButton:setTitleText("Select")
    selectButton:pos(display.right - 170, display.top - 120)
	selectButton:setTitleFontSize(25)
    selectButton:addTo(self)
	selectButton:addTouchEventListener(function(sender, eventType)
		if 2 == eventType then
			local filedialog = PlayerProtocol:getInstance():getFileDialogService()
			local locationDirectory = filedialog:openDirectory("Choose Localtion", "")
			if string.len(locationDirectory) > 0 then
				-- parse config
				local configPath = locationDirectory .. '/src/config.lua'
				if cc.FileUtils:getInstance():isFileExist(configPath) then
					local data = ""
					for line in io.lines(configPath) do
						if string.find(line, "CONFIG_SCREEN_WIDTH") then
							data = data .. line .. ',\n'
						elseif string.find(line, "CONFIG_SCREEN_HEIGHT") then
							data = data .. line .. ',\n'
						elseif string.find(line, "CONFIG_SCREEN_ORIENTATION") then
							data = data .. line .. ',\n'
						end
					end

					local config = assert(loadstring("local settings = {" .. data .. "} return settings"))()

					-- create openable obj
					local projectConfig = ProjectConfig:new()
					projectConfig:setProjectDir(locationDirectory)
					local width = tonumber(config.CONFIG_SCREEN_WIDTH)
					local height = tonumber(config.CONFIG_SCREEN_HEIGHT)
					projectConfig:setFrameSize(cc.size(width, height))
					if config.CONFIG_SCREEN_ORIENTATION == "portrait" then
						projectConfig:changeFrameOrientationToPortait()
					elseif config.CONFIG_SCREEN_ORIENTATION == "landscape" then
						projectConfig:changeFrameOrientationToLandscape()
					end
					self.projectConfig = projectConfig
					-- update display
					locationEditbox:setText(locationDirectory)
				else
					-- update display
					locationEditbox:setText("Not a valid project root")
					self.projectConfig = nil
				end
			end
		end
	end)

    -- ok or cancel
	local button = ccui.Button:create("ButtonNormal.png",
		"ButtonPressed.png", "ButtonDisabled.png", 1)
	button:setScale9Enabled(true)
	button:setContentSize(cc.size(150, 40))
	button:setTitleText("Cancel")
	button:setTitleFontSize(25)
	button:addTouchEventListener(function(sender, eventType)
		if 2 == eventType then
			self:removeFromParent(true)
		end
	end)
	button:pos(display.right - 280, 30)
	button:addTo(self)

	local OpenProjectBtn = ccui.Button:create("ButtonNormal.png",
		"ButtonPressed.png", "ButtonDisabled.png", 1)
	OpenProjectBtn:setScale9Enabled(true)
	OpenProjectBtn:setContentSize(cc.size(150, 40))
	OpenProjectBtn:setTitleText("Open")
	OpenProjectBtn:setTitleFontSize(25)
	OpenProjectBtn:addTouchEventListener(function(sender, eventType)
		if 2 == eventType then
			if self.projectConfig then
				PlayerProtocol:getInstance():openNewPlayerWithProjectConfig(self.projectConfig)
			end
		end
	end)
	OpenProjectBtn:pos(display.right - 100, 30)
	OpenProjectBtn:addTo(self)

	-- keyboard event
    local event = function(e)
        local data = json.decode(e:getDataString())
        if data == nil then return end

        -- esc keyc = 6
        if data.data == 6 then self:removeFromParent(true) end
    end
    self.eventListenerCustom_ = cc.EventListenerCustom:create("APP.EVENT", event)
    eventDispatcher:addEventListenerWithFixedPriority(self.eventListenerCustom_, 1)
end

return OpenProjectUI
