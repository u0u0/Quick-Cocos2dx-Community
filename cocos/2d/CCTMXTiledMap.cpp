/****************************************************************************
Copyright (c) 2009-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2016 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "2d/CCTMXTiledMap.h"
#include "2d/CCTMXLayer.h"
#include "2d/CCSprite.h"
#include "base/ccUTF8.h" // For StringUtils::format

NS_CC_BEGIN

// implementation TMXTiledMap

TMXTiledMap * TMXTiledMap::create(const std::string& tmxFile)
{
    TMXTiledMap *ret = new (std::nothrow) TMXTiledMap();
    if (ret->initWithTMXFile(tmxFile))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

TMXTiledMap* TMXTiledMap::createWithXML(const std::string& tmxString, const std::string& resourcePath)
{
    TMXTiledMap *ret = new (std::nothrow) TMXTiledMap();
    if (ret->initWithXML(tmxString, resourcePath))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool TMXTiledMap::initWithTMXFile(const std::string& tmxFile)
{
    CCASSERT(tmxFile.size()>0, "TMXTiledMap: tmx file should not be empty");

    _tmxFile = tmxFile;

    setContentSize(Size::ZERO);

    TMXMapInfo *mapInfo = TMXMapInfo::create(tmxFile);

    if (! mapInfo)
    {
        return false;
    }
    CCASSERT( !mapInfo->getTilesets().empty(), "TMXTiledMap: Map not found. Please check the filename.");
    buildWithMapInfo(mapInfo);

    return true;
}

bool TMXTiledMap::initWithXML(const std::string& tmxString, const std::string& resourcePath)
{
    _tmxFile = tmxString;

    setContentSize(Size::ZERO);

    TMXMapInfo *mapInfo = TMXMapInfo::createWithXML(tmxString, resourcePath);

    CCASSERT( !mapInfo->getTilesets().empty(), "TMXTiledMap: Map not found. Please check the filename.");
    buildWithMapInfo(mapInfo);

    return true;
}

TMXTiledMap::TMXTiledMap()
    :_mapSize(Size::ZERO)
    ,_tileSize(Size::ZERO)        
    ,_tmxFile("")
    , _tmxLayerNum(0)
{
}

TMXTiledMap::~TMXTiledMap()
{
}

// private
void TMXTiledMap::buildWithMapInfo(TMXMapInfo* mapInfo)
{
    _mapSize = mapInfo->getMapSize();
    _tileSize = mapInfo->getTileSize();
    _mapOrientation = mapInfo->getOrientation();
    _objectGroups = mapInfo->getObjectGroups();
    _properties = mapInfo->getProperties();
    _tileProperties = mapInfo->getTileProperties();
    
    // load layer
    int idx = 0;
    auto& layers = mapInfo->getLayers();
    for (const auto &layerInfo : layers) {
        if (layerInfo->_visible) {
            TMXLayer *child = TMXLayer::create(layerInfo, mapInfo);
            if (child == nullptr) {
                idx++;
                continue;
            }
            addChild(child, idx, idx);
            // update content size with the max size
            const Size& childSize = child->getContentSize();
            Size currentSize = this->getContentSize();
            currentSize.width = std::max(currentSize.width, childSize.width);
            currentSize.height = std::max(currentSize.height, childSize.height);
            this->setContentSize(currentSize);
            idx++;
        }
    }
    _tmxLayerNum = idx;
}

// public
TMXLayer * TMXTiledMap::getLayer(const std::string& layerName) const
{
    CCASSERT(layerName.size() > 0, "Invalid layer name!");
    
    for (auto& child : _children) {
        TMXLayer* layer = dynamic_cast<TMXLayer*>(child);
        if(layer) {
            if(layerName.compare(layer->getLayerName()) == 0) {
                return layer;
            }
        }
    }

    // layer not found
    return nullptr;
}

TMXObjectGroup * TMXTiledMap::getObjectGroup(const std::string& groupName) const
{
    CCASSERT(groupName.size() > 0, "Invalid group name!");

    for (const auto objectGroup : _objectGroups) {
        if (objectGroup && objectGroup->getGroupName() == groupName) {
            return objectGroup;
        }
    }

    // objectGroup not found
    return nullptr;
}

Value TMXTiledMap::getProperty(const std::string& propertyName) const
{
    if (_properties.find(propertyName) != _properties.end())
        return _properties.at(propertyName);
    
    return Value();
}

Value TMXTiledMap::getPropertiesForGID(int GID) const
{
    if (_tileProperties.find(GID) != _tileProperties.end())
        return _tileProperties.at(GID);
    
    return Value();
}

std::string TMXTiledMap::getDescription() const
{
    return StringUtils::format("<TMXTiledMap | Tag = %d, Layers = %d", _tag, static_cast<int>(_children.size()));
}

int TMXTiledMap::getLayerNum()
{
    return _tmxLayerNum;
}

NS_CC_END
