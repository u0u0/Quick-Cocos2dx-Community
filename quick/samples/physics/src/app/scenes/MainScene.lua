local MainScene = class("MainScene", function()
    return display.newPhysicsScene("MainScene")
end)

function MainScene:ctor()
    self:getPhysicsWorld():setGravity(cc.p(0, -100))

    local size = display.size
    local body = cc.PhysicsBody:createEdgeBox(size, cc.PHYSICSBODY_MATERIAL_DEFAULT, 3)

    local edgeNode = display.newNode()
    self:addChild(edgeNode) -- after 3.7.4, must addChild() before setPhysicsBody()
    edgeNode:setPosition(size.width / 2, size.height / 2)
    edgeNode:setPhysicsBody(body)

    -- collision detection
    local function onContactBegin(contact)
        local tag = contact:getShapeA():getBody():getNode():getTag()
        print(tag)
        return true
    end

    local contactListener = cc.EventListenerPhysicsContact:create()
    contactListener:registerScriptHandler(onContactBegin, cc.Handler.EVENT_PHYSICS_CONTACT_BEGIN)
    local eventDispatcher = cc.Director:getInstance():getEventDispatcher()
    eventDispatcher:addEventListenerWithFixedPriority(contactListener, 1)

    -- Open Physics Debug
    self:getPhysicsWorld():setDebugDrawMask(cc.PhysicsWorld.DEBUGDRAW_ALL)

    self:addNodeEventListener(cc.NODE_TOUCH_EVENT, function(event)
        if event.name == "began" then
            self:addSprite(event.x, event.y)
            return true
        end
    end)
    self:setTouchEnabled(true)
end

function MainScene:addSprite(x, y)
    -- add physics sprite to scene
    local oneSprite = display.newSprite("box.png")
    self:addChild(oneSprite)
    local oneBody = cc.PhysicsBody:createBox(oneSprite:getContentSize(), cc.PHYSICSBODY_MATERIAL_DEFAULT,cc.p(0,0))
    oneBody:setContactTestBitmask(0xFFFFFFFF)
    oneBody:applyImpulse(cc.p(0, 80000))
    oneSprite:setPhysicsBody(oneBody)
    oneSprite:setPosition(x, y)
    oneSprite:setTag(101)
end

function MainScene:onEnter()
end

function MainScene:onExit()
end

return MainScene
