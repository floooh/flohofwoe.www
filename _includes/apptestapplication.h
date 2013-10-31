#pragma once
//------------------------------------------------------------------------------
/**
    @class App::AppTestApplication

    Test the cleaned up application layer stuff.

    (C) 2013 Bigpoint GmbH
*/
#include "appgame/gameapplication.h"
#include "basegamefeatureunit.h"
#include "graphicsfeatureunit.h"
#include "physicsfeatureunit.h"
#include "mapfeature/mapfeatureunit.h"
#include "cdlodterrain/featureunit/cdlodterrainfeatureunit.h"
#include "appteststatehandler.h"

//------------------------------------------------------------------------------
namespace App
{
class AppTestApplication : public GameApplication
{
public:
    /// constructor
    AppTestApplication();
    /// destructor
    virtual ~AppTestApplication();

    /// setup entity types on the factory (replaces the old blueprints.xml)
    virtual void SetupEntityTypes();
    /// setup game features
    virtual void SetupGameFeatures();
    /// cleanup game features
    virtual void CleanupGameFeatures();
    /// setup application state handlers
    virtual void SetupStateHandlers();
    /// setup instances for a new level
    virtual bool SetupInstances(const Util::StringAtom& levelName);

private:
    Ptr<BaseGameFeature::BaseGameFeatureUnit> baseGameFeature;
    Ptr<GraphicsFeature::GraphicsFeatureUnit> graphicsFeature;
    Ptr<PhysicsFeature::PhysicsFeatureUnit> physicsFeature;
    Ptr<TerrainFeature::CdlodTerrainFeatureUnit> terrainFeature;
    Ptr<Map::MapFeatureUnit> mapFeature;
    Ptr<AppTestStateHandler> appTestStateHandler;
};

} // namespace AppTestApplication
//------------------------------------------------------------------------------

