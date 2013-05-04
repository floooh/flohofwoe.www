//------------------------------------------------------------------------------
//  clientmapitemtemplate.cc
//  (C) 2010 Bigpoint GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "clientmapitemtemplate.h"
#include "graphics2/graphicsfacade.h"
#include "graphics2/graphicsstage.h"

namespace Map
{
using namespace Util;
using namespace Math;
using namespace Physics;
using namespace Graphics2;

//------------------------------------------------------------------------------
/**
*/
ClientMapItemTemplate::ClientMapItemTemplate() :
    type(InvalidType),
    collideMeshGroupIndex(InvalidIndex),
    isValid(false),
    inVisibleInstances(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ClientMapItemTemplate::~ClientMapItemTemplate()
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
ClientMapItemTemplate::Setup(const StringAtom& gfxResId, const StringAtom& collResId, IndexT collMeshGroupIndex, const bbox& b, Type t)
{
    n_assert(!this->IsValid());
    n_assert(!this->collideShape.isvalid());

    this->graphicsResourceId = gfxResId;
    this->collideResourceId = collResId;
    this->collideMeshGroupIndex = collMeshGroupIndex;
    this->box = b;
    this->type = t;

    this->isValid = true;
}

//------------------------------------------------------------------------------
/**
*/
void
ClientMapItemTemplate::Setup(
    const Util::StringAtom& gfxResId, 
    const Util::StringAtom& gfxRootNode,
    const Util::StringAtom& phxResId, 
    const Util::StringAtom& collideResId, 
    IndexT collMeshGroupIndex, 
    const Math::bbox& box, 
    Type type,
    const Util::StringAtom& sfxEventId)
{
    this->Setup(gfxResId, collideResId, collMeshGroupIndex, box, type);
    this->graphicsResourceRootNodePath = gfxRootNode;
    this->physicsResourceId = phxResId;
    this->soundEventId = sfxEventId;
}

//------------------------------------------------------------------------------
/**
*/
void
ClientMapItemTemplate::Discard()
{
    n_assert(this->IsValid());

    if (this->collideShape.isvalid())
    {
        this->UnloadCollide();
    }

    // discard model entity
    if (this->modelEntity.isvalid())
    {
        const Ptr<GraphicsStage>& stage = GraphicsFacade::Instance()->GetDefaultStage();
        if (stage.isvalid())
        {
            stage->RemoveEntity(this->modelEntity);
        }
        GraphicsFacade::Instance()->DiscardEntity(this->modelEntity);
        this->modelEntity = 0;
    }
    this->instanceTransforms.Clear();

    this->isValid = false;
}

//------------------------------------------------------------------------------
/**
*/
void
ClientMapItemTemplate::BeginVisibleInstances()
{
    n_assert(this->IsValid());
    n_assert(!this->inVisibleInstances);
    this->inVisibleInstances = true;
    this->instanceTransforms.Clear();
    this->instanceIndices.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
ClientMapItemTemplate::AddVisibleInstance(const matrix44& instTransform, IndexT instanceIndex)
{
    n_assert(this->inVisibleInstances);
    // two arrays for faster transform pass thru to modelentity
    this->instanceTransforms.Append(instTransform);
    this->instanceIndices.Append(instanceIndex);  
}

//------------------------------------------------------------------------------
/**
*/
void
ClientMapItemTemplate::EndVisibleInstances()
{
    n_assert(this->inVisibleInstances);
    this->inVisibleInstances = false;
    // update normal model entity, will create it if not happend, sends instance transform or hide the entity if it has no visible instances
    this->UpdateModelEntity();
}

//------------------------------------------------------------------------------
/**
*/
bool
ClientMapItemTemplate::LoadCollide()
{
    n_assert(!this->collideShape.isvalid());
    n_assert(this->collideResourceId.IsValid());

    this->collideShape = MeshShape::Create();
    this->collideShape->SetFilename(this->collideResourceId);
    this->collideShape->SetMeshGroupIndex(this->collideMeshGroupIndex);

    if (!this->collideShape->Setup())
    {
        n_error("ClientMapItemTemplate: failed to setup mesh collide shape '%s'!\n", this->collideResourceId.Value());
        this->collideShape = 0;
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
ClientMapItemTemplate::UnloadCollide()
{
    if (this->collideShape.isvalid())
    {
        this->collideShape->Discard();
        this->collideShape = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
ClientMapItemTemplate::BeginSwapVisibleInstances()
{
    n_assert(!this->inSwapVisibleInstance);
    this->inSwapVisibleInstance = true;
}

//------------------------------------------------------------------------------
/**
*/
void 
ClientMapItemTemplate::SwapVisibleInstance(IndexT instanceIndex, bool toFadeOut)
{
    n_assert(this->inSwapVisibleInstance);

    if (toFadeOut)
    {
        // swap index and transform from normal model entity instance arrays to fadeout arrays
        // first copy instance index and transform
        IndexT arrayIndex = this->instanceIndices.FindIndex(instanceIndex);
        n_assert(arrayIndex != InvalidIndex && arrayIndex  < this->instanceTransforms.Size());
        this->fadeOutInstanceTransforms.Append(this->instanceTransforms[arrayIndex]);
        this->fadeOutInstanceIndices.Append(instanceIndex);

        // then remove index and transform
        this->instanceIndices.EraseIndex(arrayIndex);
        this->instanceTransforms.EraseIndex(arrayIndex);
    }
    else
    {
        // other way round: swap index and transform from fadeout arrays to normal
        // first copy instance index and transform
        IndexT arrayIndex = this->fadeOutInstanceIndices.FindIndex(instanceIndex);
        n_assert(arrayIndex != InvalidIndex && arrayIndex  < this->fadeOutInstanceTransforms.Size());
        this->instanceTransforms.Append(this->fadeOutInstanceTransforms[arrayIndex]);
        this->instanceIndices.Append(instanceIndex);
        
        // then remove index and transform
        this->fadeOutInstanceIndices.EraseIndex(arrayIndex);
        this->fadeOutInstanceTransforms.EraseIndex(arrayIndex);
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
ClientMapItemTemplate::EndSwapVisibleInstances()
{
    n_assert(this->inSwapVisibleInstance);
    this->inSwapVisibleInstance = false;
    
    // update normal model entity, will create it if not happend, sends instance transform or hide the entity if it has no visible instances
    this->UpdateModelEntity();
}

//------------------------------------------------------------------------------
/**
*/
bool 
ClientMapItemTemplate::IsInSwapVisibleInstances() const
{
    return this->inSwapVisibleInstance;
}

//------------------------------------------------------------------------------
/**
*/
bool 
ClientMapItemTemplate::HasVisibleInstance(IndexT index)
{
    return (this->instanceIndices.FindIndex(index) != InvalidIndex);
}

//------------------------------------------------------------------------------
/**
*/
void 
ClientMapItemTemplate::UpdateModelEntity()
{
    if (this->instanceTransforms.Size() > 0)
    {
        // create model entity if not happened yet
        if (!this->modelEntity.isvalid())
        {
            String resPath;
            resPath.Format("mdl:%s.n3", this->graphicsResourceId.Value());

            this->modelEntity = GraphicsFacade::Instance()->CreateModelEntity();
            this->modelEntity->Model()->SetResourceId(resPath.AsCharPtr());
//            this->modelEntity->Model()->SetIoChannel(IO::IoChannel::Environment);     // FIXME!!!

            if (this->graphicsResourceRootNodePath.IsValid()) 
            {
                this->modelEntity->Model()->SetRootNodePath(this->graphicsResourceRootNodePath);
            }

            this->modelEntity->Visibility()->SetInstanceCullingEnabled(false);
            this->modelEntity->Transform()->SetInstancingEnabled(true);

            const Ptr<GraphicsStage>& stage = GraphicsFacade::Instance()->GetDefaultStage();
            stage->AttachEntity(this->modelEntity);
        }

        if (!this->modelEntity->Visibility()->IsVisible())
        {        
            this->modelEntity->Visibility()->SetVisible(true);
        }
        this->modelEntity->Transform()->SetInstanceTransforms(this->instanceTransforms);
    }
    else
    {
        // if no instances exist for this model entity, set it to invisible
        if (this->modelEntity.isvalid() && this->modelEntity->Visibility()->IsVisible())
        {
            this->modelEntity->Visibility()->SetVisible(false);
        }
    }
}

} // namespace Map

