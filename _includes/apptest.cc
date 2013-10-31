//------------------------------------------------------------------------------
//  apptest.cc
//  (C) 2013 Bigpoint GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "apptestapplication.h"

ImplementNebulaApplication();

void
NebulaMain(const Util::CommandLineArgs& args)
{
    Util::CommandLineArgs mutableArgs = args;
    if (!mutableArgs.HasArg("-rooturl"))
    {
        #if __OSX__
        mutableArgs.AddArg("-rooturl", "httpnz://0.0.0.0:8000/cdndata");
        #else
        mutableArgs.AddArg("-rooturl", "httpnz://localhost:8000/cdndata");
        #endif
    }

    App::AppTestApplication app;
    app.SetCompanyName("Bigpoint GmbH");
    app.SetAppTitle("N3 Text Test App");
    app.SetAppID("N3DA");
    app.SetAppVersion("1.0");
    app.SetCmdLineArgs(mutableArgs);
    app.StartMainLoop();
}
