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
#include "TerrainMgr.h"
#include "TextureMgr.h"

// CMapTool 대화 상자

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CMapTool, pParent), m_SelectedTerrain(nullptr)
	, m_RadioMode(0)
{

}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_TileSelecter);
	DDX_Control(pDX, IDC_LIST3, SavedMapListBox);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
	DDX_Radio(pDX, IDC_RADIO1, m_RadioMode);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnListBox)
	ON_BN_CLICKED(IDC_BUTTON4, &CMapTool::OnCreateNewMap)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON9, &CMapTool::SaveMapData)
	ON_LBN_SELCHANGE(IDC_LIST3, &CMapTool::OnSelectTerrain)
	ON_BN_CLICKED(IDC_BUTTON8, &CMapTool::LoadSavedMap)
END_MESSAGE_MAP()


// CMapTool 메시지 처리기

void CMapTool::OnListBox()
{
	UpdateData(TRUE);

	if (m_RadioMode != 1)
		return;

	// 미리보기 창 띄우기용
	CString strFindName;

	int iIndex = m_TileSelecter.GetCurSel();

	if (LB_ERR == iIndex)
		return;

	m_TileSelecter.GetText(iIndex, strFindName);

	auto iter = m_MapPngImg.find(strFindName);

	if (iter == m_MapPngImg.end())
		return;

	m_Picture.SetBitmap(*(iter->second));

	m_strSelectImg = iter->first;

	// 메인 뷰에서 타일변경 적용되도록 설정
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
	pMainView->m_iDrawMode = m_RadioMode;
	pMainView->m_iDrawID = iTileID;

	switch (iTileID)
	{
	case 1:
		pMainView->m_iDrawOption = 1;
		break;
	case 2:
		pMainView->m_iDrawOption = 2;
		break;
	default:
		pMainView->m_iDrawOption = 0;
		break;
	}
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
	//CString strFindName;
	//CSize size;
	//int iWidth = 0;

	//CDC* pDC = m_ListBox.GetDC();

	//for (int i = 0; i < m_ListBox.GetCount(); ++i)
	//{
	//	m_ListBox.GetText(i, strFindName);

	//	size = pDC->GetTextExtent(strFindName);

	//	if (size.cx > iWidth)
	//		iWidth = size.cx;
	//}

	//m_ListBox.ReleaseDC(pDC);

	//// GetHorizontalExtent : 리스트 박스가 가로로 스크롤 할 수 있는 최대 길이를 얻어오는 함수
	//if (iWidth > m_ListBox.GetHorizontalExtent())
	//	m_ListBox.SetHorizontalExtent(iWidth);
}


void CMapTool::OnCreateNewMap()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_CreateMapTool.GetSafeHwnd())
		m_CreateMapTool.Create(IDD_CCreateMapTool);

	m_CreateMapTool.SetMapTool(this);
	m_CreateMapTool.ShowWindow(SW_SHOW);
}


void CMapTool::SaveMapData()
{
	if (m_SelectedTerrain == nullptr) return;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(FALSE, L"dat", L"*.dat",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data File(*.dat) | *.dat ||", this);

	TCHAR szPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\Data\\MapData");
	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		CString str = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = str.GetString();

		m_SelectedTerrain->Save_Data(pGetPath);
	}
}


void CMapTool::OnSelectTerrain()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMapTool::LoadSavedMap()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	int iIndex = SavedMapListBox.GetCurSel();

	if (LB_ERR == iIndex)
		return;

	CString mapKey;
	SavedMapListBox.GetText(iIndex, mapKey);
	CMyTerrain* pTemp = CTerrainMgr::Get_Instance()->Get_Terrain(mapKey.GetString());

	if (pTemp == nullptr)
		return;
	else
	{
		m_SelectedTerrain = pTemp;
		CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
		CToolView* pMainView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
		pMainView->m_pMyTerrain = m_SelectedTerrain;

		pMainView->SetScrollSizes(MM_TEXT, CSize(m_SelectedTerrain->Get_TileCntX() * TILECX, m_SelectedTerrain->Get_TileCntY() * TILECY / 2));

		pMainView->Invalidate();
	}

	UpdateData(FALSE);
}


BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateData(TRUE);

	const vector<wstring>* tempVec = CTerrainMgr::Get_Instance()->Get_TerrainKeys();
	for (auto& iter : *tempVec)
		SavedMapListBox.AddString(iter.c_str());

	CTextureMgr::Get_Instance()->Get_Texture(L"TILE", L"Tile");

	int tileCount = CTextureMgr::Get_Instance()->Get_MulTexCount(L"TILE", L"Tile");
	if (tileCount != -1)
	{
		for (int i = 0; i < tileCount; ++i)
		{
			wstring tileName = L"Tile" + to_wstring(i);
			m_TileSelecter.AddString(tileName.c_str());

			CImage* pPngImg = new CImage;

			wstring path = L"../Texture/Stage/TILE/tile/" + tileName + L".png";
			pPngImg->Load(path.c_str());

			m_MapPngImg.insert({ tileName.c_str(),pPngImg });
		}
	}

	UpdateData(FALSE);

	return TRUE;
}
