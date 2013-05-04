#pragma once
//------------------------------------------------------------------------------
/**
    @class Map::MapLoader

    Load and validate a map file. See the Nebula3 mapconverter3 tool
    for the corresponding writer class!

    (C) 2012 Bigpoint GmbH
*/
#include "core/refcounted.h"
#include "math/float4.h"
#include "util/array.h"
#include "util/stringatom.h"

//------------------------------------------------------------------------------
namespace Map
{
class MapData: public Core::RefCounted
{
    __DeclareClass(MapData);
public:
    /// constructor
    MapData();
    /// destructor
    virtual ~MapData();

    class Info
    {
    public:
        Math::float4 gridSize;
        Math::float4 center;
        Math::float4 extents;
        short mapSizeX;
        short mapSizeY;
        short mapSizeZ;
    };
    Info info;
    Util::Array<Util::StringAtom> stringTable;
    Util::Array<uint> eventList;
    Util::Array<Util::String> eventMapping;
    class Template
    {
    public:
        ushort gfxResId;
        ushort gfxRootNode; 
        ushort phxResId;  
        ushort collResId;
        ushort sfxEventId;
        Math::float4 center;
        Math::float4 extents;
        ushort collMeshGroupIndex;
        ushort type;
    };
    Util::Array<Template> templates;
    class Instance
    {
    public:
        Math::float4 pos;
        Math::float4 rot;
        bool useScaling;
        Math::float4 scale;
        bool useCollide;
        bool isVisibleForNavMeshGen;
        short templIndex;    
        short groupIndex;    
        ushort dgNameIndex;
        ushort indexToMapping;
    };
    Util::Array<Instance> instances;
    class Group
    {
    public:
        ushort nameId;
        ushort type;
        short parentGroupIndex;
        Util::Array<ushort> instanceIndices;
        Util::Array<ushort> groupIndices;
    };
    Util::Array<Group> groups;
};

} // namespace Map