//------------------------------------------------------------------------------
//  emsctestapplication.cc
//  (C) 2012 A.Weissflog
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "emsctestapplication.h"
#include "input/mouse.h"
#include "input/keyboard.h"
#include "input/touchpad.h"
#include "graphics2/graphicsfacade.h"
#include "graphics2/graphicsstage.h"
#include "graphics2/graphics2protocol.h"

#if __NEBULA3_HTTP__
#include "core/debug/corepagehandler.h"
#include "core/debug/stringatompagehandler.h"
#include "threading/debug/threadpagehandler.h"
#include "memory/debug/memorypagehandler.h"
#include "io/debug/consolepagehandler.h"
#include "io/debug/iopagehandler.h"
#endif

namespace App
{
using namespace Util;
using namespace Graphics2;
using namespace Input;
using namespace Timing;
using namespace Math;
using namespace Http;

//------------------------------------------------------------------------------
/**
 */
EMSCTestApplication::EMSCTestApplication()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Define files which must be preloaded before OnOpening() can be called.
 */
void
EMSCTestApplication::OnSetupPreloadQueue()
{
    n_printf("> OnSetupPreloadQueue()\n");

    this->AddPreloadFile("export:tables/gfxskintags.bin");
    this->AddPreloadFile(NEBULA3_PLACEHOLDER_MESH);
    this->AddPreloadFile(NEBULA3_PLACEHOLDER_COLORTEXTURE);
    this->AddPreloadFile(NEBULA3_PLACEHOLDER_BUMPTEXTURE);
    this->AddPreloadFile(NEBULA3_PLACEHOLDER_SPECTEXTURE);
    this->AddPreloadFile("mdl:system/placeholder.n3");
    PhasedApplication::OnSetupPreloadQueue();
}

//------------------------------------------------------------------------------
/**
    Setup the application. The HTTP stuff is only initialized when 
    running as native desktop application and creates a simple debug
    HTTP server. If the Opening phase takes to long, it can decide
    to spread its work across several frames.
 */
void
EMSCTestApplication::OnOpening()
{
    n_printf("> OnOpening()\n");

    // setup central time sources
    this->centralMasterTime = CentralMasterTime::Create();
    this->centralMasterTime->Setup();
    this->centralTime = CentralTime::Create();
    this->centralTime->Setup();
    
    #if __NEBULA3_HTTP__
    this->httpInterface = HttpInterface::Create();
    this->httpInterface->Open();
    this->httpServerProxy = HttpServerProxy::Create();
    this->httpServerProxy->Open();
    this->httpServerProxy->AttachRequestHandler(Debug::CorePageHandler::Create());
    this->httpServerProxy->AttachRequestHandler(Debug::StringAtomPageHandler::Create());
    this->httpServerProxy->AttachRequestHandler(Debug::ThreadPageHandler::Create());
    this->httpServerProxy->AttachRequestHandler(Debug::MemoryPageHandler::Create());
    this->httpServerProxy->AttachRequestHandler(Debug::IoPageHandler::Create());
    this->httpServerProxy->AttachRequestHandler(Debug::ConsolePageHandler::Create());
    #endif

    // setup the graphics system
    this->graphicsFacade = GraphicsFacade::Create();
    Graphics2::DisplaySetup displaySetup;
    #if __IOS__
        displaySetup.SetWidth(this->args.GetInt("-w", 1024));
        displaySetup.SetHeight(this->args.GetInt("-h", 768));
    #else
        displaySetup.SetWidth(this->args.GetInt("-w", 800));
        displaySetup.SetHeight(this->args.GetInt("-h", 452));
    #endif
    displaySetup.SetWindowTitle("CoreGraphics2 Test");
    this->graphicsFacade->Setup(displaySetup, false);
    this->graphicsFacade->SetStatisticsDisplayEnabled(true);

    // setup input server
    this->inputServer = InputServer::Create();
    this->inputServer->Open();

    #if NEBULA3_USE_CONSOLE
        this->gameConsoleFacade = GameConsole::GameConsoleFacade::Create();
        this->gameConsoleFacade->Setup();
    #endif

    // reset the camera
    this->ResetCamera();

    // setup the scene (provided by subclasses)
    this->SetupLightEntities();
    this->SetupModelEntities();

    // call parent class (ticks phase forward to Running
    PhasedApplication::OnOpening();
}

//------------------------------------------------------------------------------
/**
 */
void
EMSCTestApplication::OnClosing()
{
#if NEBULA3_USE_CONSOLE
    this->gameConsoleFacade->Discard();
    this->gameConsoleFacade = 0;
#endif

    this->ClearScene();
    this->inputServer->Close();
    this->inputServer = 0;
    this->graphicsFacade->GetDefaultStage()->RemoveAllEntities();
    this->graphicsFacade->Discard();
    
#if __NEBULA3_HTTP__
    this->httpServerProxy->Close();
    this->httpServerProxy = 0;
    this->httpInterface->Close();
    this->httpInterface = 0;
#endif
    
    this->centralTime->Discard();
    this->centralTime = 0;
    this->centralMasterTime->Discard();
    this->centralMasterTime = 0;
    
    PhasedApplication::OnClosing();
}

//------------------------------------------------------------------------------
/**
 */
void
EMSCTestApplication::ResetCamera()
{
    this->mayaCameraUtil.Reset();
    this->graphicsFacade->GetDefaultCamera()->Transform()->SetTransform(this->mayaCameraUtil.GetCameraTransform());
}

//------------------------------------------------------------------------------
/**
 */
void
EMSCTestApplication::SetupModelEntities()
{
    
}

//------------------------------------------------------------------------------
/**
 */
void
EMSCTestApplication::SetupLightEntities()
{
    // override in subclass
}

//------------------------------------------------------------------------------
/**
 */
void
EMSCTestApplication::ClearScene()
{
    // override in subclass
}

//------------------------------------------------------------------------------
/**
 */
void
EMSCTestApplication::UpdateSystem()
{
    // update time
    this->centralMasterTime->Advance();
    this->centralTime->Advance();

    // handle input
    this->HandleInput();
    
    #if NEBULA3_USE_CONSOLE
        this->gameConsoleFacade->Update();
    #endif

    #if __NEBULA3_HTTP__
        this->httpServerProxy->HandlePendingRequests();
    #endif
}

//------------------------------------------------------------------------------
/**
 */
void
EMSCTestApplication::HandleInput()
{
    InputServer* inputServer = InputServer::Instance();
    const Ptr<Keyboard>& keyboard = inputServer->GetDefaultKeyboard();
    const Ptr<Mouse>& mouse = inputServer->GetDefaultMouse();
    const Ptr<TouchPad>& touchPad = inputServer->GetDefaultTouchPad();
    
    if (touchPad.isvalid())
    {
        this->mayaCameraUtil.SetZoomButton(false);
        this->mayaCameraUtil.SetOrbitButton(false);
        if (touchPad->Pinching())
        {
            this->mayaCameraUtil.SetZoomButton(true);
            this->mayaCameraUtil.SetMouseMovement(touchPad->GetVelocity() * 0.1f);
        }
        if (touchPad->Panning())
        {
            this->mayaCameraUtil.SetOrbitButton(true);
            this->mayaCameraUtil.SetMouseMovement(touchPad->GetVelocity() * float2(0.5f, 0.5f));
        }
    }

    if (mouse.isvalid())
    {
        this->mayaCameraUtil.SetOrbitButton(mouse->ButtonPressed(MouseButton::LeftButton));
        this->mayaCameraUtil.SetPanButton(mouse->ButtonPressed(MouseButton::MiddleButton));
        this->mayaCameraUtil.SetZoomButton(mouse->ButtonPressed(MouseButton::RightButton));
        this->mayaCameraUtil.SetZoomInButton(mouse->WheelForward());
        this->mayaCameraUtil.SetZoomOutButton(mouse->WheelBackward());
        this->mayaCameraUtil.SetMouseMovement(mouse->GetMovement() * 0.5f);
    }
    
    // process keyboard input
    if (keyboard.isvalid())
    {
        float zoomIn = 0.0f;
        float zoomOut = 0.0f;
        float2 panning(0.0f, 0.0f);
        if (keyboard->KeyDown(Key::Space))
        {
            this->mayaCameraUtil.Reset();
        }
        this->mayaCameraUtil.SetZoomIn(zoomIn);
        this->mayaCameraUtil.SetZoomOut(zoomOut);
        
        if (keyboard->KeyDown(Key::M))
        {
            Memory::DumpMemoryStatus();
        }
        
        if (keyboard->KeyDown(Key::F1))
        {
            this->graphicsFacade->ToggleStatisticsDisplay();
        }        
    }

    // update the camera util
    this->mayaCameraUtil.Update();
}

//------------------------------------------------------------------------------
/**
 */
void
EMSCTestApplication::UpdateScene()
{
    // update the camera
    matrix44 cameraTransform = this->mayaCameraUtil.GetCameraTransform();
    this->graphicsFacade->GetDefaultCamera()->Transform()->SetTransform(cameraTransform);
}

//------------------------------------------------------------------------------
/**
 */
bool
EMSCTestApplication::IsQuitRequested()
{
    return this->graphicsFacade->IsQuitRequested();
}

//------------------------------------------------------------------------------
/**
 */
void
EMSCTestApplication::OnRunning()
{
    this->ioInterface->Update();
    this->inputServer->BeginFrame();
    this->inputServer->OnFrame();
    this->UpdateSystem();
    this->UpdateScene();
    this->graphicsFacade->OnFrame();
    this->inputServer->EndFrame();
    Threading::Thread::YieldThread();
    if (this->IsQuitRequested())
    {
        this->UpdateState(Closing);
    }
}

//------------------------------------------------------------------------------
/**
    Utility method for subclasses to create a character model with
    skinlist and animation.
 */
Ptr<GraphicsEntity>
EMSCTestApplication::CreateCharacter(const StringAtom& resId, const StringAtom& skinList, const StringAtom& anim, const float4& pos)
{
    GraphicsFacade* graphicsFacade = GraphicsFacade::Instance();

    matrix44 m = matrix44::identity(); // matrix44::rotationy(n_deg2rad(180.0f));
    vector translate = pos;
    m.translate(translate);

    Ptr<GraphicsEntity> modelEntity = graphicsFacade->CreateCharacterEntity();
    modelEntity->Transform()->SetTransform(m);
    modelEntity->Model()->SetResourceId(resId);
    graphicsFacade->GetDefaultStage()->AttachEntity(modelEntity);

    Ptr<Graphics2::ApplySkinList> applySkinList = Graphics2::ApplySkinList::Create();
    applySkinList->SetSkinList(skinList);
    modelEntity->Character()->PushMessage(applySkinList.cast<CharacterMessage>());
    Ptr<Graphics2::AnimPlayClip> playClip = Graphics2::AnimPlayClip::Create();
    playClip->SetClipName(anim);
    playClip->SetTrackIndex(0);
    playClip->SetLoopCount(0.0f);
    playClip->SetTimeOffset(Timing::Tick(n_rand() * 10000));
    modelEntity->Character()->PushMessage(playClip.cast<CharacterMessage>());

    return modelEntity;
}

//------------------------------------------------------------------------------
/**
    Utility method for subclasses to create a simple graphics object.
 */
Ptr<GraphicsEntity>
EMSCTestApplication::CreateModel(const StringAtom& resId)
{
    GraphicsFacade* graphicsFacade = GraphicsFacade::Instance();
    Ptr<GraphicsEntity> modelEntity = graphicsFacade->CreateModelEntity();
    modelEntity->Model()->SetResourceId(resId);
    graphicsFacade->GetDefaultStage()->AttachEntity(modelEntity);
    return modelEntity;
}

} // namespace App
