//------------------------------------------------------------------------------
//  dynamictestplugin.cc
//  (C) 2013 A.Weissflog
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "dynamictestplugin.h"
#include "coregraphics2/coregraphics2facade.h"

namespace App
{
__ImplementClass(App::DynamicTestPlugin, 'DTPN', Graphics2::GraphicsPlugin);

using namespace CoreGraphics2;
using namespace Resource2;
using namespace Frame;

//------------------------------------------------------------------------------
/**
*/
DynamicTestPlugin::DynamicTestPlugin() :
    frameCount(0)
{
    this->activePassName = "Color";
}

//------------------------------------------------------------------------------
/**
 */
DynamicTestPlugin::~DynamicTestPlugin()
{
    n_assert(!this->pipelineBuffer.IsValid());
}

//------------------------------------------------------------------------------
/**
 */
void
DynamicTestPlugin::OnEnable()
{
    n_assert(!this->pipelineBuffer.IsValid());
    CoreGraphics2Facade* cg2Facade = CoreGraphics2Facade::Instance();
    const int numTriangles = 1024;
    const int numVertices = numTriangles * 3;
    const int numVertexComponents = 1;
    const VertexComponent vertexComponents[numVertexComponents] =
    {
        VertexComponent(VertexComponent::Position, VertexComponent::Float3, 0),
    };
    const int verticesByteSize = numVertices * VertexComponent::ComputeVertexLayoutByteSize(vertexComponents, numVertexComponents);
    const PrimitiveGroup primGroup(PrimitiveTopology::TriangleList, 0, numVertices);
    
    // create a pipeline buffer
    PipelineBufferSetup pbSetup = PipelineBufferSetup::Create(Locator::Unique(), verticesByteSize);
    this->pipelineBuffer = cg2Facade->CreatePipelineBuffer(pbSetup);
    
    // ...and a dynamic mesh
    MeshSetup meshSetup = MeshSetup::CreateEmpty(Locator::Unique(), false, Usage::DynamicStream);
    meshSetup.SetNumVertices(numVertices);
    meshSetup.SetNumIndices(0);
    meshSetup.SetVertexComponents(vertexComponents, numVertexComponents);
    meshSetup.SetPrimitiveGroups(&primGroup, 1);
    this->dynMesh = cg2Facade->CreateMesh(meshSetup);
}

//------------------------------------------------------------------------------
/**
 */
void
DynamicTestPlugin::OnDisable()
{
    CoreGraphics2Facade* cg2Facade = CoreGraphics2Facade::Instance();
    if (this->dynMesh.IsValid())
    {
        cg2Facade->DiscardResource(this->dynMesh);
        this->dynMesh.Clear();
    }
    if (this->pipelineBuffer.IsValid())
    {
        cg2Facade->DiscardResource(this->pipelineBuffer);
        this->pipelineBuffer.Clear();
    }
}

//------------------------------------------------------------------------------
/**
 */
void
DynamicTestPlugin::OnFramePassBegin(FramePass* pass, IndexT frameIndex)
{
    n_assert(0 != pass);
    this->frameCount++;
    if (pass->GetName() == this->activePassName)
    {
        CoreGraphics2Facade* cg2Facade = CoreGraphics2Facade::Instance();
        
        // write a triangle to the pipeline buffer
        int byteSize = 0;
        float* const buf = (float*) cg2Facade->LockPipelineBuffer(this->pipelineBuffer, byteSize);
        float* ptr = buf;
        *ptr++ = 0.25f; *ptr++ = 0.25f; *ptr++ = 0.0f;
        *ptr++ = 0.75f; *ptr++ = 0.25f; *ptr++ = 0.0f;
        *ptr++ = 0.5f;  *ptr++ = 0.75f; *ptr++ = 0.0f;
        n_assert(ptr < (buf + (byteSize>>2)));
        cg2Facade->UnlockPipelineBuffer(this->pipelineBuffer);
        int bytesWritten = (ptr - buf) * sizeof(float);
        
        // FIXME: BeginInstances...
        
        // transfer the data into dynamic mesh
        cg2Facade->ApplyVertexData(this->pipelineBuffer, bytesWritten);
        
        // FIXME: render something using the dynamic vertex data
        // ...
    }
}

} // namespace App