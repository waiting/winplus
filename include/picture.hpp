//////////////////////////////////////////////////////////////////////////
// purpose: ͼƬ���
// author:  WT
//////////////////////////////////////////////////////////////////////////

#if !defined(__WINPLUS_PICTURE_HPP__)
#define __WINPLUS_PICTURE_HPP__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace winplus
{

// ͼƬ��� -----------------------------------------------------------

/* ����λͼ��ָ���ļ� */
bool Bitmap_SaveFile( HBITMAP bitmap, String const & filename );

// IPicturePtr

/* ���ļ�����ͼƬ(bmp,gif,jpg,png),���IPicture�ӿ� */
IPicturePtr Picture_Load( String const & pic_file );
/* IPicturePtr to HBITMAP */
inline HBITMAP Picture_GetBitmap( IPicturePtr pic )
{
	HBITMAP bitmap;
	pic->get_Handle( (OLE_HANDLE *)&bitmap );
	return bitmap;
}
/* ���IPictureͼƬ�ߴ�(pixels) */
SIZE Picture_GetDimensions( IPicturePtr pic );

// ��mem_dc���
class MemDC;
/* ���ļ�����ͼƬ(bmp,gif,jpg,png),�����mem_dc���� */
bool Picture_Load( String const & pic_file, MemDC * memdc );

// ��mem_image���
class MemImage;
/* ���ļ�����ͼƬ(bmp,gif,jpg,png),�����MemImage���� */
bool Picture_Load( String const & pic_file, MemImage * mem_img );


#define HIMETRIC_INCH   2540    // HIMETRIC units per inch

/* HIMETRICת��Ϊ�豸����(������) */
void HIMETRIC_to_DP( HDC dc, LPPOINT point );
void HIMETRIC_to_DP( HDC dc, LONG * x, LONG * y );
/* �豸����(������)ת��ΪHIMETRIC */
void DP_to_HIMETRIC( HDC dc, LPPOINT point );
void DP_to_HIMETRIC( HDC dc, LONG * x, LONG * y );

/* ͼ���� */
class Icon
{
public:
	Icon( String const & icoPath, uint cx = 0, uint cy = 0 )
	{
		new(this) Icon( icoPath.c_str(), LR_LOADFROMFILE | LR_SHARED, cx, cy );
	}
	Icon( uint icoResource, uint cx = 0, uint cy = 0 )
	{
		new(this) Icon( MAKEINTRESOURCE(icoResource), LR_SHARED, cx, cy );
	}
	Icon( LPCTSTR name, ulong flag, uint cx = 0, uint cy = 0 )
	{
		new(this) Icon( GetModuleHandle(NULL), name, flag, cx, cy );
	}
	Icon( HMODULE module, LPCTSTR name, ulong flag, uint cx = 0, uint cy = 0 ) : _ico(NULL)
	{
		attach( (HICON)LoadImage( module, name, IMAGE_ICON, cx, cy, flag ) );
	}
	~Icon()
	{
		_free();
	}
	void attach( HICON ico )
	{
		_free();
		_ico = ico;
	}
	HICON detach()
	{
		HICON ico = _ico;
		_ico = NULL;
		return ico;
	}
	operator HICON() const { return _ico; }
protected:
	void _free()
	{
		if ( _ico )
		{
			DestroyIcon(_ico);
			_ico = NULL;
		}
	}

private:
	HICON _ico;
	Icon( Icon const & other );
	Icon & operator = ( Icon const & other );
};

// gdi+ ͼƬ���
#if defined(_GDIPLUS_H)

/* ����MIME��ȡEncoderClsid */
INT ImageEncoderFromMIME( String const & mime_type, CLSID * encoder_clsid );
/* ������չ����ȡEncoderClsid */
INT ImageEncoderFromExtName( String const & extname, CLSID * encoder_clsid );

#endif

} // namespace winplus

#endif // !defined(__WINPLUS_PICTURE_HPP__)
