//------------------------------------------------------------------------------
//  dsocharviewer.cc
//  (C) 2012 A.Weissflog
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "dsocharviewerapplication.h"

ImplementNebulaApplication();

void
NebulaMain(const Util::CommandLineArgs& args)
{
    static App::DSOCharViewerApplication* app = n_new(App::DSOCharViewerApplication);
    app->SetCompanyName("Bigpoint GmbH");
    app->SetAppTitle("N3 DSO CharacterViewer Test App");
    app->SetAppID("N3CV");
    app->SetAppVersion("1.0");
    app->SetCmdLineArgs(args);
#if __EMSCRIPTEN__
    app->SetOverrideRootDirectory("httpnz://localhost/cdndata");
#elif __OSX__
    app->SetOverrideRootDirectory("httpnz://localhost:8080/cdndata");
#endif
    app->StartMainLoop();
}
