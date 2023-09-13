#pragma once

#include "UnitTool.h"
#include "MapTool.h"

// CMyform 폼 보기

class CMyform : public CFormView
{
	DECLARE_DYNCREATE(CMyform)

protected:
	CMyform();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyform();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMyform };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButton1();
	virtual void OnInitialUpdate();
	afx_msg void OnMapTool();

	CUnitTool	m_UnitTool;
	CMapTool	m_MapTool;

private:
	CFont		m_Font;

	
};


