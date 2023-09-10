#pragma once
#include "afxdialogex.h"
#include "Include.h"

// CCreateMapTool 대화 상자
class CMapTool;

class CCreateMapTool : public CDialog
{
	DECLARE_DYNAMIC(CCreateMapTool)

public:
	CCreateMapTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CCreateMapTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CCreateMapTool };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCreateMapImg();
	afx_msg void OnBackImgBox();
	afx_msg void OnDestroy();

public:
	virtual BOOL OnInitDialog();
	void	SetMapTool(CMapTool* mapTool)
	{
		m_MapTool = mapTool;
	}

public:
	// 다이얼로그
	CMapTool*  m_MapTool = nullptr;

	// 컨트롤 유닛
	CStatic m_SampleImg;
	CListBox m_MapImgList;
	CStatic m_ImageSizeText;
	CStatic m_TileNumText;
	CString m_MapName;

	// 일반 변수
	wstring m_wstrImgKey = L"";
	int m_iBgCount = 0;
	int m_iTileX = 0;
	int m_iTileY = 0;

	map<CString, CImage*>	m_MapPngImg;
};
