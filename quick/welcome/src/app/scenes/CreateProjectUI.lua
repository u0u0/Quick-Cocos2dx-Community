--
-- FILE: CreateProjectUI.lua
-- DATE: 2014-08-21
--

local eventDispatcher = cc.Director:getInstance():getEventDispatcher()

local CreateProjectUI = class("CreateProjectUI", function()
	return display.newColorLayer(cc.c4b(56, 56, 56, 250))
end)

function CreateProjectUI:ctor()
	self:addNodeEventListener(cc.NODE_EVENT, function(e)
		if e.name == "enter" then
			self:onEnter()
		elseif e.name == "exit" then
			eventDispatcher:removeEventListener(self.eventListenerCustom_)
		end
	end)
end

function CreateProjectUI:onEnter()
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
				locationEditbox:setText(locationDirectory)
			end
		end
	end)

    -- package name:
	display.newTTFLabel({
		text = "Project package name: (etc: com.mycomp.mygame)",
		size = 25,
		color = display.COLOR_WHITE,
	})
    :align(display.LEFT_CENTER, 40, display.top - 155)
    :addTo(self)

	local packageEditbox = ccui.EditBox:create(cc.size(display.width-250, 40), "ButtonNormal.png", 1)
    packageEditbox:setAnchorPoint(0,0)
	packageEditbox:pos(40, display.top - 220)
    self:addChild(packageEditbox)

    -- screen direction:
	display.newTTFLabel({
        text = "Screen Direction:",
        size = fontSize,
        color = display.COLOR_WHITE,
	})
    :align(display.LEFT_CENTER, 40, display.top - 255)
    :addTo(self)

	display.newTTFLabel({text = "Portrait", size = 25, color = display.COLOR_WHITE})
        :align(display.LEFT_CENTER, 100, display.cy)
		:addTo(self)
	local portraitCheckBox = ccui.CheckBox:create("CheckBoxButton2Off.png", nil,
		"CheckBoxButton2On.png", nil, nil)
        :align(display.LEFT_CENTER, 40, display.cy)
        :addTo(self)
	portraitCheckBox:setSelected(true)

	display.newTTFLabel({text = "Landscape", size = 25, color = display.COLOR_WHITE})
        :align(display.LEFT_CENTER, 260, display.cy)
		:addTo(self)
	local landscapeCheckBox = ccui.CheckBox:create("CheckBoxButton2Off.png", nil,
		"CheckBoxButton2On.png", nil, nil)
        :align(display.LEFT_CENTER, 200, display.cy)
        :addTo(self)
	landscapeCheckBox:setSelected(false)
	landscapeCheckBox:addEventListener(function(sender, eventType)
		if 0 == eventType then -- selected
			portraitCheckBox:setSelected(false)
		else -- 1 unselected
			portraitCheckBox:setSelected(true)
		end
	end)
	portraitCheckBox:addEventListener(function(sender, eventType)
		if 0 == eventType then -- selected
			landscapeCheckBox:setSelected(false)
		else -- 1 unselected
			landscapeCheckBox:setSelected(true)
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

	local createProjectbutton = ccui.Button:create("ButtonNormal.png",
		"ButtonPressed.png", "ButtonDisabled.png", 1)
	createProjectbutton.currState = 1
	createProjectbutton:setScale9Enabled(true)
	createProjectbutton:setContentSize(cc.size(150, 40))
	createProjectbutton:setTitleText("Create")
	createProjectbutton:setTitleFontSize(25)
	createProjectbutton:addTouchEventListener(function(sender, eventType)
		if 2 == eventType then
			if createProjectbutton.currState == 1 then
				if string.len(locationEditbox:getText()) > 0 and string.len(packageEditbox:getText()) > 0 then
					createProjectbutton:setTitleText("Processing")
					local t = packageEditbox:getText():splitBySep('.')
					self.projectFolder = locationEditbox:getText() .. device.directorySeparator .. t[#t]

					local projectConfig = ProjectConfig:new()
					projectConfig:setProjectDir(self.projectFolder)
					projectConfig:changeFrameOrientationToPortait()
					self.projectConfig = projectConfig

					local scriptPath = cc.player.quickRootPath .. "quick/bin/CreateProject.py"

					local screenDirection = ""
					if landscapeCheckBox:isSelected() then
						projectConfig:changeFrameOrientationToLandscape()
						screenDirection = " -l"
					end
					local arguments = " -p " .. packageEditbox:getText() .. " -o " .. locationEditbox:getText() .. screenDirection
					local taskId = tostring(os.time())

					print("Create Cmd:" .. scriptPath .. " " .. arguments)
					local task = PlayerProtocol:getInstance():getTaskService():createTask(taskId, scriptPath, arguments)
					eventDispatcher:addEventListenerWithFixedPriority(cc.EventListenerCustom:create(taskId, function()
						if task:getResultCode() == 0 then
							createProjectbutton:setTitleText("Open ...")
							createProjectbutton.currState = 2
						else
							createProjectbutton:setTitleText("normal", "Create")
							local messageBox = PlayerProtocol:getInstance():getMessageBoxService()
							messageBox:showMessageBox("player v3", task:getOutput())
						end
					end), 1)
					task:runInTerminal()

					createProjectbutton:setTitleText("Open ...")
					createProjectbutton.currState = 2
					local messageBox = PlayerProtocol:getInstance():getMessageBoxService()
					messageBox:showMessageBox("player v3", "Please wait create success and then click Open")
				else
					local messageBox = PlayerProtocol:getInstance():getMessageBoxService()
					messageBox:showMessageBox("player v3", "please fill all infomation..")
				end
			else
				PlayerProtocol:getInstance():openNewPlayerWithProjectConfig(self.projectConfig)
			end
		end
	end)
	createProjectbutton:pos(display.right - 100, 30)
	createProjectbutton:addTo(self)

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

return CreateProjectUI
