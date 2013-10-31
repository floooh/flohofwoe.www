//------------------------------------------------------------------------------
//  apptestapplication.cc
//  (C) 2013 A.Weissflog
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "apptestapplication.h"
#include "game/managers/datamanager.h"
#include "game/dataproviders/dbdataprovider.h"
#include "properties/lightproperty.h"
#include "properties/inputproperty.h"
#include "properties/graphicsproperty.h"
#include "properties/ambiencebubbleproperty.h"
#include "properties/physicsproperty.h"
#include "cdlodterrain/featureunit/cdlodterrainproperty.h"
#include "cdlodterrain/featureunit/cdlodterraincameraproperty.h"

namespace App
{
using namespace Game;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
AppTestApplication::AppTestApplication()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
AppTestApplication::~AppTestApplication()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
AppTestApplication::SetupEntityTypes()
{
    DataManager* dataManager = DataManager::Instance();
    
    // first load templates and global attributes from a data provider
    Ptr<DbDataProvider> dataProvider = DbDataProvider::Create();
    dataProvider->Begin("export:db/static.db4");
    dataProvider->LoadTemplates();
    dataProvider->LoadGlobalAttributes();
    dataProvider->End();

    // --- setup properties
    
    // Light entity
    dataManager->BeginCategory("Light");
    dataManager->AddProperty(GraphicsFeature::LightProperty::RTTI);
    dataManager->EndCategory();

    // a Camera entity which acts like a Maya camera
    dataManager->BeginCategory("Camera");
    dataManager->AddProperty(TerrainFeature::CdlodTerrainCameraProperty::RTTI);
    dataManager->AddProperty(GraphicsFeature::InputProperty::RTTI);
    dataManager->EndCategory();

    // a simple renderable entity, immobile entity
    dataManager->BeginCategory("Simple");
    dataManager->AddProperty(GraphicsFeature::GraphicsProperty::RTTI);
    dataManager->EndCategory();

    // the terrain entity
    dataManager->BeginCategory("Terrain");
    dataManager->AddProperty(TerrainFeature::CdlodTerrainProperty::RTTI);
    dataManager->EndCategory();
    
    // ambience bubble entity
    dataManager->BeginCategory("AmbienceBubble");
    dataManager->AddProperty(BaseGameFeature::TransformableProperty::RTTI);
    dataManager->AddProperty(GraphicsFeature::AmbienceBubbleProperty::RTTI);
    dataManager->EndCategory();
}

//------------------------------------------------------------------------------
/**
*/
void
AppTestApplication::SetupGameFeatures()
{
    // setup the BaseGameFeature
    this->baseGameFeature = BaseGameFeature::BaseGameFeatureUnit::Create();
    this->baseGameFeature->SetRenderDebug(true);
    this->gameSystem->AttachGameFeature(this->baseGameFeature.upcast<Game::FeatureUnit>());

    // setup the terrain feature (must be before GraphicsFeature, so that terrain
    // has uptodate camera position before rendering happens
    this->terrainFeature = TerrainFeature::CdlodTerrainFeatureUnit::Create();
    this->gameSystem->AttachGameFeature(this->terrainFeature.upcast<Game::FeatureUnit>());

    // setup the map feature
    this->mapFeature = Map::MapFeatureUnit::Create();
    this->mapFeature->SetRenderDebug(true);
    this->gameSystem->AttachGameFeature(this->mapFeature.upcast<Game::FeatureUnit>());

    // setup the GraphicsFeature
    this->graphicsFeature = GraphicsFeature::GraphicsFeatureUnit::Create();
    this->graphicsFeature->SetDefaultDisplayWidth(888);
    this->graphicsFeature->SetDefaultDisplayHeight(544);
    this->graphicsFeature->SetDefaultWindowTitle("Drakensang Online");
    this->graphicsFeature->SetCmdLineArgs(this->GetCmdLineArgs());
    this->graphicsFeature->SetRenderDebug(true);
    this->gameSystem->AttachGameFeature(this->graphicsFeature.cast<Game::FeatureUnit>());

    // setup the physics feature
    this->physicsFeature = PhysicsFeature::PhysicsFeatureUnit::Create();
    this->physicsFeature->SetRenderDebug(true);
    this->gameSystem->AttachGameFeature(this->physicsFeature.upcast<Game::FeatureUnit>());
}

//------------------------------------------------------------------------------
/**
*/
void
AppTestApplication::CleanupGameFeatures()
{
    this->gameSystem->RemoveGameFeature(this->terrainFeature.upcast<Game::FeatureUnit>());
    this->terrainFeature = 0;

    this->gameSystem->RemoveGameFeature(this->mapFeature.upcast<Game::FeatureUnit>());
    this->mapFeature = 0;

    this->gameSystem->RemoveGameFeature(this->physicsFeature.upcast<Game::FeatureUnit>());
    this->physicsFeature = 0;

    this->gameSystem->RemoveGameFeature(this->graphicsFeature.upcast<Game::FeatureUnit>());
    this->graphicsFeature = 0;

    this->gameSystem->RemoveGameFeature(this->baseGameFeature.upcast<Game::FeatureUnit>());
    this->baseGameFeature = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
AppTestApplication::SetupStateHandlers()
{
    this->appTestStateHandler = AppTestStateHandler::Create();
    this->appTestStateHandler->SetName("Test");
    this->appTestStateHandler->SetLevelName("got");
    this->AddStateHandler(this->appTestStateHandler.upcast<StateHandler>());
    this->RequestState("Test");
}

//------------------------------------------------------------------------------
/**
 */
bool
AppTestApplication::SetupInstances(const StringAtom& levelName)
{
    if (levelName.IsValid())
    {
        String path;
        path.Format("export:maps/%s.db4", levelName.Value());
        Ptr<DbDataProvider> dataProvider = DbDataProvider::Create();
        dataProvider->Begin(path);
        dataProvider->LoadInstances();
        dataProvider->End();
    }
    return true;
}

} // namespace App
