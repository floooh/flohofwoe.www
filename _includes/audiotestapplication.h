#pragma once
//------------------------------------------------------------------------------
/**
    @class App::AudioTestApplication
    
    Test the Audio3 subsystem.
 
    (C) 2013 A.Weissflog
*/
#include "emsctest/base/emsctestapplication.h"
#include "audio3/audiofacade.h"
#include "graphics2/graphicsentity.h"

//------------------------------------------------------------------------------
namespace App
{
class AudioTestApplication : public EMSCTestApplication
{
public:
    /// constructor
    AudioTestApplication();
    
protected:
    /// called during opening phase
    virtual void OnOpening();
    /// called during the closing phase
    virtual void OnClosing();
    /// per-frame update of system stuff (time etc...)
    virtual void UpdateSystem();
    /// handle input
    virtual void HandleInput();
    /// setup light entities
    virtual void SetupLightEntities();
    /// create model entities
    virtual void SetupModelEntities();    
    /// reset the camera
    virtual void ResetCamera();
    /// play hit anim on one of the models
    void PlayHitAnim(IndexT i);

    static const int NumPointLights = 3;
    static const int NumModels = 3;
    Ptr<Audio3::AudioFacade> audioFacade;
    Ptr<Graphics2::GraphicsEntity> globalLight;
    Ptr<Graphics2::GraphicsEntity> pointLight[NumPointLights];
    Ptr<Graphics2::GraphicsEntity> models[NumModels];
};

} // namespace App
//------------------------------------------------------------------------------
