// CreateMapTool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "CreateMapTool.h"
#include "TextureMgr.h"
#include "FileInfo.h"
#include "MapTool.h"
#include "MyTerrain.h"
#include "Include.h"
#include "TerrainMgr.h"
#include "MainFrm.h"
#include "ToolView.h"

// CCreateMapTool 대화 상자

IMPLEMENT_DYNAMIC(CCreateMapTool, CDialog)

CCreateMapTool::CCreateMapTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CCreateMapTool, pParent)
	, m_MapName(_T(""))
{
}

CCreateMapTool::~CCreateMapTool()
{
}

void CCreateMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, m_SampleImg);
	DDX_Control(pDX, IDC_LIST1, m_MapImgList);
	DDX_Control(pDX, IDC_IMGSIZE, m_ImageSizeText);
	DDX_Control(pDX, IDC_TILESIZE, m_TileNumText);
	DDX_Text(pDX, IDC_EDIT1, m_MapName);
}


BEGIN_MESSAGE_MAP(CCreateMapTool, CDialog)
	ON_BN_CLICKED(IDOK2, &CCreateMapTool::OnCreateMapImg)
	ON_LBN_SELCHANGE(IDC_LIST1, &CCreateMapTool::OnBackImgBox)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CCreateMapTool 메시지 처리기


void CCreateMapTool::OnCreateMapImg()
{
	UpdateData(TRUE);

	if (m_MapTool == nullptr)
	{
		AfxMessageBox(L"맵툴없다아앙ㅅ아아아ㄱㅈ아아아앙ㅂ아ㄱㄷ아");
		return;
	}
		
	wstring mapKey = m_MapName.GetString();

	if (FAILED(CTerrainMgr::Get_Instance()->CreateTerrain(mapKey, m_iBgCount, m_iTileX, m_iTileY)))
	{
		AfxMessageBox(L"이미 있는 이름입니다");
	}
	else
	{
		AfxMessageBox(L"맵을 생성하였습니다");
		m_MapTool->SavedMapListBox.AddString(mapKey.c_str());
	}

	UpdateData(FALSE);
}


BOOL CCreateMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	UpdateData(TRUE);

	int tileCount = CTextureMgr::Get_Instance()->Get_MulTexCount(L"Map", L"map");
	if (tileCount != -1)
	{
		for (int i = 0; i < tileCount; ++i)
		{
			wstring tileName = L"Map" + to_wstring(i);
			m_MapImgList.AddString(tileName.c_str());

			CImage* pPngImg = new CImage;

			wstring path = L"../Texture/Stage/Map/map/" + tileName + L".png";
			pPngImg->Load(path.c_str());

			m_MapPngImg.insert({ tileName.c_str(),pPngImg });

		}
	}

	UpdateData(FALSE);

	return TRUE;  
}


void CCreateMapTool::OnBackImgBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_iBgCount = m_MapImgList.GetCurSel();

	const TEXINFO* texInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Map", L"map", m_iBgCount);
	if (texInfo == nullptr) return;

	int iWidth = texInfo->tImgInfo.Width;
	int iHeight = texInfo->tImgInfo.Height;
	wstring tileSize = to_wstring(iWidth) + L" X " + to_wstring(iHeight);
	m_ImageSizeText.SetWindowText(tileSize.c_str());	

	m_iTileX = iWidth / TILECX;
	m_iTileY = iHeight / (TILECY * 0.5f);
	wstring tileNum = to_wstring(m_iTileX) + L" X " + to_wstring(m_iTileY);
	m_TileNumText.SetWindowText(tileNum.c_str());

	CString ImgName = L"";
	m_MapImgList.GetText(m_iBgCount, ImgName);
	
	CRect rect;//픽쳐 컨트롤의 크기를 저장할 CRect 객체
	m_SampleImg.GetWindowRect(rect);//GetWindowRect를 사용해서 픽쳐 컨트롤의 크기를 받는다.
	CDC* dc; //픽쳐 컨트롤의 DC를 가져올  CDC 포인터
	dc = m_SampleImg.GetDC(); //픽쳐 컨트롤의 DC를 얻는다.
	dc->SetStretchBltMode(HALFTONE);
	m_MapPngImg[ImgName.GetString()]->StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//이미지를 픽쳐 컨트롤 크기로 조정
	ReleaseDC(dc);//DC 해제

	UpdateData(FALSE);
}


void CCreateMapTool::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	for_each(m_MapPngImg.begin(), m_MapPngImg.end(), [](auto& MyPair) 
		{
			MyPair.second->Destroy();
			Safe_Delete(MyPair.second);
		});
	m_MapPngImg.clear();
}
