//------------------------------------------------------------------------------
//  dsocharviewerapplication.cc
//  (C) 2012 A.Weissflog
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "dsocharviewerapplication.h"
#include "graphics2/graphicsentity.h"
#include "graphics2/graphicsstage.h"
#include "graphics2/graphics2protocol.h"
#include "input/keyboard.h"

namespace App
{
using namespace Util;
using namespace Math;
using namespace Graphics2;
using namespace Input;

const char* Models[200][20] = {
    { "atlantide", 0 },
    { "atlantide_guard", "atlantide_member_of_council", "atlantide_priest", 0 },
    { "female_idle_spear_01", "female_idle_spear_02", "male_idle_spear_01", 0 },
    
/*
    {"barrel_dealer", 0 },
    { "barrel_dealer_01", 0 },
    { "male_idle_01", 0 },
*/
    
    { "beast_hound", 0 },
    { "beast_hound_01", "beast_hound_01_miniboss", "beast_hound_skeleton", "polarwolf_01_christmas2012", "polarwolf_01_christmas2012_miniboss", "wolf_01", 0 },
    { "combat_idle_01", "run_01", 0 },
        
    { "beastman_boar", 0 },
    { "beastman_boar_01", "beastman_boar_01_miniboss", 0 },
    { "combat_idle_01", "run_01", 0 },
 
    { "beastman_fox", 0 },
    { "beastman_fox", 0 },
    { "combat_idle_01", "run_01", 0 },

    { "beastman_lobster", 0 },
    { "beastman_lobster", "beastman_lobster_01_miniboss", 0 },
    { "combat_idle_01", "run_01", 0 },
    
    { "beastman_stag", 0 },
    { "beastman_stag_01", "beastman_stag_01_miniboss", "beastman_stag_skeleton", 0 },
    { "combat_idle_01", "run_01", 0 },
    
    { "boss_01", 0 },
    { "boss_01", 0 },
    { "combat_idle_01", 0 },
    
    { "boss_02_herald", 0 },
    { "boss_02_herald", 0 },
    { "combat_idle_01", "run_01", 0 },
    
    { "boss_03_ice_highpriest", 0 },
    { "boss_03_ice_highpriest", 0 },
    { "combat_idle_01", 0 },
    
    { "boss_04_snake", 0 },
    { "boss_04_snake", 0 },
    { "combat_idle_01", 0 },
    
    { "boss_05_deathgod", 0 },
    { "dragon_alone_01", "dragon_with_mage_01", 0 },
    { "combat_idle_01", "run_01", 0 },
    
    { "cerberus", 0 },
    { "cerberus", "cerberus_01_miniboss", 0 },
    { "combat_idle_01", "run_01", 0 },
    
    { "death_knight", 0 },
    { "death_knight_01", "death_knight_01_miniboss", 0 },
    { "combat_idle_01", "run_01", 0 },
    
    { "dragon_berserker", 0 },
    { "dragon_berserker", "dragon_berserker_01_miniboss", "dragon_berserker_02_miniboss", 0 },
    { "combat_idle_01", "run_01", 0 },

    { "dragon_brood", 0 },
    { "dragon_brood_skeleton_01", "dragon_brood_warrior", "dragon_brood_warrior_02_wounded", "dragon_brood_warrior_miniboss", 0 },
    { "combat_idle_01", "run_01", 0 },
    
    { "dragon_broodling", 0 },
    { "dragon_broodling_01", "dragon_broodling_01_miniboss", "dragon_broodling_02_easter", "dragon_broodling_03_wounded", "dragon_broodling_golden", 0 },
    { "combat_idle_01", "run_01", 0 },
    
    { "fenriswolf", 0 },
    { "fenriswolf", "fenriswolf_01_miniboss", 0 },
    { "combat_idle_01", "run_01", 0 },

    { "frogman", 0 },
    { "frogman", "frogman_01_miniboss", 0 },
    { "combat_idle_01", "run_01", 0 },

    { "gorgon", 0 },
    { "gorgon", "gorgon_01_miniboss", 0 },
    { "combat_idle_01", "run_01", 0 },
    
    { "horse", 0 },
    { "horse_common_01", "horse_common_02", "horse_common_03", "horse_uncommon_01", "horse_uncommon_02", "horse_uncommon_03", "reindeer_01", "terrorhorse", 0 },
    { "mount_idle_horse_01", "mount_run_horse_01", 0 },

    { "meanie_rascal", 0 },
    { "meanie_pet_squire", "meanie_rascal_01", "meanie_rascal_01_miniboss", "meanie_rascal_02_christmas2012", "meane_rascal_02_christmas2012_miniboss", 0 },
    { "combat_idle_01", "run_01", 0 },
    
    { "meanie_summoner", 0 },
    { "meanie_summoner", "meanie_summoner_miniboss", 0 },
    { "combat_idle_01", "run_01", 0 },
    
    { "miniboss_01_heredur", 0 },
    { "death_knight_01", "miniboss_01_heredur", "miniboss_01_sargon", 0 },
    { "combat_idle_01", "run_01", 0 },
    
    { "miniboss_02_witch", 0 },
    { "miniboss_02_witch", "minispider_01", "seedling_01", 0 },
    { "combat_idle_01", "run_01", 0 },
    
    { "miniboss_03_bear", 0 },
    { "miniboss_03_bear", "e_chr_2012_b1205_dun_miniboss_bear", 0 },
    { "combat_idle_01", "run_01", 0 },
    
    { "mounted_mage", 0 },
    { "mounted_mage_01", 0},
    { "combat_idle_01", "run_01", 0 },
    
    { "norsegnome", 0 },
    { "norsegnome_01", "norsegnome_01_miniboss", "norsegnome_02_christmas2012", "norsegnome_02_christmas2012_miniboss", 0 },
    { "combat_idle_01", "run_01", 0 },

    { "pet_dragon", 0 },
    { "pet_dragon_01", "pet_dragon_02", "pet_dragon_03", "pet_dragon_04", 0 },
    { "combat_idle_01", "run_01", 0 },

    { "pet_filbur", 0 },
    { "pet_filbur", 0 },
    { "combat_idle_01", "run_01", 0 },
    
    { "pet_gnome", 0 },
    { "pet_gnome_01", 0 },
    { "combat_idle_01", "run_01", 0 },
    
    { "pet_meanie", 0 },
    { "meanie_pet_squire", 0 },
    { "combat_idle_01", "run_01", 0 },
    
    { "pet_monkey", 0 },
    { "pet_monkey_01", 0 },
    { "combat_idle_01", "run_01", 0 },
    
    { "pet_phoenix", 0 },
    { "pet_phoenix", "bird_of_prey", 0 },
    { "combat_idle_01", "run_01", 0 },

/*
    { "pet_pixie", 0 },
    { "pet_pixie", 0 },
    { "combat_idle_01", "death", "run_01", "skill_01", "spawn", 0 },
*/
    
    { "pet_pumpkin", 0 },
    { "pet_pumpkin_01", 0 },
    { "combat_idle_01", "run_01", 0 },
    
    { "satyr", 0 },
    { "satyr_melee", "satyr_melee_01_miniboss", 0 },
    { "combat_idle_01", "run_01", 0 },

    { "satyr", 0 },
    { "satyr_ranged", "satyr_ranged_02_miniboss", 0 },
    { "combat_idle_01_ranged", "run_01_ranged", 0 },

    { "seedling", 0 },
    { "seedling_01", 0 },
    { "combat_idle_01", "run_01", "skill_01", 0 },
    
    { "skeleton_archer", 0 },
    { "skeleton_archer", "skeleton_archer_miniboss", "skeleton_archer_02_npc_male", "death_skeleton_archer_01", 0 },
    { "combat_idle_01", "run_01", 0 },
    
    { "skeleton_warrior", 0 },
    { "skeleton_warrior", "skeleton_warrior_04_male_npc", "skeleton_warrior_05_female_npc", "skeleton_warrior_death", "skeleton_warrior_miniboss", 0 },
    { "combat_idle_01", "run_01", 0 },

    { "skeleton_warrior", 0 },
    { "skeleton_warrior_02_greek_defensive", "skeleton_warrior_03_greek_offensive", 0 },
    { "combat_idle_greek_01", "run_greek_01", 0 },
    
    { "soulless_banshee", 0 },
    { "soulless_banshee_01", "soulless_banshee_01_miniboss", 0 },
    { "combat_idle_01", "run_01", 0 },
    
    { "soulless_norseman", 0 },
    { "soulless_norseman", "soulless_norseman_01_miniboss", 0 },
    { "combat_idle_01", "run_01", 0 },
    
    { "thing_roots", 0 },
    { "thing_roots", "thing_roots_miniboss", "thing_roots_02_christmas2012", "thing_roots_02_christmas2012_miniboss", 0 },
    { "combat_idle_01", "run_01", 0 },
    
    { "thing_swamp", 0 },
    { "thing_swamp", "thing_swamp_miniboss", "thing_swamp_crypta_01", "thing_swamp_02_christmas2012", "thing_swamp_02_christmas2012_miniboss", 0 },
    { "combat_idle_01", "run_01", 0 },
    
    { "troll", 0 },
    { "troll", "troll_miniboss", "cyclops", "cyclops_miniboss", "icegolem_01", "icetroll_01", "event_troll_easter", "event_troll_em", "event_troll_valentine", "gnob", "gnob_santa_01", 0 },
    { "combat_idle_01", "run_01", 0 },
        
/*
    { "twiner", 0 },
    { "twiner_01", "twiner_01_miniboss", 0 },
    { "combat_idle_01", "death", "hit", "skill_01", "skill_02", "stunned", 0 },
*/
    
    { "undead_mage", 0 },
    { "undead_mage_01", "undead_mage_01_miniboss", "undead_miller_01", 0},
    { "combat_idle_01", "run_01", 0 },
    
    { "undead_ripper", 0 },
    { "undead_ripper_01", "undead_ripper_01_miniboss", 0},
    { "combat_idle_01", "run_01", 0 },

    { 0 }, { 0 }, { 0 }
};

//------------------------------------------------------------------------------
/**
*/
DSOCharViewerApplication::DSOCharViewerApplication() :
    modelIndex(0),
    skinListIndex(0),
    animIndex(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
 */
void
DSOCharViewerApplication::PrintInfo()
{
/*
    n_printf("--- Version 1 ---\n");
    n_printf("model: %s, skinlist: %s, anim: %s\n", this->CurModel(), this->CurSkinList(), this->CurAnim());
    n_printf("Up/Down Key: next/previous model\n");
    n_printf("Right Key: next skinlist\n");
    n_printf("Left Key: next animation\n");
    n_printf("Mouse Buttons + Drag: Move Camera\n");
*/
}
    
//------------------------------------------------------------------------------
/**
 */
const char*
DSOCharViewerApplication::CurModel()
{
    return Models[this->modelIndex * 3][0];
}

//------------------------------------------------------------------------------
/**
 */
void
DSOCharViewerApplication::NextModel()
{
    this->modelIndex++;
    if (0 == Models[this->modelIndex * 3][0])
    {
        this->modelIndex--;
    }
    this->skinListIndex = 0;
    this->animIndex = 0;
}

//------------------------------------------------------------------------------
/**
 */
void
DSOCharViewerApplication::PrevModel()
{
    if (this->modelIndex > 0)
    {
        this->modelIndex--;
    }
    this->skinListIndex = 0;
    this->animIndex = 0;
}

//------------------------------------------------------------------------------
/**
 */
const char*
DSOCharViewerApplication::CurSkinList()
{
    return Models[this->modelIndex * 3 + 1][this->skinListIndex];
}

//------------------------------------------------------------------------------
/**
 */
void
DSOCharViewerApplication::NextSkinList()
{
    this->skinListIndex++;
    if (0 == Models[this->modelIndex * 3 + 1][this->skinListIndex])
    {
        this->skinListIndex = 0;
    }
}

//------------------------------------------------------------------------------
/**
 */
const char*
DSOCharViewerApplication::CurAnim()
{
    return Models[this->modelIndex * 3 + 2][this->animIndex];
}

//------------------------------------------------------------------------------
/**
 */
void
DSOCharViewerApplication::NextAnim()
{
    this->animIndex++;
    if (0 == Models[this->modelIndex * 3 + 2][this->animIndex])
    {
        this->animIndex = 0;
    }
}

//------------------------------------------------------------------------------
/**
 */
void
DSOCharViewerApplication::ResetCamera()
{
    float distance = 5.0f;
    this->mayaCameraUtil.Setup(point(0.0f, 2.0f, 0.0f), point(0.0f, 1.5f, -distance), vector(0.0f, 1.0f, 0.0f));
    this->mayaCameraUtil.Update();
    this->graphicsFacade->GetDefaultCamera()->Transform()->SetTransform(this->mayaCameraUtil.GetCameraTransform());
}

//------------------------------------------------------------------------------
/**
 */
void
DSOCharViewerApplication::SetupLightEntities()
{
    matrix44 globalLightTransform = matrix44::rotationx(n_deg2rad(-45.0f));
    globalLightTransform = matrix44::multiply(globalLightTransform, matrix44::rotationy(n_deg2rad(-135.0f)));
    this->globalLight = this->graphicsFacade->CreateGlobalLightEntity();
    this->globalLight->Transform()->SetTransform(globalLightTransform);
    this->globalLight->Lighting()->Light().SetColor(float4(1.0f, 1.0f, 1.0f, 1.0f));
    this->globalLight->Lighting()->Light().SetBackColor(float4(0.5f, 0.5f, 0.5f, 1.0f));
    this->globalLight->Lighting()->Light().SetAmbientColor(float4(0.0f, 0.0f, 0.0f, 1.0f));
    this->globalLight->Lighting()->Light().SetSpecularIntensity(1.0f);
    this->globalLight->Lighting()->Light().SetCastShadows(false);
    this->graphicsFacade->GetDefaultStage()->AttachEntity(this->globalLight);
    
/*
    this->pointLight0 = this->graphicsFacade->CreatePointLightEntity();
    this->pointLight0->Lighting()->SetTransformFromPosDirAndRange(point(-2.0f, 3.0f, -1.0f), vector::upvec(), 7.0f);
    this->pointLight0->Lighting()->Light().SetColor(float4(1.5f, 0.8f, 0.0f, 1.0f));
    this->pointLight0->Lighting()->Light().SetSpecularIntensity(0.0f);
    this->pointLight0->Lighting()->Light().SetCastShadows(false);
    this->graphicsFacade->GetDefaultStage()->AttachEntity(pointLight0);
    
    this->pointLight1 = this->graphicsFacade->CreatePointLightEntity();
    this->pointLight1->Lighting()->SetTransformFromPosDirAndRange(point(+2.0f, 1.0f, 0.0f), vector::upvec(), 7.0f);
    this->pointLight1->Lighting()->Light().SetColor(float4(0.7f, 1.5f, 0.0f, 1.0f));
    this->pointLight1->Lighting()->Light().SetSpecularIntensity(0.0f);
    this->pointLight1->Lighting()->Light().SetCastShadows(false);
    this->graphicsFacade->GetDefaultStage()->AttachEntity(this->pointLight1);
*/    
}

//------------------------------------------------------------------------------
/**
 */
void
DSOCharViewerApplication::SetupModelEntities()
{
    Ptr<GraphicsEntity> oldModelEntity = this->modelEntity;

    const char* modelName = this->CurModel();
    n_printf("Model: %s\n", modelName);
    
    const char* skinList = this->CurSkinList();
    const char* anim = this->CurAnim();
    const float4 translate(0.0f, 0.0f, 0.0f, 0.0f);
    String modelPath;
    modelPath.Format("mdl:characters/%s.n3", modelName);
    this->modelEntity = this->CreateCharacter(modelPath, skinList, anim, translate);
    
    if (oldModelEntity.isvalid())
    {
        this->graphicsFacade->GetDefaultStage()->RemoveEntity(oldModelEntity);
        this->graphicsFacade->DiscardEntity(oldModelEntity);
        oldModelEntity = 0;
    }
    this->PrintInfo();
}

//------------------------------------------------------------------------------
/**
 */
void
DSOCharViewerApplication::UpdateSkinList()
{
    const char* skinList = this->CurSkinList();
    n_printf("SkinList: %s\n", skinList);
    Ptr<Graphics2::ApplySkinList> applySkinList = Graphics2::ApplySkinList::Create();
    applySkinList->SetSkinList(skinList);
    this->modelEntity->Character()->PushMessage(applySkinList.cast<CharacterMessage>());
    this->PrintInfo();
}

//------------------------------------------------------------------------------
/**
 */
void
DSOCharViewerApplication::UpdateAnimation()
{
    const char* anim = this->CurAnim();
    n_printf("Animation: %s\n", anim);
    Ptr<Graphics2::AnimPlayClip> playClip = Graphics2::AnimPlayClip::Create();
    playClip->SetClipName(anim);
    playClip->SetTrackIndex(0);
    playClip->SetLoopCount(0.0f);
    this->modelEntity->Character()->PushMessage(playClip.cast<CharacterMessage>());
    this->PrintInfo();
}

//------------------------------------------------------------------------------
/**
 */
void
DSOCharViewerApplication::HandleInput()
{
    EMSCTestApplication::HandleInput();
    
    InputServer* inputServer = InputServer::Instance();
    const Ptr<Keyboard>& keyboard = inputServer->GetDefaultKeyboard();
    
    if (keyboard->KeyDown(Key::Up))
    {
        this->NextModel();
        this->SetupModelEntities();
    }
    if (keyboard->KeyDown(Key::Down))
    {
        this->PrevModel();
        this->SetupModelEntities();
    }
    if (keyboard->KeyDown(Key::Right))
    {
        this->NextSkinList();
        this->UpdateSkinList();
    }
    if (keyboard->KeyDown(Key::Left))
    {
        this->NextAnim();
        this->UpdateAnimation();
    }
}

} // namespace App