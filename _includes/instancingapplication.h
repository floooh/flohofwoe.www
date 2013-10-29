#pragma once
//------------------------------------------------------------------------------
/**
    @class App::InstancingApplication
 
    Instancing test/benchmark app.
 
    (C) 2012 A.Weissflog
 */
#include "emsctest/base/emsctestapplication.h"
#include "math/polar.h"
#include "graphics2/graphicsentity.h"

//------------------------------------------------------------------------------
namespace App
{
class InstancingApplication : public EMSCTestApplication
{
public:
    /// constructor
    InstancingApplication();
    
protected:
    /// called during opening phase
    virtual void OnOpening();

    /// per-frame update of scene
    virtual void UpdateScene();
    /// handle input
    virtual void HandleInput();
    /// setup light entities
    virtual void SetupLightEntities();
    /// create model entities
    virtual void SetupModelEntities();
    /// clear models in scene
    virtual void ClearModelScene();
    /// reset the camera
    virtual void ResetCamera();
    /// add instances
    void AddInstances(int num);
    /// delete instances
    void RemInstances(int num);
    /// show some info
    void PrintInfo();
    /// toggle godray / disco effect
    void ToggleGodRays();
    
    Ptr<Graphics2::GraphicsEntity> globalLight;
    Util::Array<Ptr<Graphics2::GraphicsEntity> > pointLights;
    Ptr<Graphics2::GraphicsEntity> modelEntity;
    struct InstanceData
    {
        float orbitRadius;
        float orbitSpeed;
        Math::polar polarCoord;
    };
    Util::Array<InstanceData> instances;
    Util::Array<Math::matrix44> instanceTransforms;
    bool godRaysEnabled;
};
    
} // namespace App
//------------------------------------------------------------------------------
