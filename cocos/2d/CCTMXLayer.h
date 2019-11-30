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
#ifndef __CCTMX_LAYER_H__
#define __CCTMX_LAYER_H__

#include "2d/CCSpriteBatchNode.h"
#include "2d/CCTMXXMLParser.h"
#include "base/ccCArray.h"
NS_CC_BEGIN

class TMXTiledMap;

/**
 * @addtogroup _2d
 * @{
 */

enum
{
    TMX_LAYER_UNDEFINED,
    TMX_LAYER_TILE,
    TMX_LAYER_OBJECT_GROUP,
    TMX_LAYER_IMAGE,
    TMX_LAYER_GROUP
};

class TileAnimationData {
public:
    TileAnimationData();
    ~TileAnimationData();
    
    float leftTime;
    int frameIndex;
    Vec2 pos;
    Sprite *tile;
    std::vector<uint32_t> gids;
    std::vector<float> durations;
};

class CC_DLL TMXLayer : public Node
{
public:
    /** Creates a TMXLayer with an tileset info, a layer info and a map info.
     *
     * @param layerInfo A layer info.
     * @param tileMap.
     * @return An autorelease object.
     */
    static TMXLayer * create(TMXLayerInfo *layerInfo, TMXTiledMap *tileMap);
    static TMXLayer * create(TMXImageLayerInfo *layerInfo, TMXTiledMap *tileMap);
    static TMXLayer * create(TMXGroupInfo *layerInfo, TMXTiledMap *tileMap);
    static TMXLayer * create(TMXObjectGroup *layerInfo, TMXTiledMap *tileMap);
    /**
     * @js ctor
     */
    TMXLayer();
    /**
     * @js NA
     * @lua NA
     */
    virtual ~TMXLayer();

    /** Initializes a TMXLayer with a tileset info, a layer info and a map info.
     *
     * @param layerInfo A layer info.
     * @param tileMap.
     * @return If initializes successfully, it will return true.
     */
    bool initWithLayerInfo(TMXLayerInfo *layerInfo, TMXTiledMap *tileMap);
    bool initWithLayerInfo(TMXImageLayerInfo *layerInfo, TMXTiledMap *tileMap);
    bool initWithLayerInfo(TMXGroupInfo *layerInfo, TMXTiledMap *tileMap);
    bool initWithLayerInfo(TMXObjectGroup *layerInfo, TMXTiledMap *tileMap);

    /** Returns the tile (Sprite) at a given a tile coordinate.
     * The returned Sprite will be already added to the TMXLayer. Don't add it again.
     * The Sprite can be treated like any other Sprite: rotated, scaled, translated, opacity, color, etc.
     * You can remove either by calling:
     * - layer->removeChild(sprite, cleanup);
     * - or layer->removeTileAt(Vec2(x,y));
     *
     * @param tileCoordinate A tile coordinate.
     * @return Returns the tile (Sprite) at a given a tile coordinate.
     */
    Sprite* getTileAt(const Vec2& tileCoordinate);
    
    /** Returns the tile gid at a given tile coordinate. It also returns the tile flags.
     * This method requires the tile map has not been previously released (eg. don't call [layer releaseMap]).
     * 
     * @param tileCoordinate The tile coordinate.
     * @param flags Tile flags.
     * @return Returns the tile gid at a given tile coordinate. It also returns the tile flags.
     */
	uint32_t getTileGIDAt(const Vec2& tileCoordinate, TMXTileFlags* flags = nullptr);

    /** Sets the tile gid (gid = tile global id) at a given tile coordinate.
     * The Tile GID can be obtained by using the method "tileGIDAt" or by using the TMX editor -> Tileset Mgr +1.
     * If a tile is already placed at that position, then it will be removed.
     * Use withFlags if the tile flags need to be changed as well.
     * 
     * @param gid The tile gid.
     * @param tileCoordinate The tile coordinate.
     * @param flags The tile flags.
     */
    void setTileGID(uint32_t gid, const Vec2& tileCoordinate, TMXTileFlags flags = (TMXTileFlags)0);

    /** Removes a tile at given tile coordinate. 
     *
     * @param tileCoordinate The tile coordinate.
     */
    void removeTileAt(const Vec2& tileCoordinate, bool cleanGID = true);

    /** Returns the position in points of a given tile coordinate.
     *
     * @param tileCoordinate The tile coordinate.
     * @param gid if > 0, use to get tileset's offset.
     * @return The position in points of a given tile coordinate.
     */
	Vec2 getPositionAt(const Vec2& tileCoordinate, uint32_t gid = 0);

    /** Return the value for the specific property name.
     *
     * @param propertyName The specific property name.
     * @return Return the value for the specific property name.
     */
	Value getProperty(const std::string& propertyName) const;
    
    /** Creates the tiles. */
    void setupTiles();
    
    /** Get the layer name. 
     *
     * @return The layer name.
     */
    const std::string& getLayerName() { return _layerName; }
    
    /** Set the layer name.
     *
     * @param layerName The layer name.
     */
    void setLayerName(const std::string& layerName) { _layerName = layerName; }

    /** Size of the layer in tiles.
     *
     * @return Size of the layer in tiles.
     */
    const Size& getLayerSize() const { return _layerSize; }
    
    /** Set size of the layer in tiles.
     *
     * @param size Size of the layer in tiles.
     */
    void setLayerSize(const Size& size) { _layerSize = size; }
    
    /** Size of the map's tile (could be different from the tile's size).
     *
     * @return The size of the map's tile.
     */
    const Size& getMapTileSize() const { return _mapTileSize; }
    
    /** Set the size of the map's tile.
     *
     * @param size The size of the map's tile.
     */
    void setMapTileSize(const Size& size) { _mapTileSize = size; }
    
    /** Pointer to the map of tiles.
     * @js NA
     * @lua NA
     * @return Pointer to the map of tiles.
     */
    uint32_t* getTiles() const { return _tiles; };
    
    /** Set a pointer to the map of tiles.
     *
     * @param tiles A pointer to the map of tiles.
     */
    void setTiles(uint32_t* tiles) { _tiles = tiles; };
    
    /** Layer orientation, which is the same as the map orientation.
     *
     * @return Layer orientation, which is the same as the map orientation.
     */
    int getLayerOrientation() const { return _layerOrientation; }
    
    /** Set layer orientation, which is the same as the map orientation.
     *
     * @param orientation Layer orientation,which is the same as the map orientation.
     */
    void setLayerOrientation(int orientation) { _layerOrientation = orientation; }
    
    /** Properties from the layer. They can be added using Tiled.
     *
     * @return Properties from the layer. They can be added using Tiled.
     */
    ValueMap& getProperties() { return _properties; }
    
    /** Set an Properties from to layer.
     *
     * @param properties It is used to set the layer Properties.
     */
    void setProperties(const ValueMap& properties) {
        _properties = properties;
    }
    
    /** get Tile's localZorder. */
    int getLocalZForPos(const Vec2& pos) const;
    
    /** return Layer type */
    int getLayerType() { return _layerType; };

    virtual std::string getDescription() const override;

protected:
    bool initCommon(Vec2 &layerOffset, TMXTiledMap *tileMap);
    Vec2 getPositionForIsoAt(const Vec2& pos);
    Vec2 getPositionForOrthoAt(const Vec2& pos);
    Vec2 getPositionForHexAt(const Vec2& pos);
    Vec2 getPositionForStaggeredAt(const Vec2& pos);
    Vec2 getPositionForObject(const Vec2& pos);
    Vec2 calculateLayerOffset(const Vec2& offset);

    intptr_t getZForPos(const Vec2& pos) const;
    Sprite *createTileSprite(intptr_t z, uint32_t gid);
    void setTileTexture(Sprite* sprite, uint32_t gid);
    void setupTileSprite(Sprite* sprite, const Vec2& pos, uint32_t gid);
    void setupTileAnimation(Sprite* sprite, const Vec2& pos, uint32_t gid);
    // play tile's animation
    void tilesUpdate(float dt);

    //! name of the layer
    std::string _layerName;
    //! TMX Layer supports opacity
    unsigned char _opacity;
    
    // used for retina display
    float _contentScaleFactor;
    
    /** size of the layer in tiles */
    Size _layerSize;
    /** size of the map's tile (could be different from the tile's size) */
    Size _mapTileSize;
    /** pointer to the map of tiles */
    uint32_t* _tiles;
    /** all tiles's sprite */
    std::map<intptr_t, Sprite *> _tileSprites;
    /** tiles's doing animation */
    std::map<intptr_t, TileAnimationData *> _tilesAniData;
    /** Layer orientation, which is the same as the map orientation */
    int _layerOrientation;
    /** Stagger Axis */
    int _staggerAxis;
    /** Stagger Index */
    int _staggerIndex;
    /** Hex side length*/
    int _hexSideLength;
    /** properties from the layer. They can be added using Tiled */
    ValueMap _properties;
    // weak ref to parent node
    TMXTiledMap *_tileMap;
    int _layerType;
};

// end of tilemap_parallax_nodes group
/** @} */

NS_CC_END

#endif //__CCTMX_LAYER_H__

