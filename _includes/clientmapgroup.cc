//------------------------------------------------------------------------------
//  clientmapgroup.cc
//  (C) 2010 Bigpoint GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "clientmapgroup.h"

namespace Map
{
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
ClientMapGroup::ClientMapGroup() :
    type(InvalidType)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
ClientMapGroup::Setup(const StringAtom& grpName, Type t, IndexT parentGrpIdx, const Array<IndexT>& instIndices, const Array<IndexT>& grpIndices)
{
    n_assert(!this->IsValid());
    n_assert(grpName.IsValid());

    this->groupName = grpName;
    this->type = t;
    this->parentGroupIndex = parentGrpIdx;
    this->instanceIndices = instIndices;
    this->groupIndices = grpIndices;
}

//------------------------------------------------------------------------------
/**
*/
void
ClientMapGroup::Discard()
{
    n_assert(this->IsValid());
    this->groupName.Clear();
}

} // namespace Map
