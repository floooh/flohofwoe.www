#pragma once
//------------------------------------------------------------------------------
/**
    @class Map::MapQueryTableStack

    A stack in the 2.5D map query table.

    (C) 2010 Bigpoint GmbH
*/
#include "util/array.h"
#include "mapquerytableentry.h"

//------------------------------------------------------------------------------
namespace Map
{
class MapQueryTableStack
{
public:
    /// sort an element into the stack
    void Insert(IndexT entryIndex, const Util::Array<MapQueryTableEntry>& entries);
    /// clear the stack
    void Clear();
    /// get stack size
    SizeT Size() const;
    /// get all elements withing Y-range and append to result array
    SizeT Query(IndexT queryKey, short rangeMinY, short rangeMaxY, const Util::Array<MapQueryTableEntry>& entries, Util::Array<IndexT>& outResult) const;
    /// get access to entry indices array
    const Util::Array<IndexT>& GetEntryIndices() const;

private:
    Util::Array<IndexT> minArray;
};

//------------------------------------------------------------------------------
/**
*/
inline void
MapQueryTableStack::Clear()
{
    this->minArray.Clear();
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
MapQueryTableStack::Size() const
{
    return this->minArray.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<IndexT>&
MapQueryTableStack::GetEntryIndices() const
{
    return this->minArray;
}

//------------------------------------------------------------------------------
/**
*/
inline void
MapQueryTableStack::Insert(IndexT entryIndex, const Util::Array<MapQueryTableEntry>& entries)
{
    short minY = entries[entryIndex].GetMinY();
    // short maxY = entries[entryIndex].GetMaxY();

    // linear search will do since there won't be many elements per stack
    IndexT insertIndex;
    for (insertIndex = 0; insertIndex < minArray.Size(); insertIndex++)
    {
        if (minY < entries[this->minArray[insertIndex]].GetMinY())
        {
            break;
        }
    }
    this->minArray.Insert(insertIndex, entryIndex);
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
MapQueryTableStack::Query(IndexT queryKey, short rangeMinY, short rangeMaxY, const Util::Array<MapQueryTableEntry>& entries, Util::Array<IndexT>& outResult) const
{
    SizeT numMatches = 0;
    IndexT i;
    for (i = 0; i < this->minArray.Size(); i++)
    {
        IndexT entryIndex = this->minArray[i];
        MapQueryTableEntry& curEntry = entries[entryIndex];
        short entryMinY = curEntry.GetMinY();

        // stop if we're out of range max
        if (entryMinY > rangeMaxY)
        {
            break;
        }

        // check if current entry was already collected in this query
        if (curEntry.GetQueryKey() != queryKey)
        {
            short entryMaxY = curEntry.GetMaxY();
            if ((entryMaxY > rangeMinY) && (entryMinY < rangeMaxY))
            {
                curEntry.SetQueryKey(queryKey);
                outResult.Append(entryIndex);
                numMatches++;
            }        
        }
    }
    return numMatches;
}

} // namespace Map
//------------------------------------------------------------------------------
