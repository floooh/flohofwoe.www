//------------------------------------------------------------------------------
//  clientmapiteminstance.cc
//  (C) 2010 Bigpoint GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "clientmapiteminstance.h"

namespace Map
{
using namespace Util;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
ClientMapItemInstance::ClientMapItemInstance() :
    transform(matrix44::identity()),
    templIndex(InvalidIndex),
    groupIndex(InvalidIndex),
    useCollide(true),
    isEventDefault(true)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
ClientMapItemInstance::Setup(const matrix44& m, const bbox& boundingBox, IndexT templIndex_, IndexT groupIndex_, const StringAtom& editName_, bool useCollide_, const Util::Dictionary<StringAtom, bool>& eventBits_)
{
    n_assert(!this->IsValid());
    n_assert(InvalidIndex != templIndex_);
    n_assert(InvalidIndex == this->groupIndex);

    this->transform = m;
    this->box = boundingBox;
    this->templIndex = templIndex_;
    this->groupIndex = groupIndex_;
    this->editName   = editName_;
    this->useCollide = useCollide_;
    this->eventBits = eventBits_;
    // check if any event value is true, that means it belongs to an event
    this->isEventDefault = true;
    IndexT i;
    for (i = 0; i < this->eventBits.Size(); i++)
    {
        if (this->eventBits.ValueAtIndex(i))
        {
            this->isEventDefault = false;
            break;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
ClientMapItemInstance::Discard()
{
    n_assert(this->IsValid());
    this->templIndex = InvalidIndex;
    this->groupIndex = InvalidIndex;
    this->eventBits.Clear();
}

} // namespace Map