
// ToolView.cpp: CToolView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "MiniView.h"
#include "TerrainMgr.h"
#include "TextureMgr.h"
#include"TimeMgr.h"
#include"MapTool.h"
#include"Myform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND	g_hWnd;

// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView() : m_pMyTerrain(nullptr), m_iDrawID(0), m_iDrawOption(0), m_iDrawMode(0)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView()
{
}

#pragma region 숨김
// CToolView 진단

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG

#pragma endregion 숨김
// CToolView 메시지 처리기

void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	SetScrollSizes(MM_TEXT, CSize(TILEX * TILECX, TILEY * TILECY / 2));

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT	rcWnd{};

	// 메인 윈도우의 rect정보받아오기
	pMainFrm->GetWindowRect(&rcWnd);

	SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);
	RECT rcMainView{};

	// 현재 view창의 렉트정보받아오기
	GetClientRect(&rcMainView);

	float fRowFrm = float(rcWnd.right - rcMainView.right);
	float fColFrm = float(rcWnd.bottom - rcMainView.bottom);

	// view창과 메인윈도우의 rect차이를 이용하여 윈도우의 위치와 크기 조정
	pMainFrm->SetWindowPos(nullptr, 0, 0, int(WINCX + fRowFrm), int(WINCY + fColFrm), SWP_NOZORDER);
	g_hWnd = m_hWnd;

	if (FAILED(CDevice::Get_Instance()->Init_Device()))
	{
		AfxMessageBox(L"CDevice Init FAILED");
		return;
	}

	SetTimer(2, 50, NULL);


	CTerrainMgr::Get_Instance()->Initialize(this);

	CTextureMgr::Get_Instance()->ReadImgPath(L"../Data/ImgPath.txt" , &m_pAnimInfo);
}


// CToolView 그리기

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CDevice::Get_Instance()->Render_Begin();

	if (m_pMyTerrain != nullptr)
		m_pMyTerrain->Render();


	D3DXMATRIX  matWorld, matScale, matTrans;

	//마우스 프리뷰
	if ("" != currentObjKey.strObjKey && m_iDrawMode == 0) {
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans,
			m_MousePos.x,
			m_MousePos.y,
			0.f);

		matWorld = matScale * matTrans;
		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(currentObjKey.strObjKey, currentObjKey.strStateKey, currentObjKey.iCount);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		D3DXVECTOR3 vCenter(fCenterX, fCenterY, 0.f);
		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&vCenter,
			nullptr,
			D3DCOLOR_ARGB(125, 255, 255, 255));

	}


	//이미 생성된 객체 
	for (auto& iter: m_ObjList) {

		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans,
			iter.vPos.x - GetScrollPos(0),
			iter.vPos.y - GetScrollPos(1),
			0.f);

		matWorld = matScale * matTrans;
		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(iter.strObjKey, iter.strStateKey, iter.iCount);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		D3DXVECTOR3 vCenter(fCenterX, fCenterY, 0.f);
		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&vCenter,
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}


	CDevice::Get_Instance()->Render_End();
}

void CToolView::OnDestroy()
{
	CScrollView::OnDestroy();
	CTimeMgr::Destroy_Instance();
	CTerrainMgr::Destroy_Instance();
	CTextureMgr::Destroy_Instance();
	CDevice::Destroy_Instance();
}


void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnLButtonDown(nFlags, point);

	switch (m_iDrawMode)
	{

	case 0: // 오브젝트 모드 
	{

		//여기서 리스트 세팅 

		if (currentObjKey.strObjKey == "")
			return;

		m_ObjList.push_back({ currentObjKey.strObjKey , currentObjKey.strStateKey, currentObjKey.IsReverse, {m_MousePos.x + GetScrollPos(0), m_MousePos.y + GetScrollPos(1),0.f} , currentObjKey.iCount });


		CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CMyform* pMyform = dynamic_cast<CMyform*>(pMain->m_SecondSplitter.GetPane(1, 0));

		pMyform->m_MapTool.InsertNewObj(currentObjKey.strObjKey, { m_MousePos.x + GetScrollPos(0), m_MousePos.y + GetScrollPos(1),0.f });

		//maptool list에 insert



		break;
	}
	case 1: // 타일옵션 모드{
	{
		if (m_pMyTerrain == nullptr)
			return;

		m_pMyTerrain->Tile_Change(D3DXVECTOR3(point.x + GetScrollPos(0), point.y + GetScrollPos(1), 0.f), m_iDrawID, m_iDrawOption);

		Invalidate(FALSE);

		CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMain->m_SecondSplitter.GetPane(0, 0));

		pMiniView->Invalidate(FALSE);
	}
	break;
	}
}


void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnMouseMove(nFlags, point);

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		switch (m_iDrawMode)
		{
		case 0:

			break;

		case 1: // 타일옵션 모드
		{
			if (m_pMyTerrain == nullptr)
				return;

			m_pMyTerrain->Tile_Change(D3DXVECTOR3(point.x + GetScrollPos(0), point.y + GetScrollPos(1), 0.f), m_iDrawID, m_iDrawOption);

			Invalidate(FALSE);

			CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
			CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMain->m_SecondSplitter.GetPane(0, 0));

			pMiniView->Invalidate(FALSE);
		}
		break;
		}

	}
}

//리스트 중 일부 삭제
void CToolView::DeleteObj(int _count)
{

	auto iter = m_ObjList.begin();
	for (int i = 0; i < _count; ++i) {
		++iter;
	}
	
	m_ObjList.erase(iter);

}

//리스트 전부 삭제
void CToolView::DeleteObjAll()
{
	m_ObjList.clear();
}

void CToolView::SetCurrentObjTarget(CString _objKey)
{
	currentObjKey.strObjKey = _objKey;
	currentObjKey.iCount = 0;

	if ("BlueWolf" == _objKey || "Boss" == _objKey) {
		currentObjKey.strStateKey = "STAND_LDOWN";
	}
	else if ("Benya" == _objKey || "SMITHKEEPER" == _objKey || "STOREKEEPER" == _objKey) {
		currentObjKey.strStateKey = "STAND_DOWN";
	}
	else if ("WingPig" == _objKey) {
		currentObjKey.strStateKey = "LD";
	}
	else {

		currentObjKey.strObjKey = "Object";

		//CString Obj
		CString strNumber(_T(""));
		for (int i = 0; i < _objKey.GetLength(); ++i) {

			char ch = _objKey.GetAt(i);


			if (ch >= '0' && ch <= '9')
				strNumber += ch;

		}

		_objKey.Replace(strNumber, L"");
		int iNum = _ttoi(strNumber);

		currentObjKey.strStateKey = _objKey;
		currentObjKey.iCount = iNum;
	}



}

void CToolView::SaveObjList(CString DataSavePath)
{
	CString fileName = DataSavePath.Right(DataSavePath.GetLength() - DataSavePath.ReverseFind(_T('\\')) - 1);
	fileName = L"..\\Data\\ObjectData\\" + fileName;

	HANDLE hFile = CreateFile(fileName, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD dwByte(0), dwStrByte(0);



	for (auto& iter : m_ObjList) {

		wstring temp1 = iter.strObjKey.GetString();
		dwStrByte = sizeof(temp1) * sizeof(TCHAR);
		WriteFile(hFile, &(dwStrByte), sizeof(DWORD), &dwByte, nullptr);
		WriteFile(hFile, iter.strObjKey.GetString(), dwStrByte, &dwByte, nullptr);

		temp1 = iter.strObjKey.GetString();
		dwStrByte = sizeof(temp1) * sizeof(TCHAR);
		WriteFile(hFile, &(dwStrByte), sizeof(DWORD), &dwByte, nullptr);
		WriteFile(hFile, iter.strObjKey.GetString(), dwStrByte, &dwByte, nullptr);
		
		WriteFile(hFile, &(iter.iCount), sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &(iter.IsReverse), sizeof(bool), &dwByte, nullptr);
		WriteFile(hFile, &(iter.vPos), sizeof(D3DXVECTOR3), &dwByte, nullptr);
	}

	CloseHandle(hFile);
	AfxMessageBox(L"Save Success!");


}


void CToolView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case 2: {
		Invalidate(FALSE);
		break;
	}
	default:
		break;
	}

	CScrollView::OnTimer(nIDEvent);
}
