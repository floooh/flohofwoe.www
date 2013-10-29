//------------------------------------------------------------------------------
//  terrainapplication.cc
//  (C) 2012 A.Weissflog
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "terrainapplication.h"
#include "graphics2/graphicsentity.h"
#include "graphics2/graphicsstage.h"
#include "input/keyboard.h"
#include "input/touchpad.h"

namespace App
{
using namespace Math;
using namespace Util;
using namespace Input;
using namespace Graphics2;
using namespace Terrain;

//------------------------------------------------------------------------------
/**
 */
TerrainApplication::TerrainApplication()
{
    // empty
}

//------------------------------------------------------------------------------
/**
 */
void
TerrainApplication::OnOpening()
{
    EMSCTestApplication::OnOpening();

    // setup terrain system
    this->terrainFacade = CdlodTerrainFacade::Create();
    this->terrainFacade->Setup();
    
    this->terrainDesc.dirName             = "westeros";
    this->terrainDesc.shader              = "terrain";
    this->terrainDesc.modelType           = "DecalReceiveSolid";
    this->terrainDesc.mapName             = "map";
    this->terrainDesc.mapExtension        = ".hm";
    this->terrainDesc.texExtension        = ".crn";
    this->terrainDesc.diffMap             = "";
    this->terrainDesc.detailMap           = "structure_01_color.dds";
    this->terrainDesc.worldSize           = 1000.0f;
    this->terrainDesc.worldPixelSize      = 61.035f;
    this->terrainDesc.heightRange         = 4000.0f;
    this->terrainDesc.waterLine           = 300.0f;
    this->terrainDesc.viewDistance        = 1000.0f;
    this->terrainDesc.lodDistRatio        = 1.5f;
    this->terrainDesc.morphStart          = 0.66f;
    this->terrainDesc.structScale         = 5000.0f;
    this->terrainDesc.structBlendDistance = 20.0f;
    this->terrainDesc.oceanHeight         = 300.0f;
    this->terrainDesc.heightMapLods       = 9;
    this->terrainDesc.heightMapPixels     = 32;
    this->terrainDesc.diffMapLods         = 9;
    this->terrainDesc.diffMapPixels       = 256;
    this->terrainDesc.offsetMinX          = 0;
    this->terrainDesc.offsetMaxX          = 128;
    this->terrainDesc.offsetMinY          = 0;
    this->terrainDesc.offsetMaxY          = 256;
    this->terrainDesc.sortByDistance      = true;
    this->terrainDesc.castShadow          = false;
    this->terrainDesc.Initialize();
    this->terrainFacade->LoadWorld(this->terrainDesc);
    
    // setup the camera fov
    const Ptr<GraphicsEntity>& cameraEntity = this->graphicsFacade->GetDefaultCamera();
    const float fov = n_deg2rad(60.0f);
    const float zNear = 1.0f;
    const float zFar = 1000.0f;
    Shared::CameraSettings camSettings = cameraEntity->Camera()->GetCameraSettings();
    camSettings.SetupPerspectiveFov(fov, camSettings.GetAspect(), zNear, zFar);
    cameraEntity->Camera()->SetCameraSettings(camSettings);
    
    // setup the camera position
    point centerOfInterest(256.0f, 10.0f, 500.0f);
    point eyePos(240.0f, 12.0f, 510.0f);
    this->mayaCameraUtil.Setup(centerOfInterest, eyePos, vector::upvec());
}

//------------------------------------------------------------------------------
/**
 */
void
TerrainApplication::OnClosing()
{
    this->terrainFacade->UnloadWorld();
    EMSCTestApplication::OnClosing();
}

//------------------------------------------------------------------------------
/**
 */
void
TerrainApplication::SetupLightEntities()
{
    matrix44 globalLightTransform = matrix44::rotationx(n_deg2rad(-45.0f));
    globalLightTransform = matrix44::multiply(globalLightTransform, matrix44::rotationy(n_deg2rad(-135.0f)));
    this->globalLight = this->graphicsFacade->CreateGlobalLightEntity();
    this->globalLight->Transform()->SetTransform(globalLightTransform);
    this->globalLight->Lighting()->Light().SetColor(float4(1.0f, 1.0f, 1.0f, 1.0f));
    this->globalLight->Lighting()->Light().SetBackColor(float4(0.2f, 0.2f, 0.2f, 1.0f));
    this->globalLight->Lighting()->Light().SetAmbientColor(float4(0.0f, 0.0f, 0.0f, 1.0f));
    this->globalLight->Lighting()->Light().SetSpecularIntensity(1.0f);
    this->globalLight->Lighting()->Light().SetCastShadows(false);
    this->graphicsFacade->GetDefaultStage()->AttachEntity(this->globalLight);
}

//------------------------------------------------------------------------------
/**
 */
void
TerrainApplication::SetupModelEntities()
{
    // FIXME
}

//------------------------------------------------------------------------------
/**
 */
void
TerrainApplication::HandleInput()
{
    // FIXME
    EMSCTestApplication::HandleInput();
}

//------------------------------------------------------------------------------
/**
 */
void
TerrainApplication::UpdateScene()
{
    EMSCTestApplication::UpdateScene();
    
    // feed camera attributes into terrain renderer
    const Ptr<GraphicsEntity> camera = this->graphicsFacade->GetDefaultCamera();
    point cameraPos = camera->Transform()->GetTransform().get_position();
    matrix44 viewProj = camera->Camera()->GetViewProjTransform();
    this->terrainFacade->UpdateViewer(cameraPos, viewProj);
}

    
} // namespace App
