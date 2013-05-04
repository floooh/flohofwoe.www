//------------------------------------------------------------------------------
//  dsomapviewerapplication.cc
//  (C) 2012 A.Weissflog
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "dsomapviewerapplication.h"
#include "graphics2/graphicsstage.h"
#include "graphics2/graphics2protocol.h"
#include "clientmaploader.h"
#include "input/mouse.h"
#include "input/keyboard.h"
#include "physics/meshcache.h"
#include "physics/physicsserver.h"
#include "physics/level.h"
#include "physics/charentity.h"
#include "renderutil/mouserayutil.h"
#include "io/packagestreamreader.h"
#include "grounddecals/grounddecalsplugin.h"
#include "tabledata/binarytablereader.h"

namespace App
{
using namespace Graphics2;
using namespace CoreGraphics2;
using namespace Math;
using namespace Util;
using namespace Map;
using namespace Shared;
using namespace Timing;
using namespace Input;
using namespace Physics;
using namespace IO;
using namespace TableData;

//------------------------------------------------------------------------------
/**
*/
DSOMapViewerApplication::DSOMapViewerApplication() :
    curMapIndex(0),
    curPoiX(-1),
    curPoiZ(-1),
    prevPoiX(-1),
    prevPoiZ(-1),
    camDistance(25.0f),
    touchDown(false)
{
    this->mapNames.Append("a0200_kingscity");
    this->mapNames.Append("a0502_burningcoast");
    this->mapNames.Append("a0102_peatmoor");
    this->mapNames.Append("a0103_pilgrimscrypt");
    this->mapNames.Append("a0401_oldchapel");
    this->mapNames.Append("a0101_moorvillage");
    //this->mapNames.Append("a0201_basicarena");
    //this->mapNames.Append("a0300_citypastures");
    //this->mapNames.Append("a0301_witchhole");
    //this->mapNames.Append("a0302_wildforest");
    //this->mapNames.Append("a0303_wildcave");
    //this->mapNames.Append("a0400_deadlands");
    this->mapNames.Append("a0402_culttower01");
    this->mapNames.Append("a0403_culttower02");
    this->mapNames.Append("a0404_culttower03");
    //this->mapNames.Append("a0501_princecamp");
}

//------------------------------------------------------------------------------
/**
 */
void
DSOMapViewerApplication::OnSetupPreloadQueue()
{
    this->AddPreloadFile("export:maps/a0200_kingscity.map");
    this->AddPreloadFile("export:maps/a0200_kingscity.col");
    this->AddPreloadFile("export:maps/a0200_kingscity.tbl");
    EMSCTestApplication::OnSetupPreloadQueue();
}

//------------------------------------------------------------------------------
/**
 */
void
DSOMapViewerApplication::OnOpening()
{
    EMSCTestApplication::OnOpening();

    // create PhysicsServer
    this->physicsServer = Physics::PhysicsServer::Create();
    this->physicsServer->Open();
    
    // setup stabbing check filter set
    Physics::FilterSet filterSet;
    filterSet.AddMaterialType(Physics::MaterialTable::StringToMaterialType("Character"));
    this->stabbingFilterSet = filterSet;
    
    // setup the map object
    this->SetupMap(this->mapNames[0]);
    Time time = CentralTime::Instance()->GetTime();
    this->smoothedPointOfInterest.Reset(time, 0.005f, -1.0f, this->pointOfInterest);
    
    // setup the camera fov
    this->cameraEntity = this->graphicsFacade->GetDefaultCamera();
    const float fov = n_deg2rad(25.0f);
    const float zNear = 1.0f;
    const float zFar = 150.0f;
    CameraSettings camSettings = this->cameraEntity->Camera()->GetCameraSettings();
    camSettings.SetupPerspectiveFov(fov, camSettings.GetAspect(), zNear, zFar);
    this->cameraEntity->Camera()->SetCameraSettings(camSettings);
    this->UpdateCameraTransform();
    
    // setup ground decals plugin
    this->graphicsFacade->AttachPlugin("grounddecals", GroundDecals::GroundDecalsPlugin::Create());
}

//------------------------------------------------------------------------------
/**
 */
void
DSOMapViewerApplication::OnClosing()
{
    this->DiscardMap();
    
    this->CloseTableBundle();
    
    this->physicsServer->Close();
    this->physicsServer = 0;

    EMSCTestApplication::OnClosing();
}
//------------------------------------------------------------------------------
/**
 */
void
DSOMapViewerApplication::UpdateScene()
{
    // update physics world
    this->physicsServer->SetTime(Timing::CentralTime::Instance()->GetTime());
    this->physicsServer->Trigger();
    
    // update player
    this->UpdatePlayer();
    
    // update camera from new point of interest
    this->UpdateCameraTransform();
    
    this->UpdatePointOfInterestMapCoords();
    if ((this->curPoiX != this->prevPoiX) || (this->curPoiZ != this->prevPoiZ))
    {
        this->UpdateGraphicsWorld();
    }
}

//------------------------------------------------------------------------------
/**
 */
void
DSOMapViewerApplication::SetupLightEntities()
{
    // setup a global light
    matrix44 globalLightTransform = matrix44::rotationx(n_deg2rad(-45.0f));
    globalLightTransform = matrix44::multiply(globalLightTransform, matrix44::rotationy(n_deg2rad(0.0f)));
    this->globalLight = this->graphicsFacade->CreateGlobalLightEntity();
    this->globalLight->Transform()->SetTransform(globalLightTransform);
    this->globalLight->Lighting()->Light().SetColor(float4(0.8f, 0.8f, 0.8f, 1.0f));
    this->globalLight->Lighting()->Light().SetBackColor(float4(0.3f, 0.3f, 0.3f, 1.0f));
    this->globalLight->Lighting()->Light().SetAmbientColor(float4(0.0f, 0.0f, 0.0f, 1.0f));
    this->globalLight->Lighting()->Light().SetSpecularIntensity(1.0f);
    this->globalLight->Lighting()->Light().SetCastShadows(false);
    this->graphicsFacade->GetDefaultStage()->AttachEntity(this->globalLight);
}

//------------------------------------------------------------------------------
/**
 */
void
DSOMapViewerApplication::ResetCamera()
{
    // FIXME!
}

//------------------------------------------------------------------------------
/**
 */
void
DSOMapViewerApplication::HandleInput()
{
    InputServer* inputServer = InputServer::Instance();
    const Ptr<Mouse>& mouse = inputServer->GetDefaultMouse();
    this->touchDown = false;

    // update the target position
    if (mouse->ButtonPressed(MouseButton::LeftButton))
    {
        // get mouse ray in world space
        float2 mousePos = mouse->GetScreenPosition();
        const CameraSettings& camSettings = this->cameraEntity->Camera()->GetCameraSettings();
        const matrix44& camTransform = this->cameraEntity->Transform()->GetTransform();
        line worldRay = RenderUtil::MouseRayUtil::ComputeWorldMouseRay(mousePos, 50.0f, camTransform, camSettings.GetInvProjTransform(), camSettings.GetZNear(), false);
        
        // perform stabbing check
        const ContactPoint* contact = this->physicsServer->GetClosestContactUnderMouse(worldRay, this->stabbingFilterSet);
        if (contact)
        {
            this->targetPosition = contact->GetPosition();
            this->touchDown = true;
        }
        
        /*
        n_printf("CLICK: m=(%.3f,%.3f), l=(%.3f,%.3f,%.3f->%.3f,%.3f,%.3f), c=(%.3f,%.3f,%.3f)\n",
                 mousePos.x(), mousePos.y(),
                 worldRay.start().x(), worldRay.start().y(), worldRay.start().z(),
                 worldRay.end().x(), worldRay.end().y(), worldRay.end().z(),
                 this->targetPosition.x(), this->targetPosition.y(), this->targetPosition.z());
        */
    }
}

//------------------------------------------------------------------------------
/**
 */
void
DSOMapViewerApplication::SetupMap(const String& mapName)
{
    n_assert(!this->map.isvalid());
    n_dbgout("-> SetupMap(%s)\n", mapName.AsCharPtr());

    this->OpenTableBundle(mapName);

    this->physicsLevel = Physics::Level::Create();
    this->physicsLevel->SetStepSize(0.02f);
    this->physicsServer->SetLevel(this->physicsLevel);
    
//    this->LookupEntryPointFromDb(mapName);
    this->LookupDefaultAmbienceBubble(mapName);
    this->LookupPointLights(mapName);
    this->LookupNPCs(mapName);
    this->PreloadTemplateCollides(mapName);
    
    String mapPath;
    mapPath.Format("export:maps/%s.map", mapName.AsCharPtr());
    
    this->map = ClientMap::Create();
    ClientMapLoader mapLoader;
    if (!mapLoader.Load(mapName, mapPath, this->map))
    {
        n_error("Failed to load map '%s' from path '%s'!\n", mapName.AsCharPtr(), mapPath.AsCharPtr());
    }
    this->map->LoadTemplateCollides();
    this->SetupCollideWorld();
    this->map->UnloadTemplateCollides();
    this->queryNewInstances.Reserve(512);

    this->pointOfInterest = this->map->GetBoundingBox().center();
    this->targetPosition = this->pointOfInterest;
    
    this->SetupPlayer();
    
    // attach point lights to graphics world
    n_dbgout("SetupMap(): %d point lights, %d NPCs\n", this->pointLights.Size(), this->npcs.Size());
    IndexT i;
    for (i = 0; i < this->pointLights.Size(); i++)
    {
        this->graphicsFacade->GetDefaultStage()->AttachEntity(this->pointLights[i]);
    }    
}

//------------------------------------------------------------------------------
/**
 */
void
DSOMapViewerApplication::GetPointOfInterestAsMapCoords(short& outMapX, short& outMapY, short& outMapZ) const
{
    return this->map->ConvertWorldToGridCoords(this->pointOfInterest, outMapX, outMapY, outMapZ);
}

//------------------------------------------------------------------------------
/**
 Update the integer point-of-interest map coordinates.
 */
void
DSOMapViewerApplication::UpdatePointOfInterestMapCoords()
{
    this->prevPoiX = this->curPoiX;
    this->prevPoiZ = this->curPoiZ;
    short dummyY;
    this->GetPointOfInterestAsMapCoords(this->curPoiX, dummyY, this->curPoiZ);
}
    
//------------------------------------------------------------------------------
/**
 */
void
DSOMapViewerApplication::DiscardMap()
{
    n_assert(this->map.isvalid());
    this->DiscardPlayer();
    this->DiscardCollideWorld();
    this->map->Discard();
    this->map = 0;
    this->physicsServer->SetLevel(0);
    this->physicsLevel = 0;
}

//------------------------------------------------------------------------------
/**
    Preloads template collides from collide bundle file into
    physics mesh cache. Called by client and server map manager.
 */
void
DSOMapViewerApplication::PreloadTemplateCollides(const StringAtom& mapName)
{
    // open collide bundle path with PackageStreamReader
    String collideBundlePath;
    collideBundlePath.Format("export:maps/%s.col", mapName.Value());
    Ptr<Stream> stream = IoServer::Instance()->CreateStream(collideBundlePath);
    Ptr<PackageStreamReader> reader = PackageStreamReader::Create();
    reader->SetStream(stream);
    reader->SetHeaderFourCC('MAPC');
    reader->SetHeaderVersion(1);
    if (reader->Open())
    {
        String fragmentName;
        Ptr<Stream> fragmentStream;
        do
        {
            fragmentStream = reader->ReadNextStreamData(fragmentName);
            if (fragmentStream.isvalid())
            {
                MeshCache::Instance()->NewMesh(fragmentName, fragmentStream);
            }
        }
        while (fragmentStream.isvalid());
        reader->Close();
    }
    else
    {
        n_error("ClientMapManager: failed to open collide bundle file '%s'!\n", collideBundlePath.AsCharPtr());
    }
}

//------------------------------------------------------------------------------
/**
 Setup the collide world.
 */
void
DSOMapViewerApplication::SetupCollideWorld()
{
    n_assert(!this->worldCollideMeshShape.isvalid());

    // count required number of vertices and indices
    int numVertices = 0;
    int numIndices = 0;
    int vertexWidth = 0;
    IndexT instIndex;
    SizeT numInsts = this->map->GetNumInstances();
    for (instIndex = 0; instIndex < numInsts; instIndex++)
    {
        const ClientMapItemInstance& inst = this->map->InstanceByIndex(instIndex);
        const ClientMapItemTemplate& templ = this->map->TemplateByIndex(inst.GetTemplateIndex());
        if (templ.HasCollide())
        {
            // skip instance if its inside a visibility group
            IndexT grpIndex = inst.GetGroupIndex();
            bool considerInstance = true;
            if (grpIndex != InvalidIndex)
            {
                ClientMapGroup& group = this->map->GroupByIndex(inst.GetGroupIndex());
                considerInstance = (group.GetType() != ClientMapGroup::VisibilityGroup);
            }
            
            if (considerInstance && templ.IsCollideLoaded())
            {
                const Ptr<PhysicsMesh>& physMesh = templ.GetCollideMesh()->GetMesh();
                numVertices += physMesh->GetGroupNumVertices(templ.GetCollideMeshGroupIndex());
                numIndices  += physMesh->GetGroupNumIndices(templ.GetCollideMeshGroupIndex());
                if (0 == vertexWidth)
                {
                    vertexWidth = physMesh->GetVertexWidth();
                }
                else
                {
                    n_assert(vertexWidth == physMesh->GetVertexWidth());
                }
            }
        }
    }
    
    // check if the level actually has a collide
    if (0 == numVertices)
    {
        n_error("Map '%s' has no collide data!\n", this->map->GetName().Value());
    }
    
    // for each instance with collide, create a mesh shape and add it to the
    // physics collide world
    Ptr<PhysicsMesh> collideWorldMesh = PhysicsMesh::Create();
    collideWorldMesh->BeginAppendMeshes(numVertices, numIndices, vertexWidth);
    for (instIndex = 0; instIndex < numInsts; instIndex++)
    {
        const ClientMapItemInstance& inst = this->map->InstanceByIndex(instIndex);
        const ClientMapItemTemplate& templ = this->map->TemplateByIndex(inst.GetTemplateIndex());
        if (templ.HasCollide())
        {
            // skip instance if its inside a visibility group
            IndexT grpIndex = inst.GetGroupIndex();
            bool considerInstance = true;
            if (grpIndex != InvalidIndex)
            {
                ClientMapGroup& group = this->map->GroupByIndex(inst.GetGroupIndex());
                considerInstance = (group.GetType() != ClientMapGroup::VisibilityGroup);
            }
            
            if (considerInstance && templ.IsCollideLoaded())
            {
                const Ptr<PhysicsMesh>& physMesh = templ.GetCollideMesh()->GetMesh();
                collideWorldMesh->AppendMesh(
                                             physMesh->GetGroupVertexPointer(templ.GetCollideMeshGroupIndex()),
                                             physMesh->GetGroupNumVertices(templ.GetCollideMeshGroupIndex()),
                                             physMesh->GetGroupIndexPointer(templ.GetCollideMeshGroupIndex()),
                                             physMesh->GetGroupNumIndices(templ.GetCollideMeshGroupIndex()),
                                             inst.GetTransform());
            }
        }
    }
    collideWorldMesh->EndAppendMeshes();
    
    // finally, create a MeshShape and attach it to the world
    Physics::Level* physicsLevel = PhysicsServer::Instance()->GetLevel();
    n_assert(physicsLevel);
    
    this->worldCollideMeshShape = MeshShape::Create();
    this->worldCollideMeshShape->SetMesh(collideWorldMesh);
    this->worldCollideMeshShape->SetMeshGroupIndex(0);
    this->worldCollideMeshShape->SetCategory(ShapeCategory::EnvCollide);
    this->worldCollideMeshShape->SetCategoryBits(Physics::Environment);
    this->worldCollideMeshShape->SetCollideBits(Physics::CollideBitsByCategory(Physics::Environment));
    this->worldCollideMeshShape->Setup();
    physicsLevel->AttachStaticShape(this->worldCollideMeshShape);
}

//------------------------------------------------------------------------------
/**
    Discard the global collide mesh.
 */
void
DSOMapViewerApplication::DiscardCollideWorld()
{
    if (this->worldCollideMeshShape.isvalid())
    {
        // physics level might already have been cleaned up
        Physics::Level* physicsLevel = Physics::PhysicsServer::Instance()->GetLevel();
        if (0 != physicsLevel)
        {
            physicsLevel->RemoveStaticShape(this->worldCollideMeshShape);
        }
        if (this->worldCollideMeshShape->IsValid())
        {
            this->worldCollideMeshShape->Discard();
        }
        this->worldCollideMeshShape = 0;
    }
}

//------------------------------------------------------------------------------
/**
 */
void
DSOMapViewerApplication::SetupPlayer()
{
    this->playerModelEntity = this->CreateCharacter("mdl:characters/undead_ripper.n3", "undead_ripper_01", "combat_idle_01", this->pointOfInterest);
    this->playerPhysicsEntity = this->CreatePhysicsEntity(this->pointOfInterest);
}

//------------------------------------------------------------------------------
/**
 */
void
DSOMapViewerApplication::DiscardPlayer()
{
    this->graphicsFacade->GetDefaultStage()->RemoveEntity(this->playerModelEntity.cast<GraphicsEntity>());
    this->playerModelEntity = 0;
    this->physicsLevel->RemoveEntity(this->playerPhysicsEntity);
    this->playerPhysicsEntity = 0;
}

//------------------------------------------------------------------------------
/**
 */
void
DSOMapViewerApplication::UpdatePlayer()
{
    // get current transform from physics entity
    matrix44 curTransform = this->playerPhysicsEntity->GetTransform();
    
    // update point of interest
//    this->pointOfInterest = curTransform.get_position();
    
    // update player model entity transform
    this->playerModelEntity->Transform()->SetTransform(curTransform);
    
    // compute new desired velocity vector
    const float velocity = 5.0f;
    vector dir = this->targetPosition - point(curTransform.get_position());
    dir.set_y(0.0f);
    if (dir.length() > 1.0f)
    {
        this->playerPhysicsEntity->SetDesiredLookat(dir);
        dir = vector::normalize(dir);
        vector desiredVelocity = dir * velocity;
        if (this->touchDown)
        {
            this->playerPhysicsEntity->SetDesiredVelocity(desiredVelocity);
            this->UpdatePlayerAnimation(true);
        }
    }
    else
    {
        this->playerPhysicsEntity->SetDesiredVelocity(vector::nullvec());
        this->UpdatePlayerAnimation(false);
    }
}

//------------------------------------------------------------------------------
/**
 */
void
DSOMapViewerApplication::UpdatePlayerAnimation(bool walking)
{
    // start or stop walking animation as needed
    if (walking)
    {
        Ptr<AnimPlayClip> playClip = AnimPlayClip::Create();
        playClip->SetClipName("run_01");
        playClip->SetTrackIndex(1);
        playClip->SetLoopCount(0);
        playClip->SetEnqueueMode(Animation::AnimJobEnqueueMode::IgnoreIfSameClipActive);
        this->playerModelEntity->Character()->PushMessage(playClip.cast<CharacterMessage>());
    }
    else 
    {
        Ptr<AnimStopTrack> stopTrack = AnimStopTrack::Create();
        stopTrack->SetTrackIndex(1);
        this->playerModelEntity->Character()->PushMessage(stopTrack.cast<CharacterMessage>());
    }
}

//------------------------------------------------------------------------------
/**
 */
Ptr<Physics::CharEntity>
DSOMapViewerApplication::CreatePhysicsEntity(const point& pos)
{
    matrix44 tform = matrix44::identity();
    tform.set_position(pos);
    Ptr<Physics::CharEntity> physicsEntity = Physics::CharEntity::Create();
    physicsEntity->SetRadius(0.5f);
    physicsEntity->SetHeight(2.0f);
    physicsEntity->SetHover(0.25f);
    physicsEntity->SetTransform(tform);
    this->physicsLevel->AttachEntity(physicsEntity);
    return physicsEntity;
}
    
//------------------------------------------------------------------------------
/**
 */
void
DSOMapViewerApplication::UpdateCameraTransform()
{
    const float angleY = n_deg2rad(-45.0f);
    const float angleX = n_deg2rad(-45.0f);
    
    // implement landscape/portait rotation
    const float angleZ = 0.0f;
    const float offsetX = 0.0f;
    const float offsetY = 0.0f;
    
    // perform position and heading smoothing
    this->smoothedPointOfInterest.SetGoal(this->targetPosition);
    Time time = CentralTime::Instance()->GetTime();
    this->smoothedPointOfInterest.Update(time);
    
    this->pointOfInterest = this->smoothedPointOfInterest.GetState();
    Math::matrix44 cameraTransform = Math::matrix44::rotationyawpitchroll(angleY, angleX, angleZ);
    cameraTransform.set_position(this->pointOfInterest);
    
    Math::matrix44 rotY = Math::matrix44::rotationy(angleY);
    float4 x = rotY.get_xaxis() * offsetX;
    float4 z = rotY.get_zaxis() * offsetY;
    cameraTransform.set_position(cameraTransform.get_position() + x + z);
    
    float motion = this->camDistance;
    float4 moveDir = cameraTransform.get_zaxis();
    moveDir = float4::normalize(moveDir);
    moveDir *= motion;
    
    cameraTransform.translate(moveDir);
    this->cameraEntity->Transform()->SetTransform(cameraTransform);
    
    this->graphicsFacade->SetPointOfInterest(this->pointOfInterest);
}

//------------------------------------------------------------------------------
/**
    Make sure that the area around the player is populated with
    graphics objects.
 */
void
DSOMapViewerApplication::UpdateGraphicsWorld()
{
    const MapQueryTable& queryTable = this->map->GetQueryTable();
    
    // clear query arrays
    this->queryNewInstances.Clear();
    
    // if no point of interest has been set yet, don't do anything
    if ((-1 != this->curPoiX) && (-1 != this->curPoiZ))
    {
        short curMinX = queryTable.ClampX(this->curPoiX - AreaOfInterestExtentsMinX);
        short curMaxX = queryTable.ClampX(this->curPoiX + AreaOfInterestExtentsMaxX);
        short curMinZ = queryTable.ClampZ(this->curPoiZ - AreaOfInterestExtentsMinZ);
        short curMaxZ = queryTable.ClampZ(this->curPoiZ + AreaOfInterestExtentsMaxZ);
        
        // tell templates that we're about to add visible instances
        IndexT templIndex;
        const SizeT numTempls = this->map->GetNumTemplates();
        for (templIndex = 0; templIndex < numTempls; templIndex++)
        {
            this->map->TemplateByIndex(templIndex).BeginVisibleInstances();
        }
        
        // perform a query on the query table
        queryTable.BeginQuery();
        queryTable.QueryArea(curMinX, curMaxX, curMinZ, curMaxZ, this->queryNewInstances);
        queryTable.EndQuery();
        
        // add visible instances to templates
        IndexT newIndex;
        SizeT numNew = this->queryNewInstances.Size();
        for (newIndex = 0; newIndex < numNew; newIndex++)
        {
            IndexT instIndex = this->queryNewInstances[newIndex];
            ClientMapItemInstance& inst = this->map->InstanceByIndex(instIndex);
            ClientMapItemTemplate& templ = this->map->TemplateByIndex(inst.GetTemplateIndex());
            templ.AddVisibleInstance(inst.GetTransform(), instIndex);
        }
        
        // finish adding visible instances (will setup the templates ModelEntity for instanced rendering)
        for (templIndex = 0; templIndex < numTempls; templIndex++)
        {
            this->map->TemplateByIndex(templIndex).EndVisibleInstances();
        }
    }
    else
    {
        n_warning("UpdateGraphicsWorld: no valid point of interest!\n");
    }
}

//------------------------------------------------------------------------------
/**
 */
void
DSOMapViewerApplication::OpenTableBundle(const String& mapName)
{
    if (this->tableBundle.isvalid())
    {
        this->CloseTableBundle();
    }
    
    String path;
    path.Format("export:maps/%s.tbl", mapName.AsCharPtr());
    this->tableBundle = TableBundle::Create();
    this->tableBundle->Setup();
    Ptr<BinaryTableReader> tableReader = BinaryTableReader::Create();
    if (!tableReader->ReadTables(path, this->tableBundle))
    {
        n_error("Failed to load table bundle file '%s'\n", path.AsCharPtr());
    }
}

//------------------------------------------------------------------------------
/**
 */
void
DSOMapViewerApplication::CloseTableBundle()
{
    if (this->tableBundle.isvalid())
    {
        if (this->tableBundle->IsValid())
        {
            this->tableBundle->Discard();
        }
        this->tableBundle = 0;
    }
}

//------------------------------------------------------------------------------
/**
 */
void
DSOMapViewerApplication::LookupPointLights(const String& mapName)
{
    n_assert(this->tableBundle);
    Ptr<Table> table = this->tableBundle->LookupTable("_Instance_Light");
    n_assert(table.isvalid());
    
    IndexT colLightColor = table->LookupColumnIndex("LightColor");
    IndexT colLightIntensity = table->LookupColumnIndex("LightIntensity");
    IndexT colLightRange = table->LookupColumnIndex("LightRange");
    IndexT colTransform = table->LookupColumnIndex("Transform");
    
    const vector downVec(0.0f, -1.0f, 0.0f);
    IndexT rowIndex;
    SizeT numRows = table->GetNumRows();
    for (rowIndex = 0; rowIndex < numRows; rowIndex++)
    {
        float4 lightColor    = table->GetRef<float4>(colLightColor, rowIndex);
        float lightIntensity = table->GetValue<float>(colLightIntensity, rowIndex);
        float lightRange     = table->GetValue<float>(colLightRange, rowIndex);
        matrix44 tform       = table->GetRef<matrix44>(colTransform, rowIndex);
        
        Ptr<GraphicsEntity> light = this->graphicsFacade->CreatePointLightEntity();
        light->Lighting()->SetTransformFromPosDirAndRange(tform.get_position(), downVec, lightRange);
        light->Lighting()->Light().SetColor(lightColor * lightIntensity);
        light->Lighting()->Light().SetSpecularIntensity(0.0f);
        this->pointLights.Append(light);        
    }
}

//------------------------------------------------------------------------------
/**
 */
void
DSOMapViewerApplication::LookupDefaultAmbienceBubble(const String& mapName)
{
    n_assert(this->tableBundle);
    CoreGraphics2Facade* cg2Facade = CoreGraphics2Facade::Instance();    
    Ptr<Table> table = this->tableBundle->LookupTable("_Instance_AmbienceBubble");
    n_assert(table.isvalid());
    StringAtom strName = "DefaultAmbienceBubble";
    IndexT colName                = table->LookupColumnIndex("Name");
    IndexT colLightColor          = table->LookupColumnIndex("LightColor");
    IndexT colLightOppositeColor  = table->LookupColumnIndex("LightOppositeColor");
    IndexT colLightIntensity      = table->LookupColumnIndex("LightIntensity");
    IndexT colLightAmbient        = table->LookupColumnIndex("LightAmbient");
    IndexT colTransform           = table->LookupColumnIndex("Transform");
    IndexT colBrightPassThreshold = table->LookupColumnIndex("BrightPassThreshold");
    IndexT colBloomColor          = table->LookupColumnIndex("BloomColor");
    IndexT colBloomScale          = table->LookupColumnIndex("BloomScale");
    IndexT colBalance             = table->LookupColumnIndex("Balance");
    IndexT colSaturation          = table->LookupColumnIndex("Saturation");
    IndexT rowIndex;
    SizeT numRows = table->GetNumRows();
    for (rowIndex = 0; rowIndex < numRows; rowIndex++)
    {
        if (table->GetRef<StringAtom>(colName, rowIndex) == strName)
        {
            float4 lightColor         = table->GetRef<float4>(colLightColor, rowIndex);
            float4 lightBackColor     = table->GetRef<float4>(colLightOppositeColor, rowIndex);
            float lightIntensity      = table->GetValue<float>(colLightIntensity, rowIndex);
            float4 lightAmbientColor  = table->GetRef<float4>(colLightAmbient, rowIndex);
            matrix44 tform            = table->GetRef<matrix44>(colTransform, rowIndex);
            float brightPassThreshold = table->GetValue<float>(colBrightPassThreshold, rowIndex);
            float4 bloomColor         = table->GetRef<float4>(colBloomColor, rowIndex);
            float bloomScale          = table->GetValue<float>(colBloomScale, rowIndex);
            float4 balance            = table->GetRef<float4>(colBalance, rowIndex);
            float saturation          = table->GetValue<float>(colSaturation, rowIndex);
            
            this->globalLight->Transform()->SetTransform(tform);
            this->globalLight->Lighting()->Light().SetColor(lightColor * lightIntensity);
            this->globalLight->Lighting()->Light().SetBackColor(lightBackColor);
            this->globalLight->Lighting()->Light().SetAmbientColor(lightAmbientColor);
            
            cg2Facade->ApplySharedVariable("HdrBrightPassThreshold", brightPassThreshold);
            cg2Facade->ApplySharedVariable("HdrBloomColor", bloomColor);
            cg2Facade->ApplySharedVariable("HdrBloomScale", bloomScale);
            cg2Facade->ApplySharedVariable("Balance", balance);
            cg2Facade->ApplySharedVariable("Saturation", saturation);
            
            break;
        }
    }
}

//------------------------------------------------------------------------------
/**
 */
void
DSOMapViewerApplication::LookupNPCs(const String& mapName)
{
    n_assert(this->tableBundle);
    Ptr<Table> table = this->tableBundle->LookupTable("_Instance_NPC");
    n_assert(table.isvalid());
    
    IndexT colTransform = table->LookupColumnIndex("Transform");
    IndexT rowIndex;
    SizeT numRows = table->GetNumRows();
    for (rowIndex = 0; rowIndex < numRows; rowIndex++)
    {
        matrix44 tform = table->GetRef<matrix44>(colTransform, rowIndex);
        Ptr<GraphicsEntity> npc = this->CreateCharacter("mdl:characters/soulless_norseman.n3", "soulless_norseman", "combat_idle_01", tform.get_position());
        npc->Transform()->SetTransform(tform);
        this->npcs.Append(npc);
    }
}

} // namespace App
