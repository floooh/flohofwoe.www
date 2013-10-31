//------------------------------------------------------------------------------
//  appteststatehandler.cc
//  (C) 2013 Bigpoint GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "appteststatehandler.h"
#include "game/managers/factorymanager.h"
#include "game/managers/entitymanager.h"
#include "managers/focusmanager.h"
#include "graphicsattr/graphicsattributes.h"
#include "properties/lightproperty.h"
#include "cdlodterrain/featureunit/cdlodterrainproperty.h"
#include "graphics2/graphicsfacade.h"
#include "graphics2/graphicsentity.h"

namespace App
{
__ImplementClass(App::AppTestStateHandler, 'ATSH', App::StateHandler);

using namespace Math;
using namespace Game;
using namespace App;
using namespace Graphics2;
using namespace GraphicsFeature;

//------------------------------------------------------------------------------
/**
 */
void
AppTestStateHandler::OnSetupPreloadQueue()
{
    this->AddPreloadFile("export:maps/got.db4", IO::IoChannel::Normal);
    this->AddPreloadFile("export:maps/got.map", IO::IoChannel::Normal);
    GameStateHandler::OnSetupPreloadQueue();
}

//------------------------------------------------------------------------------
/**
 */
bool
AppTestStateHandler::OnStart()
{
    if (GameStateHandler::OnStart())
    {
        FactoryManager* factoryManager = FactoryManager::Instance();
        EntityManager* entityManager = EntityManager::Instance();
        FocusManager* focusManager = FocusManager::Instance();
        
        // find the camera start point (this is a bit of a hack)
        point startPoint(0.0f, 10.0f, 0.0f);
        Ptr<Entity> startPointEntity = entityManager->GetEntityByAttr("Simple", Attr::Attribute(Attr::Name, "StartPoint"));
        if (startPointEntity.isvalid())
        {
            startPoint = startPointEntity->GetMatrix44(Attr::Transform).get_position();
        }
        startPoint.set_y(5.0f);

        // create a camera entity and make it the focus entity,
        // the placement isn't very nice
        Ptr<Entity> cameraEntity = factoryManager->CreateEntity("Camera");
        cameraEntity->SetMatrix44(Attr::Transform, matrix44::translation(startPoint));
        entityManager->AttachEntity(cameraEntity);
        focusManager->SetFocusEntity(cameraEntity);
        
        return true;
    }
    return false;
}
    
} // namespace App