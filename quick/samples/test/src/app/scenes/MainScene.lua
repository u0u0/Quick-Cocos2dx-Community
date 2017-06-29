local MainScene = class("MainScene", function()
    return display.newScene("MainScene")
end)

function MainScene:ctor()
	local tests = {
		"Test_NodeFrameEvent",
		"Test_NodeEvent",
		"Test_KeypadEvent",
		"Test_NodeTouchEvent",
		"Test_CocosStudio",
		"Test_Audio",
	}

	local scrollView = ccui.ScrollView:create()
	scrollView:addTo(self)
	scrollView:align(display.TOP_CENTER, display.cx, display.top)
	self.scrollView = scrollView

	local total = 0
	local btnSize = nil
	for i = #tests, 1, -1 do
		local btn = ccui.Button:create()
		btn:setTitleText(tests[i])
		btn:setTitleFontSize(24)
		btn:addTouchEventListener(function(sender, eventType)
			if 2 == eventType then
				scrollView:setVisible(false)
				app:createView(tests[i]):addTo(self)
			end
		end)
		if not btnSize then
			btnSize = btn:getContentSize()
		end
		btn:pos((display.width - btnSize.width) / 2 + btnSize.width / 2,
				btnSize.height * total + btnSize.height / 2)
		total = total + 1

		scrollView:addChild(btn)
	end

	local totalHeight = btnSize.height * total
	scrollView:setInnerContainerSize(cc.size(display.width, totalHeight))
	local scrollHeight = display.height
	if totalHeight < scrollHeight then
		scrollHeight = totalHeight
	end
	scrollView:setContentSize(cc.size(display.width, scrollHeight))
end

function MainScene:openScrollView()
	self.scrollView:setVisible(true)
end

function MainScene:onEnter()
	print("MainScene onEnter")
end

function MainScene:onExit()
	print("MainScene onExit")
end

return MainScene
