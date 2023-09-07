#pragma once
#include "afxdialogex.h"

#include "Include.h"
// CUnitTool 대화 상자

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CUnitTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUnitTool };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnBnClickedButton2();
//	afx_msg void OnAddPlayer();
//	afx_msg void OnListBox();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
//	afx_msg void SaveUnitData();

public:
	// value Type
	CString m_strSearchName;
	CString m_strName;
	int		m_iHp;
	int		m_iAttack;

	// control Tyype
	CListBox m_ListBox;
	CButton m_Radio[3];
	CButton m_Check[3];
	CButton m_Bitmap;

	// general
	map<CString, UNITDATA*>	m_mapUnitData;

//	afx_msg void LoadUnitData();
//	afx_msg void DeleteUnit();
};
