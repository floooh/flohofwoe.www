#pragma once
//------------------------------------------------------------------------------
/**
    @class Map::ClientMapItemTemplate

    Describes a client-side map graphics object template.

    (C) 2011 Bigpoint GmbH
*/
#include "util/stringatom.h"
#include "math/bbox.h"
#include "physics/meshshape.h"
#include "graphics2/graphicsentity.h"
#include "util/stack.h"

//------------------------------------------------------------------------------
namespace Map
{
class ClientMapItemTemplate
{
public:
    /// template types
    enum Type
    {
        Tile = 1,
        Prop = 2,
        DestructibleActive = 3,
        DestructibleInactive = 4,
        DestructibleGoingInactive = 5,
        IndependentCollide = 6,

        InvalidType
    };

    /// constructor
    ClientMapItemTemplate();
    /// destructor
    ~ClientMapItemTemplate();

    /// setup the object
    void Setup(
        const Util::StringAtom& gfxResId,
        const Util::StringAtom& collideResId,
        IndexT collMeshGroupIndex,
        const Math::bbox& box, 
        Type type);
    /// setup the object
    void Setup(
        const Util::StringAtom& gfxResId,
        const Util::StringAtom& gfxRootNode,
        const Util::StringAtom& phxResId,
        const Util::StringAtom& collideResId,
        IndexT collMeshGroupIndex, 
        const Math::bbox& box, 
        Type type,
        const Util::StringAtom& sfxEventId);
    /// discard the object
    void Discard();
    /// return true if the object is valid
    bool IsValid() const;

    /// get graphics resource id
    const Util::StringAtom& GetGraphicsResourceId() const;
    /// get graphics root node path
    const Util::StringAtom& GetGraphicsRootNodePath() const;
    /// get optional collide resource id
    const Util::StringAtom& GetCollideResourceId() const;
    /// get optional physics resource id
    const Util::StringAtom& GetPhysicsResourceId() const;
    /// get sound event id
    const Util::StringAtom& GetSoundEventId() const;
    /// get colllide mesh group index
    IndexT GetCollideMeshGroupIndex() const;
    /// get bounding box
    const Math::bbox& GetBoundingBox() const;
    /// get template type (tile or prop)
    Type GetType() const;

    /// return true if the template has a graphics resource
    bool HasGraphics() const;
    /// return true if the model entity for this template has been created
    bool HasModelEntity() const;

    /// return true if the template has a collide resource
    bool HasCollide() const;
    /// return true if the collide resource has been loaded
    bool IsCollideLoaded() const;
    /// start loading the collide resource
    bool LoadCollide();
    /// unload collide resource
    void UnloadCollide();
    /// get pointer to the collide shape object
    const Ptr<Physics::MeshShape>& GetCollideMesh() const;

    /// begin adding visible instances
    void BeginVisibleInstances();
    /// add a visible instance
    void AddVisibleInstance(const Math::matrix44& instTransform, IndexT instanceIndex);
    /// finish adding visible instances, updates the model entity for instanced rendering
    void EndVisibleInstances();

    /// check if template has visible instance index
    bool HasVisibleInstance(IndexT index);
    /// begin remove of instances, needed for visibility group handling
    void BeginSwapVisibleInstances();
    /// remove instance transform
    void SwapVisibleInstance(IndexT instanceIndex, bool toFadeOut);
    /// end  remove of visible instances, will update modelentity transforms
    void EndSwapVisibleInstances();

    /// is inside instance removing
    bool IsInSwapVisibleInstances() const;

    /// return true if the template is ready to be rendered (all resources loaded)
    bool IsReadyForRendering() const;
    /// return true if the template is still streaming data
    bool IsStreaming() const;
    /// return true if type is one of the destructible types
    bool IsDestructible() const;

private:
    /// update normal model entity, will create it if not happend, sends instance transform or hide the entity if it has no visible instances
    void UpdateModelEntity();

    Util::StringAtom graphicsResourceId;
    Util::StringAtom graphicsResourceRootNodePath;
    Util::StringAtom collideResourceId;
    Util::StringAtom physicsResourceId;
    Util::StringAtom soundEventId;
    Math::bbox box;
    Type type;
    IndexT collideMeshGroupIndex;

    Ptr<Physics::MeshShape> collideShape;
    Ptr<Graphics2::GraphicsEntity> modelEntity;
    Util::Array<Math::matrix44> instanceTransforms;
    Util::Array<Math::matrix44> fadeOutInstanceTransforms;
    Util::Array<IndexT> instanceIndices;
    Util::Array<IndexT> fadeOutInstanceIndices;

    bool isValid;
    bool inVisibleInstances;
    bool inSwapVisibleInstance;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
ClientMapItemTemplate::IsValid() const
{
    return this->isValid;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
ClientMapItemTemplate::GetGraphicsResourceId() const
{
    return this->graphicsResourceId;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::StringAtom& 
ClientMapItemTemplate::GetGraphicsRootNodePath() const
{
    return this->graphicsResourceRootNodePath;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
ClientMapItemTemplate::GetCollideResourceId() const
{
    return this->collideResourceId;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
ClientMapItemTemplate::GetPhysicsResourceId() const
{
    return this->physicsResourceId;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
ClientMapItemTemplate::GetSoundEventId() const
{
    return this->soundEventId;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
ClientMapItemTemplate::GetCollideMeshGroupIndex() const
{
    return this->collideMeshGroupIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::bbox&
ClientMapItemTemplate::GetBoundingBox() const
{
    return this->box;
}

//------------------------------------------------------------------------------
/**
*/
inline ClientMapItemTemplate::Type
ClientMapItemTemplate::GetType() const
{
    return this->type;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
ClientMapItemTemplate::HasGraphics() const
{
    return this->graphicsResourceId.IsValid();
}

//------------------------------------------------------------------------------
/**
*/
inline bool
ClientMapItemTemplate::HasCollide() const
{
    return this->collideResourceId.IsValid();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Physics::MeshShape>&
ClientMapItemTemplate::GetCollideMesh() const
{
    return this->collideShape;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
ClientMapItemTemplate::HasModelEntity() const
{
    return this->modelEntity.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline bool
ClientMapItemTemplate::IsReadyForRendering() const
{
    if (this->modelEntity.isvalid())
    {
        return this->modelEntity->IsValidForRendering();
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
ClientMapItemTemplate::IsStreaming() const
{
    if (this->modelEntity.isvalid())
    {
        return !this->modelEntity->IsValidForRendering();
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
ClientMapItemTemplate::IsCollideLoaded() const
{
    return this->collideShape.isvalid();
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
ClientMapItemTemplate::IsDestructible() const
{
    return this->type == DestructibleActive || this->type == DestructibleInactive || this->type == DestructibleGoingInactive;
}

} // namespace Map
//------------------------------------------------------------------------------
