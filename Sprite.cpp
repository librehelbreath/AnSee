// Sprite.cpp: implementation of the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AnSee.h"
#include "Sprite.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSprite::CSprite(CString cPathName, int iNthFile, CDC *pDC)
{
    m_lpDib = NULL;
	m_spData = NULL;
	char cBuff[101];
	ZeroMemory( cBuff, sizeof( cBuff ) );
	DWORD nCount, dwASDfileLoc, dwASDfileSize, dwBitmapFileStartLoc, dwBitmapFileSize;
	HANDLE hFileRead = CreateFile( cPathName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if( hFileRead == INVALID_HANDLE_VALUE ) return;
	// 이제 스프라이트 파일(*.ASD)이 시작하는 위치를 찾는다. 
	SetFilePointer(hFileRead, 24 + iNthFile*8, NULL, FILE_BEGIN);
	ReadFile(hFileRead, (char *)&dwASDfileLoc,  4, &nCount, NULL);
	ReadFile(hFileRead, (char *)&dwASDfileSize, 4, &nCount, NULL);
	// 스프라이트 화일 헤더를 읽어 확인한다. 
	SetFilePointer(hFileRead, dwASDfileLoc, NULL, FILE_BEGIN);
	ReadFile(hFileRead, (char *)cBuff,  100, &nCount, NULL);
	// 헤더가 일치하지 않으면 읽을 수 없다.
	if (memcmp(cBuff, "<Sprite File Header>", 20) != 0) {
		CloseHandle(hFileRead);
		return;
	}
	// 전체 프레임수 읽는다.
	ReadFile(hFileRead, (char *)&m_iTotalFrame,  4, &nCount, NULL);
	m_spData = new short[m_iTotalFrame*6];
	ReadFile(hFileRead, m_spData, 12*m_iTotalFrame, &nCount, NULL);
	// 비트맵 화일의 위치를 미리 계산해 놓는다.
	dwBitmapFileStartLoc = dwASDfileLoc  + (108 + (12*m_iTotalFrame));
	dwBitmapFileSize     = dwASDfileSize - (108 + (12*m_iTotalFrame));
	SetFilePointer( hFileRead, dwBitmapFileStartLoc, NULL, FILE_BEGIN );
	BITMAPFILEHEADER fh;
	ReadFile( hFileRead, &fh, 14, &nCount, NULL );
	m_lpDib = (LPSTR)new char[fh.bfSize-14];
	ZeroMemory( m_lpDib, fh.bfSize-14 );
	ReadFile( hFileRead, m_lpDib, fh.bfSize-14 , &nCount, NULL );
	CloseHandle(hFileRead);
	m_bih = (LPBITMAPINFOHEADER)m_lpDib;
	m_bmpInfo = (LPBITMAPINFO)m_lpDib;
	m_iScreenX = m_bmpInfo->bmiHeader.biWidth;
	m_iScreenY = m_bmpInfo->bmiHeader.biHeight;
	
	int i;
	if( fh.bfSize > 1024*1024 )
	{
		i = (fh.bfSize*100)/(1024*1024);
		m_cFileSize.Format( "%d.%02d MB", i/100, i%100 );
	}
	else
	{
		i = (fh.bfSize*100)/1024;
		m_cFileSize.Format( "%d.%02d KB", i/100, i%100 );
	}
	BufferDC.CreateCompatibleDC(pDC);
	CBitmap bmpBuffer;
	if( bmpBuffer.CreateCompatibleBitmap(pDC, m_iScreenX, m_iScreenY) == FALSE ) MessageBox( NULL, "Error", NULL, MB_OK );
	if( BufferDC.SelectObject(&bmpBuffer) == NULL ) return;
	bmpBuffer.DeleteObject();
	SetDIBitsToDevice( BufferDC.GetSafeHdc(), 0, 0, m_iScreenX, m_iScreenY, 0, 0, 0, m_iScreenY, m_lpDib+fh.bfOffBits-14, m_bmpInfo, DIB_RGB_COLORS );
}

void CSprite::DrawFullImage( CDC* pDC, int cx, int cy )
{
	pDC->BitBlt( cx, cy, m_iScreenX, m_iScreenY, &BufferDC, 0, 0, SRCCOPY );
}

void CSprite::DrawFrame( CDC* pDC, short sFrame )
{
	if( sFrame > m_iTotalFrame-1 ) return;
	short sTmp, sx, sy, szx, szy, pvx, pvy;
	sTmp = sFrame;
	sTmp = sTmp << 2;
	sTmp = sTmp + sFrame + sFrame;
	sx  = *(m_spData + sTmp);
	sy  = *(m_spData + sTmp + 1);
	szx = *(m_spData + sTmp + 2);
	szy = *(m_spData + sTmp + 3);
	pvx = *(m_spData + sTmp + 4);
	pvy = *(m_spData + sTmp + 5);
	pDC->BitBlt( 200+pvx, 200+pvy, szx, szy, &BufferDC, sx, sy, SRCCOPY );
}

CSprite::~CSprite()
{
	if( m_lpDib != NULL )
	{
		delete[] m_lpDib;
		m_lpDib = NULL;
	}
	if( m_spData != NULL )
	{
		delete[] m_spData;
		m_spData = NULL;
	}
}
