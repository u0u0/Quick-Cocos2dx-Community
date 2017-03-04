local MainScene = class("MainScene", function()
    return display.newScene("MainScene")
end)

function MainScene:ctor()
    cc.ui.UILabel.new({UILabelType = 2, text = "Hot Update demo", size = 64})
        :align(display.CENTER, display.cx, display.cy)
        :addTo(self)

	cc.ui.UIPushButton.new({normal = "up2.png", pressed = "up2.png"})
        :onButtonClicked(function()
            self:addhotupdate()
        end)
        :addTo(self):align(display.CENTER, display.cx, display.cy-200)
end

function MainScene:onEnter()
end

function MainScene:onExit()
end

function MainScene:addhotupdate()
	local writablepath = cc.FileUtils:getInstance():getWritablePath()
    local storagepath = writablepath .. "hotupdate/"
	
	--[[
	参数1是读取文件地址。
	参数2是下载的资源储存到哪。
	如果要将 project.manifest 放到 res/version 下的话，
	必须设置优先路径 res/version，否则 project.manifest 只能放在res目录下
	]]
    local am = cc.AssetsManagerEx:create("project.manifest",storagepath)
    am:retain()
	self.am = am 
	self.failedcount = 0
    --获得当前本地版本
    local localManifest = am:getLocalManifest()
    print(localManifest:getVersion())
	print("getPackageUrl",localManifest:getPackageUrl())
	print("getManifestFileUrl",localManifest:getManifestFileUrl())
	print("getVersionFileUrl",localManifest:getVersionFileUrl())

    if not am:getLocalManifest():isLoaded() then 
        print("加载本地project.manifest错误.")
        --进登录界面
    else 
        local listener = cc.EventListenerAssetsManagerEx:create(am,function(event)
            self:onUpdateEvent(event)
        end)
        cc.Director:getInstance():getEventDispatcher():addEventListenerWithFixedPriority(listener,1)
        am:update()
    end
end

function MainScene:onUpdateEvent(event)
    local eventCode = event:getEventCode()

	local assetId = event:getAssetId()
    local percent = event:getPercent()
    local percentByFile = event:getPercentByFile()
    local message = event:getMessage()
    printInfo("游戏更新("..eventCode.."):"..", assetId->"..assetId..", percent->"..percent..", percentByFile->"..percentByFile..", message->"..message)
    if eventCode == cc.EventAssetsManagerEx.EventCode.ERROR_NO_LOCAL_MANIFEST then
        print("找不到本地manifest文件.")
		self._perent = 100
        --进登录界面 
    elseif eventCode == cc.EventAssetsManagerEx.EventCode.UPDATE_PROGRESSION then
        print("正在更新文件 : ",event:getAssetId())
        --print("更新进度 : ",event:getPercent())
        if event:getAssetId() == cc.AssetsManagerExStatic.VERSION_ID then 
            --print("文件版本 : ",event:getPercent())
        elseif event:getAssetId() == cc.AssetsManagerExStatic.MANIFEST_ID then
            --print("文件Manifest : ",event:getPercent())
        else 
            --print("进度条的进度 : ",event:getPercent())
            --跳进度
			self._perent = event:getPercentByFile()
        end
    elseif eventCode == cc.EventAssetsManagerEx.EventCode.ERROR_DOWNLOAD_MANIFEST or 
        	eventCode == cc.EventAssetsManagerEx.EventCode.ERROR_PARSE_MANIFEST then
        print("远程资源清单文件下载失败")
		self._perent = 100
        --print("资源清单文件解析失败 ")
    elseif eventCode == cc.EventAssetsManagerEx.EventCode.ALREADY_UP_TO_DATE then 
		print("已经是服务器最新版本ALREADY_UP_TO_DATE")
		self._perent = 100
	elseif eventCode == cc.EventAssetsManagerEx.EventCode.UPDATE_FINISHED then
        print("更新到服务器最新版本UPDATE_FINISHED")
		self._perent = 100
		cc.FileUtils:getInstance():purgeCachedEntries()
		package.loaded["app.scenes.MainScene"] = nil
		package.preload["app.scenes.MainScene"] = nil
		app:run()
    elseif eventCode == cc.EventAssetsManagerEx.EventCode.ERROR_UPDATING then
        print("更新过程中遇到错误")
    elseif eventCode == cc.EventAssetsManagerEx.EventCode.NEW_VERSION_FOUND  then
        print("发现新版本，开始升级",self.am:getRemoteManifest():getVersion())
    elseif eventCode == cc.EventAssetsManagerEx.EventCode.UPDATE_FAILED  then
		print("更新失败")
		if self.failedcount > 10 then 
			self._perent = 100
		else 
			self.failedcount = self.failedcount + 1
			self.am:downloadFailedAssets()  --如果有的文件更新失败,连续更新10次,超过十次还是进游戏
		end
    end 
end

return MainScene
