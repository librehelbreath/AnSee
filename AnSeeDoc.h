// AnSeeDoc.h : interface of the CAnSeeDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANSEEDOC_H__70F9578F_D760_40C5_89DE_C1CD73397A0A__INCLUDED_)
#define AFX_ANSEEDOC_H__70F9578F_D760_40C5_89DE_C1CD73397A0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Sprite.h"
#include "LeftView.h"
#include "AnSeeView.h"

typedef struct FileSearch {
CString cPathName;
CString cFileTitle;
} FileSearch;

class CAnSeeDoc : public CDocument
{
protected: // create from serialization only
	CAnSeeDoc();
	DECLARE_DYNCREATE(CAnSeeDoc)

// Attributes
public:
	BOOL m_bNeedToOpen;
	CSprite *m_pSpr[DEF_MAXSPRITES];
	int m_iTotalimage;
	CString m_cPathName;
	CString m_cFolderName;
	CString m_cFileTitle;
	CString m_cFileSize;
	FileSearch m_filelist[400];
	int m_CurFileIndex;
	int m_MaxFile;
	int m_view;
	int m_frameview;
	int m_zoom;
	CLeftView *m_pLeftView;
	CAnSeeView *m_pRightView;
	CDC *m_pDC;

// Operations
public:
	void OnOpenFile();
	void OnKeyDown( UINT nChar );
	void OnKeyUp( UINT nChar );
	void SetLeftView( CLeftView* pView );
	void SetRightView( CAnSeeView *pView, CDC *pDC );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnSeeDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAnSeeDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAnSeeDoc)
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnFileOpen();
	afx_msg void OnUpdateMenu(CCmdUI* pCmdUI);
	afx_msg void OnFileSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANSEEDOC_H__70F9578F_D760_40C5_89DE_C1CD73397A0A__INCLUDED_)
