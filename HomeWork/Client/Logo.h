#pragma once
#include "Scene.h"
class CLogo :
    public CScene
{
public:
    CLogo();
    ~CLogo();

public:
    HRESULT Ready_Scene() override;
    void Update_Scene() override;
    void Late_Update_Scene() override;
    void Render_Scene() override;
    void Release_Scene() override;

private:

};

