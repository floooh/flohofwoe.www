//------------------------------------------------------------------------------
//  dsomapviewer.cc
//  (C) 2012 A.Weissflog
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "dsomapviewerapplication.h"

ImplementNebulaApplication();

void
NebulaMain(const Util::CommandLineArgs& args)
{
    App::DSOMapViewerApplication app;
    app.SetCompanyName("Bigpoint GmbH");
    app.SetAppTitle("DSO Map Viewer App");
    app.SetAppID("DSMV");
    app.SetAppVersion("1.0");
    app.SetCmdLineArgs(args);
#if __EMSCRIPTEN__
    app.SetOverrideRootDirectory("httpnz://localhost/cdndata");
#elif __IOS__
    app.SetOverrideRootDirectory("httpnz://127.0.0.1:8000/cdndata");
    //app->SetOverrideRootDirectory("httpnz://www.flohofwoe.net/demos/cdndata");
#elif __OSX__
    app.SetOverrideRootDirectory("httpnz://0.0.0.0:8000/cdndata");
#elif __NACL__
    app.SetOverrideRootDirectory("httpnz://localhost:8080/cdndata");
#endif
    app.StartMainLoop();
}
