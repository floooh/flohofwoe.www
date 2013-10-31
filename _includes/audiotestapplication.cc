//------------------------------------------------------------------------------
//  audiotestapplication.cc
//  (C) 2013 A.Weissflog
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "audiotestapplication.h"
#include "input/inputserver.h"
#include "input/keyboard.h"
#include "graphics2/graphicsstage.h"
#include "graphics2/graphics2protocol.h"

namespace App
{
using namespace Audio3;
using namespace Math;
using namespace Input;

//------------------------------------------------------------------------------
/**
*/
AudioTestApplication::AudioTestApplication()
{
    // empty
}

//------------------------------------------------------------------------------
/**
 */
void
AudioTestApplication::OnOpening()
{
    EMSCTestApplication::OnOpening();
    
    // setup audio subsystem
    this->audioFacade = AudioFacade::Create();
    AudioSetup audioSetup;
    audioSetup.SetRootPath("audio:dso");
    audioSetup.SetProjectName("dso");
    this->audioFacade->Setup(audioSetup);
}

//------------------------------------------------------------------------------
/**
 */
void
AudioTestApplication::OnClosing()
{
    this->globalLight = 0;
    IndexT i;
    for (i = 0; i < NumPointLights; i++)
    {
        this->pointLight[i] = 0;
    }
    for (i = 0; i < NumModels; i++)
    {
        this->models[i] = 0;
    }

    this->audioFacade->Discard();
    this->audioFacade = 0;
    
    EMSCTestApplication::OnClosing();
}

//------------------------------------------------------------------------------
/**
 */
void
AudioTestApplication::ResetCamera()
{
    float distance = 7.5f;
    this->mayaCameraUtil.Setup(point(0.0f, 1.0f, 0.0f), point(0.0f, distance * 0.3, -distance), vector(0.0f, 1.0f, 0.0f));
    this->mayaCameraUtil.Update();
    this->graphicsFacade->GetDefaultCamera()->Transform()->SetTransform(this->mayaCameraUtil.GetCameraTransform());
}

//------------------------------------------------------------------------------
/**
 */
void
AudioTestApplication::SetupLightEntities()
{
    matrix44 globalLightTransform = matrix44::rotationx(n_deg2rad(-45.0f));
    globalLightTransform = matrix44::multiply(globalLightTransform, matrix44::rotationy(n_deg2rad(-135.0f)));
    this->globalLight = this->graphicsFacade->CreateGlobalLightEntity();
    this->globalLight->Transform()->SetTransform(globalLightTransform);
    this->globalLight->Lighting()->Light().SetColor(float4(1.0f, 1.0f, 1.0f, 1.0f));
    this->globalLight->Lighting()->Light().SetBackColor(float4(0.2f, 0.2f, 0.2f, 1.0f));
    this->globalLight->Lighting()->Light().SetAmbientColor(float4(0.0f, 0.0f, 0.0f, 1.0f));
    this->globalLight->Lighting()->Light().SetSpecularIntensity(1.0f);
    this->globalLight->Lighting()->Light().SetCastShadows(false);
    this->graphicsFacade->GetDefaultStage()->AttachEntity(this->globalLight);

    const float dist = 5.0f;
    IndexT i;
    for (i = 0; i < NumPointLights; i++)
    {
        float xPos = (i - (NumPointLights / 2)) * dist;
        point pos(xPos, 3.0f, 2.0f);
        this->pointLight[i] = this->graphicsFacade->CreatePointLightEntity();
        this->pointLight[i]->Lighting()->SetTransformFromPosDirAndRange(pos, vector::upvec(), 7.0f);
        this->pointLight[i]->Lighting()->Light().SetColor(float4(1.5f, 0.0f, 0.0f, 1.0f));
        this->pointLight[i]->Lighting()->Light().SetSpecularIntensity(0.0f);
        this->pointLight[i]->Lighting()->Light().SetCastShadows(false);
        this->graphicsFacade->GetDefaultStage()->AttachEntity(this->pointLight[i]);
    }
}

//------------------------------------------------------------------------------
/**
 */
void
AudioTestApplication::SetupModelEntities()
{
    const float dist = 4.0f;
    IndexT i;
    for (i = 0; i < NumModels; i++)
    {
        float xPos = (i - (NumModels / 2)) * dist;
        point pos(xPos, 0.0f, 0.0f);
        this->models[i] = this->CreateCharacter("mdl:characters/skeleton_warrior.n3",
                                                "skeleton_warrior_03_greek_offensive",
                                                "combat_idle_01",
                                                pos);
    }
}

//------------------------------------------------------------------------------
/**
 */
void
AudioTestApplication::PlayHitAnim(IndexT i)
{
    n_assert((i >= 0) && (i < NumModels));
    Ptr<Graphics2::AnimPlayClip> playClip = Graphics2::AnimPlayClip::Create();
    playClip->SetClipName("skill_01");
    playClip->SetTrackIndex(1);
    this->models[i]->Character()->PushMessage(playClip.cast<Graphics2::CharacterMessage>());
}

//------------------------------------------------------------------------------
/**
 */
void
AudioTestApplication::UpdateSystem()
{
    const matrix44& cameraTransform = this->graphicsFacade->GetDefaultCamera()->Transform()->GetTransform();
    this->audioFacade->SetListenerTransform(cameraTransform);
    this->audioFacade->Update();
    EMSCTestApplication::UpdateSystem();
}

//------------------------------------------------------------------------------
/**
 */
void
AudioTestApplication::HandleInput()
{
    const Ptr<Keyboard>& keyboard = InputServer::Instance()->GetDefaultKeyboard();
    if (keyboard->KeyDown(Key::Key1))
    {
        const matrix44& transform = this->models[0]->Transform()->GetTransform();
        this->audioFacade->StartSound3D(AudioCategory::Effect, "/archetype/mage/female/mage_female_attack_high", transform, vector::nullvec());
        this->PlayHitAnim(0);
    }
    else if (keyboard->KeyDown(Key::Key2))
    {
        const matrix44& transform = this->models[1]->Transform()->GetTransform();
        this->audioFacade->StartSound3D(AudioCategory::Effect, "/archetype/mage/female/mage_female_attack_low", transform, vector::nullvec());
        this->PlayHitAnim(1);
    }
    else if (keyboard->KeyDown(Key::Key3))
    {
        const matrix44& transform = this->models[2]->Transform()->GetTransform();
        this->audioFacade->StartSound3D(AudioCategory::Effect, "/archetype/mage/female/mage_female_attack_mid", transform, vector::nullvec());
        this->PlayHitAnim(2);
    }
    
    EMSCTestApplication::HandleInput();
}

} // namespace Audio3
