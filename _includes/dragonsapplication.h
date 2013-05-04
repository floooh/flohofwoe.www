#pragma once
//------------------------------------------------------------------------------
/**
    @class App::DragonsApplication
    
    Character rendering test/benchmark app.
    
    (C) 2012 A.Weissflog
*/
#include "emsctest/base/emsctestapplication.h"

//------------------------------------------------------------------------------
namespace App
{
class DragonsApplication : public EMSCTestApplication
{
public:
    /// constructor
    DragonsApplication();

protected:
    /// handle input
    virtual void HandleInput();
    /// setup light entities
    virtual void SetupLightEntities();    
    /// create model entities
    virtual void SetupModelEntities();
    /// reset the camera
    virtual void ResetCamera();

    int numEntitiesAlongAxis;
    Ptr<Graphics2::GraphicsEntity> globalLight;
    Ptr<Graphics2::GraphicsEntity> pointLight0;
    Ptr<Graphics2::GraphicsEntity> pointLight1;
    Util::Array<Ptr<Graphics2::GraphicsEntity> > modelEntities;
};

} // namespace App
//------------------------------------------------------------------------------
