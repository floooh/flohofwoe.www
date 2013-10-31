#pragma once
//------------------------------------------------------------------------------
/**
    @class App::AppTestStateHandler
    
    Game state handler for AppTest
 
    (C) 2013 Bigpoint GmbH
*/
#include "statehandlers/gamestatehandler.h"
#include "game/entity.h"

//------------------------------------------------------------------------------
namespace App
{
class AppTestStateHandler : public Game::GameStateHandler
{
    __DeclareClass(AppTestStateHandler);
public:
    /// called to setup file preload queue
    virtual void OnSetupPreloadQueue();
    /// called once after all files have been preloaded
    virtual bool OnStart();
};
    
} // namespace App
//------------------------------------------------------------------------------
