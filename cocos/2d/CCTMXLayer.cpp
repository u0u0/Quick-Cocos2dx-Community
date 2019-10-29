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
#include "base/CCDirector.h"
#include "renderer/CCTextureCache.h"
#include "renderer/CCGLProgram.h"
#include "base/ccUTF8.h" // For StringUtils::format

NS_CC_BEGIN

TMXLayer * TMXLayer::create(TMXLayerInfo *layerInfo, TMXMapInfo *mapInfo)
{
	TMXLayer *ret = new (std::nothrow) TMXLayer();
	if (ret->initWithTilesetInfo(layerInfo, mapInfo))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool TMXLayer::initWithTilesetInfo(TMXLayerInfo *layerInfo, TMXMapInfo *mapInfo)
{
	Size size = layerInfo->_layerSize;
	// layerInfo
	_layerName = layerInfo->_name;
	_layerSize = size;
	_tiles = layerInfo->_tiles;
    // tell the layerinfo to release the ownership of the tiles map.
    layerInfo->_ownTiles = false;
	_opacity = layerInfo->_opacity;
	setProperties(layerInfo->getProperties());
	_contentScaleFactor = Director::getInstance()->getContentScaleFactor();

	// mapInfo
	_mapTileSize = mapInfo->getTileSize();
	_layerOrientation = mapInfo->getOrientation();
	_staggerAxis = mapInfo->getStaggerAxis();
	_staggerIndex = mapInfo->getStaggerIndex();
	_hexSideLength = mapInfo->getHexSideLength();
	_tilesets = mapInfo->getTilesets();

	// offset (after layer orientation is set);
	Vec2 offset = calculateLayerOffset(layerInfo->_offset);
	setPosition(CC_POINT_PIXELS_TO_POINTS(offset));

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

    // preload textures
	for (auto iter = _tilesets.crbegin(), iterCrend = _tilesets.crend(); iter != iterCrend; ++iter) {
		TMXTilesetInfo* tilesetInfo = *iter;
		if (tilesetInfo) {
			Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(tilesetInfo->_sourceImage);
            if (!texture) {
                return false;
            }
			texture->setAliasTexParameters();
			tilesetInfo->_imageSize = texture->getContentSizeInPixels();
		}
	}

    setupTiles();
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
{}

TMXLayer::~TMXLayer()
{
	CC_SAFE_FREE(_tiles);
}

// TMXLayer - setup Tiles
void TMXLayer::setupTiles()
{
	for (int y = 0; y < _layerSize.height; y++) {
		for (int x = 0; x < _layerSize.width; x++) {
            Vec2 pos(x, y);
			intptr_t z = getZForPos(pos);
			int gid = _tiles[z]; // Only support little endian stored gid
			if (gid != 0) {
				Sprite *tile = createTileSprite(z, gid);
				setupTileSprite(tile, pos, gid);
			}
		}
	}
}

TMXTilesetInfo *TMXLayer::getTilesetByGID(uint32_t gid) const
{
	for (auto iter = _tilesets.crbegin(), end = _tilesets.crend(); iter != end; ++iter) {
		TMXTilesetInfo *tileset = *iter;
		if (tileset->_firstGid < 0 || (gid & kTMXFlippedMask) >= static_cast<uint32_t>(tileset->_firstGid)) {
			return tileset;
		}
	}
	return nullptr;
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
	TMXTilesetInfo *tileset = getTilesetByGID(gid);
	Texture2D *texture = Director::getInstance()->getTextureCache()->getTextureForKey(tileset->_sourceImage);
	Rect rect = tileset->getRectForGID(gid);
	Sprite *tile = Sprite::createWithTexture(texture, rect);
	_tileSprites[z] = tile;
    addChild(tile);
	return tile;
}

void TMXLayer::setupTileSprite(Sprite* sprite, const Vec2& pos, uint32_t gid)
{
    Vec2 spPos = getPositionAt(pos);
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

// TMXLayer - obtaining tiles/gids
Sprite * TMXLayer::getTileAt(const Vec2& pos)
{
	CCASSERT(pos.x < _layerSize.width && pos.y < _layerSize.height && pos.x >=0 && pos.y >=0, "TMXLayer: invalid position");
	CCASSERT(_tiles, "TMXLayer: the tiles map has been released");

	return _tileSprites[getZForPos(pos)];
}

uint32_t TMXLayer::getTileGIDAt(const Vec2& pos, TMXTileFlags* flags/* = nullptr*/)
{
	CCASSERT(pos.x < _layerSize.width && pos.y < _layerSize.height && pos.x >=0 && pos.y >=0, "TMXLayer: invalid position");
	CCASSERT(_tiles, "TMXLayer: the tiles map has been released");

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
		Sprite *tile = _tileSprites[z];
		if (tile) {
			TMXTilesetInfo *tileset = getTilesetByGID(gidAndFlags);
            Texture2D *texture = Director::getInstance()->getTextureCache()->getTextureForKey(tileset->_sourceImage);
            Rect rect = tileset->getRectForGID(gidAndFlags);
            tile->setTexture(texture);
            tile->setTextureRect(rect, false, rect.size);
        } else {
            tile = createTileSprite(z, gidAndFlags);
        }
		setupTileSprite(tile, pos, gid);
        _tiles[z] = gidAndFlags;
	}
}

void TMXLayer::removeTileAt(const Vec2& pos)
{
	CCASSERT(pos.x < _layerSize.width && pos.y < _layerSize.height && pos.x >=0 && pos.y >=0, "TMXLayer: invalid position");
	CCASSERT(_tiles, "TMXLayer: the tiles map has been released");

	intptr_t z = getZForPos(pos);
	Sprite *tile = _tileSprites[z];
	if (tile) {
		tile->removeFromParent();
	}
	_tileSprites[z] = nullptr;
	_tiles[z] = 0;
}

//CCTMXLayer - obtaining positions, offset
Vec2 TMXLayer::calculateLayerOffset(const Vec2& pos)
{
	Vec2 ret;
	switch (_layerOrientation) {
		case TMXOrientationOrtho:
			ret.set(pos.x * _mapTileSize.width, -pos.y *_mapTileSize.height);
			break;
		case TMXOrientationIso:
			ret.set((_mapTileSize.width / 2) * (pos.x - pos.y),
					(_mapTileSize.height / 2) * (-pos.x - pos.y));
			break;
		case TMXOrientationHex:
			{
				if(_staggerAxis == TMXStaggerAxis_Y) {
					int diffX = (_staggerIndex == TMXStaggerIndex_Even) ? _mapTileSize.width / 2 : 0;
					ret.set(pos.x * _mapTileSize.width + diffX, -pos.y * (_mapTileSize.height - (_mapTileSize.width - _hexSideLength) / 2));
				} else if(_staggerAxis == TMXStaggerAxis_X) {
					int diffY = (_staggerIndex == TMXStaggerIndex_Odd) ? _mapTileSize.height / 2 : 0;
					ret.set(pos.x * (_mapTileSize.width - (_mapTileSize.width - _hexSideLength) / 2), -pos.y * _mapTileSize.height + diffY);
				}
				break;
			}
		case TMXOrientationStaggered:
			{
				float diffX = 0;
				if ((int)std::abs(pos.y) % 2 == 1) {
					diffX = _mapTileSize.width / 2;
				}
				ret.set(pos.x * _mapTileSize.width + diffX,
						(-pos.y) * _mapTileSize.height / 2);
			}
			break;
	}
	return ret;
}

Vec2 TMXLayer::getPositionAt(const Vec2& pos)
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
	uint32_t gid = getTileGIDAt(pos);
	TMXTilesetInfo *tileset = getTilesetByGID(gid);
	Vec2 offset = tileset->_tileOffset;

	int odd_even = (_staggerIndex == TMXStaggerIndex_Odd) ? 1 : -1;
	switch (_staggerAxis) {
		case TMXStaggerAxis_Y:
			{
				float diffX = 0;
				if ((int)pos.y % 2 == 1) {
					diffX = _mapTileSize.width / 2 * odd_even;
				}
				xy = Vec2(pos.x * _mapTileSize.width + diffX + offset.x,
						(_layerSize.height - pos.y - 1) * (_mapTileSize.height - (_mapTileSize.height-_hexSideLength) / 2) - offset.y);
				break;
			}

		case TMXStaggerAxis_X:
			{
				float diffY = 0;
				if ((int)pos.x % 2 == 1) {
					diffY = _mapTileSize.height / 2 * -odd_even;
				}
				xy = Vec2(pos.x * (_mapTileSize.width - (_mapTileSize.width - _hexSideLength) / 2) + offset.x,
						(_layerSize.height - pos.y - 1) * _mapTileSize.height + diffY - offset.y);
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
