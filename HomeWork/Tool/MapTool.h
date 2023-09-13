#pragma once
#include "afxdialogex.h"
#include "CreateMapTool.h"

// CMapTool 대화 상자
class CMyTerrain;

class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMapTool();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMapTool };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnListBox();
	afx_msg void OnDestroy();
	afx_msg void OnCreateNewMap();
	afx_msg void SaveMapData();
	afx_msg void OnSelectTerrain();
	afx_msg void LoadSavedMap();
	afx_msg void OnObjSelect();

	virtual BOOL OnInitDialog();

	void Horizontal_Scroll();
	void SetObjList(CString _path, CString _prePathName);

public:
	CCreateMapTool m_CreateMapTool;

	CListBox m_TileSelecter;

	map<CString, CImage*>	m_MapPngImg;
	CString	m_strSelectImg;
	CStatic m_Picture;

	CListBox SavedMapListBox;
	CListBox m_ObjList;
	CListCtrl m_ObjRenderList;

	int m_RadioMode;

	CMyTerrain* m_SelectedTerrain;
	CString m_strSelectedMapName = L"";

public:
	void InsertNewObj(CString _objKey , D3DXVECTOR3 _pos);
	afx_msg void OnDeleteObj();
	afx_msg void OnDeleteObjAll();
};
