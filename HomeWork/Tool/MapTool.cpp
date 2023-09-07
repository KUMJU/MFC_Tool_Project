// MapTool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "MapTool.h"
#include "FileInfo.h"
#include "MiniView.h"
#include "ToolView.h"
#include "MyTerrain.h"
#include "MainFrm.h"

// CMapTool 대화 상자

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CMapTool, pParent)
{

}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnListBox)
	ON_WM_DESTROY()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::SelectTile)
END_MESSAGE_MAP()


// CMapTool 메시지 처리기


void CMapTool::OnListBox()
{
	UpdateData(TRUE);

	CString strFindName;

	int iIndex = m_ListBox.GetCurSel();

	if (LB_ERR == iIndex)
		return;

	m_ListBox.GetText(iIndex, strFindName);

	auto iter = m_MapPngImg.find(strFindName);

	if (iter == m_MapPngImg.end())
		return;

	m_Picture.SetBitmap(*(iter->second));

	m_strSelectImg = iter->first;

	UpdateData(FALSE);
}


void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UpdateData(TRUE);
	CDialog::OnDropFiles(hDropInfo);

	TCHAR szFilePath[MAX_PATH] = L"";
	TCHAR szFileName[MIN_STR] = L"";

	// DragQueryFile : 드롭된 파일의 정보를 얻어오는 함수
	// 두 번째 매개 변수 : 0xffffffff(-1)을 지정하면 드롭된 파일의 개수를 반환 
	int iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);

		CString strRelative = CFileInfo::ConvertRelativePath(szFilePath);
		CString strFileName = PathFindFileName(strRelative);

		lstrcpy(szFileName, strFileName.GetString());

		PathRemoveExtension(szFileName);
		strFileName = szFileName;

		auto iter = m_MapPngImg.find(strFileName);

		if (iter == m_MapPngImg.end())
		{
			CImage* pPngImg = new CImage;

			pPngImg->Load(strRelative);

			m_MapPngImg.insert({ strFileName,pPngImg });
			m_ListBox.AddString(szFileName);
		}
	}

	Horizontal_Scroll();

	UpdateData(FALSE);
}


void CMapTool::OnDestroy()
{
	for_each(m_MapPngImg.begin(), m_MapPngImg.end(), [](auto& MyPair)
		{
			MyPair.second->Destroy();
			Safe_Delete(MyPair.second);
		});

	m_MapPngImg.clear();

	CDialog::OnDestroy();
}


void CMapTool::Horizontal_Scroll()
{
	CString strFindName;
	CSize size;
	int iWidth = 0;

	CDC* pDC = m_ListBox.GetDC();

	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		m_ListBox.GetText(i, strFindName);

		size = pDC->GetTextExtent(strFindName);

		if (size.cx > iWidth)
			iWidth = size.cx;
	}

	m_ListBox.ReleaseDC(pDC);

	// GetHorizontalExtent : 리스트 박스가 가로로 스크롤 할 수 있는 최대 길이를 얻어오는 함수
	if (iWidth > m_ListBox.GetHorizontalExtent())
		m_ListBox.SetHorizontalExtent(iWidth);
}


void CMapTool::SelectTile()
{
	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	CToolView* pMainView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	if (nullptr == pMainView)
		return;

	CString cStrNumber = L"";
	for (int i = 0; i < m_strSelectImg.GetLength(); ++i)
	{
		TCHAR tch = m_strSelectImg.GetAt(i);
		if (tch >= L'0' && tch <= L'9')
			cStrNumber += tch;
	}

	int iTileID = _ttoi(cStrNumber);
	pMainView->m_iDrawID = iTileID;

	UpdateData(FALSE);
}
