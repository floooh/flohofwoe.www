//------------------------------------------------------------------------------
//  dragonsapplication.cc
//  (C) 2012 A.Weissflog
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "dragonsapplication.h"
#include "graphics2/graphicsentity.h"
#include "graphics2/graphicsstage.h"
#include "input/keyboard.h"

namespace App
{
using namespace Math;
using namespace Util;
using namespace Input;
using namespace Graphics2;

//------------------------------------------------------------------------------
/**
 */
DragonsApplication::DragonsApplication() :
    numEntitiesAlongAxis(1)
{
    // empty
}

//------------------------------------------------------------------------------
/**
 */
void
DragonsApplication::ResetCamera()
{
    float distance = (float(this->numEntitiesAlongAxis) * 5.0f) + 5.0f;
    this->mayaCameraUtil.Setup(point(0.0f, 1.0f, 0.0f), point(0.0f, distance * 0.3, -distance), vector(0.0f, 1.0f, 0.0f));
    this->mayaCameraUtil.Update();
    this->graphicsFacade->GetDefaultCamera()->Transform()->SetTransform(this->mayaCameraUtil.GetCameraTransform());
}

//------------------------------------------------------------------------------
/**
 */
void
DragonsApplication::SetupLightEntities()
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
    
    this->pointLight0 = this->graphicsFacade->CreatePointLightEntity();
    this->pointLight0->Lighting()->SetTransformFromPosDirAndRange(point(-2.0f, 1.0f, 0.0f), vector::upvec(), 7.0f);
    this->pointLight0->Lighting()->Light().SetColor(float4(1.5f, 0.8f, 0.0f, 1.0f));
    this->pointLight0->Lighting()->Light().SetSpecularIntensity(0.0f);
    this->pointLight0->Lighting()->Light().SetCastShadows(false);
    this->graphicsFacade->GetDefaultStage()->AttachEntity(pointLight0);
    
    this->pointLight1 = this->graphicsFacade->CreatePointLightEntity();
    this->pointLight1->Lighting()->SetTransformFromPosDirAndRange(point(+2.0f, 1.0f, 0.0f), vector::upvec(), 7.0f);
    this->pointLight1->Lighting()->Light().SetColor(float4(0.0f, 0.5f, 1.5f, 1.0f));
    this->pointLight1->Lighting()->Light().SetSpecularIntensity(0.0f);
    this->pointLight1->Lighting()->Light().SetCastShadows(false);
    this->graphicsFacade->GetDefaultStage()->AttachEntity(this->pointLight1);
}

//------------------------------------------------------------------------------
/**
 */
void
DragonsApplication::SetupModelEntities()
{
    Array<StringAtom> anims;
    anims.Append("combat_idle_01");
    anims.Append("run_01");
    int min = -(this->numEntitiesAlongAxis - 1) / 2;
    int max = +(this->numEntitiesAlongAxis - 1) / 2;
    
    Ptr<GraphicsEntity> modelEntity;
    int i, x, z;
    for (i = 0, x = min; x <= max; x++)
    {
        for (z = min; z<= max; z++, i++)
        {
            float4 translate(float(x) * 6.0f, 0.0f, float(z) * 6.0f, 0.0f);
            if (this->modelEntities.Size() > i)
            {
                // re-use existing entity
                matrix44 m = matrix44::identity(); // matrix44::rotationy(n_deg2rad(180.0f));
                m.translate(translate);
                this->modelEntities[i]->Transform()->SetTransform(m);
            }
            else
            {
                // need to create a new model entity
                modelEntity = this->CreateCharacter("mdl:characters/dragon_brood.n3",
                                                    "dragon_brood_warrior",
                                                    anims[i % anims.Size()],
                                                    translate);
                this->modelEntities.Append(modelEntity);
            }
        }
    }
    
    // discard surplus entities
    while (this->modelEntities.Size() > i)
    {
        this->graphicsFacade->GetDefaultStage()->RemoveEntity(this->modelEntities.Back());
        this->graphicsFacade->DiscardEntity(this->modelEntities.Back());
        this->modelEntities.EraseIndex(this->modelEntities.Size() - 1);
    }
    
    n_printf("%d dragon(s)\n", this->modelEntities.Size());
    n_printf("--- Version 3 ---\n");
    n_printf("Up Key: More Dragons\n");
    n_printf("Down Key: Less Dragons\n");
    n_printf("Mouse Buttons + Drag: Move Camera\n");
}

//------------------------------------------------------------------------------
/**
 */
void
DragonsApplication::HandleInput()
{
    EMSCTestApplication::HandleInput();
    
    InputServer* inputServer = InputServer::Instance();
    const Ptr<Keyboard>& keyboard = inputServer->GetDefaultKeyboard();
    
    if (keyboard->KeyDown(Key::Up))
    {
        // more dragons
        this->numEntitiesAlongAxis += 2;
        this->SetupModelEntities();
        this->ResetCamera();
    }
    if (keyboard->KeyDown(Key::Down))
    {
        // less dragons
        if (this->numEntitiesAlongAxis > 1)
        {
            this->numEntitiesAlongAxis -= 2;
            this->SetupModelEntities();
            this->ResetCamera();
        }
    }    
}
    
} // namespace App
