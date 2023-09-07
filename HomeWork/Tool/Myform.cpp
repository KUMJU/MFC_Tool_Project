// Myform.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "Myform.h"


// CMyform

IMPLEMENT_DYNCREATE(CMyform, CFormView)

CMyform::CMyform()
	: CFormView(IDD_CMyform)
{

}

CMyform::~CMyform()
{
}

void CMyform::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyform, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyform::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CMyform::OnMapTool)
END_MESSAGE_MAP()


// CMyform 진단

#ifdef _DEBUG
void CMyform::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyform::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyform 메시지 처리기


void CMyform::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_UnitTool.GetSafeHwnd())
		m_UnitTool.Create(IDD_CUnitTool);

	m_UnitTool.ShowWindow(SW_SHOW);
}


void CMyform::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_Font.CreatePointFont(180, L"둥근모꼴");
	GetDlgItem(IDC_BUTTON1)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON4)->SetFont(&m_Font);	
}


void CMyform::OnMapTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_MapTool.GetSafeHwnd())
		m_MapTool.Create(IDD_CMapTool);

	m_MapTool.ShowWindow(SW_SHOW);	
}
