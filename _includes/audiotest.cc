//------------------------------------------------------------------------------
//  audiotest.cc
//  (C) 2013 A.Weissflog
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "audiotestapplication.h"

ImplementNebulaApplication();

void
NebulaMain(const Util::CommandLineArgs& args)
{
    static App::AudioTestApplication* app = n_new(App::AudioTestApplication);
    app->SetCompanyName("Bigpoint GmbH");
    app->SetAppTitle("N3 Audio Test App");
    app->SetAppID("N3IA");
    app->SetAppVersion("1.0");
    app->SetCmdLineArgs(args);
    #if __EMSCRIPTEN__
    app->SetOverrideRootDirectory("httpnz://localhost/cdndata");
    #elif __OSX__
    app->SetOverrideRootDirectory("httpnz://localhost:8080/cdndata");
    #endif
    app->StartMainLoop();
}
