#pragma once
//------------------------------------------------------------------------------
/**
    @class Map::ClientMap

    Represents the client-side map.

    (C) 2010 Radon Labs GmbH
*/
#include "clientmapitemtemplate.h"
#include "clientmapiteminstance.h"
#include "clientmapgroup.h"
#include "mapquerytable.h"

//------------------------------------------------------------------------------
namespace Map
{
class ClientMap : public Core::RefCounted
{
    __DeclareClass(ClientMap);
public:
    /// constructor
    ClientMap();
    /// destructor
    virtual ~ClientMap();

    /// setup the client map object
    void Setup(const Util::StringAtom& name, const Math::vector& gridSize, const Math::bbox& mapBox, short mapSizeX, short mapSizeY, short mapSizeZ);
    /// discard the client map object
    void Discard();
    /// return true if the object has been setup
    bool IsValid() const;

    /// get map name
    const Util::StringAtom& GetName() const;
    /// get grid size
    const Math::vector& GetGridSize() const;
    /// get map bounding box
    const Math::bbox& GetBoundingBox() const;
    /// get X map size in grid element
    short GetMapSizeX() const;
    /// get Y map size in grid element
    short GetMapSizeY() const;
    /// get Z map size in grid element
    short GetMapSizeZ() const;

    /// access to query table
    const MapQueryTable& GetQueryTable() const;
    /// convert a world coordinate into map grid coordinates (also clamps against map size to account for fp inaccuracies)
    void ConvertWorldToGridCoords(const Math::float4& world, short& outMapX, short& outMapY, short& outMapZ);

    /// reserve space for template object
    void ReserveTemplates(SizeT num);
    /// add a template object which has only a collide
    void AddTemplate(
        const Util::StringAtom& collideResId, 
        IndexT collMeshGroupIndex, 
        const Math::bbox& box, 
        ClientMapItemTemplate::Type type);
    /// add a template object
    void AddTemplate(
        const Util::StringAtom& gfxResId, 
        const Util::StringAtom& collideResId, 
        IndexT collMeshGroupIndex, 
        const Math::bbox& box, 
        ClientMapItemTemplate::Type type);
    /// add a template object
    void AddTemplate(
        const Util::StringAtom& gfxResId, 
        const Util::StringAtom& rootNodePath,
        const Util::StringAtom& phxResId, 
        const Util::StringAtom& collideResId, 
        IndexT collMeshGroupIndex, 
        const Math::bbox& box, 
        ClientMapItemTemplate::Type type,
        const Util::StringAtom& sfxEventId);
    /// get number of templates
    SizeT GetNumTemplates() const;
    /// lookup template index by (graphics) resource id, return InvalidIndex if not found
    IndexT LookupTemplateIndex(const Util::StringAtom& gfxResId) const;
    /// lookup template index by (graphics) resource id, return InvalidIndex if not found
    IndexT LookupTemplateIndex(const Util::StringAtom& gfxResId, const Util::StringAtom& rootNodePath) const;
    /// get template by index
    ClientMapItemTemplate& TemplateByIndex(IndexT index);
    /// get template by index, read only
    const ClientMapItemTemplate& TemplateByIndex(IndexT index) const;

    /// reserve space for instance objects
    void ReserveInstances(SizeT num);
    /// add an instance object
    void AddInstance(const Math::matrix44& m, const Math::bbox& box, IndexT templateIndex, IndexT groupIndex, const Util::StringAtom& editName, bool useCollide, const Util::Dictionary<Util::StringAtom, bool>& eventBits);
    /// get number of instances
    SizeT GetNumInstances() const;
    /// lookup instance index by (optional) edit name, return InvalidIndex if not found
    IndexT LookupInstanceIndex(const Util::StringAtom& editName) const;
    /// get instance by index
    ClientMapItemInstance& InstanceByIndex(IndexT index);
    /// give instance a new template index
    void EditInstance(IndexT instanceIndex, IndexT templateIndex);

    /// reserve space for group objects
    void ReserveGroups(SizeT num);
    /// add a group object
    void AddGroup(const Util::StringAtom& name, ClientMapGroup::Type type, IndexT parentGroupIndex, const Util::Array<IndexT>& instIndices, const Util::Array<IndexT>& groupIndices);
    /// get number of groups
    SizeT GetNumGroups() const;
    /// lookup group index by name, return InvalidIndex if not found
    IndexT LookupGroupIndex(const Util::StringAtom& groupName) const;
    /// get group by index
    ClientMapGroup& GroupByIndex(IndexT index);

    /// load template collide objects
    bool LoadTemplateCollides();
    /// unloade template collide objects
    void UnloadTemplateCollides();

    /// set possible events
    void SetPossibleEventIds(const Util::Dictionary<Util::StringAtom, uchar>& events);
    /// check if map has a map for eventId
    bool HasEventMap(const Util::StringAtom& eventId) const;

    /// append a list of indices
    void UpdateGroup(IndexT groupIndex, const Util::Array<IndexT>& instIndices);

private:
    Util::StringAtom mapName;
    Math::vector gridSize;
    Math::vector oneDivGridSize;
    Math::bbox box;
    short mapSizeX;
    short mapSizeY;
    short mapSizeZ;

    Util::Array<ClientMapItemTemplate> templates;
    Util::Dictionary<Util::StringAtom, IndexT> templateIndexMap;

    Util::Array<ClientMapItemInstance> instances;
    Util::Dictionary<Util::StringAtom, IndexT> instanceIndexMap;

    Util::Array<ClientMapGroup> groups;
    Util::Dictionary<Util::StringAtom, IndexT> groupIndexMap;

    Util::Dictionary<Util::StringAtom, uchar> possibleEvents; 
    MapQueryTable queryTable;
    bool isValid;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
ClientMap::IsValid() const
{
    return this->isValid;
}

//------------------------------------------------------------------------------
/**
*/
inline const MapQueryTable&
ClientMap::GetQueryTable() const
{
    return this->queryTable;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
ClientMap::GetName() const
{
    return this->mapName;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::vector&
ClientMap::GetGridSize() const
{
    return this->gridSize;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::bbox&
ClientMap::GetBoundingBox() const
{
    return this->box;
}

//------------------------------------------------------------------------------
/**
*/
inline short
ClientMap::GetMapSizeX() const
{
    return this->mapSizeX;
}

//------------------------------------------------------------------------------
/**
*/
inline short
ClientMap::GetMapSizeY() const
{
    return this->mapSizeY;
}

//------------------------------------------------------------------------------
/**
*/
inline short
ClientMap::GetMapSizeZ() const
{
    return this->mapSizeZ;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
ClientMap::GetNumTemplates() const
{
    return this->templates.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline ClientMapItemTemplate&
ClientMap::TemplateByIndex(IndexT i)
{
    return this->templates[i];
}

//------------------------------------------------------------------------------
/**
*/
inline 
const ClientMapItemTemplate&
ClientMap::TemplateByIndex(IndexT i) const
{
    return this->templates[i];
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
ClientMap::GetNumInstances() const
{
    return this->instances.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline ClientMapItemInstance&
ClientMap::InstanceByIndex(IndexT i)
{
    return this->instances[i];
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
ClientMap::GetNumGroups() const
{
    return this->groups.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline ClientMapGroup&
ClientMap::GroupByIndex(IndexT i)
{
    return this->groups[i];
}

//------------------------------------------------------------------------------
/**
*/
inline void 
ClientMap::SetPossibleEventIds(const Util::Dictionary<Util::StringAtom, uchar>& events)
{
    this->possibleEvents = events;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
ClientMap::HasEventMap(const Util::StringAtom& eventId) const
{
    return this->possibleEvents.Contains(eventId);
}

} // namespace Map
//------------------------------------------------------------------------------

