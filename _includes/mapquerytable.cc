//------------------------------------------------------------------------------
//  mapquerytable.cc
//  (C) 2010 Bigpoint GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "mapquerytable.h"

namespace Map
{

//------------------------------------------------------------------------------
/**
*/
MapQueryTable::MapQueryTable() :
    isValid(false),
    queryKey(0),
    inQuery(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
MapQueryTable::~MapQueryTable()
{
    if (this->IsValid())
    {
        this->Discard();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
MapQueryTable::Setup(short mapSizeX, short mapSizeZ)
{
    n_assert(!this->IsValid());
    this->isValid = true;
    this->queryTable.SetSize(mapSizeX, mapSizeZ);
}

//------------------------------------------------------------------------------
/**
*/
void
MapQueryTable::Discard()
{
    n_assert(this->IsValid());
    this->isValid = false;
    this->entries.Clear();
    this->queryTable.SetSize(0, 0);
}

//------------------------------------------------------------------------------
/**
*/
void
MapQueryTable::ReserveEntries(SizeT numEntries)
{
    this->entries.Reserve(numEntries);
}

//------------------------------------------------------------------------------
/**
*/
void
MapQueryTable::AddEntry(IndexT instIndex, short minX, short maxX, short minY, short maxY, short minZ, short maxZ)
{
    n_assert(!this->inQuery);
    n_assert(this->IsValid());
    n_assert((maxX < this->queryTable.Width()) && (maxZ < this->queryTable.Height()))

    MapQueryTableEntry newEntry;
    newEntry.Set(instIndex, minX, maxX, minY, maxY, minZ, maxZ);
    this->entries.Append(newEntry);

    IndexT z;
    for (z = minZ; z <= maxZ; z++)
    {
        short x;
        for (x = minX; x <= maxX; x++)
        {
            this->queryTable.At(x, z).Insert(this->entries.Size() - 1, this->entries);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
MapQueryTable::BeginQuery() const
{
    n_assert(!this->inQuery);
    this->inQuery = true;
    this->queryKey++;
}

//------------------------------------------------------------------------------
/**
*/
void
MapQueryTable::EndQuery() const
{
    n_assert(this->inQuery);
    this->inQuery = false;
}

//------------------------------------------------------------------------------
/**
*/
SizeT
MapQueryTable::QueryCell(short x, short minY, short maxY, short z, Util::Array<IndexT>& outResult) const
{
    n_assert(this->inQuery);
    return this->queryTable.At(x, z).Query(this->queryKey, minY, maxY, this->entries, outResult);
}

//------------------------------------------------------------------------------
/**
*/
SizeT
MapQueryTable::QueryBox(short minX, short maxX, short minY, short maxY, short minZ, short maxZ, Util::Array<IndexT>& outResult) const
{
    n_assert(this->inQuery);

    // iterate over x->z area
    SizeT numMatches = 0;
    short z;
    for (z = minZ; z <= maxZ; z++)
    {
        short x;
        for (x = minX; x <= maxX; x++)
        {
            numMatches += this->queryTable.At(x, z).Query(this->queryKey, minY, maxY, this->entries, outResult);
        }
    }
    return numMatches;
}

//------------------------------------------------------------------------------
/**
*/
SizeT
MapQueryTable::QueryArea(short minX, short maxX, short minZ, short maxZ, Util::Array<IndexT>& outResult) const
{
    n_assert(this->inQuery);
    
    // iterate over x->z area
    SizeT numMatches = 0;
    short z;
    for (z = minZ; z <= maxZ; z++)
    {
        short x;
        for (x = minX; x <= maxX; x++)
        {
            numMatches += this->queryTable.At(x, z).Query(this->queryKey, -1000, +1000, this->entries, outResult);
        }
    }
    return numMatches;
}

} // namespace Map