#pragma once
//------------------------------------------------------------------------------
/**
    @class Map::MapQueryTable

    A 2.5D table object to speed up spatial queries on the map.

    (C) 2010 Bigpoint GmbH
*/
#include "util/array.h"
#include "util/fixedtable.h"
#include "mapqueryindex.h"
#include "mapquerytableentry.h"
#include "mapquerytablestack.h"
#include "math/scalar.h"

//------------------------------------------------------------------------------
namespace Map
{
class MapQueryTable
{
public:
    /// constructor
    MapQueryTable();
    /// destructor
    ~MapQueryTable();

    /// setup the map query table
    void Setup(short mapSizeX, short mapSizeZ);
    /// discard the query table
    void Discard();
    /// return true if query table is valid
    bool IsValid() const;
    /// reserve space in entries array (to prevent reallocation)
    void ReserveEntries(SizeT numEntries);

    /// clamp X coord against map size
    short ClampX(short x) const;
    /// clamp Z coord against map size
    short ClampZ(short z) const;

    /// add a map query element to the table, returns entry index
    void AddEntry(IndexT instIndex, short minX, short maxX, short minY, short maxY, short minZ, short maxZ);

    /// begin a query sequence, this will create a new query uniqueness key
    void BeginQuery() const;
    /// query a single cell
    SizeT QueryCell(short x, short minY, short maxY, short z, Util::Array<IndexT>& outResult) const;
    /// perform a box query, returns unique instance indices
    SizeT QueryBox(short minX, short maxX, short minY, short maxY, short minZ, short maxZ, Util::Array<IndexT>& outResult) const;
    /// perform a 2D area query
    SizeT QueryArea(short minX, short maxX, short minZ, short maxZ, Util::Array<IndexT>& outResult) const;
    /// @todo: perform a precomputed query (for instance fixed camera)
    // SizeT QueryFixed(short posX, short posY, const Util::Array<MapQueryIndex>& indices, Util::Array<IndexT>& outResult) const;
    /// end current query
    void EndQuery() const;

    /// get map x size
    short GetMapSizeX() const;
    /// get map z size
    short GetMapSizeZ() const;
    /// access to a specific query table stack
    const MapQueryTableStack& GetStack(short x, short z) const;
    /// get entries array
    const Util::Array<MapQueryTableEntry>& GetEntries() const;

private:
    bool isValid;
    mutable IndexT queryKey;
    Util::Array<MapQueryTableEntry> entries;
    Util::FixedTable<MapQueryTableStack> queryTable;
    mutable bool inQuery;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
MapQueryTable::IsValid() const
{
    return this->isValid;
}

//------------------------------------------------------------------------------
/**
*/
inline short
MapQueryTable::ClampX(short x) const
{
    return Math::n_clamp<short>(x, 0, this->queryTable.Width() - 1);
}

//------------------------------------------------------------------------------
/**
*/
inline short
MapQueryTable::ClampZ(short z) const
{
    return Math::n_clamp<short>(z, 0, this->queryTable.Height() - 1);
}

//------------------------------------------------------------------------------
/**
*/
inline short
MapQueryTable::GetMapSizeX() const
{
    return this->queryTable.Width();
}

//------------------------------------------------------------------------------
/**
*/
inline short
MapQueryTable::GetMapSizeZ() const
{
    return this->queryTable.Height();
}

//------------------------------------------------------------------------------
/**
*/
inline const MapQueryTableStack&
MapQueryTable::GetStack(short x, short z) const
{
    return this->queryTable.At(x, z);
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<MapQueryTableEntry>& 
MapQueryTable::GetEntries() const
{
    return this->entries;
}

} // namespace Map
//------------------------------------------------------------------------------
