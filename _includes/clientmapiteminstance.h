#pragma once
//------------------------------------------------------------------------------
/**
    @class Map::ClientMapItemInstance

    Represents an actual map item instance.

    (C) 2010 Bigpoint GmbH
*/
#include "math/matrix44.h"
#include "util/stringatom.h"
#include "math/bbox.h"

//------------------------------------------------------------------------------
namespace Map
{
class ClientMapItemInstance
{
public:
    /// constructor
    ClientMapItemInstance();

    /// setup the instance
    void Setup(const Math::matrix44& m, const Math::bbox& box, IndexT templateIndex, IndexT groupIndex, const Util::StringAtom& editName, bool useCollide, const Util::Dictionary<Util::StringAtom, bool>& eventBits);
    /// discard the instance
    void Discard();
    /// return true if instance has been setup
    bool IsValid() const;

    /// return true if edit mode is enabled (editName is valid)
    bool IsEditMode() const;

    /// set transform matrix (only allowed in edit-mode)
    void SetTransform(const Math::matrix44& m);
    /// get transform matrix
    const Math::matrix44& GetTransform() const;
    /// set bounding box (only allowed in edit-mode)
    void SetBoundingBox(const Math::bbox& box);
    /// get bounding box
    const Math::bbox& GetBoundingBox() const;
    /// get template index
    IndexT GetTemplateIndex() const;
    /// get group index (InvalidIndex if not in a group)
    IndexT GetGroupIndex() const;
    /// get (optional) edit name
    const Util::StringAtom& GetEditName() const;
    /// get use collide flag
    bool GetUseCollideFlag() const;
    /// get eventbits
    const Util::Dictionary<Util::StringAtom, bool>& GetEventBits() const;
    /// check if instance is visible in event
    bool IsVisibleForEvent(const Util::Array<Util::StringAtom>& eventIds) const;

private:
    /// update the instance bounding box
    void UpdateBoundingBox();

    Math::matrix44 transform;
    Math::bbox box;
    IndexT templIndex;
    IndexT groupIndex;
    Util::StringAtom editName;
    bool useCollide;
    bool isEventDefault;
    Util::Dictionary<Util::StringAtom, bool> eventBits; 
};

//------------------------------------------------------------------------------
/**
*/
inline bool
ClientMapItemInstance::IsValid() const
{
    return (InvalidIndex != this->templIndex);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
ClientMapItemInstance::IsEditMode() const
{
    return this->editName.IsValid();
}

//------------------------------------------------------------------------------
/**
*/
inline void
ClientMapItemInstance::SetTransform(const Math::matrix44& m)
{
    n_assert(this->IsValid() && this->IsEditMode());
    this->transform = m;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
ClientMapItemInstance::GetTransform() const
{
    return this->transform;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ClientMapItemInstance::SetBoundingBox(const Math::bbox& b)
{
    n_assert(this->IsValid() && this->IsEditMode());
    this->box = b;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::bbox&
ClientMapItemInstance::GetBoundingBox() const
{
    return this->box;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
ClientMapItemInstance::GetTemplateIndex() const
{
    return this->templIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
ClientMapItemInstance::GetGroupIndex() const
{
    return this->groupIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
ClientMapItemInstance::GetEditName() const
{
    return this->editName;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
ClientMapItemInstance::GetUseCollideFlag() const
{
    return this->useCollide;
}

//------------------------------------------------------------------------------
/**
*/
inline 
const Util::Dictionary<Util::StringAtom, bool>& 
ClientMapItemInstance::GetEventBits() const
{
    return this->eventBits;
}

//------------------------------------------------------------------------------
/** 
    this methode assumes that if an event is found in the bit list,
     the others need not to be checked cause the set should be disjunct
     
    FIXME FIXME FIXME: actually really handle this through an bitfield !!!
*/
inline bool 
ClientMapItemInstance::IsVisibleForEvent(const Util::Array<Util::StringAtom>& eventIds) const
{
    IndexT i;
    for (i = 0; i < eventIds.Size(); i++)
    {
        //assuming that if an event is found in eventBits,
        //others events need not to be checked
        // sets should be disjunct
        IndexT index = this->eventBits.FindIndex(eventIds[i]);
        if (index != InvalidIndex)
        {
            return this->eventBits.ValueAtIndex(index);
        }
    }
    return this->isEventDefault;
}

} // namespace Map
//------------------------------------------------------------------------------

