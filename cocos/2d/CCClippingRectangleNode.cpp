#include "CCClippingRectangleNode.h"
#include "base/CCDirector.h"
#include "renderer/CCRenderer.h"
#include "math/Vec2.h"

NS_CC_BEGIN

ClippingRectangleNode* ClippingRectangleNode::create()
{
    ClippingRectangleNode* node = new (std::nothrow) ClippingRectangleNode();
    if (node && node->init()) {
        // ClippingRectangleNode mostly use for UI, enable below two attributes.
        node->setCascadeColorEnabled(true);
        node->setCascadeOpacityEnabled(true);
        node->autorelease();
    } else {
        CC_SAFE_DELETE(node);
    }
    
    return node;
}

void ClippingRectangleNode::setClippingRegion(const Rect &clippingRegion)
{
    _clippingRegion = clippingRegion;
}

void ClippingRectangleNode::onBeforeVisitScissor()
{
    if (_clippingEnabled) {
        // record old ClipRegion
        GLView* glView = Director::getInstance()->getOpenGLView();
        if (glView->isScissorEnabled()) {
            GLfloat params[4];
            glGetFloatv(GL_SCISSOR_BOX, params);
            _preClipRegion = Rect(params[0], params[1], params[2], params[3]);
        } else {
            _preClipRegion = Rect::ZERO;
        }
        glEnable(GL_SCISSOR_TEST);
        
        float scaleX = _scaleX;
        float scaleY = _scaleY;
        Node *parent = this->getParent();
        while (parent) {
            scaleX *= parent->getScaleX();
            scaleY *= parent->getScaleY();
            parent = parent->getParent();
        }
        
        // push this ClipRegion
        const Point pos = convertToWorldSpace(Point(_clippingRegion.origin.x, _clippingRegion.origin.y));
        glView->setScissorInPoints(pos.x,
                                   pos.y,
                                   _clippingRegion.size.width * scaleX,
                                   _clippingRegion.size.height * scaleY);
    }
}

void ClippingRectangleNode::onAfterVisitScissor()
{
    if (_clippingEnabled)
    {
        // rollback to old ClipRegion
        if (!_preClipRegion.equals(Rect::ZERO)) {
            glScissor(_preClipRegion.origin.x,
                      _preClipRegion.origin.y,
                      _preClipRegion.size.width,
                      _preClipRegion.size.height);
        } else {
            glDisable(GL_SCISSOR_TEST);
        }
    }
}

void ClippingRectangleNode::visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags)
{
    if (!_visible)
    {
        return;
    }
    
    if (isVisitableByVisitingCamera())
    {
        _beforeVisitCmdScissor.init(_globalZOrder);
        _beforeVisitCmdScissor.func = CC_CALLBACK_0(ClippingRectangleNode::onBeforeVisitScissor, this);
        renderer->addCommand(&_beforeVisitCmdScissor);
        
        Node::visit(renderer, parentTransform, parentFlags);
        
        _afterVisitCmdScissor.init(_globalZOrder);
        _afterVisitCmdScissor.func = CC_CALLBACK_0(ClippingRectangleNode::onAfterVisitScissor, this);
        renderer->addCommand(&_afterVisitCmdScissor);
    }
}

NS_CC_END
