// AnSeeView.cpp : implementation of the CAnSeeView class
//

#include "stdafx.h"
#include "AnSee.h"

#include "AnSeeDoc.h"
#include "AnSeeView.h"
#include "TimeDialog.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnSeeView

IMPLEMENT_DYNCREATE(CAnSeeView, CView)

BEGIN_MESSAGE_MAP(CAnSeeView, CView)
	//{{AFX_MSG_MAP(CAnSeeView)
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnSeeView construction/destruction

CAnSeeView::CAnSeeView()
{
	m_originpoint.x = 0;
	m_originpoint.y = 0;
	m_movepoint.x = 0;
	m_movepoint.y = 0;
	m_movingpoint.x = 0;
	m_movingpoint.y = 0;
	m_nTimer = 0;
}

CAnSeeView::~CAnSeeView()
{
}

BOOL CAnSeeView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CAnSeeView drawing

void CAnSeeView::OnDraw(CDC* pDC)
{
	RECT rt;
	GetClientRect( &rt );
	pDC->BitBlt( 0, 0, rt.right, rt.bottom, &BufferDC, 0, 0, SRCCOPY );
}

void CAnSeeView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	CAnSeeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CDC *pDC = GetDC();
	BufferDC.CreateCompatibleDC(pDC);
	CBitmap bmpBuffer;
	RECT rt;
	GetClientRect(&rt);
	bmpBuffer.CreateCompatibleBitmap(pDC, rt.right, rt.bottom);
	BufferDC.SelectObject(&bmpBuffer);
	bmpBuffer.DeleteObject();
	pDoc->SetRightView( this, &BufferDC );
	ReleaseDC(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CAnSeeView printing

BOOL CAnSeeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CAnSeeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CAnSeeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CAnSeeView diagnostics

#ifdef _DEBUG
void CAnSeeView::AssertValid() const
{
	CView::AssertValid();
}

void CAnSeeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAnSeeDoc* CAnSeeView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAnSeeDoc)));
	return (CAnSeeDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAnSeeView message handlers

void CAnSeeView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CView::OnRButtonDown(nFlags, point);
	ClientToScreen( &point );
	CMenu menu;
	menu.CreatePopupMenu();
	menu.AppendMenu( MF_STRING, ID_FILE_OPEN, "Pak파일 열기\t(Ctrl+&O)" );
	CAnSeeDoc* pDoc = GetDocument();
	if( pDoc->m_pSpr[pDoc->m_view] == NULL )
	{
		menu.AppendMenu( MF_GRAYED, ID_FILE_SAVE, "ASD파일로 압축풀기\t(Ctrl+&S)" );
		menu.AppendMenu( MF_GRAYED, ID_EDIT_COPY, "SPD와 BMP로 압축풀기\t(Ctrl+&C)" );
		menu.AppendMenu( MF_GRAYED, ID_EDIT_PASTE, "PAK파일 만들기\t(Ctrl+&V)" );
		menu.AppendMenu( MF_GRAYED, ID_EDIT_CUT, "ASD파일 바꿔치기\t(Ctrl+&X)" );
	}
	else
	{
		menu.AppendMenu( MF_STRING, ID_FILE_SAVE, "ASD파일로 압축풀기\t(Ctrl+&S)" );
		menu.AppendMenu( MF_STRING, ID_EDIT_COPY, "SPD와 BMP로 압축풀기\t(Ctrl+&C)" );
		menu.AppendMenu( MF_STRING, ID_EDIT_PASTE, "PAK파일 만들기\t(Ctrl+&V)" );
		menu.AppendMenu( MF_STRING, ID_EDIT_CUT, "ASD파일 바꿔치기\t(Ctrl+&X)" );
	}
	SetForegroundWindow();
	menu.TrackPopupMenu( TPM_LEFTALIGN, point.x, point.y, this);
}

void CAnSeeView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CAnSeeDoc* pDoc = GetDocument();
	pDoc->OnKeyDown( nChar );
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CAnSeeView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CAnSeeDoc* pDoc = GetDocument();
	if( nChar == VK_SPACE )
	{
		if( m_nTimer != 0 )
		{
			KillTimer( m_nTimer );
			m_nTimer = 0;
		}
		else
		{
			CTimeDialog dlg;
			if( dlg.DoModal() == IDOK )
			{
				pDoc->m_frameview = 0;
				m_nTimer = SetTimer( 1, dlg.m_iDelayTime, NULL );
			}
		}
		return;
	}
	BeginWaitCursor();
	pDoc->OnKeyUp( nChar );
	EndWaitCursor();
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CAnSeeView::RefreshView()
{
	CAnSeeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	RECT rt;
	GetClientRect( &rt );
	if( pDoc->m_pSpr[pDoc->m_view] == NULL ) pDoc->m_view = 0;
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	if( pDoc->m_pSpr[pDoc->m_view] == NULL )
	{
		if( pDoc->m_cPathName != "" )
		{
			status0.Format( "%d/%d", pDoc->m_CurFileIndex+1, pDoc->m_MaxFile+1 );
			status1.Format( "파일오류" );
			status2.Format( "파일오류" );
			status3.Format( "파일오류" );
			status4.Format( "100%%" );
			status5.Format( "Pak파일에 문제가 있습니다." );
			rt.top = 250;
			rt.bottom = 400;
			BufferDC.DrawText( "Pak파일에 문제가 있습니다.", &rt, DT_CENTER );
		}
		else
		{
			status0.Format( "0/0" );
			status1.Format( "ASD number : 0/0" );
			status2.Format( "0x0x0" );
			status3.Format( "0 KB / 0 KB" );
			status4.Format( "100%%" );
			status5.Format( "NONE" );
		}
	}
	else
	{
		int bx = pDoc->m_pSpr[pDoc->m_view]->m_bih->biWidth;
		int by = pDoc->m_pSpr[pDoc->m_view]->m_bih->biHeight;
		int zoom = pDoc->m_zoom;
		int dszx = bx*zoom/100;
		int dszy = by*zoom/100;
		int cx = (rt.right-dszx)/2 + m_movepoint.x + m_movingpoint.x;
		int cy = (rt.bottom-dszy)/2 + m_movepoint.y + m_movingpoint.y;
		BufferDC.BitBlt( 0, 0, rt.right, rt.bottom, NULL, 0, 0, BLACKNESS );
		if( pDoc->m_frameview == -1 )
		{
			if( pDoc->m_zoom == 100 ) BufferDC.BitBlt( cx, cy, bx, by, &(pDoc->m_pSpr[pDoc->m_view]->BufferDC), 0, 0, SRCCOPY );
			else BufferDC.StretchBlt( cx, cy, dszx, dszy, &(pDoc->m_pSpr[pDoc->m_view]->BufferDC), 0, 0, bx, by, SRCCOPY );
		}
		else
		{
			pDoc->m_pSpr[pDoc->m_view]->DrawFrame( &BufferDC, pDoc->m_frameview );
		}
		status0.Format( "%d/%d", pDoc->m_CurFileIndex+1, pDoc->m_MaxFile+1 );
		status1.Format( "ASD number : %d/%d", pDoc->m_view+1, pDoc->m_iTotalimage );
		if( pDoc->m_pSpr[pDoc->m_view]->m_bih->biBitCount == 24 ) status2.Format( "%dx%dx24b", bx, by );
		else if( pDoc->m_pSpr[pDoc->m_view]->m_bih->biBitCount == 8 ) status2.Format( "%dx%dx256", bx, by );
		else status2.Format( "%dx%dx%d", bx, by, pDoc->m_pSpr[pDoc->m_view]->m_bih->biBitCount );
		status3.Format( "%s / %s", pDoc->m_pSpr[pDoc->m_view]->m_cFileSize, pDoc->m_cFileSize );
		status4.Format( "%d%%", pDoc->m_zoom );
		status5.Format( "%s.pak", pDoc->m_cFileTitle );
	}
	pFrame->m_wndStatusBar.SetPaneText(0, status0 );
	pFrame->m_wndStatusBar.SetPaneText(1, status1 );
	pFrame->m_wndStatusBar.SetPaneText(2, status2 );
	pFrame->m_wndStatusBar.SetPaneText(3, status3 );
	pFrame->m_wndStatusBar.SetPaneText(4, status4 );
	pFrame->m_wndStatusBar.SetPaneText(5, status5 );
	GetClientRect( &rt );
	InvalidateRect( &rt, FALSE );
}

void CAnSeeView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CAnSeeDoc* pDoc = GetDocument();
	if( pDoc->m_pSpr[pDoc->m_view] == NULL ) return;
	m_originpoint.x = point.x;
	m_originpoint.y = point.y;
	m_movingpoint.x = 0;
	m_movingpoint.y = 0;
	CView::OnLButtonDown(nFlags, point);
}

void CAnSeeView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CAnSeeDoc* pDoc = GetDocument();
	if( pDoc->m_pSpr[pDoc->m_view] == NULL ) return;
	if( nFlags == 1 )
	{
		m_movingpoint.x = point.x - m_originpoint.x;
		m_movingpoint.y = point.y - m_originpoint.y;
		RefreshView();
	}
	CView::OnMouseMove(nFlags, point);
}

void CAnSeeView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CAnSeeDoc* pDoc = GetDocument();
	if( pDoc->m_pSpr[pDoc->m_view] == NULL ) return;
	m_movepoint.x = m_movepoint.x + m_movingpoint.x;
	m_movepoint.y = m_movepoint.y + m_movingpoint.y;
	m_movingpoint.x = 0;
	m_movingpoint.y = 0;
	CView::OnLButtonUp(nFlags, point);
}

BOOL CAnSeeView::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;	
	return CView::OnEraseBkgnd(pDC);
}

void CAnSeeView::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent == 1 )
	{
		CAnSeeDoc* pDoc = GetDocument();
		pDoc->m_frameview ++;
		if( pDoc->m_frameview > pDoc->m_pSpr[pDoc->m_view]->m_iTotalFrame -1 ) pDoc->m_frameview = 0;
		RefreshView();
	}
	
	CView::OnTimer(nIDEvent);
}
