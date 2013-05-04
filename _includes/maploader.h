#pragma once
//------------------------------------------------------------------------------
/**
    @class Map::MapLoader

    Load and validate a map file. See the Nebula3 mapconverter3 tool
    for the corresponding writer class!

    (C) 2012 Bigpoint GmbH
*/
#include "util/fourcc.h"
#include "core/ptr.h"
#include "io/uri.h"
#include "io/binaryreader.h"
#include "mapdata.h"

//------------------------------------------------------------------------------
namespace Map
{
class MapLoader
{
public:
    /// constructor
    MapLoader();
    /// destructor
    ~MapLoader();

    /// Validate file and save the data to saveToMapData parameter.
    bool LoadMap(const IO::URI& fromUri, Ptr<MapData>& outMapData);
    /// load event data for instances and groups from .eve file
    bool LoadAdditionalEvents(const IO::URI& fromUri, Util::Array<MapData::Instance>& outInstances, Util::Array<MapData::Group>& outGroups);
private:
    /// load data from map file or eve file
    bool Load(const IO::URI& fromUri, bool loadEvents, Ptr<MapData>& outMapData);

    /// try to load map data
    bool TryLoadMap();
    /// try to load event data
    bool TryLoadEvent();
    /// read header info and make sure it's a map file
    bool ParseHeader();
    /// load map info block 
    bool LoadMapInfo();
    /// load string table
    bool LoadStringTable();
    /// load event list into map object
    bool LoadEventList();
    /// load event mapping into map object
    bool LoadEventMapping();
    /// load templates into map object
    bool LoadTemplates();
    /// load instances into map object
    bool LoadInstances();
    /// load groups into map object
    bool LoadGroups();

    // FIXME! remove this chunk out of the file
    bool LoadSets();
    // FIXME! remove this chunk out of the file
    bool LoadNavBlockers();

    bool CheckTag(Util::FourCC tag, const char* errorMsg);

    Util::String error;
    Ptr<IO::BinaryReader> reader;
    Ptr<MapData> outMap;              

    static const uint Version = 5;           
};

} // namespace Map