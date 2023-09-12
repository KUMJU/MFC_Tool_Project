#pragma once
#include "Obj.h"
#include "MultiTexture.h"
#include "Include.h"

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

private:
    void Key_Input();
    void Update_Move();
    void Update_State();

private:
    D3DXVECTOR3     m_vDstPos;
    PLAYER_STATE    m_ePrevState;
    PLAYER_STATE    m_eCurState;
};

