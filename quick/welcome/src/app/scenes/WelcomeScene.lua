local scheduler = require(cc.PACKAGE_NAME .. ".scheduler")

local WelcomeScene = class("WelcomeScene", function()
    return display.newScene("WelcomeScene")
end)

function WelcomeScene:ctor()
    local bg = cc.LayerColor:create(cc.c4b(56, 56, 56, 255))
    self:addChild(bg)

    self:createLogo(bg)
    self:createTabWidget(bg)
    self:createCopyright(bg)
end

local function stripPath(path, maxLen)
    local l = string.len(path)
    if l <= maxLen then
        return path
    else
        local arr = string.split(path, device.directorySeparator)
        -- return arr[#arr - 1]
        return "... " .. string.sub(path, l - maxLen)
    end
end

function WelcomeScene:createLogo(node)
    display.newSprite("#Logo.png")
        :align(display.LEFT_TOP, display.left + 48, display.top - 24)
        :addTo(node)

    local label = display.newTTFLabel({
        text = __VERSION__,
        size = 30,
        color = display.COLOR_WHITE,
        x = 138,
        y = display.top - 55,
        })
    label:align(display.LEFT_CENTER)
    node:addChild(label)
end

function WelcomeScene:createButtons(node)
    local buttonWidth = 170
    local buttonHeight = 48
    local padding = 120

	local buttns = {
		{"捐献", display.top - 55, function()
			device.openURL("http://cocos2d-lua.org/donate/index.md")
		end},
		{"打开", display.top - 190, function()
			local projectConfig = ProjectConfig:new()
			local argumentVector = vector_string_:new_local()
			local index = self._selectHistoryIndex
			if index > 0 then
				local arguments = cc.player.settings.PLAYER_OPEN_RECENTS[index].args
				for _,v in ipairs(arguments) do
					argumentVector:push_back(v)
				end
				projectConfig:parseCommandLine(argumentVector)
				PlayerProtocol:getInstance():openNewPlayerWithProjectConfig(projectConfig)
			end
		end},
		{"移除", display.top - 270, function()
			local index = self._selectHistoryIndex
			self._historyListView:removeItem(index - 1)
			if index > 0 then
				table.remove(cc.player.settings.PLAYER_OPEN_RECENTS, index)
				cc.player:saveSetting()
			end

			local count = #self._historyListView:getItems()
			if self._selectHistoryIndex > count then
				self._selectHistoryIndex = count
			end

			self._selectHistoryItem = self._historyListView:getItem(self._selectHistoryIndex - 1)
			if self._selectHistoryItem then
				self._selectHistoryItem.bg:setVisible(true)
			end
		end},
		{"新建项目", display.top - 360, function()
			require("app.scenes.CreateProjectUI"):new()
				:addTo(self)
		end},
	}

	self._rightBtns = {}
	for _, info in ipairs(buttns) do
		local btn = ccui.Button:create("ButtonNormal.png",
			"ButtonPressed.png", "ButtonDisabled.png", 1)
		btn:setScale9Enabled(true)
		btn:setContentSize(cc.size(buttonWidth, buttonHeight))
		btn:setTitleText(info[1])
		btn:setTitleFontSize(24)
		btn:addTouchEventListener(function(sender, eventType)
			if 2 == eventType then
				info[3]()
			end
		end)
		btn:pos(display.width - padding, info[2])
		btn:addTo(node)

		table.insert(self._rightBtns, btn)
	end
end

function WelcomeScene:createTabWidget(node)
    self:createOpenRecents(cc.player.settings.PLAYER_OPEN_RECENTS, node)
    self:createSamples(node)
    self:createHeaders(node)
    self:createButtons(node)
end

function WelcomeScene:_createItem(icon, title, path, cb)
    local container = ccui.Layout:create()
	container:setTouchEnabled(true)
    container:setContentSize(40*16, 90)
    container.path = path
	container:addTouchEventListener(function(sender, eventType)
		if 2 == eventType then
			cb(sender)
		end
	end)

	-- bg
	local bgItem = display.newScale9Sprite("#ItemSelected.png")
	bgItem:setContentSize(cc.size(40*16, 90))
	bgItem:setAnchorPoint(cc.p(0, 0))
	bgItem:addTo(container)
	bgItem:setVisible(false)
	container.bg = bgItem

    -- icon
    display.newSprite(icon)
		:pos(40, 40)
		:addTo(container)

    -- title
    title = title:splitBySep('/')
    local titleLabel = display.newTTFLabel({
            text = title[#title],
            size = 26,
            color = display.COLOR_WHITE})
    titleLabel:align(display.LEFT_CENTER, 80, 50)
    titleLabel:addTo(container)

    -- path
    display.newTTFLabel({
        text = stripPath(path, 80),
        size = 12,
        color = display.COLOR_WHITE,
        })
    :align(display.LEFT_CENTER, 80,15)
    :addTo(container)

    display.newLine(
        {{70, 1}, {40*16 -10, 1}},
        {borderColor = cc.c4f(0, 0, 0, 1.0)})
    :addTo(container)
    return container
end

function WelcomeScene:createOpenRecents(recents, node)
	local listView = ccui.ListView:create()
	listView:setContentSize(40*17, 40*9+28)
	listView:setPosition(40,92)
    listView:setDirection(1)
	listView:addTo(node)

	local cb = function(sender)
		self._historyListView:getItem(self._selectHistoryIndex - 1).bg:setVisible(false)
		self._selectHistoryIndex = self._historyListView:getIndex(sender) + 1
		sender.bg:setVisible(true)
	end
    -- add items
    for i,v in ipairs(recents) do
        local item = self:_createItem("#Logo.png", v.title, v.title, cb)
		item.index = i
        listView:pushBackCustomItem(item)
		if 1 == i then
			item.bg:setVisible(true)
		end
    end
	self._selectHistoryIndex = 1
	self._historyListView = listView
end

function WelcomeScene:createSamples(node)
	self.samples = dofile(cc.player.quickRootPath .. "quick/samples/samples.lua")

	local listView = ccui.ListView:create()
	listView:setContentSize(40*17, 40*9+28)
	listView:setPosition(40,92)
    listView:setDirection(1)
	listView:addTo(node)

    -- add items
    for i,v in ipairs(self.samples) do
		local cb = function (sender)
			self:openDemoWithPath(v.path)
		end
		local image = cc.player.quickRootPath .. "quick/" .. v.path .. "/logo.png"
        local item = self:_createItem(image, v.title, v.description, cb)
        listView:pushBackCustomItem(item)
    end
	listView:setVisible(false)
	self._demoListView = listView
end

function WelcomeScene:createHeaders(node)
	local left = display.left + 130
    local top  = display.top - 136

    local buttonWidth = 150
    local buttonHeight = 48

	local buttons = {
		{"我的项目", function ()
			self._demoListView:setVisible(false)
			self._historyListView:setVisible(true)
			for i = 2, #self._rightBtns do
				self._rightBtns[i]:setVisible(true)
			end
		end},
		{"示例", function ()
			self._demoListView:setVisible(true)
			self._historyListView:setVisible(false)
			for i = 2, #self._rightBtns do
				self._rightBtns[i]:setVisible(false)
			end
		end},
	}

	for index, info in ipairs(buttons) do
		local btn = ccui.Button:create("TabButtonNormal.png", "TabButtonSelected.png", nil, 1)
		btn:pos(left, top)
		btn:addTo(node)
		btn:setScale9Enabled(true)
		btn:setContentSize(cc.size(buttonWidth, buttonHeight))
		btn:setTitleText(info[1])
		btn:setTitleFontSize(24)
		btn:addTouchEventListener(function(sender, eventType)
			if 2 == eventType then
				info[2]()
				-- change select stat
				self._selectTab:setBrightStyle(0)
				self._selectTab = btn
				btn:setBrightStyle(1)
			end
		end)
		left = left + 170

		-- init select stat
		if 1 == index then
			btn:setBrightStyle(1)
			self._selectTab = btn
		end
	end
end

function WelcomeScene:createCopyright(node)
    local bg = cc.LayerColor:create(cc.c4b(83, 83, 83, 255))
    bg:setContentSize(cc.size(display.width, 48))
    node:addChild(bg)

    local label = display.newTTFLabel({
        text = "Copyright (c) 2017 cocos2d-lua.org, Powered by Quick-Cocos2dx-Community",
        size = 15,
        color = cc.c3b(128, 128, 128),
        x = 48,
        y = display.bottom + 24,
    })
    label:align(display.LEFT_CENTER)
    node:addChild(label)

    label = display.newTTFLabel({
        text = "QQ群:361920466",
        size = 15,
        color = cc.c3b(128, 128, 128),
        x = display.width - 38,
        y = display.bottom + 24,
        })
    label:setAnchorPoint(1, 0.5)
    node:addChild(label)
end

function WelcomeScene:openDemoWithPath(path)
    local configPath = cc.player.quickRootPath .. "quick/" .. path .. "/src/config.lua"
    local projectConfig = ProjectConfig:new()
    projectConfig:setProjectDir(cc.player.quickRootPath .. "quick/" .. path)

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

        local with = tonumber(config.CONFIG_SCREEN_WIDTH)
        local height = tonumber(config.CONFIG_SCREEN_HEIGHT)
        projectConfig:setProjectDir(cc.player.quickRootPath .. "quick/" .. path)
        projectConfig:setFrameSize(with, height)

        -- screen direction
        if config.CONFIG_SCREEN_ORIENTATION == "portrait" then
            projectConfig:changeFrameOrientationToPortait()
        else
            projectConfig:changeFrameOrientationToLandscape()
        end
    end
    PlayerProtocol:getInstance():openNewPlayerWithProjectConfig(projectConfig)
end


return WelcomeScene
