#pragma once
#include "Scene.h"

class CMyTerrain;
class CObj;

class CTown : public CScene
{
public:
	CTown();
	virtual ~CTown();

public:
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void Late_Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

private:
	CObj* m_pPlayer;
};


