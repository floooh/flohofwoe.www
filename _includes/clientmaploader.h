#pragma once
//------------------------------------------------------------------------------
/**
    @class Map::ClientMapLoader

    Load a map file into ClientMap object. See the Nebula3 mapconverter3 tool
    for the corresponding writer class!

    (C) 2010 Bigpoint GmbH
*/
#include "clientmap.h"

//------------------------------------------------------------------------------
namespace Map
{
class ClientMapLoader
{
public:
    /// load a map file into a client map object
    bool Load(const Util::StringAtom& mapName, const IO::URI& uri, const Ptr<ClientMap>& outMap);
    /// get error if loading failed
    const Util::String& GetError() const;
    /// load additional content into map
    bool LoadEventContent(const Util::StringAtom& eventID, const IO::URI& uri, const Ptr<ClientMap>& parentMap);
private:
    /// lookup a string in the string table, return invalid string if string index is invalid index (0xffff)
    Util::StringAtom LookupString(ushort stringIndex);

    static const ushort InvalidStringIndex = 0xffff;

    Util::StringAtom mapName;
    Util::Array<Util::StringAtom> stringTable;
    Util::String error;
    Util::Array<Util::StringAtom> eventList;
    Util::Array<Util::String> eventMappingTable;
};

//------------------------------------------------------------------------------
/**
*/
inline const Util::String&
ClientMapLoader::GetError() const
{
    return this->error;
}

} // namespace Map
//------------------------------------------------------------------------------
