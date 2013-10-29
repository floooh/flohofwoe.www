#pragma once
//------------------------------------------------------------------------------
/**
 * @class EMSC::EMSCTEstApplication
 *
 * emscripten test application.
 *
 * (C) 2012 A.Weissflog
 */
#include "app/phasedapplication.h"
#include "timing/centralmastertime.h"
#include "timing/centraltime.h"
#include "graphics2/graphicsfacade.h"
#include "renderutil/mayacamerautil.h"
#include "input/inputserver.h"
#if __NEBULA3_HTTP__
#include "http/httpinterface.h"
#include "http/httpserverproxy.h"
#endif

//------------------------------------------------------------------------------
namespace App
{
class EMSCTestApplication : public PhasedApplication
{
public:
    /// constructor
    EMSCTestApplication();
    
protected:
    /// called to setup the file preloading queue
    virtual void OnSetupPreloadQueue();
    /// called during opening phase
    virtual void OnOpening();
    /// called during running phase
    virtual void OnRunning();
    /// called during closing phase
    virtual void OnClosing();

    /// per-frame update of system stuff (time etc...)
    virtual void UpdateSystem();
    /// per-frame update of scene
    virtual void UpdateScene();
    /// per-frame callback to render the scene (usually not overriden)
    virtual void DoRender();
    /// per-frame callback whether app should quit (usually not overriden)
    virtual bool IsQuitRequested();
    /// handle input
    virtual void HandleInput();
    /// setup light entities
    virtual void SetupLightEntities();
    /// create model entities
    virtual void SetupModelEntities();
    /// clear the scene (discard all created graphics entities)
    virtual void ClearScene();
    /// reset the camera
    virtual void ResetCamera();

    /// create a character object
    Ptr<Graphics2::GraphicsEntity> CreateCharacter(const Util::StringAtom& resId, const Util::StringAtom& skinList, const Util::StringAtom& anim, const Math::float4& pos);
    /// create a model entity
    Ptr<Graphics2::GraphicsEntity> CreateModel(const Util::StringAtom& resId);

    Ptr<Timing::CentralMasterTime> centralMasterTime;
    Ptr<Timing::CentralTime> centralTime;
    #if __NEBULA3_HTTP__
    Ptr<Http::HttpInterface> httpInterface;
    Ptr<Http::HttpServerProxy> httpServerProxy;
    #endif
    Ptr<Graphics2::GraphicsFacade> graphicsFacade;
    Ptr<Input::InputServer> inputServer;

    RenderUtil::MayaCameraUtil mayaCameraUtil;
};

} // namespace App
//------------------------------------------------------------------------------
