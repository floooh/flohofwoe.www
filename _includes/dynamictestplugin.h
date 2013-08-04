#pragma once
//------------------------------------------------------------------------------
/**
    @class App::DynamicTestPlugin
    
    Render plugin to test rendering dynamic geometry.
 
    (C) 2013 A.Weissflog
*/
#include "graphics2/graphicsplugin.h"
#include "resource2/resourceid.h"

//------------------------------------------------------------------------------
namespace App
{
class DynamicTestPlugin : public Graphics2::GraphicsPlugin
{
    __DeclareClass(DynamicTestPlugin);
public:
    /// constructor
    DynamicTestPlugin();
    /// destructor
    virtual ~DynamicTestPlugin();
    
    /// called when plugin is enabled
    virtual void OnEnable();
    /// called when plugin is disabled
    virtual void OnDisable();
    /// called at start of pass (before batches)
    virtual void OnFramePassBegin(Frame::FramePass* framePass, IndexT frameIndex);

private:
    Resource2::ResourceId pipelineBuffer;
    Resource2::ResourceId dynMesh;
    IndexT frameCount;
    Util::StringAtom activePassName;
};

} // namespace App
//------------------------------------------------------------------------------
