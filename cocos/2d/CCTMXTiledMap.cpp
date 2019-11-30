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
#include "base/CCDirector.h"
#include "renderer/CCTextureCache.h"

NS_CC_BEGIN

// implementation TMXTiledMap

TMXTiledMap * TMXTiledMap::create(const std::string& tmxFile, bool setupTiles)
{
    TMXTiledMap *ret = new (std::nothrow) TMXTiledMap();
    ret->_setupTiles = setupTiles;
    if (ret->initWithTMXFile(tmxFile)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

TMXTiledMap* TMXTiledMap::createWithXML(const std::string& tmxString, const std::string& resourcePath, bool setupTiles)
{
    TMXTiledMap *ret = new (std::nothrow) TMXTiledMap();
    ret->_setupTiles = setupTiles;
    if (ret->initWithXML(tmxString, resourcePath)) {
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
    if (! mapInfo) {
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
    if (! mapInfo) {
        return false;
    }
    CCASSERT( !mapInfo->getTilesets().empty(), "TMXTiledMap: Map not found. Please check the filename.");
    buildWithMapInfo(mapInfo);
    return true;
}

TMXTiledMap::TMXTiledMap()
:_mapSize(Size::ZERO)
,_tileSize(Size::ZERO)
,_tmxFile("")
,_setupTiles(true)
{
}

TMXTiledMap::~TMXTiledMap()
{
    // unload texture
    TextureCache *textureCache = Director::getInstance()->getTextureCache();
    for (auto iter = _tilesets.crbegin(), iterCrend = _tilesets.crend(); iter != iterCrend; ++iter) {
        TMXTilesetInfo* tilesetInfo = *iter;
        if (tilesetInfo) {
            Texture2D *texture = textureCache->addImage(tilesetInfo->_sourceImage);
            if (texture) {
                texture->release();
            }
        }
    }
}

// private
Node *TMXTiledMap::createChild(Ref *childInfo)
{
    TMXLayerInfo *layerInfo = dynamic_cast<TMXLayerInfo *>(childInfo);
    if (layerInfo) {
        TMXLayer *layer = TMXLayer::create(layerInfo, this);
        if (layer == nullptr) return nullptr;
        if (_setupTiles) {
            layer->setupTiles();
        }
        layer->setVisible(layerInfo->_visible);
        // update content size with the max size
        const Size& childSize = layer->getContentSize();
        Size currentSize = this->getContentSize();
        currentSize.width = std::max(currentSize.width, childSize.width);
        currentSize.height = std::max(currentSize.height, childSize.height);
        this->setContentSize(currentSize);
        return layer;
    }
    
    TMXImageLayerInfo *imageLayerInfo = dynamic_cast<TMXImageLayerInfo *>(childInfo);
    if (imageLayerInfo) {
        TMXLayer *layer = TMXLayer::create(imageLayerInfo, this);
        if (layer == nullptr) return nullptr;
        layer->setVisible(imageLayerInfo->_visible);
        return layer;
    }
    
    TMXGroupInfo *groupInfo = dynamic_cast<TMXGroupInfo *>(childInfo);
    if (groupInfo) {
        TMXLayer *layer = TMXLayer::create(groupInfo, this);
        if (layer == nullptr) return nullptr;
        layer->setVisible(groupInfo->_visible);
        for (const auto &child : groupInfo->_children) {
            Node *n = createChild(child);
            if (n) {
                layer->addChild(n);
            }
        }
        return layer;
    }
    
    TMXObjectGroup *objectGroupInfo = dynamic_cast<TMXObjectGroup *>(childInfo);
        if (objectGroupInfo) {
        TMXLayer *layer = TMXLayer::create(objectGroupInfo, this);
        if (layer == nullptr) return nullptr;
        layer->setVisible(objectGroupInfo->isVisible());
        return layer;
    }
    return nullptr;
}

void TMXTiledMap::buildWithMapInfo(TMXMapInfo* mapInfo)
{
    _mapSize = mapInfo->getMapSize();
    _tileSize = mapInfo->getTileSize();
    _mapOrientation = mapInfo->getOrientation();
    _staggerAxis = mapInfo->getStaggerAxis();
    _staggerIndex = mapInfo->getStaggerIndex();
    _hexSideLength = mapInfo->getHexSideLength();
    _objectGroups = mapInfo->getObjectGroups();
    _properties = mapInfo->getProperties();
    _tileProperties = mapInfo->getTileProperties();
    _tilesets = mapInfo->getTilesets();
    
    // preload textures
    TextureCache *textureCache = Director::getInstance()->getTextureCache();
    for (auto iter = _tilesets.crbegin(), iterCrend = _tilesets.crend(); iter != iterCrend; ++iter) {
        TMXTilesetInfo* tilesetInfo = *iter;
        if (tilesetInfo) {
            Texture2D *texture = textureCache->addImage(tilesetInfo->_sourceImage);
            CC_ASSERT(texture != nullptr);
            // By default all the tiles are aliased, avoid black line.
            texture->setAliasTexParameters();
            tilesetInfo->_imageSize = texture->getContentSizeInPixels();
            texture->retain();
        }
    }
    
    // load children
    auto& children = mapInfo->getChildren();
    for (const auto &child : children) {
        Node *n = createChild(child);
        if (n) {
            addChild(n);
        }
    }
}

TMXLayer *TMXTiledMap::findLayer(const Node *parent, const std::string& layerName) const
{
    for (auto& child : parent->getChildren()) {
        TMXLayer* layer = dynamic_cast<TMXLayer*>(child);
        if (layer) {
            if (layerName.compare(layer->getLayerName()) == 0) {
                return layer;
            }
            if (layer->getLayerType() == TMX_LAYER_GROUP) {
                TMXLayer* find = findLayer(layer, layerName);
                if (find) {
                    return find;
                }
            }
        }
    }
    return nullptr;
}

// public
TMXLayer *TMXTiledMap::getLayer(const std::string& layerName) const
{
    CCASSERT(layerName.size() > 0, "Invalid layer name!");
    return findLayer(this, layerName);
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

TMXTilesetInfo *TMXTiledMap::getTilesetByGID(uint32_t gid) const
{
    for (auto iter = _tilesets.crbegin(), end = _tilesets.crend(); iter != end; ++iter) {
        TMXTilesetInfo *tileset = *iter;
        if (tileset->_firstGid < 0 || (gid & kTMXFlippedMask) >= static_cast<uint32_t>(tileset->_firstGid)) {
            return tileset;
        }
    }
    return nullptr;
}

std::string TMXTiledMap::getDescription() const
{
    return StringUtils::format("<TMXTiledMap | Tag = %d, Layers = %d", _tag, static_cast<int>(_children.size()));
}

NS_CC_END
