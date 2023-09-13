// MiniAnimView.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "MiniAnimView.h"
#include"TimeMgr.h"
#include<time.h>
#include"TextureMgr.h"
#include"Device.h"


// CMiniAnimView

IMPLEMENT_DYNCREATE(CMiniAnimView, CView)

CMiniAnimView::CMiniAnimView()
{

}

CMiniAnimView::~CMiniAnimView()
{
}

BEGIN_MESSAGE_MAP(CMiniAnimView, CView)
	ON_WM_MOUSEACTIVATE()
	ON_WM_TIMER()
	ON_WM_CREATE()
END_MESSAGE_MAP()


void CMiniAnimView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	ZeroMemory(&m_currentAnimInfo, sizeof(SPRITEINFO));
	ZeroMemory(&m_currentFrame, sizeof(FRAME));

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}

// CMiniAnimView 그리기

void CMiniAnimView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

	if (!m_IsSelectSprite)
		return;

	D3DXMATRIX  matWorld, matScale, matTrans;

	RECT    rc{};
	GetClientRect(&rc);

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 4.f, 3.f, 0.f);
	D3DXMatrixTranslation(&matTrans,
		350.f,
		350.f,
		0.f);

	matWorld = matScale * matTrans;

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CDevice::Get_Instance()->Render_Begin();

	int iCount = (int)m_currentFrame.fFrame;
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(m_currentAnimInfo.strObjKey, m_currentAnimInfo.strStateKey, (int)m_currentFrame.fFrame);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	D3DXVECTOR3 vCenter(fCenterX, fCenterY, 0.f);
	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&vCenter,
		nullptr,
		D3DCOLOR_ARGB( m_currentAnimInfo.iA, m_currentAnimInfo.iR, m_currentAnimInfo.iG, m_currentAnimInfo.iB));

	CDevice::Get_Instance()->Render_End(m_hWnd);

	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CMiniAnimView 진단

#ifdef _DEBUG
void CMiniAnimView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniAnimView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMiniAnimView 메시지 처리기


int CMiniAnimView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}


//여기 안에서 시간을 잰 다음에 moveFrame해주기
void CMiniAnimView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	switch (nIDEvent)
	{
	case 1: {
		CTimeMgr::Get_Instance()->Update();
		MoveFrame();
		Invalidate(FALSE);
		break;
	}
	default:
		break;
	}

	CView::OnTimer(nIDEvent);
}



int CMiniAnimView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	SetTimer(1, 1, NULL);
	CTimeMgr::Get_Instance()->Initialize();

	return 0;
}

void CMiniAnimView::MoveFrame()
{
	if (0.f == m_currentAnimInfo.fSpeed || !m_IsAnimPlay)
		return;


	m_fActTime += CTimeMgr::Get_Instance()->Get_TimeDelta() * 2;

	if (m_fActTime >= m_currentAnimInfo.fSpeed) {
		m_fActTime = 0.f;

		m_currentFrame.fFrame += 1.f;

		if (m_currentFrame.fFrame == m_currentFrame.fMax)
			m_currentFrame.fFrame = 0.f;
	}
}
