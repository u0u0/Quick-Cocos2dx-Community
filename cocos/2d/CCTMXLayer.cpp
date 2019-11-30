/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
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

#include "2d/CCTMXLayer.h"
#include "2d/CCTMXTiledMap.h"
#include "2d/CCTMXXMLParser.h"
#include "2d/CCSprite.h"
#include "2d/CCLabel.h"
#include "base/CCDirector.h"
#include "renderer/CCTextureCache.h"
#include "renderer/CCGLProgram.h"
#include "base/ccUTF8.h" // For StringUtils::format

NS_CC_BEGIN

TileAnimationData::TileAnimationData()
:leftTime(0.0)
,frameIndex(0)
{}

TileAnimationData::~TileAnimationData()
{}

TMXLayer * TMXLayer::create(TMXLayerInfo *layerInfo, TMXTiledMap *tileMap)
{
	TMXLayer *ret = new (std::nothrow) TMXLayer();
	if (ret->initWithLayerInfo(layerInfo, tileMap))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

TMXLayer * TMXLayer::create(TMXImageLayerInfo *layerInfo, TMXTiledMap *tileMap)
{
    TMXLayer *ret = new (std::nothrow) TMXLayer();
    if (ret->initWithLayerInfo(layerInfo, tileMap))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

TMXLayer * TMXLayer::create(TMXGroupInfo *layerInfo, TMXTiledMap *tileMap)
{
    TMXLayer *ret = new (std::nothrow) TMXLayer();
    if (ret->initWithLayerInfo(layerInfo, tileMap))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

TMXLayer * TMXLayer::create(TMXObjectGroup *layerInfo, TMXTiledMap *tileMap)
{
    TMXLayer *ret = new (std::nothrow) TMXLayer();
    if (ret->initWithLayerInfo(layerInfo, tileMap))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool TMXLayer::initWithLayerInfo(TMXLayerInfo *layerInfo, TMXTiledMap *tileMap)
{
    _layerType = TMX_LAYER_TILE;
	_layerName = layerInfo->_name;
	_layerSize = layerInfo->_layerSize;
	_tiles = layerInfo->_tiles;
    // tell the layerinfo to release the ownership of the tiles map.
    layerInfo->_ownTiles = false;
	_opacity = layerInfo->_opacity;
	setProperties(layerInfo->getProperties());
    return initCommon(layerInfo->_offset, tileMap);
}

bool TMXLayer::initWithLayerInfo(TMXImageLayerInfo *layerInfo, TMXTiledMap *tileMap)
{
    _layerType = TMX_LAYER_IMAGE;
    _layerName = layerInfo->_name;
    _layerSize = tileMap->getMapSize();
    _tiles = nullptr;
    _opacity = layerInfo->_opacity;
    setProperties(layerInfo->getProperties());

    initCommon(layerInfo->_offset, tileMap);
    // after setContentSize create sprite of image layer
    Sprite * sp = Sprite::create(layerInfo->_sourceImage);
    sp->setOpacity(_opacity);
    sp->setAnchorPoint(Vec2(0, 1));
    sp->setPositionY(getContentSize().height);
    addChild(sp);
    return true;
}

bool TMXLayer::initWithLayerInfo(TMXGroupInfo *layerInfo, TMXTiledMap *tileMap)
{
    _layerType = TMX_LAYER_GROUP;
    _layerName = layerInfo->_name;
    _layerSize = tileMap->getMapSize();
    _tiles = nullptr;
    _opacity = layerInfo->_opacity;
    setProperties(layerInfo->getProperties());
    return initCommon(layerInfo->_offset, tileMap);
}

bool TMXLayer::initWithLayerInfo(TMXObjectGroup *layerInfo, TMXTiledMap *tileMap)
{
    _layerType = TMX_LAYER_OBJECT_GROUP;
    _layerName = layerInfo->getGroupName();
    _layerSize = tileMap->getMapSize();
    _tiles = nullptr;
    _opacity = layerInfo->getOpacity();
    setProperties(layerInfo->getProperties());
    
    initCommon((Vec2 &)layerInfo->getPositionOffset(), tileMap);
    
    // init gid objects
    Vec2 pos(0, 0);// fake a tile pos, make tile api happy
    for (auto& obj : layerInfo->getObjects())
    {
        pos.x += 1;
        ValueMap& dict = obj.asValueMap();
        // convert object pos to cocos2d-x pos, then write back
        Vec2 objPos(dict["x"].asFloat(), dict["y"].asFloat());
        Vec2 xPos = getPositionForObject(objPos);
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
        // draw image object
        if ("tile" == objectType) {
            int gid = dict["gid"].asUnsignedInt();
            intptr_t z = getZForPos(pos);
            Sprite *tile = createTileSprite(z, gid);
            setupTileAnimation(tile, pos, gid);
            // set data for sort
            Vec2 *data = new Vec2(xPos);
            tile->setUserData((void *)data);
            // fix size
            Size textureSize = tile->getContentSize();
            float scaleX = objSize.width / textureSize.width;
            float scaleY = objSize.height / textureSize.height;
            tile->setScaleX(scaleX);
            tile->setScaleY(scaleY);
            // fix rotation
            if (dict.find("rotation") != dict.end()) {
                tile->setRotation(dict["rotation"].asFloat());
            }
            // fix AnchorPoint, set position
            if (TMXOrientationIso == _layerOrientation) {
                tile->setAnchorPoint(Vec2(0.5, 0));
            }
            tile->setPosition(xPos);
            tile->setVisible(dict["visible"].asBool());
        } else if ("text" == objectType) {
            std::string text = dict["text"].asString();
            
            std::string font = "sans-serif";
            if (dict.find("fontfamily") != dict.end()) {
                font = dict["fontfamily"].asString();
            }
            
            float fontSize = 16.0f;
            if (dict.find("pixelsize") != dict.end()) {
                fontSize = dict["pixelsize"].asFloat();
            }
            fontSize /= CC_CONTENT_SCALE_FACTOR();
            
            TextHAlignment hAlignment = TextHAlignment::LEFT;
            if (dict.find("halign") != dict.end()) {
                std::string v = dict["halign"].asString();
                if (v == "center") {
                    hAlignment = TextHAlignment::CENTER;
                } else if (v == "right") {
                    hAlignment = TextHAlignment::RIGHT;
                }
            }
            
            TextVAlignment vAlignment = TextVAlignment::TOP;
            if (dict.find("valign") != dict.end()) {
                std::string v = dict["valign"].asString();
                if (v == "center") {
                    vAlignment = TextVAlignment::CENTER;
                } else if (v == "bottom") {
                    vAlignment = TextVAlignment::BOTTOM;
                }
            }
            
            int r = 0, g = 0, b = 0;
            if (dict.find("color") != dict.end()) {
                std::string v = dict["color"].asString();
                if (v.length() == 7) { // #ffffff
                    sscanf(v.c_str(), "#%02x%02x%02x", &r, &g, &b);
                }
            }
            Color3B color(r, g, b);
            
            float rotation = 0;
            if (dict.find("rotation") != dict.end()) {
                rotation = dict["rotation"].asFloat();
            }
            
            Label *label = Label::createWithSystemFont(text, font, fontSize, objSize, hAlignment, vAlignment);
            addChild(label);
            label->setPosition(xPos);
            label->setAnchorPoint(Vec2(0, 1));
            label->setColor(color);
            label->setRotation(rotation);
            label->setVisible(dict["visible"].asBool());
            // set data for sort
            Vec2 *data = new Vec2(xPos);
            label->setUserData((void *)data);
        } else if ("rectangle" == objectType) {
            if (TMXOrientationIso == _layerOrientation) {
                // It's a prism in cocos2d-x, need convert to polygon.
                Vec2 zero = getPositionForObject(Vec2(0, 0));
                Vec2 p2 = getPositionForObject(Vec2(objSize.width, 0));
                Vec2 p3 = getPositionForObject(Vec2(objSize.width, objSize.height));
                Vec2 p4 = getPositionForObject(Vec2(0, objSize.height));

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
                dict["y"] = xPos.y - objSize.height; // fix y for other _layerOrientation
            }
        } else if ("polygon" == objectType) {
            // fix point position
            ValueVector &pointsArray = dict["points"].asValueVector();
            Vec2 zero = getPositionForObject(Vec2(0, 0));
            for (auto& p : pointsArray) {
                ValueMap &a = p.asValueMap();
                Vec2 newP = getPositionForObject(Vec2(a["x"].asFloat(), a["y"].asFloat()));
                a["x"] = newP.x - zero.x;
                a["y"] = newP.y - zero.y;
            }
        } else if ("polyline" == objectType) {
            // fix point position
            ValueVector &pointsArray = dict["polylinePoints"].asValueVector();
            Vec2 zero = getPositionForObject(Vec2(0, 0));
            for (auto& p : pointsArray) {
                ValueMap &a = p.asValueMap();
                Vec2 newP = getPositionForObject(Vec2(a["x"].asFloat(), a["y"].asFloat()));
                a["x"] = newP.x - zero.x;
                a["y"] = newP.y - zero.y;
            }
        }
    }
    
    // sort children
    _reorderChildDirty = false;
    std::stable_sort(std::begin(_children), std::end(_children), [](Node *a, Node *b) {
        Vec2 *aP = (Vec2 *)a->getUserData();
        Vec2 *bP = (Vec2 *)b->getUserData();
        if (aP->y == bP->y) {
            return aP->x < bP->x;
        }
        return aP->y > bP->y;
    });
    // release tmp data
    for(const auto &sp : _children) {
        Vec2 *p = (Vec2 *)sp->getUserData();
        delete p;
        sp->setUserData(nullptr);
    }
    return true;
}

bool TMXLayer::initCommon(Vec2 &layerOffset, TMXTiledMap *tileMap)
{
    _contentScaleFactor = Director::getInstance()->getContentScaleFactor();
    // mapInfo
    _tileMap = tileMap;
    _mapTileSize = tileMap->getTileSize();
    _layerOrientation = tileMap->getMapOrientation();
    _staggerAxis = tileMap->getStaggerAxis();
    _staggerIndex = tileMap->getStaggerIndex();
    _hexSideLength = tileMap->getHexSideLength();

    // offset (after layer orientation is set);
    Vec2 offset = calculateLayerOffset(layerOffset);
    setPosition(offset);

    float width = 0;
    float height = 0;
    if (_layerOrientation == TMXOrientationHex) {
        if (_staggerAxis == TMXStaggerAxis_X) {
            height = _mapTileSize.height * (_layerSize.height + 0.5);
            width = (_mapTileSize.width + _hexSideLength) * ((int)(_layerSize.width / 2)) + _mapTileSize.width * ((int)_layerSize.width % 2);
        } else {
            width = _mapTileSize.width * (_layerSize.width + 0.5);
            height = (_mapTileSize.height + _hexSideLength) * ((int)(_layerSize.height / 2)) + _mapTileSize.height * ((int)_layerSize.height % 2);
        }
    } else {
        width = _layerSize.width * _mapTileSize.width;
        height = _layerSize.height * _mapTileSize.height;
    }
    setContentSize(CC_SIZE_PIXELS_TO_POINTS(Size(width, height)));

    return true;
}

TMXLayer::TMXLayer()
:_layerName("")
,_opacity(0)
,_contentScaleFactor(1.0f)
,_layerSize(Size::ZERO)
,_mapTileSize(Size::ZERO)
,_tiles(nullptr)
,_layerOrientation(TMXOrientationOrtho)
,_staggerAxis(TMXStaggerAxis_Y)
,_staggerIndex(TMXStaggerIndex_Even)
,_hexSideLength(0)
,_tileMap(nullptr)
,_layerType(TMX_LAYER_UNDEFINED)
{}

TMXLayer::~TMXLayer()
{
	CC_SAFE_FREE(_tiles);
    for(auto it = _tilesAniData.begin(); it != _tilesAniData.end(); ++it) {
        delete it->second;
    }
}

// TMXLayer - setup Tiles
void TMXLayer::setupTiles()
{
    CCASSERT(_layerType == TMX_LAYER_TILE, "TMXLayer: invalid layer type");
    
	for (int y = 0; y < _layerSize.height; y++) {
		for (int x = 0; x < _layerSize.width; x++) {
            Vec2 pos(x, y);
			intptr_t z = getZForPos(pos);
			int gid = _tiles[z]; // Only support little endian stored gid
			if (gid != 0) {
				Sprite *tile = createTileSprite(z, gid);
                setupTileAnimation(tile, pos, gid);
			}
		}
	}
}

// TMXLayer - Properties
Value TMXLayer::getProperty(const std::string& propertyName) const
{
	if (_properties.find(propertyName) != _properties.end())
		return _properties.at(propertyName);

	return Value();
}

Sprite *TMXLayer::createTileSprite(intptr_t z, uint32_t gid)
{
	TMXTilesetInfo *tileset = _tileMap->getTilesetByGID(gid);
	Texture2D *texture = Director::getInstance()->getTextureCache()->getTextureForKey(tileset->_sourceImage);
	Rect rect = tileset->getRectForGID(gid);
    rect = CC_RECT_PIXELS_TO_POINTS(rect);
    // only apply fixArtifacts on TMXOrientationOrtho
	Sprite *tile = Sprite::createWithTexture(texture, rect, false, _layerOrientation == TMXOrientationOrtho);
    tile->setCameraMask(getCameraMask());
    _tileSprites.insert(std::pair<intptr_t, Sprite *>(z, tile));
    addChild(tile);
	return tile;
}

void TMXLayer::setTileTexture(Sprite* sprite, uint32_t gid)
{
    TMXTilesetInfo *tileset = _tileMap->getTilesetByGID(gid);
    Texture2D *texture = Director::getInstance()->getTextureCache()->getTextureForKey(tileset->_sourceImage);
    Rect rect = tileset->getRectForGID(gid);
    sprite->setTexture(texture);
    sprite->setTextureRect(rect, false, rect.size);
}

void TMXLayer::setupTileSprite(Sprite* sprite, const Vec2& pos, uint32_t gid)
{
    Vec2 spPos = getPositionAt(pos, gid);
	sprite->setPosition(spPos);
    sprite->setLocalZOrder(getLocalZForPos(pos));
	sprite->setOpacity(_opacity);

	//issue 1264, flip can be undone as well
	sprite->setFlippedX(false);
	sprite->setFlippedY(false);
	sprite->setRotation(0.0f);
	sprite->setAnchorPoint(Vec2::ZERO);

	// Rotation in tiled is achieved using 3 flipped states, flipping across the horizontal, vertical, and diagonal axes of the tiles.
	if (gid & kTMXTileDiagonalFlag) {
		// put the anchor in the middle for ease of rotation.
		sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
		sprite->setPosition(spPos.x + sprite->getContentSize().height / 2,
				spPos.y + sprite->getContentSize().width / 2);

		auto flag = gid & (kTMXTileHorizontalFlag | kTMXTileVerticalFlag);

		// handle the 4 diagonally flipped states.
		if (flag == kTMXTileHorizontalFlag) {
			sprite->setRotation(90.0f);
		} else if (flag == kTMXTileVerticalFlag) {
			sprite->setRotation(270.0f);
		} else if (flag == (kTMXTileVerticalFlag | kTMXTileHorizontalFlag)) {
			sprite->setRotation(90.0f);
			sprite->setFlippedX(true);
		} else {
			sprite->setRotation(270.0f);
			sprite->setFlippedX(true);
		}
	} else {
		if (gid & kTMXTileHorizontalFlag) {
			sprite->setFlippedX(true);
		}
		if (gid & kTMXTileVerticalFlag) {
			sprite->setFlippedY(true);
		}
	}
}

// init tile's animation
void TMXLayer::setupTileAnimation(Sprite* sprite, const Vec2& pos, uint32_t gid)
{
    intptr_t z = getZForPos(pos);
    auto it = _tilesAniData.find(z);
    if (it != _tilesAniData.end()) { // remove old data
        delete it->second;
        _tilesAniData.erase(z);
    }
    Value prop = _tileMap->getPropertiesForGID(gid);
    if (prop.getType() == Value::Type::MAP) {
        Value& ani = prop.asValueMap()["animation"];
        if (ani.getType() == Value::Type::VECTOR) { // add new data
            TileAnimationData *data = new TileAnimationData();
            data->frameIndex = 0;
            data->pos = pos;
            data->tile = sprite;
            ValueVector &vector = ani.asValueVector();
            for (size_t i = 0; i < vector.size(); i = i + 2) {
                uint32_t tileid = vector[i].asUnsignedInt();
                float duration = vector[i + 1].asUnsignedInt() / 1000.0f;//ms => s
                data->gids.push_back(tileid);
                data->durations.push_back(duration);
                // init texture to first frame
                if (0 == i) {
                    data->leftTime = duration;
                    setTileTexture(sprite, tileid);
                    gid = tileid; // real render gid
                }
            }
            _tilesAniData.insert(std::pair<intptr_t, TileAnimationData *>(z, data));
        }
    }
    setupTileSprite(sprite, pos, gid);// do for real gid
    
    // start or stop schedule
    if (_tilesAniData.size() == 1) {
        schedule(CC_SCHEDULE_SELECTOR(TMXLayer::tilesUpdate), 0);
    } else if (_tilesAniData.size() == 0) {
        unschedule(CC_SCHEDULE_SELECTOR(TMXLayer::tilesUpdate));
    }
}

void TMXLayer::tilesUpdate(float dt)
{
    for(auto it = _tilesAniData.begin(); it != _tilesAniData.end(); ++it) {
        TileAnimationData *data = it->second;
        data->leftTime -= dt;
        if (data->leftTime > 0) {
            continue;
        }
        // display next frame
        data->frameIndex++;
        if (data->frameIndex == data->gids.size()) {
            data->frameIndex = 0;
        }
        uint32_t gid = data->gids[data->frameIndex];
        data->leftTime = data->durations[data->frameIndex];
        setTileTexture(data->tile, gid);
        setupTileSprite(data->tile, data->pos, gid);
    }
}

// TMXLayer - obtaining tiles/gids
Sprite * TMXLayer::getTileAt(const Vec2& pos)
{
	CCASSERT(pos.x < _layerSize.width && pos.y < _layerSize.height && pos.x >=0 && pos.y >=0, "TMXLayer: invalid position");
	CCASSERT(_tiles, "TMXLayer: the tiles map has been released");
    CCASSERT(_layerType == TMX_LAYER_TILE, "TMXLayer: invalid layer type");

    intptr_t z = getZForPos(pos);
    auto it = _tileSprites.find(z);
    if (it == _tileSprites.end()) {
        Sprite *tile = nullptr;
        int gid = _tiles[z];
        if (gid != 0) { // try create it
            tile = createTileSprite(z, gid);
            setupTileAnimation(tile, pos, gid);
        }
        return tile;
    }
    return it->second;
}

uint32_t TMXLayer::getTileGIDAt(const Vec2& pos, TMXTileFlags* flags/* = nullptr*/)
{
	CCASSERT(pos.x < _layerSize.width && pos.y < _layerSize.height && pos.x >=0 && pos.y >=0, "TMXLayer: invalid position");
	CCASSERT(_tiles, "TMXLayer: the tiles map has been released");
    CCASSERT(_layerType == TMX_LAYER_TILE, "TMXLayer: invalid layer type");

	// Bits on the far end of the 32-bit global tile ID are used for tile flags
	uint32_t tile = _tiles[getZForPos(pos)];
	// issue1264, flipped tiles can be changed dynamically
	if (flags) {
		*flags = (TMXTileFlags)(tile & kTMXFlipedAll);
	}

	return (tile & kTMXFlippedMask);
}

intptr_t TMXLayer::getZForPos(const Vec2& pos) const
{
	intptr_t z = -1;
    int newX = pos.x;
    // fix correct render ordering in Hexagonal maps when stagger axis == x
    if (_staggerAxis == TMXStaggerAxis_X && _layerOrientation == TMXOrientationHex) {
        if (_staggerIndex == TMXStaggerIndex_Odd) {
            if (pos.x >= _layerSize.width / 2)
                newX = (pos.x - std::ceil(_layerSize.width / 2)) * 2 + 1;
            else
                newX = pos.x * 2;
        } else {
            // TMXStaggerIndex_Even
            if (pos.x >= static_cast<int>(_layerSize.width / 2))
                newX = (pos.x - static_cast<int>(_layerSize.width / 2)) * 2;
            else
                newX = pos.x * 2 + 1;
        }
    }
    z = (newX + pos.y * _layerSize.width);

	CCASSERT(z != -1, "Invalid Z");
	return z;
}

// TMXLayer - adding / remove tiles
void TMXLayer::setTileGID(uint32_t gid, const Vec2& pos, TMXTileFlags flags)
{
	CCASSERT(pos.x < _layerSize.width && pos.y < _layerSize.height && pos.x >=0 && pos.y >=0, "TMXLayer: invalid position");
	CCASSERT(_tiles, "TMXLayer: the tiles map has been released");
    CCASSERT(_layerType == TMX_LAYER_TILE, "TMXLayer: invalid layer type");

	if (gid <= 0) {
		removeTileAt(pos);
		return;
	}

	TMXTileFlags currentFlags;
	uint32_t currentGID = getTileGIDAt(pos, &currentFlags);
	if (currentGID != gid || currentFlags != flags)
	{
		uint32_t gidAndFlags = gid | flags;
		intptr_t z = getZForPos(pos);
        Sprite *tile = nullptr;
        auto it = _tileSprites.find(z);
        if (it != _tileSprites.end()) {
            tile = it->second;
        }
		if (tile) {
            setTileTexture(tile, gidAndFlags);
        } else {
            tile = createTileSprite(z, gidAndFlags);
        }
        setupTileAnimation(tile, pos, gidAndFlags);
        _tiles[z] = gidAndFlags;
	}
}

void TMXLayer::removeTileAt(const Vec2& pos, bool cleanGID)
{
	CCASSERT(pos.x < _layerSize.width && pos.y < _layerSize.height && pos.x >=0 && pos.y >=0, "TMXLayer: invalid position");
	CCASSERT(_tiles, "TMXLayer: the tiles map has been released");
    CCASSERT(_layerType == TMX_LAYER_TILE, "TMXLayer: invalid layer type");

	intptr_t z = getZForPos(pos);
    auto it = _tileSprites.find(z);
    if (it != _tileSprites.end()) {
        it->second->removeFromParent();
        _tileSprites.erase(z);
    }
    auto itdata = _tilesAniData.find(z);
    if (itdata != _tilesAniData.end()) {
        delete itdata->second;
        _tilesAniData.erase(z);
        if (_tilesAniData.size() == 0) {
            unschedule(CC_SCHEDULE_SELECTOR(TMXLayer::tilesUpdate));
        }
    }
    if (cleanGID) {
        _tiles[z] = 0;
    }
}

//CCTMXLayer - obtaining positions, offset
Vec2 TMXLayer::calculateLayerOffset(const Vec2& pos)
{
	Vec2 ret;
	switch (_layerOrientation) {
		case TMXOrientationOrtho:
			ret.set(pos.x, -pos.y);
			break;
		case TMXOrientationIso:
            ret.set(pos.x, -pos.y);
			break;
		case TMXOrientationHex:
			{
				if(_staggerAxis == TMXStaggerAxis_Y) {
					int diffX = (_staggerIndex == TMXStaggerIndex_Even) ? _mapTileSize.width / 2 : 0;
					ret.set(pos.x + diffX, -pos.y);
				} else if(_staggerAxis == TMXStaggerAxis_X) {
					int diffY = (_staggerIndex == TMXStaggerIndex_Odd) ? _mapTileSize.height / 2 : 0;
					ret.set(pos.x, -pos.y + diffY);
				}
				break;
			}
		case TMXOrientationStaggered:
			ret.set(pos.x, -pos.y);
			break;
	}
    ret = CC_POINT_PIXELS_TO_POINTS(ret);
	return ret;
}

Vec2 TMXLayer::getPositionAt(const Vec2& pos, uint32_t gid)
{
    int newX = pos.x;
    // fix correct render ordering in Hexagonal maps when stagger axis == x
    if (_staggerAxis == TMXStaggerAxis_X && _layerOrientation == TMXOrientationHex) {
        if (_staggerIndex == TMXStaggerIndex_Odd) {
            if (pos.x >= _layerSize.width / 2)
                newX = (pos.x - std::ceil(_layerSize.width / 2)) * 2 + 1;
            else
                newX = pos.x * 2;
        } else {
            // TMXStaggerIndex_Even
            if (pos.x >= static_cast<int>(_layerSize.width / 2))
                newX = (pos.x - static_cast<int>(_layerSize.width / 2)) * 2;
            else
                newX = pos.x * 2 + 1;
        }
    }
    Vec2 newPos(newX, pos.y);
    
	Vec2 ret;
	switch (_layerOrientation) {
		case TMXOrientationOrtho:
			ret = getPositionForOrthoAt(newPos);
			break;
		case TMXOrientationIso:
			ret = getPositionForIsoAt(newPos);
			break;
		case TMXOrientationHex:
			ret = getPositionForHexAt(newPos);
			break;
		case TMXOrientationStaggered:
			ret = getPositionForStaggeredAt(newPos);
			break;
	}
    
    if (gid == 0) { // not a default value
        gid = getTileGIDAt(pos);
    }
    if (gid > 0) { // apply tileset->_tileOffset
        TMXTilesetInfo *tileset = _tileMap->getTilesetByGID(gid);
        Vec2 offset = tileset->_tileOffset;
        ret.x += offset.x;
        ret.y -= offset.y;
    }
    
	ret = CC_POINT_PIXELS_TO_POINTS(ret);
	return ret;
}

Vec2 TMXLayer::getPositionForOrthoAt(const Vec2& pos)
{
	return Vec2(pos.x * _mapTileSize.width,
			(_layerSize.height - pos.y - 1) * _mapTileSize.height);
}

Vec2 TMXLayer::getPositionForIsoAt(const Vec2& pos)
{
	return Vec2(_mapTileSize.width / 2 * (_layerSize.width + pos.x - pos.y - 1),
			_mapTileSize.height / 2 * ((_layerSize.height * 2 - pos.x - pos.y) - 2));
}

Vec2 TMXLayer::getPositionForHexAt(const Vec2& pos)
{
	Vec2 xy;

	int odd_even = (_staggerIndex == TMXStaggerIndex_Odd) ? 1 : -1;
	switch (_staggerAxis) {
		case TMXStaggerAxis_Y:
			{
				float diffX = 0;
				if ((int)pos.y % 2 == 1) {
					diffX = _mapTileSize.width / 2 * odd_even;
				}
				xy = Vec2(pos.x * _mapTileSize.width + diffX,
						(_layerSize.height - pos.y - 1) * (_mapTileSize.height - (_mapTileSize.height-_hexSideLength) / 2));
				break;
			}

		case TMXStaggerAxis_X:
			{
				float diffY = 0;
				if ((int)pos.x % 2 == 1) {
					diffY = _mapTileSize.height / 2 * -odd_even;
				}
				xy = Vec2(pos.x * (_mapTileSize.width - (_mapTileSize.width - _hexSideLength) / 2),
						(_layerSize.height - pos.y - 1) * _mapTileSize.height + diffY);
				break;
			}
	}
	return xy;
}

Vec2 TMXLayer::getPositionForStaggeredAt(const Vec2 &pos)
{
	float diffX = 0;
	if ((int)pos.y % 2 == 1) {
		diffX = _mapTileSize.width / 2;
	}
	return Vec2(pos.x * _mapTileSize.width + diffX,
			(_layerSize.height - pos.y - 1) * _mapTileSize.height / 2);
}

Vec2 TMXLayer::getPositionForObject(const Vec2& pos)
{
    Vec2 ret(0, 0);
    switch (_layerOrientation) {
        case TMXOrientationOrtho:
            ret.x = pos.x;
            ret.y = _layerSize.height * _mapTileSize.height - pos.y;
            break;
        case TMXOrientationIso:
        {
            float min = std::min(_mapTileSize.width, _mapTileSize.height);
            ret.x = (_layerSize.width * 2 + (pos.x - pos.y) / min) * _mapTileSize.width / 2;
            ret.y = (ret.x / _mapTileSize.width - pos.x / min) * _mapTileSize.height;
            ret.x -= _layerSize.width * _mapTileSize.width / 2;
            break;
        }
        case TMXOrientationHex:
            {
                float offsetx = 0;
                float height = 0;
                if (_staggerAxis == TMXStaggerAxis_X) {
                    height = _mapTileSize.height * _layerSize.height;
                    if (_staggerIndex == TMXStaggerIndex_Even) {
                        height +=  _mapTileSize.height / 2;
                    }
                 } else {
                     if (_staggerIndex == TMXStaggerIndex_Even) {
                         offsetx = -_mapTileSize.width / 2;
                     }
                     height = (_mapTileSize.height + _hexSideLength) * ((int)(_layerSize.height / 2)) + _mapTileSize.height * ((int)_layerSize.height % 2);
                     height += _hexSideLength / 2; // need special fix ??
                 }
                ret.x = pos.x + offsetx;
                ret.y = height - pos.y;
                break;
            }
        case TMXOrientationStaggered:
            ret.x = pos.x;
            ret.y = (_layerSize.height + 1) * _mapTileSize.height / 2.0f - pos.y;
            break;
    }
    ret = CC_POINT_PIXELS_TO_POINTS(ret);
    return ret;
}

int TMXLayer::getLocalZForPos(const Vec2& pos) const
{
    int ret = 0;
    switch (_layerOrientation) {
        case TMXOrientationIso:
            ret = static_cast<int>(pos.x + _layerSize.width * pos.y);
            break;
        case TMXOrientationOrtho:
            ret = static_cast<int>(pos.x + _layerSize.width * pos.y);
            break;
        case TMXOrientationStaggered:
            ret = static_cast<int>(pos.x + _layerSize.width * pos.y);
            break;
        case TMXOrientationHex:
        {
            int newX = pos.x;
            if (_staggerAxis == TMXStaggerAxis_X) {
                int factor = _staggerIndex == TMXStaggerIndex_Even ? 1 : 0;
                if (newX % 2 == factor) { // heigher(in pixel) tile of this row
                    newX = newX - 1;
                } else {
                    newX = newX + 1;
                }
            }
            ret = static_cast<int>(newX + _layerSize.width * pos.y) + 1;// above zero
            break;
        }
        default:
            CCASSERT(0, "TMX invalid value");
            break;
    }
    
//    printf("x:%d, y:%d, z:%d\n", (int)pos.x, (int)pos.y, ret);
    return ret;
}

std::string TMXLayer::getDescription() const
{
	return StringUtils::format("<TMXLayer | tag = %d, size = %d,%d>", _tag, (int)_mapTileSize.width, (int)_mapTileSize.height);
}

NS_CC_END
