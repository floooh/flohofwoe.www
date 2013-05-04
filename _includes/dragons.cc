//------------------------------------------------------------------------------
//  dragons.cc
//  (C) 2012 A.Weissflog
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "dragonsapplication.h"

ImplementNebulaApplication();

void
NebulaMain(const Util::CommandLineArgs& args)
{
    static App::DragonsApplication* app = n_new(App::DragonsApplication);
    app->SetCompanyName("Bigpoint GmbH");
    app->SetAppTitle("N3 Dragons Test App");
    app->SetAppID("N3DA");
    app->SetAppVersion("1.0");
    app->SetCmdLineArgs(args);
#if __EMSCRIPTEN__
    app->SetOverrideRootDirectory("httpnz://localhost/cdndata");
#elif __OSX__
    app->SetOverrideRootDirectory("httpnz://localhost:8080/cdndata");
#endif
    app->StartMainLoop();
}
