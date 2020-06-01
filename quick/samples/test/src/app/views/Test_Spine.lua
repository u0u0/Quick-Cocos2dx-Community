local BaseLayer = require("app.scenes.BaseLayer")

local TestCase = class("Test_Spine", BaseLayer)

function TestCase:ctor()
	self.super.ctor(self)

	-- tips
	local spineSP = sp.SkeletonAnimation:createWithJsonFile(
		"spine/spineboy-pro.json", "spine/spineboy.atlas"
	)
	spineSP:setScale(0.5)
	spineSP:pos(display.cx, 100):addTo(self)
	spineSP:setAnimation(0, "run", true)
	-- aim mode
	spineSP:setAnimation(1, "aim", false)
end

return TestCase
