#include "pch.h"
#include "Obj.h"
#include "TimeMgr.h"
//#include "TimeMgr.h"

D3DXVECTOR3 CObj::m_vScroll{};

CObj::CObj():m_fTime(0.f)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	D3DXMatrixIdentity(&m_tInfo.matWorld);

	m_tInfo.vLook = { 1.f, 0.f, 0.f };
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}


CObj::~CObj()
{
}


void CObj::Move_Frame(void)
{
	m_fTime += CTimeMgr::Get_Instance()->Get_TimeDelta();
	if (m_fTime > m_tFrame.fSpeed)
	{
		m_fTime = 0.f;
		m_tFrame.fFrame++;
		if (m_tFrame.fFrame >= m_tFrame.fMax)
			m_tFrame.fFrame = 0;
	}
}
