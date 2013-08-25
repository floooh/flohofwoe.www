#pragma once
//------------------------------------------------------------------------------
/**
    @class App::DynamicTestApplication

    Test dynamic geometry rendering.
 
    (C) 2013 A.Weissflog
*/
#include "emsctest/base/emsctestapplication.h"
#include "resource2/resourceid.h"

//------------------------------------------------------------------------------
namespace App
{
class DynamicTestApplication : public EMSCTestApplication
{
public:
    /// constructor
    DynamicTestApplication();
    /// called during opening phase
    virtual void OnOpening();
};

} // namespace App
//------------------------------------------------------------------------------
