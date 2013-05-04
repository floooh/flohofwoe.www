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
    static App::DSOMapViewerApplication* app = n_new(App::DSOMapViewerApplication);
    app->SetCompanyName("Bigpoint GmbH");
    app->SetAppTitle("DSO Map Viewer App");
    app->SetAppID("DSMV");
    app->SetAppVersion("1.0");
    app->SetCmdLineArgs(args);
    #if __EMSCRIPTEN__
    app->SetOverrideRootDirectory("httpnz://localhost/cdndata");
    #elif __OSX__
    app->SetOverrideRootDirectory("httpnz://localhost:8080/cdndata");
    #endif
    app->StartMainLoop();
}
