-- for new json file export from ccs2.x 


local UILoaderUtilitys = import(".UILoaderUtilitys")
local CCSUILoader = class("CCSUILoader")

function CCSUILoader:load(json, params)
	if params then
		self.bUseEditBox = params.bUseEditBox or false
	else
		self.bUseEditBox = false
	end

	if cc.bPlugin_ and self.bUseEditBox then
		-- CCEditbox(C++ widget) not support no background
		self.bUseEditBox = false

		print("Error! not support CCEditbox in cocostudio layout file")
	end

	self.texturesPng = json.texturesPng
	self:loadTexture(json)

	local node, bAdaptScreen = self:parserJson(json)
	self.texturesPng = nil
	if bAdaptScreen then
		return node, display.width, display.height
	else
		return node, json.designWidth, json.designHeight
	end
end

function CCSUILoader:loadFile(jsonFile, params)
	local fileUtil = cc.FileUtils:getInstance()
	local fullPath = fileUtil:fullPathForFilename(jsonFile)
	local jsonStr = fileUtil:getStringFromFile(fullPath)
	local jsonVal = json.decode(jsonStr)

	UILoaderUtilitys.addSearchPathIf(io.pathinfo(fullPath).dirname)

	local node, w, h = self:load(jsonVal, params)

	UILoaderUtilitys.clearPath()

	return node, w, h
end

function CCSUILoader:parserJson(jsonVal)
	local root = jsonVal.Content.Content.ObjectData
	if not root then
		printInfo("CCSUILoader - parserJson havn't found root node")
		return
	end
	self:prettyJson(root)
	local uiRoot = self:generateUINode(root)

	return uiRoot
end

-- generate a ui node and invoke self to generate child ui node
function CCSUILoader:generateUINode(jsonNode, parent)
	local clsName = string.sub(jsonNode.ctype, 1, -11)
	local uiNode = self:createUINode(clsName, jsonNode, parent)
	if not uiNode then
		return
	end

	uiNode.name = jsonNode.Name or "unknow node"

	--for seek
	uiNode.subChildren = {}
	if parent then
		parent.subChildren[uiNode.name] = uiNode
	end

	-- print("CCSUILoader set node params:" .. uiNode.name)

	if uiNode.setFlippedX then
		uiNode:setFlippedX(jsonNode.FlipX or false)
	end
	if uiNode.setFlippedY then
		uiNode:setFlippedY(jsonNode.FlipY or false)
	end

	uiNode:setTag(jsonNode.Tag or 0)
	uiNode:setRotation(jsonNode.Rotation or 0)
	uiNode:setSkewX(jsonNode.RotationSkewX or 0)
	uiNode:setSkewY(jsonNode.RotationSkewY or 0)

	-- uiNode:setVisible(jsonNode.visible) -- ccs havn't export visible attribute
	if jsonNode.Scale then
		uiNode:setScaleX((jsonNode.Scale.ScaleX or 1) * uiNode:getScaleX())
		uiNode:setScaleY((jsonNode.Scale.ScaleY or 1) * uiNode:getScaleY())
	end



	local emptyNode
	if "ScrollView" == clsName then
		emptyNode = cc.Node:create()
		emptyNode:setPosition(jsonNode.Position.X, jsonNode.Position.Y)
		uiNode:addScrollNode(emptyNode)
	end

	local children = jsonNode.Children
	if children then
	for i,v in ipairs(children) do
		local childrenNode = self:generateUINode(v, uiNode)
		if childrenNode then
			if "ScrollView" == clsName then
				emptyNode:addChild(childrenNode)
			elseif "ListView" == clsName then
				local item = uiNode:newItem()
				item:addContent(childrenNode)
				local size = childrenNode:getContentSize()
				item:setItemSize(size.width, size.height)
				uiNode:addItem(item)

				if "Button" == v.classname then
					children:setTouchSwallowEnabled(false)
				end
			elseif "PageView" == clsName then
				local item = uiNode:newItem()
				childrenNode:setPosition(0, 0)
				item:addChild(childrenNode)
				item:setTag(10001)
				uiNode:addItem(item)
			else
				uiNode:addChild(childrenNode)
			end
		end
	end
	end

	if "ListView" == clsName or "PageView" == clsName then
		uiNode:reload()
	elseif "ScrollView" == clsName then
		uiNode:resetPosition()
	end

	return uiNode
end

function CCSUILoader:createUINode(clsName, options, parent)
	if not clsName then
		return
	end

	printInfo("CCSUILoader - createUINode:" .. clsName)

	local node

	if clsName == "SingleNode" or clsName == "Node" then
		node = self:createNode(options)
	elseif clsName == "Sprite" or clsName == "Scale9Sprite" then
		node = self:createSprite(options)
	elseif clsName == "ImageView" then
		node = self:createImage(options)
	elseif clsName == "Button" then
		node = self:createButton(options)
	elseif clsName == "LoadingBar" then
		node = self:createLoadingBar(options)
	elseif clsName == "Slider" then
		node = self:createSlider(options)
	elseif clsName == "CheckBox" then
		node = self:createCheckBox(options)
    elseif clsName == "TextBMFont" then
    	node = self:createBMFontLabel(options)
    elseif clsName == "Text" then
    	node = self:createLabel(options)
    elseif clsName == "LabelAtlas" then
    	node = self:createLabelAtlas(options)
	elseif clsName == "TextField" then
		node = self:createEditBox(options)
	elseif clsName == "Panel" then
		node = self:createPanel(options)
	elseif clsName == "ScrollView" then
		node = self:createScrollView(options)
	elseif clsName == "ListView" then
		node = self:createListView(options)
	elseif clsName == "PageView" then
		node = self:createPageView(options)
	elseif clsName == "ProjectNode" then
		node = self:createProjectNode(options)
	elseif clsName == "Layer" then
        node = self:createLayer(options)
	else
		printInfo("CCSUILoader not support node:" .. clsName)
	end

	return node
end

function CCSUILoader:getChildOptionJson(json)
	return json.options.layoutParameter
end

function CCSUILoader:newWapperNode(oldNode, layoutParameter)
	local newNode = display.newNode()
	local size = oldNode:getContentSize()
	size.width = size.width + layoutParameter.marginLeft + layoutParameter.marginRight
	size.height = size.height + layoutParameter.marginTop + layoutParameter.marginDown
	newNode:setContentSize(size)
	newNode:addChild(oldNode)
	oldNode:setPosition()
end

function CCSUILoader:getButtonStateImages(options)
	local images = {}
	if options.NormalFileData then
		images.normal = self:transResName(options.NormalFileData)
	end
	if options.PressedFileData then
		images.pressed = self:transResName(options.PressedFileData)
	end
	if options.DisabledFileData then
		images.disabled = self:transResName(options.DisabledFileData)
	end

	return images
end

function CCSUILoader:getAnchorType(anchorX, anchorY)
	if 1 == anchorX then
		if 1 == anchorY then
			return display.RIGHT_TOP
		elseif 0.5 == anchorY then
			return display.RIGHT_CENTER
		else
			return display.RIGHT_BOTTOM
		end
	elseif 0.5 == anchorX then
		if 1 == anchorY then
			return display.CENTER_TOP
		elseif 0.5 == anchorY then
			return display.CENTER
		else
			return display.CENTER_BOTTOM
		end
	else
		if 1 == anchorY then
			return display.LEFT_TOP
		elseif 0.5 == anchorY then
			return display.LEFT_CENTER
		else
			return display.LEFT_BOTTOM
		end
	end
end


function CCSUILoader:getCheckBoxImages(options)
	local images = {}

	local getBackgroundImage = function(state)
		local image = options.NormalBackFileData
		if "pressed" == state then
			image = options.PressedBackFileData
		elseif "disabled" == state then
			image = options.DisableBackFileData
		end

		return image
	end

	-- local getFrontgroundImage = function(state)
	-- 	local image = options.backGroundBoxData.path
	-- 	if "pressed" == state then
	-- 		image = options.backGroundBoxSelectedData.path
	-- 	end
	-- 	if "disabled" == state then
	-- 		image = options.backGroundBoxDisabledData.path
	-- 	end

	-- 	return image
	-- end

	images.off = self:transResName(getBackgroundImage("normal"))
	images.off_pressed = self:transResName(getBackgroundImage("pressed"))
	images.off_disabled = self:transResName(getBackgroundImage("disabled"))
	images.on = {images.off, self:transResName(options.NodeNormalFileData)}
	images.on_pressed = images.on
	images.on_disabled = {images.off_disabled,
		self:transResName(options.NodeDisableFileData)}

	return images
end

function CCSUILoader:loadTexture(json)
	-- cc.FileUtils:getInstance():addSearchPath("res/")

	if not json.textures then
		return
	end

	for i,v in ipairs(json.textures) do
		self.bUseTexture = true
		-- if json.texturesPng then
		-- 	png = self:getTexturePng(json.texturesPng[i])
		-- end
		UILoaderUtilitys.loadTexture(v)
	end

end

function CCSUILoader:getTexturePng(plist)
	if not plist then
		return
	end

	local info = io.pathinfo(plist)

	local png
	if info.dirname then
		png = info.dirname .. info.basename .. ".png"
	else
		png = info.basename .. ".png"
	end

	return png
end

function CCSUILoader:transResName(fileData)
	if not fileData then
		return
	end

	local name = fileData.Path
	if not name then
		return name
	end

	UILoaderUtilitys.loadTexture(fileData.Plist)
	if "PlistSubImage" == fileData.Type then
		return "#" .. name
	else
		return UILoaderUtilitys.getFileFullName(name)
	end
end

function CCSUILoader:createNode(options)
	local node = cc.Node:create()
	if options.Size then
		node:setContentSize(cc.size(options.Size.X or 0, options.Size.Y or 0))
	end
	node:setPositionX(options.Position.X or 0)
	node:setPositionY(options.Position.Y or 0)
	node:setAnchorPoint(
		cc.p(options.AnchorPoint.ScaleX or 0, options.AnchorPoint.ScaleY or 0))

	return node
end

function CCSUILoader:createLayer(options)
    local node = cc.Layer:create()
    if options.Size then
        node:setContentSize(cc.size(options.Size.X or 0, options.Size.Y or 0))
    end
    node:setPositionX(options.Position.X or 0)
    node:setPositionY(options.Position.Y or 0)
    node:setAnchorPoint(
        cc.p(options.AnchorPoint.ScaleX or 0, options.AnchorPoint.ScaleY or 0))

    return node
end

function CCSUILoader:createSprite(options)
	local node = display.newSprite(self:transResName(options.FileData))
	if options.Size then
		node:setContentSize(cc.size(options.Size.X or 0, options.Size.Y or 0))
	end
	if options.Alpha then
		node:setOpacity(options.Alpha)
	end
	node:setPositionX(options.Position.X or 0)
	node:setPositionY(options.Position.Y or 0)
	node:setAnchorPoint(
		cc.p(options.AnchorPoint.ScaleX or 0, options.AnchorPoint.ScaleY or 0))

	return node
end

function CCSUILoader:createImage(options)
	local params = {}
	params.scale9 = options.Scale9Enable
	if params.scale9 then
		params.capInsets = cc.rect(options.Scale9OriginX, options.Scale9OriginY,
			options.Scale9Width, options.Scale9Height)
	end
	local node = cc.ui.UIImage.new(self:transResName(options.FileData), params)

	if not options.Scale9Enable then
		local originSize = node:getContentSize()
		if options.Size.X then
			options.Scale.ScaleX = (options.Scale.ScaleX or 1) * options.Size.X/originSize.width
		end
		if options.Size.Y then
			options.Scale.ScaleY = (options.Scale.ScaleY or 1) * options.Size.Y/originSize.height
		end
	end
	if options.Size then
		node:setLayoutSize(options.Size.X, options.Size.Y)
	end
	node:setPositionX(options.Position.X or 0)
	node:setPositionY(options.Position.Y or 0)
	node:setAnchorPoint(
		cc.p(options.AnchorPoint.ScaleX or 0, options.AnchorPoint.ScaleY or 0))

	if options.TouchEnable then
		node:setTouchEnabled(true)
		node:setTouchSwallowEnabled(true)
	end
	if options.Alpha then
		node:setOpacity(options.Alpha)
	end

	return node
end

function CCSUILoader:createButton(options)
	local node = cc.ui.UIPushButton.new(self:getButtonStateImages(options),
		{scale9 = options.Scale9Enable,
		flipX = options.FlipX,
		flipY = options.FlipY})

	if options.Alpha then
		node:setCascadeOpacityEnabled(true)
		node:setOpacity(options.Alpha)
	end
	if options.ButtonText then
		node:setButtonLabel(
			cc.ui.UILabel.new({text = options.ButtonText,
				size = options.FontSize,
				font = options.FontResource and options.FontResource.Path,
				color = cc.c3b(options.TextColor.R, options.TextColor.G, options.TextColor.B)}))
	end
	if options.Size then
		node:setButtonSize(options.Size.X, options.Size.Y)
	end
	node:align(self:getAnchorType(options.AnchorPoint.ScaleX or 0, options.AnchorPoint.ScaleY or 0),
		options.Position.X or 0, options.Position.Y or 0)

	return node
end

function CCSUILoader:createLoadingBar(options)
	local params = {}
	params.image = self:transResName(options.ImageFileData)
	params.scale9 = options.Scale9Enable
	params.capInsets = cc.rect(options.Scale9OriginX, options.Scale9OriginY,
			options.Scale9Width, options.Scale9Height)
	if "Right_To_Left" == options.ProgressType then
		params.direction = cc.ui.UILoadingBar.DIRECTION_RIGHT_TO_LEFT
	else
		params.direction = cc.ui.UILoadingBar.DIRECTION_LEFT_TO_RIGHT
	end
	params.percent = options.ProgressInfo or 100
	params.viewRect = cc.rect(options.Position.X, options.Position.Y, options.Size.X, options.Size.Y)

	local node = cc.ui.UILoadingBar.new(params)

	node:setDirction(params.direction)
	node:setPositionX(options.Position.X or 0)
	node:setPositionY(options.Position.Y or 0)
	node:setContentSize(options.Size.X, options.Size.Y)
	node:setAnchorPoint(
		cc.p(options.AnchorPoint.ScaleX or 0, options.AnchorPoint.ScaleY or 0))

	return node
end

function CCSUILoader:createSlider(options)
	local node = cc.ui.UISlider.new(display.LEFT_TO_RIGHT,
		{bar = self:transResName(options.BackGroundData),
		barfg = self:transResName(options.ProgressBarData),
		button = self:transResName(options.BallNormalData),
		button_pressed = self:transResName(options.BallPressedData),
		button_disabled = self:transResName(options.BallDisabledData),},
		{scale9 = options.Scale9Enable})

	if options.Size then
		node:setSliderSize(options.Size.X, options.Size.Y)
	end
	node:align(self:getAnchorType(options.AnchorPoint.ScaleX or 0, options.AnchorPoint.ScaleY or 0),
		options.Position.X or 0, options.Position.Y or 0)
    node:setSliderValue(options.PercentInfo)

	return node
end

function CCSUILoader:createCheckBox(options)
	local node = cc.ui.UICheckBoxButton.new(
		self:getCheckBoxImages(options))

	node:align(self:getAnchorType(options.AnchorPoint.ScaleX or 0, options.AnchorPoint.ScaleY or 0),
		options.Position.X or 0, options.Position.Y or 0)

	return node
end

function CCSUILoader:createBMFontLabel(options)
	local node = cc.ui.UILabel.new({
		UILabelType = 1,
		text = options.LabelText,
		font = options.LabelBMFontFile_CNB.Path,
		textAlign = cc.ui.TEXT_ALIGN_CENTER})
	node:setColor(cc.c4b(options.CColor.R or 255, options.CColor.G or 255, options.CColor.B or 255, options.Alpha or 255))
	node:align(self:getAnchorType(options.AnchorPoint.ScaleX or 0, options.AnchorPoint.ScaleY or 0),
		options.Position.X or 0, options.Position.Y or 0)

	return node
end

function CCSUILoader:createLabel(options)
	local hAlignment
	if "HT_Center" == options.HorizontalAlignmentType then
		hAlignment = cc.TEXT_ALIGNMENT_CENTER
	elseif "HT_Right" == options.HorizontalAlignmentType then
		hAlignment = cc.TEXT_ALIGNMENT_RIGHT
	else
		hAlignment = cc.TEXT_ALIGNMENT_LEFT
	end

	local vAlignment
	if "VT_Center" == options.VerticalAlignmentType then
		vAlignment = cc.VERTICAL_TEXT_ALIGNMENT_CENTER
	elseif "VT_Bottom" == options.VerticalAlignmentType then
		vAlignment = cc.VERTICAL_TEXT_ALIGNMENT_BOTTOM
	else
		vAlignment = cc.VERTICAL_TEXT_ALIGNMENT_TOP
	end

	local node = cc.ui.UILabel.new({text = options.LabelText,
		font = options.FontResource and options.FontResource.Path,
		size = options.FontSize,
		color = cc.c3b(options.CColor.R or 255, options.CColor.G or 255, options.CColor.B or 255),
		align = hAlignment,
		valign = vAlignment,
		dimensions = options.IsCustomSize and cc.size(options.Size.X or 0, options.Size.Y or 0),
		x = options.Position.X, y = options.Position.Y})

	node:align(self:getAnchorType(options.AnchorPoint.ScaleX or 0, options.AnchorPoint.ScaleY or 0),
		options.Position.X or 0, options.Position.Y or 0)

	return node
end

function CCSUILoader:createLabelAtlas(options)
	local labelAtlas
	if "function" == type(cc.LabelAtlas._create) then
		labelAtlas = cc.LabelAtlas:_create()
		labelAtlas:initWithString(options.stringValue,
			options.charMapFileData.path,
			options.itemWidth,
			options.itemHeight,
			string.byte(options.startCharMap))
	else
		labelAtlas = cc.LabelAtlas:create(
			options.stringValue,
			options.charMapFileData.path,
			options.itemWidth,
			options.itemHeight,
			string.byte(options.startCharMap))
	end

	labelAtlas:setAnchorPoint(
		cc.p(options.anchorPointX or 0.5, options.anchorPointY or 0.5))
	labelAtlas:setPosition(options.x, options.y)
	if not options.ignoreSize then
		labelAtlas:setContentSize(options.width, options.height)
	end
	return labelAtlas
end

function CCSUILoader:createEditBox(options)
	local editBox

	if self.bUseEditBox then
		editBox = cc.ui.UIInput.new({
			UIInputType = 1,
	        size = cc.size(options.width, options.height)
	    	})
	    editBox:setPlaceHolder(options.placeHolder)
	    editBox:setFontName(options.fontName)
	    editBox:setFontSize(options.fontSize or 20)
	    editBox:setText(options.text)
	    if options.passwordEnable then
	    	editBox:setInputFlag(cc.EDITBOX_INPUT_FLAG_PASSWORD)
		end
		if options.maxLengthEnable then
			editBox:setMaxLength(options.maxLength)
		end
		editBox:setPosition(options.x, options.y)
	else
		editBox = cc.ui.UIInput.new({
		UIInputType = 2,
        placeHolder = options.PlaceHolderText,
        x = options.Position.X,
        y = options.Position.Y,
        text = options.LabelText,
        size = cc.size(options.Size.X, options.Size.Y),
        passwordEnable = options.PasswordEnable,
        font = options.FontResource and options.FontResource.Path,
        fontSize = options.FontSize,
        maxLength = options.MaxLengthEnable and options.MaxLengthText
        })
	end

	editBox:setAnchorPoint(
		cc.p(options.AnchorPoint.ScaleX or 0, options.AnchorPoint.ScaleY or 0))

	return editBox
end

function CCSUILoader:createPanel(options)
	local node
	if options.ClipAble then
		node = cc.ClippingRegionNode:create()
	else
		node = display.newNode()
	end
	local clrLayer
	local bgLayer

	if 1 == options.ComboBoxIndex then
		-- single color
		clrLayer = cc.LayerColor:create()
		if not cc.bPlugin_ then
			clrLayer:resetCascadeBoundingBox()
		end
		clrLayer:setTouchEnabled(false)
		clrLayer:setColor(cc.c3b(options.SingleColor.R or 255,
			options.SingleColor.G or 255, options.SingleColor.B or 255))
	elseif 2 == options.ComboBoxIndex then
		-- gradient
		clrLayer = cc.LayerGradient:create()
		if not cc.bPlugin_ then
			clrLayer:resetCascadeBoundingBox()
		end
		clrLayer:setTouchEnabled(false)
		clrLayer:setStartColor(cc.c3b(options.FirstColor.R or 255,
			options.FirstColor.G or 255, options.FirstColor.B or 255))
		clrLayer:setEndColor(cc.c3b(options.EndColor.R or 255,
			options.EndColor.G or 255, options.EndColor.B or 255))
		clrLayer:setVector(cc.p(options.ColorVector.ScaleX or 0, options.ColorVector.ScaleY or 0))
	end

	if clrLayer then
		clrLayer:setAnchorPoint(cc.p(0, 0))
		clrLayer:setOpacity(options.BackColorAlpha or 255)
	end

	-- background layer
	if options.Scale9Enable then
		if options.FileData and options.FileData.Path then
			local capInsets = cc.rect(options.Scale9OriginX, options.Scale9OriginY,
						options.Scale9Width, options.Scale9Height)
			local scale9sp = ccui.Scale9Sprite or cc.Scale9Sprite
			if "Normal" == options.FileData.Type then
				bgLayer = scale9sp:create(
					capInsets, options.FileData.Path)
			else
				bgLayer = scale9sp:createWithSpriteFrameName(
					options.FileData.Path, capInsets)
			end
			bgLayer:setContentSize(cc.size(options.Size.X, options.Size.Y))
		end
	else
		if options.FileData and options.FileData.Path then
			bgLayer = display.newSprite(
				self:transResName(options.FileData))
		end
	end

	local conSize
	conSize = cc.size(options.Size.X, options.Size.Y)

	if options.ClipAble then
		node:setClippingRegion(cc.rect(0, 0, options.Size.X, options.Size.Y))
	end
	if clrLayer then
		clrLayer:setContentSize(conSize)
	end
	if bgLayer then
		bgLayer:setPosition(conSize.width/2, conSize.height/2)
	end
	node:setContentSize(conSize)
	if clrLayer then
		node:addChild(clrLayer)
	end
	if bgLayer then
		node:addChild(bgLayer)
	end
	node:setPositionX(options.Position.X or 0)
	node:setPositionY(options.Position.Y or 0)
	node:setAnchorPoint(
		cc.p(options.AnchorPoint.ScaleX or 0, options.AnchorPoint.ScaleY or 0))

	return node
end

function CCSUILoader:createScrollView(options)
	local params =
		{viewRect = cc.rect(options.Position.X, options.Position.Y, options.Size.X, options.Size.Y)}

	if 1 == options.ComboBoxIndex then
		-- single color
		params.bgColor = cc.c4b(options.SingleColor.R or 255, options.SingleColor.G or 255, options.SingleColor.B or 255, options.BackColorAlpha or 255)
	elseif 2 == options.ComboBoxIndex then
		-- gradient
		params.bgStartColor = cc.c4b(options.FirstColor.R or 255, options.FirstColor.G or 255, options.FirstColor.B or 255, options.BackColorAlpha or 255)
		params.bgEndColor = cc.c4b(options.EndColor.R or 255, options.EndColor.G or 255, options.EndColor.B or 255, options.BackColorAlpha or 255)
		params.bgVector = cc.p(options.ColorVector.ScaleX, options.ColorVector.ScaleY)
	end
	params.bg = self:transResName(options.FileData)
	if options.Scale9Enable then
		params.bgScale9 = options.Scale9Enable
		params.capInsets = cc.rect(options.Scale9OriginX, options.Scale9OriginY,
						options.Scale9Width, options.Scale9Height)
	end

	local node = cc.ui.UIScrollView.new(params)
	local dir = 0
	-- quick 0:both 1:vertical 2:horizontal
	if "Vertical" == options.ScrollDirectionType then
		dir = 1
	elseif "Horizontal" == options.ScrollDirectionType then
		dir = 2
	else
		dir = 0
	end
	node:setDirection(dir)
	node:setBounceable(options.IsBounceEnabled or false)

	return node
end

function CCSUILoader:createListView(options)
	local params =
		{viewRect = cc.rect(options.Position.X, options.Position.Y, options.Size.X, options.Size.Y)}

	if 1 == options.ComboBoxIndex then
		-- single color
		params.bgColor = cc.c4b(options.SingleColor.R or 255, options.SingleColor.G or 255, options.SingleColor.B or 255, options.BackColorAlpha or 255)
	elseif 2 == options.ComboBoxIndex then
		-- gradient
		params.bgStartColor = cc.c4b(options.FirstColor.R or 255, options.FirstColor.G or 255, options.FirstColor.B or 255, options.BackColorAlpha or 255)
		params.bgEndColor = cc.c4b(options.EndColor.R or 255, options.EndColor.G or 255, options.EndColor.B or 255, options.BackColorAlpha or 255)
		params.bgVector = cc.p(options.ColorVector.ScaleX, options.ColorVector.ScaleY)
	end

	local node = cc.ui.UIListView.new(params)
	local dir = 0
	-- quick 0:both 1:vertical 2:horizontal
	if "Vertical" == options.DirectionType then
		dir = 1
	elseif "Horizontal" == options.DirectionType then
		dir = 2
	else
		dir = 0
	end
	node:setDirection(dir)
	node:setBounceable(options.IsBounceEnabled or false)

	return node
end

function CCSUILoader:createPageView(options)
	local params = {}
	params.column = 1
	params.row = 1
	params.viewRect = cc.rect(options.Position.X, options.Position.Y, options.Size.X, options.Size.Y)

	local node = cc.ui.UIPageView.new(params)

	return node
end

function CCSUILoader:createProjectNode(options)
	local jsonFile = options.FileData.Path
	local node = CCSUILoader.new():loadFile(jsonFile)

	node:setPosition(cc.p(options.Position.X, options.Position.Y))

	return node
end

function CCSUILoader:prettyJson(json)
	local prettyNode
	prettyNode = function(node, parent)
		if not node then
			return
		end

		-- 调整百分比的子节点宽高,位置
		if node.PositionPercentXEnabled then
			node.Position = node.Position or {}
			node.Position.X = parent.Size.X * node.PrePosition.X
			node.Position.Y = parent.Size.Y * node.PrePosition.Y
		end
		if node.PercentWidthEnable then
			node.Size = node.Size or {}
			node.Size.X = parent.Size.X * node.PreSize.X
			node.Size.Y = parent.Size.Y * node.PreSize.Y
		end
		node.AnchorPoint = node.AnchorPoint or {} 
		if not node.children then
			return
		end
		if 0 == #node.children then
			return
		end

		for i,v in ipairs(node.children) do
			prettyNode(v, node)
		end
	end

	prettyNode(json)
end



return CCSUILoader

