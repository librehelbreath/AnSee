// Sprite.h: interface for the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPRITE_H__EF55BB89_B759_4127_87AB_1C6FD9D9006E__INCLUDED_)
#define AFX_SPRITE_H__EF55BB89_B759_4127_87AB_1C6FD9D9006E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSprite  
{
public:
	CSprite(CString cPathName, int iNthFile, CDC *pDC);
	void DrawFullImage( CDC* pDC, int cx, int cy );
	void DrawFrame( CDC* pDC, short sFrame );
	virtual ~CSprite();
	LPBITMAPINFOHEADER m_bih; //bmp 정보헤더
	LPBITMAPINFO m_bmpInfo;
	CString m_cFileSize;
	int m_iScreenX;
	int m_iScreenY;
	short *	m_spData;
	LPSTR m_lpDib;
	CDC   BufferDC;
	int m_iTotalFrame;
};

#endif // !defined(AFX_SPRITE_H__EF55BB89_B759_4127_87AB_1C6FD9D9006E__INCLUDED_)
