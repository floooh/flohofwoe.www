#pragma once
//------------------------------------------------------------------------------
/**
    @class Map::ClientMapGroup

    A client-map group object. Groups may contain instances and other groups. 
    Instances and groups may only be part of one parent group.

    (C) 2010 Bigpoint GmbH
*/
#include "util/stringatom.h"
#include "util/array.h"

//------------------------------------------------------------------------------
namespace Map
{
class ClientMapGroup
{
public:
    /// group types
    enum Type
    {
        VisibilityGroup = 0,
        VariationGroup,

        InvalidType,
    };

    /// constructor
    ClientMapGroup();

    /// setup the group object
    void Setup(const Util::StringAtom& name, Type type, IndexT parentGroupIndex, const Util::Array<IndexT>& instIndices, const Util::Array<IndexT>& groupIndices);
    /// discard the group object
    void Discard();
    /// return true if group object has been setup
    bool IsValid() const;

    /// get group name
    const Util::StringAtom& GetName() const;
    /// get group type
    Type GetType() const;
    /// get parent group index (invalid index if no parent group)
    IndexT GetParentGroupIndex() const;
    /// get child instance indices
    const Util::Array<IndexT>& GetInstanceIndices() const;
    /// get child group indices
    const Util::Array<IndexT>& GetGroupIndices() const;
    /// add instances to group
    void AddInstanceIndices(const Util::Array<IndexT>& additionalInstances);

private:
    Util::StringAtom groupName;
    Type type;
    IndexT parentGroupIndex;
    Util::Array<IndexT> instanceIndices;
    Util::Array<IndexT> groupIndices;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
ClientMapGroup::IsValid() const
{
    return this->groupName.IsValid();
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
ClientMapGroup::GetName() const
{
    return this->groupName;
}

//------------------------------------------------------------------------------
/**
*/
inline ClientMapGroup::Type
ClientMapGroup::GetType() const
{
    return this->type;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
ClientMapGroup::GetParentGroupIndex() const
{
    return this->parentGroupIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<IndexT>&
ClientMapGroup::GetInstanceIndices() const
{
    return this->instanceIndices;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
ClientMapGroup::AddInstanceIndices(const Util::Array<IndexT>& additionalInstances) 
{
    return this->instanceIndices.AppendArray(additionalInstances);
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<IndexT>&
ClientMapGroup::GetGroupIndices() const
{
    return this->groupIndices;
}

} // namespace Map
//------------------------------------------------------------------------------
