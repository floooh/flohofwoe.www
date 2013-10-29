#pragma once
//------------------------------------------------------------------------------
/**
    @class App::TerrainApplication
    
    Terrain test application.
 
    (C) 2013 Bigpoint GmbH
*/
#include "emsctest/base/emsctestapplication.h"
#include "cdlodterrain/cdlodterrainfacade.h"

//------------------------------------------------------------------------------
namespace App
{
class TerrainApplication : public EMSCTestApplication
{
public:
    /// constructor
    TerrainApplication();

    /// called during opening phase
    virtual void OnOpening();
    /// called during closing phase
    virtual void OnClosing();
    /// per-frame update of scene
    virtual void UpdateScene();

protected:
    /// handle input
    virtual void HandleInput();
    /// setup light entities
    virtual void SetupLightEntities();    
    /// create model entities
    virtual void SetupModelEntities();

    Ptr<Terrain::CdlodTerrainFacade> terrainFacade;
    Terrain::CdlodQuadTreeGameDescription terrainDesc;
    Ptr<Graphics2::GraphicsEntity> globalLight;
    Util::Array<Ptr<Graphics2::GraphicsEntity> > modelEntities;
};

} // namespace App
//------------------------------------------------------------------------------
