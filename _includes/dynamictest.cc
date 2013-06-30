//------------------------------------------------------------------------------
//  dynamictest.cc
//  (C) 2013 A.Weissflog
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "dynamictestapplication.h"

ImplementNebulaApplication();

void
NebulaMain(const Util::CommandLineArgs& args)
{
    static App::DynamicTestApplication* app = n_new(App::DynamicTestApplication);
    app->SetCompanyName("Bigpoint GmbH");
    app->SetAppTitle("N3 Dynamic Test App");
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
