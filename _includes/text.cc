//------------------------------------------------------------------------------
//  text.cc
//  (C) 2013 Bigpoint GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "textapplication.h"

ImplementNebulaApplication();

void
NebulaMain(const Util::CommandLineArgs& args)
{
    App::TextApplication app;
    app.SetCompanyName("Bigpoint GmbH");
    app.SetAppTitle("N3 Text Test App");
    app.SetAppID("N3DA");
    app.SetAppVersion("1.0");
    app.SetCmdLineArgs(args);

#if __EMSCRIPTEN__
    app.SetOverrideRootDirectory("httpnz://localhost/cdndata");
#elif __IOS__
    app.SetOverrideRootDirectory("httpnz://127.0.0.1:8000/cdndata");
// app->SetOverrideRootDirectory("httpnz://10.164.252.166:8000/cdndata");
#elif __OSX__
    app.SetOverrideRootDirectory("httpnz://0.0.0.0:8000/cdndata");
#elif __NACL__
    app.SetOverrideRootDirectory("httpnz://localhost:8080/cdndata");
#endif
    app.StartMainLoop();
}
