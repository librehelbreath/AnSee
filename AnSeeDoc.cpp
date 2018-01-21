// AnSeeDoc.cpp : implementation of the CAnSeeDoc class
//

#include "stdafx.h"
#include "AnSee.h"

#include "AnSeeDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnSeeDoc

IMPLEMENT_DYNCREATE(CAnSeeDoc, CDocument)

BEGIN_MESSAGE_MAP(CAnSeeDoc, CDocument)
	//{{AFX_MSG_MAP(CAnSeeDoc)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateMenu)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateMenu)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateMenu)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateMenu)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnSeeDoc construction/destruction

CAnSeeDoc::CAnSeeDoc()
{
	m_pLeftView = NULL;
	m_pRightView = NULL;
	m_bNeedToOpen = FALSE;
	m_view = 0;
	m_frameview = -1;
	m_zoom = 100;
	m_iTotalimage = 0;
	for( int i=0 ; i<DEF_MAXSPRITES ; i++ ) m_pSpr[i] = NULL;
	m_CurFileIndex = 0;
	m_MaxFile = 0;
	CAnSeeApp *pApp = (CAnSeeApp*)AfxGetApp();
	if( pApp->m_strCommand != "" )
	{
		m_cPathName = pApp->m_strCommand;
		int a = pApp->m_strCommand.ReverseFind( '.' );
		int b = pApp->m_strCommand.ReverseFind( '\\' );
		m_cFileTitle = pApp->m_strCommand.Mid( b+1, a-b-1 );
		m_cFolderName = m_cPathName.Left( m_cPathName.ReverseFind( '\\' ) );
		SetCurrentDirectory( m_cFolderName );

		CFileFind filefind;
		for( int j=0; j<400 ; j++ )//was 200
		{
			m_filelist[j].cFileTitle = "";
			m_filelist[j].cPathName = "";
		}
		if( filefind.FindFile("*.pak") )
		{
			for( int i=0 ; i < 400 ; i ++ )
			{
				if( filefind.FindNextFile() == FALSE )
				{
					m_filelist[i].cFileTitle = filefind.GetFileTitle();
					m_filelist[i].cPathName = m_cFolderName + "\\" + filefind.GetFileName();
					if( m_cPathName == m_filelist[i].cPathName ) m_CurFileIndex = i;
					m_MaxFile = i;
					break;
				}
				m_filelist[i].cFileTitle = filefind.GetFileTitle();
				m_filelist[i].cPathName = m_cFolderName + "\\" + filefind.GetFileName();
				if( m_cPathName == m_filelist[i].cPathName ) m_CurFileIndex = i;
			}
		}
		m_bNeedToOpen = TRUE;
	}	
}

CAnSeeDoc::~CAnSeeDoc()
{
	for( int i=0 ; i<DEF_MAXSPRITES ; i++ )
	{
		if( m_pSpr[i] != NULL )
		{
			delete m_pSpr[i];
			m_pSpr[i] = NULL;
		}
	}
}

BOOL CAnSeeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	for( int i=0 ; i<DEF_MAXSPRITES ; i++ )
	{
		if( m_pSpr[i] != NULL )
		{
			delete m_pSpr[i];
			m_pSpr[i] = NULL;
		}
	}
	m_iTotalimage = 0;
	m_CurFileIndex = 0;
	m_MaxFile = 0;
	m_cPathName = "";
	m_cFileTitle = "";
	m_cFileSize = "";
	m_cFolderName = "";

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CAnSeeDoc serialization

void CAnSeeDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAnSeeDoc diagnostics

#ifdef _DEBUG
void CAnSeeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAnSeeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAnSeeDoc commands

void CAnSeeDoc::SetLeftView( CLeftView* pView )
{
	m_pLeftView = pView;
}

void CAnSeeDoc::SetRightView( CAnSeeView* pView, CDC* pDC )
{
	m_pDC = pDC;
	m_pRightView = pView;
	if( m_bNeedToOpen ) OnOpenFile();
}

void CAnSeeDoc::OnFileOpen() 
{
	CFileDialog dlg( TRUE, NULL, "*.pak", OFN_FILEMUSTEXIST | OFN_READONLY, "*.pak |", NULL );
	if( m_cPathName != "" ) strcpy( dlg.m_ofn.lpstrFile, m_cPathName );
	if( dlg.DoModal() != IDOK ) return;
	BeginWaitCursor();
	m_cPathName = dlg.GetPathName();
	m_cFileTitle = dlg.GetFileTitle();
	m_cFolderName = m_cPathName.Left( m_cPathName.ReverseFind( '\\' ) );
	SetCurrentDirectory( m_cFolderName );

	CFileFind filefind;
	for( int j=0; j<200 ; j++ )
	{
		m_filelist[j].cFileTitle = "";
		m_filelist[j].cPathName = "";
	}
	if( filefind.FindFile("*.pak") )
	{
		for( int i=0 ; i < 200 ; i ++ )
		{
			if( filefind.FindNextFile() == FALSE )
			{
				m_filelist[i].cFileTitle = filefind.GetFileTitle();
				m_filelist[i].cPathName = m_cFolderName + "\\" + filefind.GetFileName();
				if( m_cPathName == m_filelist[i].cPathName ) m_CurFileIndex = i;
				m_MaxFile = i;
				break;
			}
			m_filelist[i].cFileTitle = filefind.GetFileTitle();
			m_filelist[i].cPathName = m_cFolderName + "\\" + filefind.GetFileName();
			if( m_cPathName == m_filelist[i].cPathName ) m_CurFileIndex = i;
		}
	}
	OnOpenFile();
	m_pLeftView->ChangeTree();
}

void CAnSeeDoc::OnOpenFile()
{
	CFile fp;
	HANDLE hFileRead;
	DWORD  nCount;
	CString FileSize;
	char cBuff[21];
	ZeroMemory( cBuff, sizeof(cBuff) );
	int iTemp;
	hFileRead = CreateFile(m_cPathName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	DWORD filesize = GetFileSize( hFileRead, NULL );
	if( filesize > 1024*1024 )
	{
		iTemp = (filesize*100)/(1024*1024);
		m_cFileSize.Format( "%d.%02d MB", iTemp/100, iTemp%100 );
	}
	else
	{
		iTemp = (filesize*100)/1024;
		m_cFileSize.Format( "%d.%02d KB", iTemp/100, iTemp%100 );
	}
	if (hFileRead == INVALID_HANDLE_VALUE) return;
	SetTitle( m_cPathName );
	SetFilePointer(hFileRead, 0, NULL, FILE_BEGIN);
	ReadFile(hFileRead, (char *)cBuff, 20, &nCount, NULL);
	if( memcmp(cBuff, "<Pak file header>", 17 ) != 0 )
	{
		if (hFileRead != INVALID_HANDLE_VALUE) CloseHandle(hFileRead);
		MessageBox( NULL, "Pak File이 아닙니다.", "Error", MB_OK );
		return;
	}
	SetFilePointer(hFileRead, 20, NULL, FILE_BEGIN);
	ReadFile(hFileRead, (char *)&m_iTotalimage, 4, &nCount, NULL);
	CloseHandle(hFileRead);
	// 총 몇개의 파일로 구성되어있는지 알아본다.
	for( int i=0 ; i<DEF_MAXSPRITES ; i++ )
	{
		if( m_pSpr[i] != NULL )
		{
			delete m_pSpr[i];
			m_pSpr[i] = NULL;
		}
	}
	for( int iNthFile=0 ; iNthFile < m_iTotalimage ; iNthFile++ )
	{
		m_pSpr[iNthFile] = new class CSprite(m_cPathName, iNthFile, m_pDC);
		if( m_pSpr[iNthFile]->m_lpDib == NULL )
		{
			delete m_pSpr[iNthFile];
			m_pSpr[iNthFile] = NULL;
		}
	}
	// 화일을 닫는다.
	m_view = 0;
	m_frameview = -1;
	m_zoom = 100;
	if( m_pRightView != NULL )
	{
		m_pRightView->m_movepoint.x = 0;
		m_pRightView->m_movepoint.y = 0;
		m_pRightView->m_movingpoint.x = 0;
		m_pRightView->m_movingpoint.y = 0;
		m_pRightView->RefreshView();
	}
	EndWaitCursor();
}

void CAnSeeDoc::OnEditCopy() 
{
	BeginWaitCursor();
	for( int view=0 ; view<m_iTotalimage ; view++ )
	{
		char cSprName[40], cBmpName[40];
		ZeroMemory( cSprName, sizeof(cSprName) );
		ZeroMemory( cBmpName, sizeof(cBmpName) );
		wsprintf( cSprName, "%s%03d.spd", m_cFileTitle, view+1 );
		wsprintf( cBmpName, "%s%03d.bmp", m_cFileTitle, view+1 );
		CFile fp;
		HANDLE hFileRead, hFileSprite, hFileBitmap;
		DWORD  nCount, dwASDfileLoc, dwASDfileSize, dwBitmapFileStartLoc, dwBitmapFileSize;
		int iTotalFrame;
		char cBuff[101];
		ZeroMemory( cBuff, sizeof( cBuff ) );
		hFileRead = CreateFile(m_cPathName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
		if (hFileRead == INVALID_HANDLE_VALUE) return;
		SetTitle( m_cPathName );
		SetFilePointer(hFileRead, 0, NULL, FILE_BEGIN);
		ReadFile(hFileRead, (char *)cBuff, 20, &nCount, NULL);
		if( memcmp(cBuff, "<Pak file header>", 17 ) != 0 )
		{
			if (hFileRead != INVALID_HANDLE_VALUE) CloseHandle(hFileRead);
			MessageBox( NULL, "Pak File이 아닙니다.", "Error", MB_OK );
			return;
		}
		// 이제 스프라이트 파일(*.ASD)이 시작하는 위치를 찾는다. 
		SetFilePointer(hFileRead, 24 + view*8, NULL, FILE_BEGIN);
		ReadFile(hFileRead, (char *)&dwASDfileLoc,  4, &nCount, NULL);
		ReadFile(hFileRead, (char *)&dwASDfileSize, 4, &nCount, NULL);
		// 스프라이트 화일 헤더를 읽어 확인한다. 
		SetFilePointer(hFileRead, dwASDfileLoc, NULL, FILE_BEGIN);
		ReadFile(hFileRead, (char *)cBuff,  100, &nCount, NULL);
		// 헤더가 일치하지 않으면 읽을 수 없다.
		if (memcmp(cBuff, "<Sprite File Header>", 20) != 0) {
			if (hFileRead != INVALID_HANDLE_VALUE) CloseHandle(hFileRead);
			MessageBox( NULL, "이 PAK파일은 잘못된 Sprite파일을 갖고 있습니다.", "Error", MB_OK );
			return;
		}
		// 전체 프레임수 읽는다.
		ReadFile(hFileRead, (char *)&iTotalFrame,  4, &nCount, NULL);
		// 비트맵 화일의 위치를 미리 계산해 놓는다.
		dwBitmapFileStartLoc = dwASDfileLoc  + (108 + (12*iTotalFrame));
		dwBitmapFileSize     = dwASDfileSize - (108 + (12*iTotalFrame));
		char *cSprite, *cBitmap;
		cSprite = new char[85+iTotalFrame*12];
		ZeroMemory( cSprite, 85+iTotalFrame*12 );
		cBitmap = new char[dwBitmapFileSize+1];
		ZeroMemory( cBitmap, dwBitmapFileSize+1 );
		memcpy( cSprite, "<Sprite Info File Header>", 26 );
		memcpy( cSprite+26, cBmpName, 54 );
		memcpy( cSprite+80, (char*)&iTotalFrame, 4 );
		SetFilePointer(hFileRead, (dwASDfileLoc+104), NULL, FILE_BEGIN);
		ReadFile( hFileRead, cSprite+84, (DWORD)(iTotalFrame*12), &nCount, NULL );
		hFileSprite = CreateFile(cSprName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, NULL, NULL);
		WriteFile( hFileSprite, cSprite, (DWORD)(84+iTotalFrame*12), &nCount, NULL );
		CloseHandle( hFileSprite );
		delete[] cSprite;
		cSprite = NULL;
		hFileBitmap = CreateFile(cBmpName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, NULL, NULL);
		SetFilePointer(hFileRead, dwBitmapFileStartLoc, NULL, FILE_BEGIN );
		ReadFile( hFileRead, cBitmap, dwBitmapFileSize, &nCount, NULL );
		WriteFile( hFileBitmap, cBitmap, dwBitmapFileSize, &nCount, NULL );
		CloseHandle( hFileBitmap );
		delete[] cBitmap;
		cBitmap = NULL;
		// 화일을 닫는다.
		if (hFileRead != INVALID_HANDLE_VALUE) CloseHandle(hFileRead);
	}
	EndWaitCursor();
}

void CAnSeeDoc::OnEditPaste() 
{
	BeginWaitCursor();
	CString cSpdFileName;
	HANDLE hFileSprite, hFileBitmap, hFileASD;
	DWORD nCount, bmpfilesize, ASDfilesize, dwPakFileSize;
	char cBuffer[27];
	char cBmpFileName[55];
	char cASDFileName[64];
	int iTotalFrame = 0;
	char *cSpdContent, *cWriteBuffer;
	CString filepath = m_cPathName.Left( m_cPathName.ReverseFind( '\\' ) );
	dwPakFileSize = m_iTotalimage*8 + 24;
	for( int i=0 ; i < m_iTotalimage ; i++ )
	{
		ZeroMemory( cBuffer, sizeof(cBuffer) );
		ZeroMemory( cBmpFileName, sizeof(cBmpFileName) );
		cSpdFileName.Format( "%s%03d.spd", m_cFileTitle, i+1 );
		cSpdFileName = filepath + "\\" + cSpdFileName;
		hFileSprite = CreateFile(cSpdFileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
		if( hFileSprite == INVALID_HANDLE_VALUE )
		{
			return;
		}
		ReadFile( hFileSprite, cBuffer, 26, &nCount, NULL );
		if (strcmp(cBuffer,"<Sprite Info File Header>") != 0)
		{
			MessageBox( NULL, "Sprite파일에 문제가 있습니다.", "Error", MB_OK );
			continue;
		}
		ReadFile( hFileSprite, cBmpFileName, 54, &nCount, NULL );
		ReadFile( hFileSprite, (char*)&iTotalFrame, 4, &nCount, NULL );
		cSpdContent = new char[iTotalFrame*12+1];
		ZeroMemory( cSpdContent, iTotalFrame*12+1 );
		ReadFile( hFileSprite, cSpdContent, iTotalFrame*12, &nCount, NULL );
		hFileBitmap = CreateFile(cBmpFileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
		if( hFileBitmap == NULL )
		{
			MessageBox( NULL, "Bitmap파일을 여는데 실패하였습니다.", "Error", MB_OK );
			continue;
		}
		bmpfilesize = GetFileSize( hFileBitmap, NULL );
		ASDfilesize = bmpfilesize + iTotalFrame*12 + 108;
		dwPakFileSize += ASDfilesize;
		cWriteBuffer = new char[ASDfilesize+1];
		ZeroMemory( cWriteBuffer, ASDfilesize+1 );
		memcpy( cWriteBuffer, "<Sprite File Header>", 20 );
		memcpy( cWriteBuffer+100, (char*)&iTotalFrame, 4 );
		memcpy( cWriteBuffer+104, cSpdContent, iTotalFrame*12 );
		ReadFile( hFileBitmap, cWriteBuffer+108+iTotalFrame*12, bmpfilesize, &nCount, NULL );
		ZeroMemory( cASDFileName, sizeof(cASDFileName) );
		strcpy( cASDFileName, cSpdFileName.Left(cSpdFileName.GetLength()-4)+".asd" );
		hFileASD = CreateFile( cASDFileName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, NULL, NULL);
		WriteFile( hFileASD, cWriteBuffer, ASDfilesize, &nCount, NULL );
		if (hFileSprite != INVALID_HANDLE_VALUE) CloseHandle( hFileSprite );
		if (hFileBitmap != INVALID_HANDLE_VALUE) CloseHandle( hFileBitmap );
		if (hFileASD != INVALID_HANDLE_VALUE) CloseHandle( hFileASD );
		delete[] cSpdContent;
		cSpdContent = NULL;
		delete[] cWriteBuffer;
		cWriteBuffer = NULL;
		DeleteFile( cSpdFileName );
		DeleteFile( cBmpFileName );
	}
	DWORD FilePointer = m_iTotalimage*8 + 24;
	char *cPakBuffer;
	cPakBuffer = new char[dwPakFileSize+1];
	ZeroMemory( cPakBuffer, dwPakFileSize+1 );
	memcpy( cPakBuffer, "<Pak file header>", 20 );
	memcpy( cPakBuffer+20, (char*)&m_iTotalimage, 4 );
	for( i=0 ; i < m_iTotalimage ; i++ )
	{
		memcpy( cPakBuffer + i*8 + 24, (char*)&FilePointer, 4 );
		ZeroMemory( cASDFileName, sizeof(cASDFileName) );
		wsprintf( cASDFileName, "%s%03d.asd", m_cFileTitle, i+1 );
		hFileASD = CreateFile( cASDFileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
		ASDfilesize = GetFileSize( hFileASD, NULL );
		cWriteBuffer = new char[ASDfilesize+1];
		ZeroMemory( cWriteBuffer, ASDfilesize+1 );
		ReadFile( hFileASD, cWriteBuffer, ASDfilesize, &nCount, NULL );
		memcpy( cPakBuffer + i*8 + 28, (char*)&ASDfilesize, 4 );
		memcpy( cPakBuffer + FilePointer, cWriteBuffer, ASDfilesize );
		FilePointer += ASDfilesize;
		CloseHandle( hFileASD );
		delete[] cWriteBuffer;
		cWriteBuffer = NULL;
		DeleteFile( cASDFileName );
	}
	HANDLE hFilePak = CreateFile( m_cPathName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, NULL, NULL);
	WriteFile( hFilePak, cPakBuffer, dwPakFileSize, &nCount, NULL );
	CloseHandle( hFilePak );
	OnOpenFile();
	EndWaitCursor();
}

void CAnSeeDoc::OnEditCut() 
{
	CFileDialog dlg( TRUE, NULL, "*.asd", OFN_FILEMUSTEXIST | OFN_READONLY, "*.asd |", NULL );
	if( dlg.DoModal() != IDOK ) return;////불러들일 ASD파일을 얻어온다.
	DWORD  nCount, dwASDfileLoc, dwASDfileSize;
	for( int i=0 ; i<DEF_MAXSPRITES ; i++ )
	{
		delete m_pSpr[i];
		m_pSpr[i] = NULL;
	}
	int iRet = MessageBox( NULL, "현재의 ASD를 백업받으시겠습니까?", "주의사항", MB_YESNO );

	HANDLE hFileRead, hFileCopy;
	//hFileCopy는 끼어넣을 ASD파일의 핸들..
	hFileCopy = CreateFile(dlg.GetPathName(), GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	DWORD filesize2 = GetFileSize( hFileCopy, NULL );
	//ASD파일을 닫는다.
	hFileRead = CreateFile(m_cPathName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	DWORD filesize = GetFileSize( hFileRead, NULL );
	if (hFileRead == INVALID_HANDLE_VALUE) return;
	char *cWriteBuffer, *cp;
	cWriteBuffer = new char[filesize+filesize2];
	ZeroMemory( cWriteBuffer, filesize+filesize2 );
	memcpy(cWriteBuffer, "<Pak file header>", 17 );
	cp = cWriteBuffer;
	SetFilePointer(hFileRead, 20, NULL, FILE_BEGIN);
	ReadFile(hFileRead, (char *)&m_iTotalimage, 4, &nCount, NULL);
	memcpy(cWriteBuffer+20, (char*)&m_iTotalimage, 4 );
	cp += (24+m_iTotalimage*8);
	// 총 몇개의 파일로 구성되어있는지 알아본다.
	DWORD filelength = 24+m_iTotalimage*8;
	for( int iNthFile=0 ; iNthFile < m_iTotalimage ; iNthFile++ )
	{
		if( iNthFile == m_view )
		{
			if( iRet == IDYES )
			{
				SetFilePointer(hFileRead, 24 + iNthFile*8, NULL, FILE_BEGIN);
				ReadFile(hFileRead, (char *)&dwASDfileLoc,  4, &nCount, NULL);
				ReadFile(hFileRead, (char *)&dwASDfileSize, 4, &nCount, NULL);
				char *cReadBuffer;
				cReadBuffer = new char[dwASDfileSize+1];
				ZeroMemory( cReadBuffer, dwASDfileSize+1 );
				SetFilePointer(hFileRead, dwASDfileLoc, NULL, FILE_BEGIN);
				ReadFile( hFileRead, (char*)cReadBuffer, dwASDfileSize, &nCount, NULL );
				char cBackup[256];
				ZeroMemory( cBackup, sizeof(cBackup) );
				wsprintf( cBackup, "%s%03d.asd", m_cFileTitle, iNthFile+1 );
				HANDLE hFileBackup = CreateFile( cBackup, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, NULL, NULL);
				WriteFile( hFileBackup, cReadBuffer, dwASDfileSize, &nCount, NULL );
				CloseHandle(hFileBackup);
			}
			char *cCopyBuffer;
			cCopyBuffer = new char[filesize2+1];
			ZeroMemory( cCopyBuffer, filesize2+1 );
			SetFilePointer( hFileCopy, 0, NULL, FILE_BEGIN );
			ReadFile(hFileCopy, (char*)cCopyBuffer, filesize2, &nCount, NULL);
			memcpy( cp, cCopyBuffer, filesize2 );
			//cCopyBuffer는 ASD파일로 부터 읽어들인 내용..
			CloseHandle( hFileCopy );
			cp += filesize2;
			memcpy( (cWriteBuffer+24+(8*iNthFile)), (char*)&filelength, 4 );
			memcpy( (cWriteBuffer+28+(8*iNthFile)), (char*)&filesize2, 4 );
			filelength += filesize2;
			delete[] cCopyBuffer;
			cCopyBuffer = NULL;
		}
		else
		{
			// 이제 스프라이트 파일(*.ASD)이 시작하는 위치를 찾는다. 
			SetFilePointer(hFileRead, 24 + iNthFile*8, NULL, FILE_BEGIN);
			ReadFile(hFileRead, (char *)&dwASDfileLoc,  4, &nCount, NULL);
			ReadFile(hFileRead, (char *)&dwASDfileSize, 4, &nCount, NULL);
			char *cReadBuffer;
			cReadBuffer = new char[dwASDfileSize+1];
			ZeroMemory( cReadBuffer, dwASDfileSize+1 );
			SetFilePointer(hFileRead, dwASDfileLoc, NULL, FILE_BEGIN);
			ReadFile( hFileRead, (char*)cReadBuffer, dwASDfileSize, &nCount, NULL );
			memcpy( cp, cReadBuffer, dwASDfileSize );
			cp += dwASDfileSize;
			memcpy( cWriteBuffer+24+8*iNthFile, (char*)&filelength, 4 );
			memcpy( cWriteBuffer+28+8*iNthFile, (char*)&dwASDfileSize, 4 );
			filelength += dwASDfileSize;
			delete[] cReadBuffer;
			cReadBuffer = NULL;
		}
	}
	CloseHandle(hFileRead);
	HANDLE hFilePaste = CreateFile(m_cPathName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, NULL, NULL);
	SetFilePointer( hFilePaste, 0, 0, FILE_BEGIN );
	WriteFile( hFilePaste, cWriteBuffer, filelength, &nCount, NULL );
	CloseHandle( hFilePaste );
	delete[] cWriteBuffer;
	cWriteBuffer = NULL;
	OnOpenFile();
	m_pLeftView->ChangeTree();
}


void CAnSeeDoc::OnFileSave() 
{
	HANDLE hFileRead, hFileSave;
	DWORD  nCount, dwASDfileLoc, dwASDfileSize;
	char cSaveName[256];
	hFileRead = CreateFile(m_cPathName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFileRead == INVALID_HANDLE_VALUE) return;
	SetFilePointer(hFileRead, 20, NULL, FILE_BEGIN);
	ReadFile(hFileRead, (char *)&m_iTotalimage, 4, &nCount, NULL);
	// 총 몇개의 파일로 구성되어있는지 알아본다.
	for( int iNthFile=0 ; iNthFile < m_iTotalimage ; iNthFile++ )
	{
		// 이제 스프라이트 파일(*.ASD)이 시작하는 위치를 찾는다. 
		SetFilePointer(hFileRead, 24 + iNthFile*8, NULL, FILE_BEGIN);
		ReadFile(hFileRead, (char *)&dwASDfileLoc,  4, &nCount, NULL);
		ReadFile(hFileRead, (char *)&dwASDfileSize, 4, &nCount, NULL);
		ZeroMemory( cSaveName, sizeof( cSaveName ) );
		wsprintf( cSaveName, "%s%03d.asd", m_cFileTitle, iNthFile+1 );
		hFileSave = CreateFile(cSaveName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, NULL, NULL);
		char *cWriteBuffer;
		cWriteBuffer = new char[dwASDfileSize+1];
		SetFilePointer(hFileRead, dwASDfileLoc, NULL, FILE_BEGIN);
		ZeroMemory( cWriteBuffer, dwASDfileSize+1 );
		ReadFile( hFileRead, cWriteBuffer, dwASDfileSize, &nCount, NULL );
		WriteFile( hFileSave, cWriteBuffer, dwASDfileSize, &nCount, NULL );
		delete[] cWriteBuffer;
		cWriteBuffer = NULL;
		CloseHandle(hFileSave);
	}
	// 화일을 닫는다.
	if (hFileRead != INVALID_HANDLE_VALUE) CloseHandle(hFileRead);
}

void CAnSeeDoc::OnUpdateMenu(CCmdUI* pCmdUI) 
{
	if( m_pSpr[m_view] == NULL ) pCmdUI->Enable( FALSE );
	else pCmdUI->Enable( TRUE );
}

void CAnSeeDoc::OnKeyDown( UINT nChar )
{
	if( nChar == VK_ADD )
	{
		if( m_zoom == 25 ) m_zoom = 50;
		else if( m_zoom == 50 ) m_zoom = 75;
		else if( m_zoom == 75 ) m_zoom = 100;
		else if( m_zoom == 100 ) m_zoom = 150;
		else if( m_zoom == 150 ) m_zoom = 200;
		else if( m_zoom == 200 ) m_zoom = 300;
		else if( m_zoom == 300 ) m_zoom = 500;
		else if( m_zoom == 500 ) m_zoom = 700;
		else if( m_zoom == 700 ) m_zoom = 1000;
		else if( m_zoom == 1000 ) m_zoom = 2000;
		else return;
		m_pRightView->RefreshView();
	}
	if( nChar == VK_SUBTRACT )
	{
		if( m_zoom == 50 ) m_zoom = 25;
		else if( m_zoom == 75 ) m_zoom = 50;
		else if( m_zoom == 100 ) m_zoom = 75;
		else if( m_zoom == 150 ) m_zoom = 100;
		else if( m_zoom == 200 ) m_zoom = 150;
		else if( m_zoom == 300 ) m_zoom = 200;
		else if( m_zoom == 500 ) m_zoom = 300;
		else if( m_zoom == 700 ) m_zoom = 500;
		else if( m_zoom == 1000 ) m_zoom = 700;
		else if( m_zoom == 2000 ) m_zoom = 1000;
		else return;
		m_pRightView->RefreshView();
	}
	if( nChar == VK_RIGHT )
	{
		m_view ++;
		if( m_view >= m_iTotalimage ) m_view = 0;
		if( m_pRightView != NULL ) m_pRightView->RefreshView();
	}
	if( nChar == VK_SPACE )
	{
//		m_frameview ++;
//		if( m_frameview > m_pSpr[m_view]->m_iTotalFrame -1 ) m_frameview = 0;
//		if( m_pRightView != NULL ) m_pRightView->RefreshView();
	}
	if( nChar == VK_LEFT )
	{
		m_view --;
		if( m_view < 0 ) m_view = m_iTotalimage-1;
		if( m_pRightView != NULL ) m_pRightView->RefreshView();
	}
	if( (nChar==VK_NEXT) || (nChar==VK_DOWN) )
	{
		if( m_MaxFile < 1 ) return;
		m_CurFileIndex ++;
		if( m_CurFileIndex > m_MaxFile ) m_CurFileIndex = 0;
		m_view = 0;
		m_cPathName = m_filelist[m_CurFileIndex].cPathName;
		m_cFileTitle = m_filelist[m_CurFileIndex].cFileTitle;
		if( m_pRightView != NULL ) m_pRightView->RefreshView();
	}
	if( (nChar==VK_PRIOR) || (nChar==VK_UP) )
	{
		if( m_MaxFile < 1 ) return;
		m_CurFileIndex --;
		if( m_CurFileIndex < 0 ) m_CurFileIndex = m_MaxFile;
		m_view = 0;
		m_cPathName = m_filelist[m_CurFileIndex].cPathName;
		m_cFileTitle = m_filelist[m_CurFileIndex].cFileTitle;
		if( m_pRightView != NULL ) m_pRightView->RefreshView();
	}
}

void CAnSeeDoc::OnKeyUp( UINT nChar )
{
	if( nChar == VK_NEXT || nChar == VK_PRIOR || nChar==VK_UP || nChar==VK_DOWN )
	{
		BeginWaitCursor();
		m_view = 0;
		OnOpenFile();
		if( m_pRightView != NULL ) m_pRightView->RefreshView();
		if( m_pLeftView != NULL ) m_pLeftView->SetSelectedItem();
		EndWaitCursor();
	}
}
