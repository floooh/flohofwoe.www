#pragma once
//------------------------------------------------------------------------------
/**
    @class Map::MapQueryTableEntry

    Describes an entry in the MapQueryTable.

    (C) 2010 Bigpoint GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Map
{
class MapQueryTableEntry
{
public:
    /// constructor
    MapQueryTableEntry();
    /// set values
    void Set(IndexT instIndex, short minX, short maxX, short minY, short maxY, short minZ, short maxZ);
    /// set last query key
    void SetQueryKey(IndexT k);
    /// get last query key
    IndexT GetQueryKey() const;
    /// get instance index
    IndexT GetInstIndex() const;
    /// get min x
    short GetMinX() const;
    /// get max x
    short GetMaxX() const;
    /// get min Y
    short GetMinY() const;
    /// get max Y
    short GetMaxY() const;
    /// get min z
    short GetMinZ() const;
    /// get max z
    short GetMaxZ() const;

private:
    IndexT queryKey;
    IndexT instIndex;
    short minX;
    short maxX;
    short minY;
    short maxY;
    short minZ;
    short maxZ;
};

//------------------------------------------------------------------------------
/**
*/
inline
MapQueryTableEntry::MapQueryTableEntry() :
    queryKey(InvalidIndex),
    instIndex(InvalidIndex),
    minX(0),
    maxX(0),
    minY(0),
    maxY(0),
    minZ(0),
    maxZ(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline void
MapQueryTableEntry::Set(IndexT i, short x0, short x1, short y0, short y1, short z0, short z1)
{
    this->instIndex = i;
    this->minX = x0;
    this->maxX = x1;
    this->minY = y0;
    this->maxY = y1;
    this->minZ = z0;
    this->maxZ = z1;
}

//------------------------------------------------------------------------------
/**
*/
inline void
MapQueryTableEntry::SetQueryKey(IndexT k)
{
    this->queryKey = k;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
MapQueryTableEntry::GetQueryKey() const
{
    return this->queryKey;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
MapQueryTableEntry::GetInstIndex() const
{
    return this->instIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline short
MapQueryTableEntry::GetMinX() const
{
    return this->minX;
}

//------------------------------------------------------------------------------
/**
*/
inline short
MapQueryTableEntry::GetMaxX() const
{
    return this->maxX;
}

//------------------------------------------------------------------------------
/**
*/
inline short
MapQueryTableEntry::GetMinY() const
{
    return this->minY;
}

//------------------------------------------------------------------------------
/**
*/
inline short
MapQueryTableEntry::GetMaxY() const
{
    return this->maxY;
}

//------------------------------------------------------------------------------
/**
*/
inline short
MapQueryTableEntry::GetMinZ() const
{
    return this->minZ;
}

//------------------------------------------------------------------------------
/**
*/
inline short
MapQueryTableEntry::GetMaxZ() const
{
    return this->maxZ;
}

} // namespace Map
//------------------------------------------------------------------------------
