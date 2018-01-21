// AnSeeView.h : interface of the CAnSeeView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANSEEVIEW_H__87DB7C49_4DE0_4A3F_9AD8_47ABD0C0535A__INCLUDED_)
#define AFX_ANSEEVIEW_H__87DB7C49_4DE0_4A3F_9AD8_47ABD0C0535A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAnSeeView : public CView
{
protected: // create from serialization only
	CAnSeeView();
	DECLARE_DYNCREATE(CAnSeeView)

// Attributes
public:
	CDC BufferDC;
	CAnSeeDoc* GetDocument();
	CString status0, status1, status2, status3, status4, status5;
	CPoint m_originpoint;
	CPoint m_movepoint;
	CPoint m_movingpoint;
	UINT m_nTimer;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnSeeView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void RefreshView();
	virtual ~CAnSeeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAnSeeView)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in AnSeeView.cpp
inline CAnSeeDoc* CAnSeeView::GetDocument()
   { return (CAnSeeDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANSEEVIEW_H__87DB7C49_4DE0_4A3F_9AD8_47ABD0C0535A__INCLUDED_)
