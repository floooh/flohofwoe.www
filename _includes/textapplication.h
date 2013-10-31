#pragma once
//------------------------------------------------------------------------------
/**
    @class App::TextApplication
    
    Test text rendering.
    
    (C) 2013 A.Weissflog
*/
#include "emsctest/base/emsctestapplication.h"
#include "coretext/fontid.h"

//------------------------------------------------------------------------------
namespace App
{
class TextApplication : public EMSCTestApplication
{
public:
    /// constructor
    TextApplication();
    
    /// called during opening phase
    virtual void OnOpening();
    /// called during running phase
    virtual void OnRunning();
    
private:
    CoreText::FontId fontId;
};
    
} // namespace App
//------------------------------------------------------------------------------
