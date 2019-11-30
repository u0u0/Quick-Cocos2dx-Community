/****************************************************************************
Copyright (c) 2011      Максим Аксенов 
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

#include "2d/CCTMXXMLParser.h"
#include <unordered_map>
#include <sstream>
#include "2d/CCTMXTiledMap.h"
#include "base/ZipUtils.h"
#include "base/base64.h"
#include "base/CCDirector.h"
#include "platform/CCFileUtils.h"

using namespace std;

NS_CC_BEGIN

// implementation TMXLayerInfo
TMXLayerInfo::TMXLayerInfo()
: _name("")
, _tiles(nullptr)
, _ownTiles(true)
{
}

TMXLayerInfo::~TMXLayerInfo()
{
    CCLOGINFO("deallocing TMXLayerInfo: %p", this);
    if (_ownTiles && _tiles)
    {
        free(_tiles);
        _tiles = nullptr;
    }
}

ValueMap& TMXLayerInfo::getProperties()
{
    return _properties;
}

void TMXLayerInfo::setProperties(ValueMap var)
{
    _properties = var;
}

// implementation TMXImageLayerInfo
TMXImageLayerInfo::TMXImageLayerInfo()
: _name("")
, _visible(true)
, _opacity(255)
{
}

TMXImageLayerInfo::~TMXImageLayerInfo()
{
    CCLOGINFO("deallocing TMXImageLayerInfo: %p", this);
}

ValueMap& TMXImageLayerInfo::getProperties()
{
    return _properties;
}

void TMXImageLayerInfo::setProperties(ValueMap var)
{
    _properties = var;
}

// implementation TMXGroupInfo
TMXGroupInfo::TMXGroupInfo()
: _name("")
, _visible(true)
, _opacity(255)
{
}

TMXGroupInfo::~TMXGroupInfo()
{
    CCLOGINFO("deallocing TMXImageLayerInfo: %p", this);
}

ValueMap& TMXGroupInfo::getProperties()
{
    return _properties;
}

void TMXGroupInfo::setProperties(ValueMap var)
{
    _properties = var;
}

// implementation TMXTilesetInfo
TMXTilesetInfo::TMXTilesetInfo()
    :_firstGid(0)
    ,_tileSize(Size::ZERO)
    ,_spacing(0)
    ,_margin(0)
    ,_imageSize(Size::ZERO)
{
}

TMXTilesetInfo::~TMXTilesetInfo()
{
    CCLOGINFO("deallocing TMXTilesetInfo: %p", this);
}

Rect TMXTilesetInfo::getRectForGID(uint32_t gid)
{
    Rect rect;
    rect.size = _tileSize;
    gid &= kTMXFlippedMask;
    gid = gid - _firstGid;
    // max_x means the column count in tile map
    // in the origin:
    // max_x = (int)((_imageSize.width - _margin*2 + _spacing) / (_tileSize.width + _spacing));
    // but in editor "Tiled", _margin variable only effect the left side
    // for compatible with "Tiled", change the max_x calculation
    int max_x = (int)((_imageSize.width - _margin + _spacing) / (_tileSize.width + _spacing));
    
    rect.origin.x = (gid % max_x) * (_tileSize.width + _spacing) + _margin;
    rect.origin.y = (gid / max_x) * (_tileSize.height + _spacing) + _margin;
    return rect;
}

// implementation TMXMapInfo

TMXMapInfo * TMXMapInfo::create(const std::string& tmxFile)
{
    TMXMapInfo *ret = new (std::nothrow) TMXMapInfo();
    if (ret->initWithTMXFile(tmxFile))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

TMXMapInfo * TMXMapInfo::createWithXML(const std::string& tmxString, const std::string& resourcePath)
{
    TMXMapInfo *ret = new (std::nothrow) TMXMapInfo();
    if (ret->initWithXML(tmxString, resourcePath))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void TMXMapInfo::internalInit(const std::string& tmxFileName, const std::string& resourcePath)
{
    if (!tmxFileName.empty()) {
        _TMXFileName = tmxFileName;
    }
    
    if (!resourcePath.empty())
    {
        _resources = resourcePath;
    }
    
    _objectGroups.reserve(4);

    // tmp vars
    _currentString = "";
    _storingCharacters = false;
    _layerAttribs = TMXLayerAttribNone;
    _parentElement = TMXPropertyNone;
    _currentFirstGID = -1;
}

bool TMXMapInfo::initWithXML(const std::string& tmxString, const std::string& resourcePath)
{
    internalInit("", resourcePath);
    return parseXMLString(tmxString);
}

bool TMXMapInfo::initWithTMXFile(const std::string& tmxFile)
{
    internalInit(tmxFile, "");
    return parseXMLFile(_TMXFileName);
}

TMXMapInfo::TMXMapInfo()
: _orientation(TMXOrientationOrtho)
, _staggerAxis(TMXStaggerAxis_Y)
, _staggerIndex(TMXStaggerIndex_Even)
, _hexSideLength(0)
, _mapSize(Size::ZERO)
, _tileSize(Size::ZERO)
, _parentElement(TMXPropertyNone)
, _parentGID(0)
, _layerAttribs(0)
, _storingCharacters(false)
, _xmlTileIndex(0)
, _currentFirstGID(-1)
, _recordFirstGID(true)
, _curImageLayer(nullptr)
{
}

TMXMapInfo::~TMXMapInfo()
{
    CCLOGINFO("deallocing TMXMapInfo: %p", this);
}

bool TMXMapInfo::parseXMLString(const std::string& xmlString)
{
    size_t len = xmlString.size();
    if (len <= 0)
        return false;

    SAXParser parser;

    if (false == parser.init("UTF-8")) {
        return false;
    }

    parser.setDelegator(this);

    return parser.parse(xmlString.c_str(), len);
}

bool TMXMapInfo::parseXMLFile(const std::string& xmlFilename)
{
    SAXParser parser;
    
    if (false == parser.init("UTF-8")) {
        return false;
    }
    
    parser.setDelegator(this);

    return parser.parse(FileUtils::getInstance()->fullPathForFilename(xmlFilename));
}

// the XML parser calls here with all the elements
void TMXMapInfo::startElement(void* /*ctx*/, const char *name, const char **atts)
{
    std::string elementName = name;
    ValueMap attributeDict;
    if (atts && atts[0]) {
        for (int i = 0; atts[i]; i += 2) {
            std::string key = atts[i];
            std::string value = atts[i+1];
            attributeDict.emplace(key, Value(value));
        }
    }
    
    if (elementName == "map") {
        std::string version = attributeDict["version"].asString();
        if (version != "1.2") {
            CCLOG("cocos2d: TMXFormat: Unsupported TMX version: %s", version.c_str());
        }
        std::string orientationStr = attributeDict["orientation"].asString();
        if (orientationStr == "orthogonal") {
            _orientation = TMXOrientationOrtho;
        } else if (orientationStr  == "isometric") {
            _orientation = TMXOrientationIso;
        } else if (orientationStr == "hexagonal") {
            _orientation = TMXOrientationHex;
        } else if (orientationStr == "staggered") {
            _orientation = TMXOrientationStaggered;
        } else {
            CCLOG("cocos2d: TMXFomat: Unsupported orientation: %d", _orientation);
        }
        
        std::string staggerAxisStr = attributeDict["staggeraxis"].asString();
        if (staggerAxisStr == "x") {
            _staggerAxis = TMXStaggerAxis_X;
        } else if (staggerAxisStr  == "y") {
            _staggerAxis = TMXStaggerAxis_Y;
        }

        std::string staggerIndex = attributeDict["staggerindex"].asString();
        if (staggerIndex == "odd") {
            _staggerIndex = TMXStaggerIndex_Odd;
        } else if (staggerIndex == "even") {
            _staggerIndex = TMXStaggerIndex_Even;
        }

        float hexSideLength = attributeDict["hexsidelength"].asFloat();
        _hexSideLength = hexSideLength;

        Size s;
        s.width = attributeDict["width"].asFloat();
        s.height = attributeDict["height"].asFloat();
        _mapSize = s;

        s.width = attributeDict["tilewidth"].asFloat();
        s.height = attributeDict["tileheight"].asFloat();
        _tileSize = s;

        // The parent element is now "map"
        _parentElement = TMXPropertyMap;
    } else if (elementName == "tileset") {
        // If this is an external tileset then start parsing that
        std::string extTilesetName = attributeDict["source"].asString();
        if (extTilesetName != "") {
            // image file is relative to the map file. Also use relative path here to make hot update happey.
            if (_TMXFileName.size() > 0) {
                string dir = _TMXFileName.substr(0, _TMXFileName.find_last_of("/") + 1);
                extTilesetName = dir + extTilesetName;
            } else {
                extTilesetName = _resources + "/" + extTilesetName;
            }
            
            _currentFirstGID = attributeDict["firstgid"].asInt();
            if (_currentFirstGID < 0) {
                _currentFirstGID = 0;
            }
            _recordFirstGID = false;
            
            parseXMLFile(extTilesetName);
        } else {
            TMXTilesetInfo *tileset = new (std::nothrow) TMXTilesetInfo();
            tileset->_name = attributeDict["name"].asString();
            
            if (_recordFirstGID) {
                // unset before, so this is tmx file.
                tileset->_firstGid = attributeDict["firstgid"].asInt();
                if (tileset->_firstGid < 0) {
                    tileset->_firstGid = 0;
                }
            } else {
                tileset->_firstGid = _currentFirstGID;
                _currentFirstGID = 0;
            }
            
            tileset->_spacing = attributeDict["spacing"].asInt();
            tileset->_margin = attributeDict["margin"].asInt();
            Size s;
            s.width = attributeDict["tilewidth"].asFloat();
            s.height = attributeDict["tileheight"].asFloat();
            tileset->_tileSize = s;

            _tilesets.pushBack(tileset);
            tileset->release();
        }
    } else if (elementName == "tile") {
        if (_parentElement == TMXPropertyLayer) {
            TMXLayerInfo* layer = _layers.back();
            Size layerSize = layer->_layerSize;
            uint32_t gid = static_cast<uint32_t>(attributeDict["gid"].asUnsignedInt());
            int tilesAmount = layerSize.width*layerSize.height;
            if (_xmlTileIndex < tilesAmount) {
                layer->_tiles[_xmlTileIndex++] = gid;
            }
        } else {
            TMXTilesetInfo* info = _tilesets.back();
            _parentGID = info->_firstGid + attributeDict["id"].asInt();
            _tileProperties[_parentGID] = Value(ValueMap());
            _parentElement = TMXPropertyTile;
        }
    } else if (elementName == "layer") {
        TMXLayerInfo *layer = new (std::nothrow) TMXLayerInfo();
        layer->_name = attributeDict["name"].asString();

        Size s;
        s.width = attributeDict["width"].asFloat();
        s.height = attributeDict["height"].asFloat();
        layer->_layerSize = s;

        Value& visibleValue = attributeDict["visible"];
        layer->_visible = visibleValue.isNull() ? true : visibleValue.asBool();

        Value& opacityValue = attributeDict["opacity"];
        layer->_opacity = opacityValue.isNull() ? 255 : (unsigned char)(255.0f * opacityValue.asFloat());

        float x = attributeDict["offsetx"].asFloat();
        float y = attributeDict["offsety"].asFloat();
        layer->_offset.set(x, y);

        _layers.pushBack(layer);// for set property
        if (_groupStack.size() > 0) {
            _groupStack.back()->_children.pushBack(layer);//add to cur group
        } else {
            _children.pushBack(layer);//add to map
        }
        layer->release();

        // The parent element is now "layer"
        _parentElement = TMXPropertyLayer;
    } else if (elementName == "imagelayer") {
        TMXImageLayerInfo *layer = new (std::nothrow) TMXImageLayerInfo();
        layer->_name = attributeDict["name"].asString();

        Value& visibleValue = attributeDict["visible"];
        layer->_visible = visibleValue.isNull() ? true : visibleValue.asBool();

        Value& opacityValue = attributeDict["opacity"];
        layer->_opacity = opacityValue.isNull() ? 255 : (unsigned char)(255.0f * opacityValue.asFloat());

        float x = attributeDict["offsetx"].asFloat();
        float y = attributeDict["offsety"].asFloat();
        layer->_offset.set(x, y);

        if (_groupStack.size() > 0) {
            _groupStack.back()->_children.pushBack(layer);//add to cur group
        } else {
            _children.pushBack(layer);//add to map
        }
        _curImageLayer = layer;// for set property
        layer->release();

        // The parent element is now "layer"
        _parentElement = TMXPropertyImageLayer;
    } else if (elementName == "group") {
        TMXGroupInfo *group = new (std::nothrow) TMXGroupInfo();
        group->_name = attributeDict["name"].asString();

        Value& visibleValue = attributeDict["visible"];
        group->_visible = visibleValue.isNull() ? true : visibleValue.asBool();

        Value& opacityValue = attributeDict["opacity"];
        group->_opacity = opacityValue.isNull() ? 255 : (unsigned char)(255.0f * opacityValue.asFloat());

        float x = attributeDict["offsetx"].asFloat();
        float y = attributeDict["offsety"].asFloat();
        group->_offset.set(x, y);

        if (_groupStack.size() > 0) {
            _groupStack.back()->_children.pushBack(group);//add to cur group
        } else {
            _children.pushBack(group);//add to map
        }
        _groupStack.pushBack(group);//set to cur
        group->release();

        // The parent element is now "layer"
        _parentElement = TMXPropertyGroup;
    } else if (elementName == "objectgroup") {
        TMXObjectGroup *objectGroup = new (std::nothrow) TMXObjectGroup();
        objectGroup->setGroupName(attributeDict["name"].asString());
        
        Vec2 positionOffset;
        positionOffset.x = attributeDict["offsetx"].asFloat() * _tileSize.width;
        positionOffset.y = attributeDict["offsety"].asFloat() * _tileSize.height;
        objectGroup->setPositionOffset(positionOffset);
        
        Value& visibleValue = attributeDict["visible"];
        objectGroup->setVisible(visibleValue.isNull() ? true : visibleValue.asBool());
        
        Value& opacityValue = attributeDict["opacity"];
        objectGroup->setOpacity(opacityValue.isNull() ? 255 : (unsigned char)(255.0f * opacityValue.asFloat()));

        _objectGroups.pushBack(objectGroup);// for set property
        if (_groupStack.size() > 0) {
            _groupStack.back()->_children.pushBack(objectGroup);//add to cur group
        } else {
            _children.pushBack(objectGroup);//add to map
        }
        objectGroup->release();

        // The parent element is now "objectgroup"
        _parentElement = TMXPropertyObjectGroup;
    } else if (elementName == "tileoffset") {
        TMXTilesetInfo* tileset = _tilesets.back();
        float tileOffsetX = attributeDict["x"].asFloat();
        float tileOffsetY = attributeDict["y"].asFloat();
        tileset->_tileOffset = Vec2(tileOffsetX, tileOffsetY);
    } else if (elementName == "image") {
        if (_parentElement == TMXPropertyImageLayer) {
            std::string imagename = attributeDict["source"].asString();
            _curImageLayer->_originSourceImage = imagename;
            // image file is relative to the map file. Also use relative path here to make hot update happey.
            if (_TMXFileName.size() > 0) {
                string dir = _TMXFileName.substr(0, _TMXFileName.find_last_of("/") + 1);
                _curImageLayer->_sourceImage = dir + imagename;
            } else {
                _curImageLayer->_sourceImage = _resources + "/" + imagename;
            }
        } else {
            TMXTilesetInfo* tileset = _tilesets.back();
            std::string imagename = attributeDict["source"].asString();
            tileset->_originSourceImage = imagename;
            // image file is relative to the map file. Also use relative path here to make hot update happey.
            if (_TMXFileName.size() > 0) {
                string dir = _TMXFileName.substr(0, _TMXFileName.find_last_of("/") + 1);
                tileset->_sourceImage = dir + imagename;
            } else {
                tileset->_sourceImage = _resources + "/" + imagename;
            }
        }
    } else if (elementName == "data") {
        std::string encoding = attributeDict["encoding"].asString();
        std::string compression = attributeDict["compression"].asString();

        if (encoding == "") {
            _layerAttribs |= TMXLayerAttribNone;
            
            TMXLayerInfo* layer = _layers.back();
            Size layerSize = layer->_layerSize;
            int tilesAmount = layerSize.width*layerSize.height;

            uint32_t *tiles = (uint32_t*) malloc(tilesAmount*sizeof(uint32_t));
            // set all value to 0
            memset(tiles, 0, tilesAmount*sizeof(int));

            layer->_tiles = tiles;
        } else if (encoding == "base64") {
            _layerAttribs |= TMXLayerAttribBase64;
            _storingCharacters = true;
            
            if (compression == "gzip") {
                _layerAttribs |= TMXLayerAttribGzip;
            } else if (compression == "zlib") {
                _layerAttribs |= TMXLayerAttribZlib;
            }
        } else if (encoding == "csv") {
            _layerAttribs |= TMXLayerAttribCSV;
            _storingCharacters = true;
        }
    } else if (elementName == "object") {
        TMXObjectGroup* objectGroup = _objectGroups.back();

        // The value for "type" was blank or not a valid class name
        // Create an instance of TMXObjectInfo to store the object and its properties
        ValueMap dict;
        dict["id"] = attributeDict["id"];
        dict["x"] = attributeDict["x"];
        dict["y"] = attributeDict["y"];
        
        dict["visible"] = true;
        if (!attributeDict["visible"].isNull()) {
            dict["visible"] = attributeDict["visible"].asBool();
        }
        if (!attributeDict["width"].isNull()) {
            dict["width"] = attributeDict["width"];
            dict["height"] = attributeDict["height"];
            dict["objectType"] = "rectangle"; // object type
        }
        if (!attributeDict["name"].isNull()) {
            dict["name"] = attributeDict["name"];
        }
        if (!attributeDict["type"].isNull()) {
            dict["type"] = attributeDict["type"];
        }
        if (!attributeDict["rotation"].isNull()) {
            dict["rotation"] = attributeDict["rotation"];
        }
        if (!attributeDict["gid"].isNull()) {
            dict["gid"] = attributeDict["gid"];
            dict["objectType"] = "tile"; // override rectangle
        }

        // Add the object to the objectGroup
        objectGroup->getObjects().push_back(Value(dict));

        // The parent element is now "object"
        _parentElement = TMXPropertyObject;
    } else if (elementName == "property") {
        if (_parentElement == TMXPropertyNone) {
            CCLOG("TMX tile map: Parent element is unsupported. Cannot add property named '%s' with value '%s'",
                  attributeDict["name"].asString().c_str(), attributeDict["value"].asString().c_str());
        } else if (_parentElement == TMXPropertyMap) {
            // The parent element is the map
            Value value = attributeDict["value"];
            std::string key = attributeDict["name"].asString();
            _properties.emplace(key, value);
        } else if (_parentElement == TMXPropertyLayer) {
            // The parent element is the last layer
            TMXLayerInfo* layer = _layers.back();
            Value value = attributeDict["value"];
            std::string key = attributeDict["name"].asString();
            // Add the property to the layer
            layer->getProperties().emplace(key, value);
        } else if (_parentElement == TMXPropertyImageLayer) {
            Value value = attributeDict["value"];
            std::string key = attributeDict["name"].asString();
            // Add the property to the layer
            _curImageLayer->getProperties().emplace(key, value);
        } else if (_parentElement == TMXPropertyGroup) {
            Value value = attributeDict["value"];
            std::string key = attributeDict["name"].asString();
            // Add the property to the layer
            _groupStack.back()->getProperties().emplace(key, value);
        } else if (_parentElement == TMXPropertyObjectGroup) {
            // The parent element is the last object group
            TMXObjectGroup* objectGroup = _objectGroups.back();
            Value value = attributeDict["value"];
            std::string key = attributeDict["name"].asString();
            objectGroup->getProperties().emplace(key, value);
        } else if (_parentElement == TMXPropertyObject) {
            // The parent element is the last object
            TMXObjectGroup* objectGroup = _objectGroups.back();
            ValueMap& dict = objectGroup->getObjects().rbegin()->asValueMap();

            std::string propertyName = attributeDict["name"].asString();
            dict[propertyName] = attributeDict["value"];
        } else if (_parentElement == TMXPropertyTile) {
            ValueMap& dict = _tileProperties.at(_parentGID).asValueMap();

            std::string propertyName = attributeDict["name"].asString();
            dict[propertyName] = attributeDict["value"];
        }
    } else if (elementName == "animation") {
        if (_parentElement == TMXPropertyTile) {
            ValueMap& dict = _tileProperties.at(_parentGID).asValueMap();
            dict["animation"] = Value(ValueVector());
        }
    } else if (elementName == "frame") {
        // parse tile animation belog to tile properties, auto add key "animation"
        if (_parentElement == TMXPropertyTile) {
            ValueMap& dict = _tileProperties.at(_parentGID).asValueMap();
            // one frame data hava two properites, make a pairs.
            ValueVector& vector = dict["animation"].asValueVector();
            vector.push_back(Value(attributeDict["tileid"].asUnsignedInt() + 1));// gid++
            vector.push_back(attributeDict["duration"]);
        }
    } else if (elementName == "text") {
        // find parent object's dict and add polygon-points to it
        TMXObjectGroup* objectGroup = _objectGroups.back();
        ValueMap& dict = objectGroup->getObjects().rbegin()->asValueMap();
        
        dict["objectType"] = "text"; // object type
        dict["fontfamily"] = attributeDict["fontfamily"];
        dict["pixelsize"] = attributeDict["pixelsize"].asUnsignedInt();
        dict["wrap"] = attributeDict["wrap"].asBool();
        dict["color"] = attributeDict["color"];
        dict["halign"] = attributeDict["halign"];
        dict["valign"] = attributeDict["valign"];
        _storingCharacters = true;
    } else if (elementName == "polygon") {
        // find parent object's dict and add polygon-points to it
        TMXObjectGroup* objectGroup = _objectGroups.back();
        ValueMap& dict = objectGroup->getObjects().rbegin()->asValueMap();

        // get points value string
        std::string value = attributeDict["points"].asString();
        if (!value.empty()) {
            ValueVector pointsArray;
            pointsArray.reserve(10);

            // parse points string into a space-separated set of points
            stringstream pointsStream(value);
            string pointPair;
            while (std::getline(pointsStream, pointPair, ' ')) {
                // parse each point combo into a comma-separated x,y point
                stringstream pointStream(pointPair);
                string xStr, yStr;
                
                ValueMap pointDict;
                // set x
                if (std::getline(pointStream, xStr, ',')) {
                    pointDict["x"] = Value(atof(xStr.c_str()));
                }
                // set y
                if (std::getline(pointStream, yStr, ',')) {
                    pointDict["y"] = Value(atof(yStr.c_str()));
                }
                
                // add to points array
                pointsArray.push_back(Value(pointDict));
            }
            
            dict["points"] = Value(pointsArray);
            dict["objectType"] = "polygon";
        }
    } else if (elementName == "polyline") {
        // find parent object's dict and add polyline-points to it
        TMXObjectGroup* objectGroup = _objectGroups.back();
        ValueMap& dict = objectGroup->getObjects().rbegin()->asValueMap();
        
        // get points value string
        std::string value = attributeDict["points"].asString();
        if (!value.empty()) {
            ValueVector pointsArray;
            pointsArray.reserve(10);
            
            // parse points string into a space-separated set of points
            stringstream pointsStream(value);
            string pointPair;
            while (std::getline(pointsStream, pointPair, ' ')) {
                // parse each point combo into a comma-separated x,y point
                stringstream pointStream(pointPair);
                string xStr, yStr;
                
                ValueMap pointDict;
                // set x
                if (std::getline(pointStream, xStr, ',')) {
                    pointDict["x"] = Value(atof(xStr.c_str()));
                }
                // set y
                if (std::getline(pointStream, yStr, ',')) {
                    pointDict["y"] = Value(atof(yStr.c_str()));
                }
                
                // add to points array
                pointsArray.push_back(Value(pointDict));
            }
            
            dict["polylinePoints"] = Value(pointsArray);
            dict["objectType"] = "polyline";
        }
    }
}

void TMXMapInfo::endElement(void* /*ctx*/, const char *name)
{
    std::string elementName = name;

    if (elementName == "data") {
        if (_layerAttribs & TMXLayerAttribBase64) {
            _storingCharacters = false;
            TMXLayerInfo* layer = _layers.back();
            
            unsigned char *buffer;
            auto len = base64Decode((unsigned char*)_currentString.c_str(), (unsigned int)_currentString.length(), &buffer);
            if (!buffer) {
                CCLOG("cocos2d: TiledMap: decode data error");
                return;
            }
            
            if (_layerAttribs & (TMXLayerAttribGzip | TMXLayerAttribZlib)) {
                unsigned char *deflated = nullptr;
                Size s = layer->_layerSize;
                // int sizeHint = s.width * s.height * sizeof(uint32_t);
                ssize_t sizeHint = s.width * s.height * sizeof(unsigned int);
                
                ssize_t CC_UNUSED inflatedLen = ZipUtils::inflateMemoryWithHint(buffer, len, &deflated, sizeHint);
                CCASSERT(inflatedLen == sizeHint, "inflatedLen should be equal to sizeHint!");
                
                free(buffer);
                buffer = nullptr;
                
                if (!deflated) {
                    CCLOG("cocos2d: TiledMap: inflate data error");
                    return;
                }
                
                layer->_tiles = reinterpret_cast<uint32_t*>(deflated);
            } else {
                layer->_tiles = reinterpret_cast<uint32_t*>(buffer);
            }
            
            _currentString = "";
        } else if (_layerAttribs & TMXLayerAttribCSV) {
            unsigned char *buffer;
            TMXLayerInfo* layer = _layers.back();
            _storingCharacters = false;

            vector<string> gidTokens;
            istringstream filestr(_currentString);
            string sRow;
            while(getline(filestr, sRow, '\n')) {
                string sGID;
                istringstream rowstr(sRow);
                while (getline(rowstr, sGID, ',')) {
                    gidTokens.push_back(sGID);
                }
            }

            // 32-bits per gid
            buffer = (unsigned char*)malloc(gidTokens.size() * 4);
            if (!buffer) {
                CCLOG("cocos2d: TiledMap: CSV buffer not allocated.");
                return;
            }

            uint32_t* bufferPtr = reinterpret_cast<uint32_t*>(buffer);
            for(const auto& gidToken : gidTokens) {
                auto tileGid = (uint32_t)strtoul(gidToken.c_str(), nullptr, 10);
                *bufferPtr = tileGid;
                bufferPtr++;
            }

            layer->_tiles = reinterpret_cast<uint32_t*>(buffer);

            _currentString = "";
        } else if (_layerAttribs & TMXLayerAttribNone) {
            _xmlTileIndex = 0;
        }
    } else if (elementName == "text") {
        if (_parentElement == TMXPropertyObject) {
            // find parent object's dict and add polygon-points to it
            TMXObjectGroup* objectGroup = _objectGroups.back();
            ValueMap& dict = objectGroup->getObjects().rbegin()->asValueMap();
            dict["text"] = _currentString;
        }
        _storingCharacters = false;
        _currentString = "";
    } else if (elementName == "point") {
        if (_parentElement == TMXPropertyObject) {
            // find parent object's dict and add polygon-points to it
            TMXObjectGroup* objectGroup = _objectGroups.back();
            ValueMap& dict = objectGroup->getObjects().rbegin()->asValueMap();
            dict["objectType"] = "point";
        }
    } else if (elementName == "ellipse") {
        if (_parentElement == TMXPropertyObject) {
            // find parent object's dict and add polygon-points to it
            TMXObjectGroup* objectGroup = _objectGroups.back();
            ValueMap& dict = objectGroup->getObjects().rbegin()->asValueMap();
            dict["objectType"] = "ellipse";
        }
    } else if (elementName == "map") {
        // The map element has ended
        _parentElement = TMXPropertyNone;
    } else if (elementName == "layer") {
        // The layer element has ended
        _parentElement = TMXPropertyNone;
    } else if (elementName == "imagelayer") {
        // The imagelayer element has ended
        _curImageLayer = nullptr;
        _parentElement = TMXPropertyNone;
    } else if (elementName == "group") {
        // The group element has ended
        _groupStack.popBack();
        _parentElement = TMXPropertyNone;
    } else if (elementName == "objectgroup") {
        // The objectgroup element has ended
        _parentElement = TMXPropertyNone;
    } else if (elementName == "object") {
        // The object element has ended
        _parentElement = TMXPropertyNone;
    } else if (elementName == "tileset") {
        _recordFirstGID = true;
    }
}

void TMXMapInfo::textHandler(void* /*ctx*/, const char *ch, size_t len)
{
    std::string text(ch, 0, len);
    if (_storingCharacters) {
        _currentString += text;
    }
}

NS_CC_END
