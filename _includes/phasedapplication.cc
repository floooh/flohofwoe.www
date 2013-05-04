//------------------------------------------------------------------------------
//  ohasedapplication.cc
//  (C) 2012 A.Weissflog
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "phasedapplication.h"
#if __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#elif __FLASCC__
#include <AS3/AS3.h>
#endif

extern "C" void N3EnterFrame(void)
{
    App::PhasedApplication::OnPhasedFrame();
}

namespace App
{
using namespace Util;
using namespace IO;

PhasedApplication* PhasedApplication::self = 0;

//------------------------------------------------------------------------------
/**
 */
PhasedApplication::PhasedApplication() :
    state(Initial)
{
    self = this;
}

//------------------------------------------------------------------------------
/**
 */
PhasedApplication::~PhasedApplication()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This method is called by NebulaMain() to enter the game loop. On some
    platforms this method will never return, on others it will return
    when the game finishes.
 */
void
PhasedApplication::StartMainLoop()
{
    n_assert(0 != self);
    #if __EMSCRIPTEN__
    emscripten_set_main_loop(OnPhasedFrame, 0, 0);
    #elif __FLASCC__
    AS3_GoAsync();
    #else
    while (Terminated != this->state)
    {
        OnPhasedFrame();
    }
    #endif
}

//------------------------------------------------------------------------------
/**
    This is the general on-frame callback which will branch into
    specific on-frame methods based on the current state. 
 */
void
PhasedApplication::OnPhasedFrame()
{
    // branch to handler methods depending on current state
    // the handler methods are responsible to tick the state forward
    n_assert(0 != self);
    switch (self->state)
    {
        case Initial:           self->OnInitial(); break;
        case SetupPreloadQueue: self->OnSetupPreloadQueue(); break;
        case Preloading:        self->OnPreloading(); break;
        case Opening:           self->OnOpening(); break;
        case Running:           self->OnRunning(); break;
        case Closing:           self->OnClosing(); break;
        case Quit:              self->OnQuit(); break;
        default:                break;
    }
}

//------------------------------------------------------------------------------
/**
 */
void
PhasedApplication::UpdateState(State newState)
{
    this->state = newState;
}

//------------------------------------------------------------------------------
/**
    Called by subclasses to add a file to the preload queue, and start 
    loading/downloading the file.
 */
void
PhasedApplication::AddPreloadFile(const String& path)
{
    n_dbgout("PhasedApplication::AddPreloadFile(%s)\n", path.AsCharPtr());

    Ptr<IO::ReadStream> ioRequest = IO::ReadStream::Create();
    ioRequest->SetURI(path);
    this->ioInterface->Send(ioRequest);
    this->preloadQueue.Append(ioRequest);
}

//------------------------------------------------------------------------------
/**
    Setup the most minimal Nebula3 runtime and tick state forward
    to SetupPreloadQueue.
 */
void
PhasedApplication::OnInitial()
{
    n_dbgout("--- PhasedApplication::OnInitial()\n");

    StringAtom rootDir = "home:";
    if (this->overrideRootDirectory.IsValid())
    {
        rootDir = this->overrideRootDirectory;
    }

    // initialize core subsystem
    this->coreServer = Core::CoreServer::Create();
    this->coreServer->SetCompanyName(this->companyName);
    this->coreServer->SetAppTitle(this->appTitle);
    this->coreServer->SetAppID(this->appID);
    this->coreServer->SetRootDirectory(rootDir);
    this->coreServer->SetRootKey(this->rootKey);
    this->coreServer->Open();

    #if __NEBULA3_HTTP_FILESYSTEM__
    // setup http subsystem
    this->httpClientRegistry = Http::HttpClientRegistry::Create();
    this->httpClientRegistry->Setup();
    #endif

    // initialize io subsystem
    this->ioServer = IoServer::Create();
    this->ioInterface = IoInterface::Create();
    this->ioInterface->Open();

    // advance to next state
    this->UpdateState(SetupPreloadQueue);
}

//------------------------------------------------------------------------------
/**
    This method is overriden in subclasses to populate the file preload-queue.
    Subclasses should then call their parent class to tick the state 
    to Preloading.
 */
void
PhasedApplication::OnSetupPreloadQueue()
{
    n_dbgout("--- PhasedApplication::OnSetupPreloadQueue()\n");
    this->UpdateState(Preloading);
}

//------------------------------------------------------------------------------
/**
    This method is called each frame during the Preload phase. It checks 
    whether all files in the preloadQueue are downloaded (or failed to 
    download), and then tick the current state forward to Opening.
 */
void
PhasedApplication::OnPreloading()
{
    // need to trigger IO interface for async io to work
    this->ioInterface->Update();

    // check through pending preloads and switch to next state
    // when all are finished
    // NOTE: we do not delete the preloadQueue, instead we
    // open the contained stream objects, this will pin the
    // preloaded data into memory
    bool allDone = true;
    IndexT i;
    for (i = 0; i < this->preloadQueue.Size(); i++)
    {
        const Ptr<IO::ReadStream>& curMsg = this->preloadQueue[i];
        if (curMsg->Handled())
        {
            // open the stream, to prevent the data from being unloaded
            if (curMsg->GetResult() && !curMsg->GetStream()->IsOpen())
            {
                curMsg->GetStream()->Open();
            }
        }
        else
        {
            allDone = false;
            break;
        }
    }
    if (allDone)
    {
        this->UpdateState(Opening);
    }
}

//------------------------------------------------------------------------------
/**
    Called during the Opening phase. Subclasses do initialization work 
    here which depends on preloaded files. After all opening work is
    done, the subclass should either call the parent class to tick
    the state forward, or call UpdateState(Running) themselves.
 */
void
PhasedApplication::OnOpening()
{
    n_dbgout("--- PhasedApplication::OnOpening()\n");
    this->UpdateState(Running);
}

//------------------------------------------------------------------------------
/**
    This is the actual on-frame callback for the "game loop". Subclasses
    should *not* call the parent class method, since this would tick the
    app state forward to OnClosing (I think this a bit of a design fault...).
 */
void
PhasedApplication::OnRunning()
{
    this->ioInterface->Update();
    this->UpdateState(Closing);
}

//------------------------------------------------------------------------------
/**
    Called during the Closing phase, when the app should shutdown. Should
    be overriden by the subclass to perform cleanup work. Similar
    to the Opening phase, if the method takes too long it should spread
    its work across several frames. Calling the parent class method will
    then switch the app to Quit.
 */
void
PhasedApplication::OnClosing()
{
    n_dbgout("--- PhasedApplication::OnClosing()\n");
    this->UpdateState(Quit);
}

//------------------------------------------------------------------------------
/**
    This is the counterpart to OnInitial() and should be called
    exactly once. 
 */
void
PhasedApplication::OnQuit()
{
    n_dbgout("--- PhasedApplication::OnQuit()\n");

    // clear the preload queue, this will "unpin" the data from memory
    IndexT i;
    for (i = 0; i < this->preloadQueue.Size(); i++)
    {
        const Ptr<IO::ReadStream>& curMsg = this->preloadQueue[i];
        if (curMsg->GetStream().isvalid() && curMsg->GetStream()->IsOpen())
        {
            curMsg->GetStream()->Close();
        }
    }
    this->preloadQueue.Clear();
    this->ioInterface->Close();
    this->ioInterface = 0;
    this->ioServer = 0;
    #if __NEBULA3_HTTP_FILESYSTEM__
    this->httpClientRegistry->Discard();
    this->httpClientRegistry = 0;
    #endif
    this->coreServer = 0;
    this->UpdateState(Terminated);
    #if __EMSCRIPTEN__
    emscripten_cancel_main_loop();
    #endif
    
}

//------------------------------------------------------------------------------
/**
    This is a relict from Nebula3's default Application class which must not be 
    called in a "phased runtime environment".
 */
bool
PhasedApplication::Open()
{
    n_error("PhasedApplication::Open() called!\n");
    return false;
}

//------------------------------------------------------------------------------
/**
    This is a relict from Nebula3's default Application class which must not be
    called in a "phased runtime environment".
 */
void
PhasedApplication::Close()
{
    n_error("PhasedApplication::Close() called!\n");
}

//------------------------------------------------------------------------------
/**
    This is a relict from Nebula3's default Application class which must not be
    called in a "phased runtime environment".
 */
void
PhasedApplication::Exit()
{
    n_error("PhasedApplication::Exit() called!\n");
}

//------------------------------------------------------------------------------
/**
    This is a relict from Nebula3's default Application class which must not be
    called in a "phased runtime environment".
 */
void
PhasedApplication::OnFrame()
{
    n_error("PhasedApplication::OnFrame() called!\n");
}

} // namespace App
