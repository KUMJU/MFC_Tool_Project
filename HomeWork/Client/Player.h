#pragma once
#include "Obj.h"
#include "MultiTexture.h"
#include "Include.h"

enum PlayerState{RUN_UP, RUN_DOWN, STAND_UP, STAND_DOWN};

class CPlayer :
    public CObj
{
public:
    CPlayer();
    virtual ~CPlayer();

public:
    // CObj을(를) 통해 상속됨
    HRESULT Initialize(void) override;
    int Update(void) override;
    void Late_Update(void) override;
    void Render(void) override;
    void Release(void) override;

public:
    void Stop_Astar();

private:
    void Key_Input();
    void Update_State();

    void Move_Astar();
private:
    D3DXVECTOR3     m_vDstPos;
    PlayerState     m_ePrevState;
    PlayerState     m_eCurrentState;
};

