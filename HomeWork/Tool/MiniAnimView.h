#pragma once

#include"Include.h"
// CMiniAnimView 보기

class CMiniAnimView : public CView
{
	DECLARE_DYNCREATE(CMiniAnimView)

protected:
	CMiniAnimView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMiniAnimView();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnInitialUpdate();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

public:

	void MoveFrame();
	void SetAnimInfo(SPRITEINFO _info) { 
		m_currentAnimInfo = _info; 
		m_currentFrame.fFrame = 0.f;
		m_currentFrame.fMax = _info.iCount;
	}


	void PlayAnimation() { m_IsAnimPlay = !m_IsAnimPlay; }
	void SelectSprite(bool _value) { m_IsSelectSprite = _value; }

private:

	FRAME m_currentFrame;
	SPRITEINFO m_currentAnimInfo;

	float m_fActTime;

	BOOL m_IsAnimPlay = false;
	BOOL m_IsSelectSprite = false;

};


