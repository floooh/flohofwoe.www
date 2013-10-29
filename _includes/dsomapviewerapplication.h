#pragma once
//------------------------------------------------------------------------------
/**
    @class App::DSOMapViewerApplication
  
    Drakensang Online map viewer demo app.
 
    (C) 2013 A.Weissflog
*/
#include "emsctest/base/emsctestapplication.h"
#include "graphics2/graphicsentity.h"
#include "math/pfeedbackloop.h"
#include "clientmap.h"
#include "physics/physicsserver.h"
#include "physics/charentity.h"
#include "physics/level.h"
#include "tabledata/tablebundle.h"

//------------------------------------------------------------------------------
namespace App
{
class DSOMapViewerApplication : public EMSCTestApplication
{
public:
    /// constructor
    DSOMapViewerApplication();
    
protected:
    /// called to setup the file preloading queue
    virtual void OnSetupPreloadQueue();
    /// called during opening phase
    virtual void OnOpening();
    /// called during closing phase
    virtual void OnClosing();
    /// setup light entities
    virtual void SetupLightEntities();
    /// per-frame update of scene
    virtual void UpdateScene();
    /// reset the camera
    virtual void ResetCamera();
    /// handle input
    virtual void HandleInput();
    /// clear the scene (discard all created graphics entities)
    virtual void ClearScene();    

    /// setup a new map
    void SetupMap(const Util::String& mapName);
    /// discard current map
    void DiscardMap();
    /// load the next map
//    void LoadNextMap();
    /// open the map's table bundle
    void OpenTableBundle(const Util::String& mapName);
    /// close the current table bundle
    void CloseTableBundle();
    /// update point-of-interest map coords
    void UpdatePointOfInterestMapCoords();
    /// update the graphics world
    void UpdateGraphicsWorld();
    /// get point of interest in map coordinates
    void GetPointOfInterestAsMapCoords(short& outPosX, short& outPosY, short& outPosZ) const;
    /// update the camera transform
    void UpdateCameraTransform();
    /// lookup the map entry point from the game database
//    void LookupEntryPointFromDb(const Util::String& mapName);
    /// lookup the default ambience bubble from the game database
    void LookupDefaultAmbienceBubble(const Util::String& mapName);
    /// lookup point lights from the game database
    void LookupPointLights(const Util::String& mapName);
    /// lookup npcs in map
    void LookupNPCs(const Util::String& mapName);
    /// preload collide meshes for a map
    void PreloadTemplateCollides(const Util::StringAtom& mapName);
    /// setup the collide world for current map
    void SetupCollideWorld();
    /// discard the collide world for current map
    void DiscardCollideWorld();
    /// setup character physics entity at given position
    Ptr<Physics::CharEntity> CreatePhysicsEntity(const Math::point& pos);
    /// setup the player object
    void SetupPlayer();
    /// discard the player object
    void DiscardPlayer();
    /// update player object
    void UpdatePlayer();
    /// update player's animation (called from UpdatePlayer)
    void UpdatePlayerAnimation(bool walking);
    /// update current world-space touch position
    void UpdateTouchPosition();

    static const short AreaOfInterestExtentsMinX = 3; // 3;
    static const short AreaOfInterestExtentsMaxX = 5; // 8;
    static const short AreaOfInterestExtentsMinZ = 5; // 7;
    static const short AreaOfInterestExtentsMaxZ = 3; // 4;
    
    Ptr<TableData::TableBundle> tableBundle;
    Util::Array<Util::String> mapNames;
    IndexT curMapIndex;
    Math::matrix44 rot180;
    Ptr<Physics::PhysicsServer> physicsServer;
    Ptr<Physics::Level> physicsLevel;
    Ptr<Physics::MeshShape> worldCollideMeshShape;
    Physics::FilterSet stabbingFilterSet;
    Ptr<Graphics2::GraphicsEntity> cameraEntity;
    Ptr<Graphics2::GraphicsEntity> globalLight;
    Util::Array<Ptr<Graphics2::GraphicsEntity> > pointLights;
    Util::Array<Ptr<Graphics2::GraphicsEntity> > npcs;
    Ptr<Map::ClientMap> map;
    Util::Array<IndexT> queryNewInstances;
    Math::point pointOfInterest;
    Math::point targetPosition;
    
    Ptr<Graphics2::GraphicsEntity> playerModelEntity;
    Ptr<Physics::CharEntity> playerPhysicsEntity;
    Math::PFeedbackLoop<Math::float4> smoothedPointOfInterest;
    
    short curPoiX;
    short curPoiZ;
    short prevPoiX;
    short prevPoiZ;
    float camDistance;
    bool touchDown;
};
    
} // namespace App
//------------------------------------------------------------------------------
