//------------------------------------------------------------------------------
//  instancing.cc
//  (C) 2012 A.Weissflog
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "instancingapplication.h"

ImplementNebulaApplication();

void
NebulaMain(const Util::CommandLineArgs& args)
{
    App::InstancingApplication app;
    app.SetCompanyName("Bigpoint GmbH");
    app.SetAppTitle("N3 Instancing Test App");
    app.SetAppID("N3IA");
    app.SetAppVersion("1.0");
    app.SetCmdLineArgs(args);
#if __EMSCRIPTEN__
    app.SetOverrideRootDirectory("httpnz://localhost/cdndata");
#elif __OSX__
    app.SetOverrideRootDirectory("httpnz://localhost:8080/cdndata");
#elif __NACL__
    app.SetOverrideRootDirectory("httpnz://localhost:8080/cdndata");
#endif
    app.StartMainLoop();
}
