#pragma once
//------------------------------------------------------------------------------
/**
    @class App::DSOCharViewerApplication
    
    Drakensang Online character viewer demo app.
 
    (C) 2012 A.Weissflog
*/
#include "emsctest/base/emsctestapplication.h"

//------------------------------------------------------------------------------
namespace App
{
class DSOCharViewerApplication : public EMSCTestApplication
{
public:
    /// constructor
    DSOCharViewerApplication();

protected:
    /// handle input
    virtual void HandleInput();
    /// setup light entities
    virtual void SetupLightEntities();
    /// create model entities
    virtual void SetupModelEntities();
    /// reset the camera
    virtual void ResetCamera();
    
    /// return current model
    const char* CurModel();
    /// set to next model
    void NextModel();
    /// set to prev model
    void PrevModel();
    /// return current skinlist
    const char* CurSkinList();
    /// set to skinlist
    void NextSkinList();
    /// return current animation
    const char* CurAnim();
    /// set to next animation
    void NextAnim();
    /// update skinlist on model
    void UpdateSkinList();
    /// update animation on model
    void UpdateAnimation();
    /// print usage info
    void PrintInfo();
    
    IndexT modelIndex;
    IndexT skinListIndex;
    IndexT animIndex;
    Ptr<Graphics2::GraphicsEntity> globalLight;
    Ptr<Graphics2::GraphicsEntity> pointLight0;
    Ptr<Graphics2::GraphicsEntity> pointLight1;
    Ptr<Graphics2::GraphicsEntity> modelEntity;
};

} // namespace App
//------------------------------------------------------------------------------
