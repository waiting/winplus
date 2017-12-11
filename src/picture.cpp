#include <windows.h>
#include <comdef.h>
#include "../include/definitions.hpp"
#include "../include/picture.hpp"
#include "../include/compoundfile.hpp"
#include "../include/strings.hpp"
#include "../include/graphics.hpp"
#include <tchar.h>

namespace winplus
{

bool Bitmap_SaveFile( HBITMAP bitmap, String const & filename )
{
	HDC hDC; //�豸������
	INT nBits;  //��ǰ��ʾ�ֱ�����ÿ��������ռλ��
	WORD wBitCount = 0;    //λͼ��ÿ��������ռλ��
	//��ɫ���С��λͼ�������ֽڴ�С��λͼ�ļ���С��д���ļ��ֽ���
	DWORD dwPaletteSize = 0, dwBmBitsSize, dwDIBSize, dwWritten;
	BITMAP bmpObj; //λͼ����
	BITMAPFILEHEADER bmfHdr; //λͼ�ļ�ͷ
	BITMAPINFOHEADER bi;//λͼ��Ϣͷ
	LPBITMAPINFOHEADER lpbi; //ָ��λͼ��Ϣͷ
	//�ļ�����������ڴ�������ɫ����
	HANDLE hFile, hDibData;
	HPALETTE hPal, hOldPal = NULL;
	//����λͼ�ļ�ÿ��������ռλ��
	hDC = CreateDC( TEXT("DISPLAY"), NULL, NULL, NULL );
	nBits = GetDeviceCaps( hDC, BITSPIXEL ) * GetDeviceCaps( hDC, PLANES );
	DeleteDC(hDC);
	if ( nBits <= 1 )
		wBitCount = 1;
	else if ( nBits <= 4 )
		wBitCount = 4;
	else if ( nBits <= 8 )
		wBitCount = 8;
	else if ( nBits <= 24 )
		wBitCount = 24;
	else
		wBitCount = 32;
	//�����ɫ���С��4,8λλͼ���轨����ɫ��
	if ( wBitCount <= 8 )
		dwPaletteSize = ( 1 << wBitCount ) * sizeof(RGBQUAD);

	//����λͼ��Ϣͷ
	GetObject( bitmap, sizeof(BITMAP), &bmpObj );
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bmpObj.bmWidth;
	bi.biHeight = bmpObj.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;
	//�������������ֽ���
	dwBmBitsSize = ( ( bmpObj.bmWidth * wBitCount + 31 ) / 32 ) * 4 * bmpObj.bmHeight;
	//Ϊλͼ���ݷ����ڴ�
	hDibData = GlobalAlloc( GHND, dwBmBitsSize + dwPaletteSize + sizeof( BITMAPINFOHEADER ) );
	lpbi = ( LPBITMAPINFOHEADER )GlobalLock( hDibData );
	*lpbi = bi;
	//�����ɫ��      
	hPal = ( HPALETTE )GetStockObject(DEFAULT_PALETTE);
	if ( hPal )
	{
		hDC = GetDC(NULL);
		hOldPal = SelectPalette( hDC, hPal, FALSE );
		RealizePalette(hDC);
	}
	//��ȡ�õ�ɫ�����µ�����ֵ
	GetDIBits( hDC, bitmap, 0, (UINT)bmpObj.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize, (LPBITMAPINFO)lpbi, DIB_RGB_COLORS );
	//�ָ���ɫ��
	if (hOldPal)
	{
		SelectPalette(hDC, hOldPal, TRUE);
		RealizePalette(hDC);
		ReleaseDC(NULL, hDC);
	}
	//����λͼ�ļ�
	hFile = CreateFile( filename.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL );
	if ( hFile == INVALID_HANDLE_VALUE )
		return false;
	//����λͼ�ļ�ͷ
	bmfHdr.bfType = 0x4D42; //"BM"
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	//д��λͼ�ļ�ͷ
	WriteFile(hFile, &bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	//д��λͼ�ļ���������
	WriteFile(hFile, lpbi, dwDIBSize, &dwWritten, NULL);
	//���
	GlobalUnlock(hDibData);
	GlobalFree(hDibData);
	CloseHandle(hFile);
	return true;
}

IPicturePtr Picture_Load( String const & pic_file )
{
	HRESULT hr;
	IPicturePtr pic;
	IStreamPtr stream = CreateStreamExistingFile(pic_file);
	if ( !(bool)stream )
	{
		return NULL;
	}
	hr = OleLoadPicture( stream, 0, FALSE, IID_IPicture, (LPVOID *)&pic );

	if ( FAILED(hr) ) // ����IPicture�ӿ�ʧ��,����null
	{
		return NULL;
	}

	return pic;
}

SIZE Picture_GetDimensions( IPicturePtr pic )
{
	SIZE dimensions;
	HRESULT hr;
	LONG x, y;
	HDC dc;
	hr = pic->get_Width(&x);
	hr = pic->get_Height(&y);

	dc = GetDC(HWND_DESKTOP);
	HIMETRIC_to_DP( dc, &x, &y );
	ReleaseDC( HWND_DESKTOP, dc );

	dimensions.cx = x;
	dimensions.cy = y;

	return dimensions;
}

bool Picture_Load( String const & pic_file, MemImage * mem_img )
{
	Gdiplus::Bitmap bmp( StringToUnicode(pic_file).c_str() );
	mem_img->create( bmp.GetWidth(), bmp.GetHeight() );
	mem_img->copy(&bmp);
	return true;
}

bool Picture_Load( String const & pic_file, MemDC * memdc )
{
	IPicturePtr pic;
	if ( pic = Picture_Load(pic_file) )
	{
		HDC hDC = GetDC(HWND_DESKTOP);
		MemDC temp_dc;
		HBITMAP hBitmap = Picture_GetBitmap(pic);
		temp_dc.create(hDC);
		temp_dc.attachBitmap(hBitmap);
		memdc->copy(temp_dc);
		temp_dc.detachBitmap();
		ReleaseDC( HWND_DESKTOP, hDC );
		
		return true;
	}
	return false;
}
//
void HIMETRIC_to_DP( HDC dc, LPPOINT point )
{
	INT mapMode;
	if ( ( mapMode = GetMapMode(dc) ) < MM_ISOTROPIC && mapMode != MM_TEXT )
	{
		// when using a constrained map mode, map against physical inch
		SetMapMode( dc, MM_HIMETRIC );
		LPtoDP( dc, point, 1 );
		SetMapMode( dc, mapMode );
	}
	else
	{
		// map against logical inch for non-constrained mapping modes
		INT cxPerInch, cyPerInch;
		cxPerInch = GetDeviceCaps( dc, LOGPIXELSX );
		cyPerInch = GetDeviceCaps( dc, LOGPIXELSY );
		point->x = MulDiv( point->x, cxPerInch, HIMETRIC_INCH );
		point->y = MulDiv( point->y, cyPerInch, HIMETRIC_INCH );
	}
}

void HIMETRIC_to_DP( HDC dc, LONG * x, LONG * y )
{
	POINT pt = { *x, *y };
	HIMETRIC_to_DP( dc, &pt );
	*x = pt.x;
	*y = pt.y;
}

void DP_to_HIMETRIC( HDC dc, LPPOINT point )
{
	INT mapMode;
	if ( ( mapMode = GetMapMode(dc) ) < MM_ISOTROPIC && mapMode != MM_TEXT )
	{
		// when using a constrained map mode, map against physical inch
		SetMapMode( dc, MM_HIMETRIC );
		DPtoLP( dc, point, 1 );
		SetMapMode( dc, mapMode );
	}
	else
	{
		// map against logical inch for non-constrained mapping modes
		INT cxPerInch, cyPerInch;
		cxPerInch = GetDeviceCaps( dc, LOGPIXELSX );
		cyPerInch = GetDeviceCaps( dc, LOGPIXELSY );
		point->x = MulDiv( point->x, HIMETRIC_INCH, cxPerInch );
		point->y = MulDiv( point->y, HIMETRIC_INCH, cyPerInch );
	}
}

void DP_to_HIMETRIC( HDC dc, LONG * x, LONG * y )
{
	POINT pt = { *x, *y };
	DP_to_HIMETRIC( dc, &pt );
	*x = pt.x;
	*y = pt.y;
}

// gdi+ ��� --------------------------------------------------------------
#if defined(_GDIPLUS_H)

#pragma comment ( lib, "gdiplus" )

INT ImageEncoderFromMIME( String const & mime_type, CLSID * encoder_clsid )
{
	using namespace Gdiplus;
	UINT num = 0;          // number of image encoders
	UINT size = 0;         // size of the image encoder array in bytes
	ImageCodecInfo * pImageCodecInfo = NULL;
	GetImageEncodersSize( &num, &size );
	if ( size == 0 ) return -1;  // Failure
	pImageCodecInfo = (ImageCodecInfo *)malloc(size);
	if ( pImageCodecInfo == NULL ) return -1;  // Failure
	GetImageEncoders( num, size, pImageCodecInfo );
	UINT i;
	for( i = 0; i < num; ++i )
	{
		ImageCodecInfo & info = pImageCodecInfo[i];
		String strMimeType = UnicodeToString( info.MimeType ? info.MimeType : L"" );
		if ( strMimeType == mime_type )
		{
			*encoder_clsid = info.Clsid;
			break;
		}
	}
	free(pImageCodecInfo);
	if ( i < num )
	{
		return i; // Success
	}
	return -1;  // Failure
}

INT ImageEncoderFromExtName( String const & extname, CLSID * encoder_clsid )
{
	using namespace Gdiplus;
	UINT num = 0;          // number of image encoders
	UINT size = 0;         // size of the image encoder array in bytes
	ImageCodecInfo * pImageCodecInfo = NULL;
	GetImageEncodersSize( &num, &size );
	if ( size == 0 ) return -1;  // Failure
	pImageCodecInfo = (ImageCodecInfo *)malloc(size);
	if ( pImageCodecInfo == NULL ) return -1;  // Failure
	GetImageEncoders( num, size, pImageCodecInfo );
	UINT i;
	String strExtName = extname;
	for( i = 0; i < num; ++i )
	{
		ImageCodecInfo & info = pImageCodecInfo[i];
		String strFilenameExtension = UnicodeToString( info.FilenameExtension ? info.FilenameExtension : L"" );
		StringArray arrExtName;
		INT nExtNameCount = StrSplit( strFilenameExtension.c_str(), TEXT(";"), &arrExtName );
		INT j;
		for ( j = 0; j < nExtNameCount; ++j )
		{
			//arrExtName[j] = arrExtName[j].substr(2); // *.xxx skip 2 chars
			_tcslwr(&arrExtName[j][0]);
			_tcslwr(&strExtName[0]);
			if ( arrExtName[j].find(strExtName) != String::npos )
			{
				break;
			}
		}
		if ( j < nExtNameCount )
		{
			*encoder_clsid = info.Clsid;
			break;
		}
	}
	free(pImageCodecInfo);
	if ( i < num )
	{
		return i; // Success
	}
	return -1;  // Failure
}

#endif

} // namespace winplus