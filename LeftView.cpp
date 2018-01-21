// LeftView.cpp : implementation of the CLeftView class
//

#include "stdafx.h"
#include "AnSee.h"

#include "AnSeeDoc.h"
#include "LeftView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	//{{AFX_MSG_MAP(CLeftView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftView construction/destruction

CLeftView::CLeftView()
{
}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_BORDER | TVS_HASBUTTONS /*| TVS_LINESATROOT*/ | TVS_HASLINES | TVS_SHOWSELALWAYS;
	return CTreeView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView drawing

void CLeftView::OnDraw(CDC* pDC)
{
	CAnSeeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	MessageBox( "hehe" );
}


/////////////////////////////////////////////////////////////////////////////
// CLeftView printing

void CLeftView::OnInitialUpdate()
{
	CTreeCtrl& pTree = GetTreeCtrl();
	CTreeView::OnInitialUpdate();
	CAnSeeDoc* pDoc = GetDocument();
	pDoc->SetLeftView( this );
	if( pDoc->m_cPathName != "" ) ChangeTree();
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CAnSeeDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAnSeeDoc)));
	return (CAnSeeDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLeftView message handlers

void CLeftView::ChangeTree()
{
	CAnSeeDoc* pDoc = GetDocument();
	CTreeCtrl& pTree = GetTreeCtrl();
	pTree.DeleteAllItems();
	TV_INSERTSTRUCT tvinsert;
	tvinsert.hParent = NULL;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT;
	tvinsert.item.state = 0;
	tvinsert.item.stateMask = 0;
	tvinsert.item.cchTextMax = 60;
	tvinsert.item.iSelectedImage = 1;
	tvinsert.item.cChildren = 0;
	char cFolderName[256];
	ZeroMemory( cFolderName, sizeof(cFolderName) );
	strcpy( cFolderName, pDoc->m_cFolderName );
	tvinsert.item.pszText = cFolderName;
	tvinsert.item.iImage = 0;
	HTREEITEM hRoot = pTree.InsertItem(&tvinsert);
	pTree.SetItemData( hRoot, -1 );
	for( int i=0; i<pDoc->m_MaxFile+1; i++ )
	{
		tvinsert.hParent = hRoot;
		char cFileTitle[30];
		ZeroMemory( cFileTitle, sizeof(cFileTitle) );
		strcpy( cFileTitle, pDoc->m_filelist[i].cFileTitle );
		tvinsert.item.pszText = cFileTitle;
		HTREEITEM hItem = pTree.InsertItem(&tvinsert);
		pTree.SetItemData( hItem, i );
//		pTree.SetCheck( hItem );
	}
	pTree.Expand( hRoot, TVE_EXPAND );
}

void CLeftView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CAnSeeDoc* pDoc = GetDocument();
	CTreeCtrl& pTree = GetTreeCtrl();
	HTREEITEM hItem = pTree.GetSelectedItem();
	if( hItem != NULL )
	{
		int a = pTree.GetItemState( hItem, TVIS_SELECTED );
		int iFileIndex = pTree.GetItemData(hItem);
		if( iFileIndex >= 0 )
		{
			BeginWaitCursor();
			pDoc->m_CurFileIndex = iFileIndex;
			pDoc->m_cPathName = pDoc->m_filelist[iFileIndex].cPathName;
			pDoc->m_cFileTitle = pDoc->m_filelist[iFileIndex].cFileTitle;
			pDoc->OnOpenFile();
			EndWaitCursor();
		}
	}
	*pResult = 0;
}

void CLeftView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CAnSeeDoc* pDoc = GetDocument();
	if( nChar == VK_SPACE || nChar == VK_LEFT  || nChar == VK_RIGHT )
	pDoc->OnKeyDown( nChar );
	if( nChar != VK_LEFT ) CTreeView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CLeftView::SetSelectedItem()
{
	CAnSeeDoc* pDoc = GetDocument();
	CTreeCtrl& pTree = GetTreeCtrl();
	HTREEITEM hRoot = pTree.GetRootItem();
	HTREEITEM hItem = pTree.GetChildItem(hRoot);
	int iData = (int)(pTree.GetItemData( hItem ));
	if( iData == pDoc->m_CurFileIndex ) {
		pTree.SelectItem(hItem);
//		pTree.SetCheck(hItem);
		return;
	}
	while(1)
	{
		hItem = pTree.GetNextItem( hItem, TVGN_NEXT );
		if( hItem == NULL ) return;
		iData = pTree.GetItemData( hItem );
		if( iData == pDoc->m_CurFileIndex ) 
		{
			pTree.SelectItem( hItem );
//			pTree.SetCheck(hItem);
			return;
		}
	}
}