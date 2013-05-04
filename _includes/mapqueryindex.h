#pragma once
//------------------------------------------------------------------------------
/**
    @class Map::MapQueryIndex

    A query index for precomputed map queries.

    (C) 2010 Bigpoint GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Map
{
class MapQueryIndex
{
public:
    /// constructor
    MapQueryIndex();
    
    /// set values
    void Set(short x, short z, short minY, short maxY);
    /// get x position offset
    short GetXOffset() const;
    /// get z position offset
    short GetZOffset() const;
    /// get min Y
    short GetMinY() const;
    /// get max Y
    short GetMaxY() const;

private:
    short x;
    short z;
    short minY;
    short maxY;
};

//------------------------------------------------------------------------------
/**
*/
inline 
MapQueryIndex::MapQueryIndex() :
    x(0),
    z(0),
    minY(0),
    maxY(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline void
MapQueryIndex::Set(short x_, short z_, short minY_, short maxY_)
{
    this->x = x_;
    this->z = z_;
    this->minY = minY_;
    this->maxY = maxY_;
}

//------------------------------------------------------------------------------
/**
*/
inline short
MapQueryIndex::GetXOffset() const
{
    return this->x;
}

//------------------------------------------------------------------------------
/**
*/
inline short
MapQueryIndex::GetZOffset() const
{
    return this->z;
}

//------------------------------------------------------------------------------
/**
*/
inline short
MapQueryIndex::GetMinY() const
{
    return this->minY;
}

//------------------------------------------------------------------------------
/**
*/
inline short
MapQueryIndex::GetMaxY() const
{
    return this->maxY;
}

} // namespace Map
//------------------------------------------------------------------------------
