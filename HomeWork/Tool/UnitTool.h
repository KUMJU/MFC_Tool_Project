#pragma once
#include "afxdialogex.h"

#include "Include.h"
#include"MiniAnimView.h"
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

	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();

	//init 때 디렉토리를 읽고 트리뷰를 세팅
	void SetTreeItems(CString _path, int iCount, HTREEITEM _item);

public:
	//트리 아이템 선택 시 동작
	afx_msg void OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult); // 트리뷰 아이템 선택
	afx_msg void OnBnClickedButton1();
	afx_msg void OnSelectList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAnimPlay();
	afx_msg void OnDropDownSelChange();


	void SetStateKeyList(CString _objKey, HTREEITEM _item);  // 특정 오브젝트 키를 선택했을 때 맵에 있는 자료를 꺼내 채워주는 함수
	void SettingStateList(CString _objKey, HTREEITEM _item); // 특정 오브젝트 키의 맵이 비어있을 때 채워주는 함수
	int SetStateImg(CString _path , CString _stateKey, CString _objKey);
	//void ChangeStateKeyItem(); //스테이트키 바뀔때를 따로 나눠야 편할듯

	//트리뷰 컨트롤러
	CTreeCtrl m_Tree;
	CListCtrl m_ListCtrl;
	CComboBox m_ComboBox;


	CString m_ObjKey;
	int m_iA; // A 입력 변수
	int m_iR; // R 입력 변수
	int m_iG; // G 입력 변수
	int m_iB; // B 입력 변수
	float fSpeed; // Speed 입력 변수

	//이미지 정보 출력 (static text)
	float m_fAnimTime;
	int m_iImgCount;
	CString m_strImgSize;
	CString m_strTexType;
	

	map<CString, vector<SPRITEINFO*>> m_SpriteMap;
	list<HTREEITEM> m_ObjKeyList;

	FRAME m_currentFrame;
	float m_ActTime; 

	CButton m_PlayButton;
	CEdit m_SpeedControl; //스피드 입력칸 컨트롤러

	CSliderCtrl m_SliderA;
	CSliderCtrl m_SliderR;
	CSliderCtrl m_SliderG;
	CSliderCtrl m_SliderB;

private:
	
	CMiniAnimView* m_pAnimView; //애니메이션 미리보기창

public:
	afx_msg void OnSpeedChange();
	afx_msg void OnAlphaChange();
	afx_msg void OnRedChange();
	afx_msg void OnGreenChange();
	afx_msg void OnBlueChange();
//	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
