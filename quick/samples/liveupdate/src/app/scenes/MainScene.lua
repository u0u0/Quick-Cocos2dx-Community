--
-- Author: rsma
-- Date: 2016-03-17 15:39:34
--

require("app.util.UpdateUtil")

local MainScene = class("MainScene", function()
    return display.newScene("MainScene")
end)

function MainScene:ctor()
    checkUpdateFile()
end

function MainScene:onEnter()
	display.newSprite("bg.png")
		:align(display.CENTER, display.cx, display.cy)
		:setScale(100)
		:addTo(self)
	self.lblmsg = require("app.scenes.UILiveUpdate").new()
		:addTo(self)
	self.btnupdate = cc.ui.UIPushButton.new("update.png", {scale9 = true})
        :setButtonSize(180, 78)
        :setButtonLabel(cc.ui.UILabel.new({text = "更新游戏版本", size = 26, color = display.COLOR_BLUE}))
        :onButtonPressed(function(event)
            event.target:getButtonLabel():setColor(display.COLOR_RED)
        end)
        :onButtonRelease(function(event)
            event.target:getButtonLabel():setColor(display.COLOR_BLUE)
        end)
        :onButtonClicked(function(event)
            self:startUpdate()
        end)
        :align(display.CENTER,display.cx, display.cy-100)
        :addTo(self)
    self.txtlog=cc.ui.UILabel.new({
            text = "",
            color = cc.c3b(255, 0, 0),
            shadowColor = cc.c3b(106, 57, 6),
            size = 30,
        })
            :align(display.CENTER_BOTTOM,display.cx,50)
            :addTo(self)
    -- self:buildUpdateUI()
end

function MainScene:startUpdate()
    if UpdateResConfig and GAME_VERSION >= UpdateResConfig.ver then
        print("已是最新版本，可删除/upd目录，重新执行更新!")
        self.txtlog:setString("已是最新版本，可删除/upd目录，重新执行更新!")
        return
    end
    self.currUpdAction=nil --记录当前更新过程中单个阶段
    self.currUpdFileIndex=1 ----当前更新的进度
    self.numFile=0
    self.numFileCheck = 0
    if not checkDirOK(UpdatePath) then
        self:exitUpdate()
        return
    end
    self:buildUpdateUI()
    self.txtprocess:setString("校验版本中.....")
    self.currUpdAction="CheckVersion"
    self:requestFromServer("ResList")
end

--构建更新进度UI
--更新界面-面向玩家显示整个更新过程
function MainScene:buildUpdateUI()
    self.progress = display.newSprite("ui_loading_bg.png")
        :align(display.CENTER,display.cx,160)
        :addTo(self)
        -- :hide()
    self.fill = cc.ui.UILoadingBar.new({
                scale9 = true,
                capInsets = cc.rect(0,0,0,0),
                percent = self.percent,
                direction = 0,
                image =  "ui_loading_bar.png",
                viewRect = cc.rect(0,0,525,26),
            })
        :addTo(self.progress)
    self.fill:setPosition(8, 8)
    self.txtprocess=cc.ui.UILabel.new({
        text = "正在检查资源......",
        color = cc.c3b(245, 237, 223),
        shadowColor = cc.c3b(106, 57, 6),
        size = 25,
    })
        :align(display.CENTER_BOTTOM,display.cx,100)
        :addTo(self)
    -- self.fill:setPercent(100)
end

function MainScene:cleanUpdateUI()
    self.fill:removeSelf()
    self.progress:removeSelf()
    self.txtprocess:removeSelf()
end

--向服务器请求资源
--@filename 资源名
--@waittime 连接等待时长
function MainScene:requestFromServer(filename, waittime)
    math.newrandomseed()
    local url = RES_SERVER .. filename .. "?dev="..device.platform .. "&version=" .. math.floor(math.random()*10000)
    print("*****Resource  requestFromServer**********")
    local request = network.createHTTPRequest(function(event)
        self:onResponse(event)
    end, url, "GET")
    -- end, url, "POST")
    request:setTimeout(15)
    request:start()
end

--响应服务器下载
function MainScene:onResponse(event)
    local request = event.request
    if event.dltotal and event.dltotal>0 and self.numFile>0 then
        self.fill:setPercent((self.numFileCheck/self.numFile)*100)
    end
    if event.name == "completed" then
        local rscode=request:getResponseStatusCode()
        printf("getResponseStatusCode() = %d", rscode)
        if rscode ~= 200 then
            if (rscode==404) then
                self:onUpdateEnd(true)
            else
                self:onUpdateEnd(false)
            end
        else
            if self.currUpdAction == "CheckVersion" then
                self:checkVersion(request:getResponseData())
            elseif self.currUpdAction == "UpdateRes" then
                self:updateResource(request:getResponseData())
            end
        end
    elseif event.name== "failed" then
        printf("onRequest error REQUEST %d - getErrorCode() = %d, getErrorMessage() = %s", index, request:getErrorCode(), request:getErrorMessage())
        self:onUpdateEnd(false)
    end
end

function MainScene:onUpdatePercent(i_Msg,i_Event)
    if i_Event and i_Event.dltotal and i_Event.dltotal>0 and self.numFile>0 then
        self.fill:setPercent((self.numFileCheck/self.numFile)*100)
    end
    self.txtprocess:setString(i_Msg or "")
end

--检查要更新文件列表
function MainScene:checkVersion(dataRecv)
    if dataRecv then
        io.writefile(UpdatePath.. UpdateResName, dataRecv)
        UpdateResConfig = dofile(UpdatePath.. UpdateResName)
        if GAME_VERSION>=UpdateResConfig.ver then
            self:onUpdateEnd(true)
            return
        end
         GAME_VERSION=UpdateResConfig.ver
        self.currUpdFileIndex=1
        --需要更新的文件总数
        self.numFile=#UpdateResConfig.stage
        self.numFileCheck = 0
        self.currUpdAction = "UpdateRes"
        self.txtprocess:setString("正在更新资源，请耐心等候......")
        self:requestFromServer(UpdateResConfig.stage[self.currUpdFileIndex].path..UpdateResConfig.stage[self.currUpdFileIndex].name)
    end
end

function MainScene:updateResource(dataRecv)
    if dataRecv then
        io.writefile(UpdatePath.. UpdateResConfig.stage[self.currUpdFileIndex].name, dataRecv)
    end
    if self.currUpdFileIndex >= #UpdateResConfig.stage then
        self:updateFiles()
    else
        self.currUpdFileIndex = self.currUpdFileIndex + 1
        self.txtprocess:setString("正在更新资源，请耐心等候......")
        self:requestFromServer(UpdateResConfig.stage[self.currUpdFileIndex].path..UpdateResConfig.stage[self.currUpdFileIndex].name)
    end
end

--更新本地的下载资源列表及将.upd文件改成正常文件
function MainScene:updateFiles()
    self.txtprocess:setString("资源解压中......")
    --将更新的资源加载到内存
    if UpdateResConfig then
        if  UpdateResConfig.stage then
            for i,v in ipairs( UpdateResConfig.stage) do
                if v.act=="load" then
                    cc.LuaLoadChunksFromZIP(UpdatePath..v.path..v.name)
                end
            end
        end
        --更新版本号
        UPDATE_VERSION=UpdateResConfig.ver
    end
    self:onUpdateEnd(true)
end

function MainScene:onUpdateEnd()
    self.btnupdate:setVisible(false)
    self.txtprocess:setString("更新完成!")
    self.fill:setPercent(100)
    self:performWithDelay(handler(self,self.exitUpdate),1)
end

-- 退出更新
function MainScene:exitUpdate()
    self:cleanUpdateUI()
    self.lblmsg:removeSelf()
    package.loaded[ "app.scenes.UILiveUpdate"]  = nil
    require("app.scenes.UILiveUpdate").new()
        :addTo(self)
end

return MainScene
