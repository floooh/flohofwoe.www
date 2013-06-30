//------------------------------------------------------------------------------
//  clientmap.cc
//  (C) 2010 Bigpoint GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "clientmap.h"

namespace Map
{
using namespace Math;
using namespace Util;

__ImplementClass(Map::ClientMap, 'CMAP', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
ClientMap::ClientMap() :
    gridSize(vector::nullvec()),
    mapSizeX(0),
    mapSizeY(0),
    mapSizeZ(0),
    isValid(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ClientMap::~ClientMap()
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
ClientMap::Setup(const StringAtom& name, const vector& gridSize_, const bbox& mapBox_, short sizeX_, short sizeY_, short sizeZ_)
{
    n_assert(!this->IsValid());

    this->mapName = name;
    this->gridSize = gridSize_;
    this->oneDivGridSize = vector(1.0f / this->gridSize.x(), 1.0f / this->gridSize.y(), 1.0f / this->gridSize.z());
    this->box = mapBox_;
    this->mapSizeX = sizeX_;
    this->mapSizeY = sizeY_;
    this->mapSizeZ = sizeZ_;
    this->queryTable.Setup(this->mapSizeX, this->mapSizeZ);
    this->isValid = true;
}

//------------------------------------------------------------------------------
/**
*/
void
ClientMap::Discard()
{
    n_assert(this->IsValid());

    this->UnloadTemplateCollides();

    IndexT i;
    for (i = 0; i < this->templates.Size(); i++)
    {
        this->templates[i].Discard();
    }
    this->templates.Clear();

    for (i = 0; i < this->instances.Size(); i++)
    {
        this->instances[i].Discard();
    }
    this->instances.Clear();

    for (i = 0; i < this->groups.Size(); i++)
    {
        this->groups[i].Discard();
    }
    this->groups.Clear();

    this->isValid = false;
}

//------------------------------------------------------------------------------
/**
*/
void
ClientMap::ReserveTemplates(SizeT num)
{
    n_assert(this->IsValid());
    this->templates.Reserve(num);
    this->templateIndexMap.Reserve(num);
}

//------------------------------------------------------------------------------
/**
*/
void
ClientMap::AddTemplate(const StringAtom& collideResId, IndexT collMeshGroupIndex, const bbox& box, ClientMapItemTemplate::Type type)
{
    n_assert(this->IsValid());
    n_assert(!this->templateIndexMap.Contains(collideResId));
    ClientMapItemTemplate newTempl;
    this->templates.Append(newTempl);
    this->templates.Back().Setup("", collideResId, collMeshGroupIndex, box, type);
}

//------------------------------------------------------------------------------
/**
*/
void
ClientMap::AddTemplate(const StringAtom& gfxResId, const StringAtom& collideResId, IndexT collMeshGroupIndex, const bbox& box, ClientMapItemTemplate::Type type)
{
    n_assert(this->IsValid());
    n_assert(!this->templateIndexMap.Contains(gfxResId));
    ClientMapItemTemplate newTempl;
    this->templates.Append(newTempl);
    this->templates.Back().Setup(gfxResId, collideResId, collMeshGroupIndex, box, type);
    this->templateIndexMap.Add(gfxResId, this->templates.Size() - 1);
}

//------------------------------------------------------------------------------
/**
    Add a template that uses a subnode inside the model file associated with
    gfxResId.
*/
void
ClientMap::AddTemplate(
    const StringAtom& gfxResId, 
    const StringAtom& rootNodePath,
    const StringAtom& phxResId, 
    const StringAtom& collideResId, 
    IndexT collMeshGroupIndex, 
    const bbox& box, 
    ClientMapItemTemplate::Type type,
    const StringAtom& sfxEventId)
{
    n_assert(this->IsValid());
    n_assert(!this->templateIndexMap.Contains(gfxResId.AsString() + rootNodePath.AsString()));
    ClientMapItemTemplate newTempl;
    this->templates.Append(newTempl);
    this->templates.Back().Setup(gfxResId, rootNodePath, phxResId, collideResId, collMeshGroupIndex, box, type, sfxEventId);
    this->templateIndexMap.Add(gfxResId.AsString() + rootNodePath.AsString(), this->templates.Size() - 1);
}

//------------------------------------------------------------------------------
/**
*/
IndexT
ClientMap::LookupTemplateIndex(const StringAtom& gfxResId) const
{
    n_assert(this->IsValid());
    IndexT i = this->templateIndexMap.FindIndex(gfxResId);
    if (InvalidIndex != i)
    {
        return this->templateIndexMap.ValueAtIndex(i);
    }
    else
    {
        return InvalidIndex;
    }
}

//------------------------------------------------------------------------------
/**
*/
IndexT
ClientMap::LookupTemplateIndex(const Util::StringAtom& gfxResId, const Util::StringAtom& rootNodePath) const
{
    n_assert(this->IsValid());
    IndexT i = this->templateIndexMap.FindIndex(gfxResId.AsString() + rootNodePath.AsString());
    if (InvalidIndex != i)
    {
        return this->templateIndexMap.ValueAtIndex(i);
    }
    else
    {
        return InvalidIndex;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
ClientMap::ReserveInstances(SizeT num)
{
    n_assert(this->IsValid());
    this->instances.Reserve(num);
    this->instanceIndexMap.Reserve(num);
    this->queryTable.ReserveEntries(num);
}

//------------------------------------------------------------------------------
/**
*/
void
ClientMap::AddInstance(const matrix44& m, const bbox& box, IndexT templIndex, IndexT groupIndex, const StringAtom& editName, bool useCollide, const Util::Dictionary<Util::StringAtom, bool>& eventBits)
{
    n_assert(this->IsValid());
    ClientMapItemInstance newInst;
    this->instances.Append(newInst);
    this->instances.Back().Setup(m, box, templIndex, groupIndex, editName, useCollide, eventBits);
    if (editName.IsValid())
    {
        n_assert(!this->instanceIndexMap.Contains(editName));
        this->instanceIndexMap.Add(editName, this->instances.Size() - 1);
    }

    // add the instance to the query table
    short minX, minY, minZ, maxX, maxY, maxZ;
    this->ConvertWorldToGridCoords(box.pmin(), minX, minY, minZ);
    this->ConvertWorldToGridCoords(box.pmax(), maxX, maxY, maxZ);
    this->queryTable.AddEntry(this->instances.Size() - 1, minX, maxX, minY, maxY, minZ, maxZ);
}

//------------------------------------------------------------------------------
/**
*/
IndexT
ClientMap::LookupInstanceIndex(const StringAtom& editName) const
{
    n_assert(this->IsValid());
    IndexT i = this->instanceIndexMap.FindIndex(editName);
    if (InvalidIndex != i)
    {
        return this->instanceIndexMap.ValueAtIndex(i);
    }
    else
    {
        return InvalidIndex;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
ClientMap::EditInstance(IndexT instanceIndex, IndexT templateIndex)
{
    n_assert(0 <= instanceIndex && instanceIndex < this->instances.Size());
    n_assert(0 <= templateIndex && templateIndex < this->templates.Size());
    ClientMapItemInstance& inst = this->instances[instanceIndex];
    this->InstanceByIndex(instanceIndex).Discard();
    this->InstanceByIndex(instanceIndex).Setup(inst.GetTransform(), inst.GetBoundingBox(), templateIndex, inst.GetGroupIndex(), inst.GetEditName(), inst.GetUseCollideFlag(), inst.GetEventBits());
}

//------------------------------------------------------------------------------
/**
*/
void
ClientMap::ReserveGroups(SizeT num)
{
    this->groups.Reserve(num);
    this->groupIndexMap.Reserve(num);
}

//------------------------------------------------------------------------------
/**
*/
void
ClientMap::AddGroup(const StringAtom& groupName, ClientMapGroup::Type type, IndexT parentGroupIndex, const Array<IndexT>& instIndices, const Array<IndexT>& groupIndices)
{
    n_assert(this->IsValid());
    n_assert(InvalidIndex == this->groupIndexMap.FindIndex(groupName));
    ClientMapGroup newGroup;
    this->groups.Append(newGroup);
    this->groups.Back().Setup(groupName, type, parentGroupIndex, instIndices, groupIndices);
    this->groupIndexMap.Add(groupName, this->groups.Size() - 1);
}

//------------------------------------------------------------------------------
/**
*/
void
ClientMap::UpdateGroup(IndexT groupIndex, const Array<IndexT>& instIndices)
{
    n_assert(this->IsValid());
    n_assert(groupIndex >= 0 && groupIndex < this->groups.Size());

    this->groups[groupIndex].AddInstanceIndices(instIndices);
}

//------------------------------------------------------------------------------
/**
*/
IndexT
ClientMap::LookupGroupIndex(const StringAtom& groupName) const
{
    n_assert(this->IsValid());
    IndexT i = this->groupIndexMap.FindIndex(groupName);
    if (InvalidIndex != i)
    {
        return this->groupIndexMap.ValueAtIndex(i);
    }
    else
    {
        return InvalidIndex;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
ClientMap::ConvertWorldToGridCoords(const float4& world, short& outMapX, short& outMapY, short& outMapZ)
{
    // shift to bounding box space
    vector shift = point::origin() - this->box.pmin();
    float4 v = float4::multiply(world + shift, this->oneDivGridSize);
    outMapX = n_clamp<short>(short(v.x()), 0, this->mapSizeX - 1);
    outMapY = n_clamp<short>(short(v.y()), 0, this->mapSizeY - 1);
    outMapZ = n_clamp<short>(short(v.z()), 0, this->mapSizeZ - 1);
}

//------------------------------------------------------------------------------
/**
*/
bool
ClientMap::LoadTemplateCollides()
{
    n_assert(this->IsValid());
    bool result = true;
    IndexT i;
    SizeT num = this->GetNumTemplates();
    for (i = 0; i < num; i++)
    {
        ClientMapItemTemplate& templ = this->TemplateByIndex(i);
        if (templ.HasCollide() && templ.GetCollideMeshGroupIndex() == 0)
        {
            result &= templ.LoadCollide();
        }
    }
    return result;
}

//------------------------------------------------------------------------------
/**
*/
void
ClientMap::UnloadTemplateCollides()
{
    n_assert(this->IsValid());
    IndexT i;
    SizeT num = this->GetNumTemplates();
    for (i = 0; i < num; i++)
    {
        this->TemplateByIndex(i).UnloadCollide();
    }
}

} // namespace Map