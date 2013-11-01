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
    App::DragonsApplication app;
    app.SetCompanyName("Bigpoint GmbH");
    app.SetAppTitle("N3 Dragons Test App");
    app.SetAppID("N3DA");
    app.SetAppVersion("1.0");
    app.SetCmdLineArgs(args);
    Util::String rootUrl;
#if __OSX__
    rootUrl = "httpnz://0.0.0.0:8000/cdndata";
#else
    rootUrl = "httpnz://localhost:8000/cdndata";
#endif
    rootUrl = args.GetString("-rooturl", rootUrl);
    app.SetOverrideRootDirectory(rootUrl);
    app.StartMainLoop();
}
