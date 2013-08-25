//------------------------------------------------------------------------------
//  dynamictestapplication.cc
//  (C) 2013 A.Weissflog
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "dynamictestapplication.h"
#include "dynamictestplugin.h"
#include "graphics2/graphicsfacade.h"

namespace App
{
using namespace CoreGraphics2;
using namespace Resource2;
using namespace Graphics2;

//------------------------------------------------------------------------------
/**
 */
DynamicTestApplication::DynamicTestApplication()
{
    // empty
}

//------------------------------------------------------------------------------
/**
 */
void
DynamicTestApplication::OnOpening()
{
    EMSCTestApplication::OnOpening();
    GraphicsFacade::Instance()->AttachPlugin("dyntest", DynamicTestPlugin::Create());
}

} // namespace App
