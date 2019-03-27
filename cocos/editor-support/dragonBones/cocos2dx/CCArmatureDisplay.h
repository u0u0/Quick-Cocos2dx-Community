/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2012-2018 DragonBones team and other contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef DRAGONBONES_CC_ARMATURE_DISPLAY_CONTAINER_H
#define DRAGONBONES_CC_ARMATURE_DISPLAY_CONTAINER_H

#include "dragonBones/DragonBonesHeaders.h"
#include "cocos2d.h"
#include "base/ccTypes.h"

DRAGONBONES_NAMESPACE_BEGIN
/**
 * @inheritDoc
 */
class CCArmatureDisplay : public cocos2d::Node, public virtual IArmatureProxy
{
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(CCArmatureDisplay)

public:
    /**
     * @internal
     */
    static CCArmatureDisplay* create();
public:

protected:
    Armature* _armature;
    cocos2d::EventDispatcher* _dispatcher;

public:
    CCArmatureDisplay() :
        _armature(nullptr),
        _dispatcher(nullptr)
    {
        _dispatcher = new cocos2d::EventDispatcher();
        setEventDispatcher(_dispatcher);
        _dispatcher->setEnabled(true);
    }
    ~CCArmatureDisplay() {}

public:
    /**
     * @inheritDoc
     */
    virtual void dbInit(Armature* armature) override;
    /**
     * @inheritDoc
     */
    virtual void dbClear() override;
    /**
     * @inheritDoc
     */
    virtual void dbUpdate() override;
    /**
     * @inheritDoc
     */
    virtual void dispose(bool disposeProxy = true) override;
    /**
     * @inheritDoc
     */
    inline virtual bool hasDBEventListener(const std::string& type) const override
    {
        return _dispatcher->isEnabled();
    }
    /**
     * @inheritDoc
     */
    virtual void dispatchDBEvent(const std::string& type, EventObject* value) override;
    /**
     * @inheritDoc
     */
    virtual void addDBEventListener(const std::string& type, const std::function<void(EventObject*)>& listener) override;
    /**
     * @inheritDoc
     */
    virtual void removeDBEventListener(const std::string& type, const std::function<void(EventObject*)>& listener) override;
    /**
     * @inheritDoc
     */
    inline virtual Armature* getArmature() const override
    {
        return _armature;
    }
    /**
     * @inheritDoc
     */
    inline virtual Animation* getAnimation() const override
    {
        return _armature->getAnimation();
    }
    /**
    * @inheritDoc
    */
    virtual cocos2d::Rect getBoundingBox() const override;
};
/**
 * @internal
 */
class DBCCSprite : public cocos2d::Sprite
{
    DRAGONBONES_DISALLOW_COPY_AND_ASSIGN(DBCCSprite)

public:
    static DBCCSprite* create();

protected:
    DBCCSprite();
    ~DBCCSprite();

    cocos2d::TrianglesCommand::Triangles *_triangles;
    cocos2d::TrianglesCommand _command;
public:
    virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
    /**
     * Modify for Quick-Cocos2dx-Community
     */
    void setTriangles(cocos2d::TrianglesCommand::Triangles *triangles);
    cocos2d::TrianglesCommand::Triangles* getTriangles() { return _triangles; };
};

DRAGONBONES_NAMESPACE_END
#endif // DRAGONBONES_CC_ARMATURE_DISPLAY_CONTAINER_H
