//------------------------------------------------------------------------------
//  textapplication.cc
//  (C) 2013 A.Weissflog
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "textapplication.h"
#include "util/wstring.h"

namespace App
{
using namespace CoreText;
using namespace Util;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
TextApplication::TextApplication() :
    fontId(CoreText::InvalidFontId)
{
    // empty
}

//------------------------------------------------------------------------------
/**
 */
void
TextApplication::OnOpening()
{
    EMSCTestApplication::OnOpening();
    
    // setup a font
    n_printf("Before LoadFont (sizeof wchar_t '%lu')\n", sizeof(wchar_t));
    this->fontId = CoreTextFacade::Instance()->LoadFont("tahoma", 1, Math::float2(512, 512), 2.0f);
    n_printf("After LoadFint\n");
}

//------------------------------------------------------------------------------
/**
 */
void
TextApplication::OnRunning()
{
    CoreTextFacade* coreTextFacade = CoreTextFacade::Instance();
    if (coreTextFacade->AreAllFontsLoaded())
    {
        coreTextFacade->TextState().SetFont(this->fontId);
        const wchar_t* text = L"Hello World\nHello Saturn!";
        SizeT numChars = WString::StrLen(text);
        SizeT outNumLines = 0;
        float2 textExtents = coreTextFacade->ComputeTextExtents(text, numChars, outNumLines);
        n_printf("Extents: %f %f\n", textExtents.x(), textExtents.y());
    }
    EMSCTestApplication::OnRunning();
}

} // namespace App