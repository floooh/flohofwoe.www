//------------------------------------------------------------------------------
//  instancingapplication.cc
//  (C) 2012 A.Weissflog
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "instancingapplication.h"
#include "graphics2/graphicsentity.h"
#include "graphics2/graphicsstage.h"
#include "input/keyboard.h"

namespace App
{
using namespace Math;
using namespace Util;
using namespace Input;
using namespace Graphics2;
using namespace Frame;

//------------------------------------------------------------------------------
/**
 */
InstancingApplication::InstancingApplication() :
    godRaysEnabled(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
 */
void
InstancingApplication::PrintInfo()
{
    n_printf("--- Version 1 ---\n");
    n_printf("%d cubes, disco effect: %s\n", this->instances.Size(), this->godRaysEnabled ? "ON" : "OFF");
    n_printf("Up Key: More Cubes\n");
    n_printf("Down Key: Less Cubes\n");
    n_printf("Space Key: DISCO!\n");
    n_printf("Mouse Buttons + Drag: Move Camera\n");
}

//------------------------------------------------------------------------------
/**
 */
void
InstancingApplication::OnOpening()
{
    EMSCTestApplication::OnOpening();
    this->PrintInfo();
}

//------------------------------------------------------------------------------
/**
 */
void
InstancingApplication::ResetCamera()
{
    float distance = 96.0f;
    this->mayaCameraUtil.Setup(point(0.0f, 1.0f, 0.0f), point(0.0f, 0.0f, -distance), vector(0.0f, 1.0f, 0.0f));
    this->mayaCameraUtil.Update();
    this->graphicsFacade->GetDefaultCamera()->Transform()->SetTransform(this->mayaCameraUtil.GetCameraTransform());
}

//------------------------------------------------------------------------------
/**
 */
void
InstancingApplication::ToggleGodRays()
{
    this->godRaysEnabled = !this->godRaysEnabled;
    Ptr<FrameShader> frameShader = FrameServer::Instance()->LookupFrameShader("default");
    Ptr<FramePassBase> composePass = frameShader->GetFramePassBaseByName("Compose");
    Ptr<FramePassBase> godRaysPass = frameShader->GetFramePassBaseByName("ComposeWithGodRays");
    if (this->godRaysEnabled)
    {
        this->globalLight->Lighting()->Light().SetColor(float4(0.0f, 0.0f, 0.0f, 1.0f));
        this->globalLight->Lighting()->Light().SetBackColor(float4(0.0f, 0.0f, 0.0f, 1.0f));
        composePass->SetEnabled(false);
        godRaysPass->SetEnabled(true);
    }
    else
    {
        this->globalLight->Lighting()->Light().SetColor(float4(0.3f, 0.3f, 0.6f, 1.0f));
        this->globalLight->Lighting()->Light().SetBackColor(float4(0.1f, 0.1f, 0.3f, 1.0f));
        composePass->SetEnabled(true);
        godRaysPass->SetEnabled(false);
    }
}

//------------------------------------------------------------------------------
/**
 */
void
InstancingApplication::SetupLightEntities()
{
    // setup a global light
    matrix44 globalLightTransform = matrix44::rotationx(n_deg2rad(-45.0f));
    globalLightTransform = matrix44::multiply(globalLightTransform, matrix44::rotationy(n_deg2rad(-135.0f)));
    this->globalLight = this->graphicsFacade->CreateGlobalLightEntity();
    this->globalLight->Transform()->SetTransform(globalLightTransform);
    this->globalLight->Lighting()->Light().SetColor(float4(0.3f, 0.3f, 0.6f, 1.0f));
    this->globalLight->Lighting()->Light().SetBackColor(float4(0.1f, 0.1f, 0.3f, 1.0f));
    this->globalLight->Lighting()->Light().SetAmbientColor(float4(0.0f, 0.0f, 0.0f, 1.0f));
    this->globalLight->Lighting()->Light().SetSpecularIntensity(1.0f);
    this->globalLight->Lighting()->Light().SetCastShadows(false);
    this->graphicsFacade->GetDefaultStage()->AttachEntity(this->globalLight);
    
    // setup point lights
    srand(int(this->centralTime->GetTime() * 10000.0));
    int i;
    int num = 128;
    this->pointLights.Reserve(num);
    for (i = 0; i < num; i++)
    {
        Ptr<GraphicsEntity> pointLight = this->graphicsFacade->CreatePointLightEntity();
        point pos(n_rand(-60.0f, +60.0f), n_rand(-60.0f, +60.0f), n_rand(-60.0f, +60.0f));
        float4 color(n_rand(0.25f, 2.0f), n_rand(0.25f, 2.0f), n_rand(0.0f, 0.75f), 1.0f);
        pointLight->Lighting()->SetTransformFromPosDirAndRange(pos, vector::upvec(), 20.0f);
        pointLight->Lighting()->Light().SetColor(color);
        pointLight->Lighting()->Light().SetSpecularIntensity(0.0f);
        pointLight->Lighting()->Light().SetCastShadows(false);
        this->graphicsFacade->GetDefaultStage()->AttachEntity(pointLight);
        this->pointLights.Append(pointLight);
    }
}

//------------------------------------------------------------------------------
/**
 */
void
InstancingApplication::HandleInput()
{
    this->mayaCameraUtil.SetOrbiting(float2(0.03f, 0.0f));
    EMSCTestApplication::HandleInput();
    
    InputServer* inputServer = InputServer::Instance();
    const Ptr<Keyboard>& keyboard = inputServer->GetDefaultKeyboard();
    
    if (keyboard->KeyDown(Key::Up))
    {
        // more instances
        this->AddInstances(1000);
    }
    if (keyboard->KeyDown(Key::Down))
    {
        // less dragons
        if (this->instances.Size() > 1000)
        {
            this->RemInstances(1000);
        }
    }
    if (keyboard->KeyDown(Key::Space))
    {
        // toggle disco effect
        this->ToggleGodRays();
    }
}

//------------------------------------------------------------------------------
/**
 */
void
InstancingApplication::SetupModelEntities()
{
    this->modelEntity = this->CreateModel("mdl:dummies/dummy_cube.n3");
    this->modelEntity->Transform()->SetInstancingEnabled(true);
    this->AddInstances(1000);
}

//------------------------------------------------------------------------------
/**
 */
void
InstancingApplication::ClearModelScene()
{
    this->graphicsFacade->GetDefaultStage()->RemoveEntity(this->modelEntity);
    this->graphicsFacade->DiscardEntity(this->modelEntity);
    this->modelEntity = 0;
}

//------------------------------------------------------------------------------
/**
 */
void
InstancingApplication::AddInstances(int num)
{
    const float minOrbitRadius = 15.0f;
    const float maxOrbitRadius = 64.0f;
    this->instances.Reserve(this->instances.Size() + num);
    InstanceData instData;
    int i;
    for (i = 0; i < num; i++)
    {
        instData.orbitRadius = n_rand(minOrbitRadius, maxOrbitRadius);
        instData.orbitSpeed = n_rand(-N_PI, +N_PI) * 0.1f;;
        float rho = n_rand(-N_PI, +N_PI);
        float theta = n_rand(-N_PI, +N_PI);
        instData.polarCoord = polar(theta, rho);
        this->instances.Append(instData);
    }
    this->PrintInfo();
}

//------------------------------------------------------------------------------
/**
 */
void
InstancingApplication::RemInstances(int num)
{
    if (this->instances.Size() > num)
    {
        int i;
        for (i = 0; i < num; i++)
        {
            this->instances.EraseIndex(this->instances.Size() - 1);
        }
    }
    this->PrintInfo();
}

//------------------------------------------------------------------------------
/**
 */
void
InstancingApplication::UpdateScene()
{
    EMSCTestApplication::UpdateScene();
    float frameTime = this->centralTime->GetFrameTime();
    
    // update instance transforms
    this->instanceTransforms.Clear();
    this->instanceTransforms.Reserve(this->instances.Size());
    int i;
    int num = this->instances.Size();
    float4 pos;
    for (i = 0; i < num; i++)
    {
        InstanceData& curInstance = this->instances[i];
        curInstance.polarCoord.theta += curInstance.orbitSpeed * frameTime;
        pos = curInstance.polarCoord.get_cartesian() * curInstance.orbitRadius;
        pos.set_w(1.0f);
        this->instanceTransforms.Append(matrix44::lookatrh(pos, point::origin(), vector::upvec()));
    }
    this->modelEntity->Transform()->SetInstanceTransforms(this->instanceTransforms);
}

} // namespace App