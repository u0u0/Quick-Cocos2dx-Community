/****************************************************************************
Copyright (c) 2009-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2016 Chukong Technologies Inc.
Copyright (c) 2020 cocos2d-lua.org

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
            for(auto it = tilesetInfo->_images.begin(); it != tilesetInfo->_images.end(); ++it) {
                Texture2D *texture = textureCache->addImage(it->second->sourceImage);
                CC_SAFE_RELEASE(texture);
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
            for(auto it = tilesetInfo->_images.begin(); it != tilesetInfo->_images.end(); ++it) {
                Texture2D *texture = textureCache->addImage(it->second->sourceImage);
                CC_ASSERT(texture != nullptr);
                // By default all the tiles are aliased, avoid black line.
                texture->setAliasTexParameters();
                it->second->imageSize = texture->getContentSizeInPixels();
                texture->retain();
            }
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
    
    // convert tile's properties: objectgroup's pos and size to cocos pos and size
    for (auto iter = _tileProperties.begin(); iter != _tileProperties.end(); ++iter)
    {
        uint32_t gid = iter->first;
        TMXTilesetInfo *tileset = getTilesetByGID(gid);
        Size& gridSize = tileset->_gridSize;
        std::string& gridOrientation = tileset->_gridOrientation;
        
        ValueMap& property = (iter->second).asValueMap();
        if (!property["objectgroup"].isNull()) {
            ValueVector& objects = property["objectgroup"].asValueVector();
            for (auto &object : objects) {
                ValueMap& dict = object.asValueMap();
                Vec2 objPos(dict["x"].asFloat(), dict["y"].asFloat());
                Vec2 xPos = getPositionForTileObject(gridOrientation, gridSize, objPos);
                dict["x"] = xPos.x;
                dict["y"] = xPos.y;
                // convert object size to cocos2d-x size, then write back
                Size objSize(0, 0);
                if (dict.find("width") != dict.end()) {
                    objSize.width = dict["width"].asFloat();
                    objSize.height = dict["height"].asFloat();
                    objSize = CC_SIZE_PIXELS_TO_POINTS(objSize);
                    dict["width"] = objSize.width;
                    dict["height"] = objSize.height;
                }
                
                std::string objectType = dict["objectType"].asString();
                if ("rectangle" == objectType) {
                    if (gridOrientation == "isometric") {
                        // It's a prism in cocos2d-x, need convert to polygon.
                        Vec2 zero = getPositionForTileObject(gridOrientation, gridSize, Vec2(0, 0));
                        Vec2 p2 = getPositionForTileObject(gridOrientation, gridSize, Vec2(objSize.width, 0));
                        Vec2 p3 = getPositionForTileObject(gridOrientation, gridSize, Vec2(objSize.width, objSize.height));
                        Vec2 p4 = getPositionForTileObject(gridOrientation, gridSize, Vec2(0, objSize.height));

                        ValueVector pointsArray;
                        pointsArray.reserve(4);
                        ValueMap pd1;
                        pd1["x"] = 0.0f;
                        pd1["y"] = 0.0f;
                        pointsArray.push_back(Value(pd1));
                        ValueMap pd2;
                        pd2["x"] = p2.x - zero.x;
                        pd2["y"] = p2.y - zero.y;
                        pointsArray.push_back(Value(pd2));
                        ValueMap pd3;
                        pd3["x"] = p3.x - zero.x;
                        pd3["y"] = p3.y - zero.y;
                        pointsArray.push_back(Value(pd3));
                        ValueMap pd4;
                        pd4["x"] = p4.x - zero.x;
                        pd4["y"] = p4.y - zero.y;
                        pointsArray.push_back(Value(pd4));
                
                        dict["points"] = Value(pointsArray);
                        dict["objectType"] = "polygon";
                    } else {
                        dict["y"] = xPos.y - objSize.height; // fix y for other _mapOrientation
                    }
                } else if ("polygon" == objectType) {
                    // fix point position
                    ValueVector &pointsArray = dict["points"].asValueVector();
                    Vec2 zero = getPositionForTileObject(gridOrientation, gridSize, Vec2(0, 0));
                    for (auto& p : pointsArray) {
                        ValueMap &a = p.asValueMap();
                        Vec2 newP = getPositionForTileObject(gridOrientation, gridSize, Vec2(a["x"].asFloat(), a["y"].asFloat()));
                        a["x"] = newP.x - zero.x;
                        a["y"] = newP.y - zero.y;
                    }
                } else if ("polyline" == objectType) {
                    // fix point position
                    ValueVector &pointsArray = dict["polylinePoints"].asValueVector();
                    Vec2 zero = getPositionForTileObject(gridOrientation, gridSize, Vec2(0, 0));
                    for (auto& p : pointsArray) {
                        ValueMap &a = p.asValueMap();
                        Vec2 newP = getPositionForTileObject(gridOrientation, gridSize, Vec2(a["x"].asFloat(), a["y"].asFloat()));
                        a["x"] = newP.x - zero.x;
                        a["y"] = newP.y - zero.y;
                    }
                }
            }
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

Vec2 TMXTiledMap::getPositionForTileObject(std::string& gridOrientation, Size& gridSize, const Vec2& pos)
{
    Vec2 ret(0, 0);
    if (gridOrientation == "isometric") { // 45 degree
        float min = std::min(gridSize.width, gridSize.height);
        ret.x = (2 + (pos.x - pos.y) / min) * gridSize.width / 2;
        ret.y = (ret.x / gridSize.width - pos.x / min) * gridSize.height;
        ret.x -= gridSize.width / 2;
    } else { // normal
        ret.x = pos.x;
        ret.y = gridSize.height - pos.y;
    }
    ret = CC_POINT_PIXELS_TO_POINTS(ret);
    return ret;
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
