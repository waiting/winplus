#include <windows.h>
#include <shlobj.h>
#include "../include/definitions.hpp"
#include "../include/shell.hpp"
#include "../include/strings.hpp"

namespace winplus
{

// ���(Shell)��� --------------------------------------------------------

class SimpleFileSysBindData : public IFileSystemBindData
{ 
public:
	SimpleFileSysBindData() : _cRef(1)
	{
		ZeroMemory( &_fd, sizeof(_fd) );
	}
	// *** IUnknown methods ***
	STDMETHODIMP QueryInterface( REFIID riid, void * * ppvObj )
	{
		if ( riid == IID_IFileSystemBindData )
		{
			*ppvObj = static_cast<IFileSystemBindData *>(this);
			AddRef();
			return S_OK;
		}
		else if ( riid == IID_IUnknown )
		{
			*ppvObj = static_cast<IUnknown *>(this);
			AddRef();
			return S_OK;
		}
		return E_NOINTERFACE;
	}
	STDMETHODIMP_(ULONG) AddRef(void)
	{
		return InterlockedIncrement(&_cRef);
	}
	STDMETHODIMP_(ULONG) Release(void)
	{
		LONG cRef = InterlockedDecrement(&_cRef);
		if ( !cRef )
			delete this;
		return cRef;
	}
	
	// *** IFileSystemBindData Methods ***
	STDMETHODIMP SetFindData( WIN32_FIND_DATAW const * pfd )
	{
		_fd = *pfd;
		return S_OK;
	}
	STDMETHODIMP GetFindData( WIN32_FIND_DATAW * pfd )
	{
		*pfd = _fd;
		return S_OK;
	}
private:
	~SimpleFileSysBindData()
	{
	}
	
	LONG _cRef;
	WIN32_FIND_DATAW _fd;
};

HRESULT ItemIdListFromPathCustom( LPWSTR path, WIN32_FIND_DATAW * pfd, LPITEMIDLIST * ppidl )
{
	IFileSystemBindData * pfsbd;
	HRESULT hr = S_OK;
	//IUnknown * pI;
	//pI = static_cast<IFileSystemBindData *>(new SimpleFileSysBindData);
	//hr = pI->QueryInterface( IID_IFileSystemBindData, (void * *)&pfsbd );
	//pI->Release();
	pfsbd = static_cast<IFileSystemBindData *>(new SimpleFileSysBindData);

	if ( FAILED(hr) )
		return hr;

	if ( pfd )
		pfsbd->SetFindData(pfd);

	LPBINDCTX pBindCtx;
	hr = CreateBindCtx( 0, &pBindCtx );
	if ( FAILED(hr) )
	{
		pfsbd->Release();
		return hr;
	}
	pBindCtx->RegisterObjectParam( STR_FILE_SYS_BIND_DATA, pfsbd );
	pfsbd->Release();

	// �õ������Ŀ¼
	LPSHELLFOLDER pFolder;
	hr = SHGetDesktopFolder(&pFolder);
	if ( FAILED(hr) )
	{
		pBindCtx->Release();
		return hr;
	}
	hr = pFolder->ParseDisplayName( NULL, pBindCtx, (LPOLESTR)path, NULL, ppidl, NULL );
	pFolder->Release();
	pBindCtx->Release();

	return hr;
}

LPITEMIDLIST ItemIdListFromPath( String const & filePath )
{
	if ( filePath.empty() )
	{
		return NULL;
	}

	HRESULT hr;
	// �õ������Ŀ¼
	LPSHELLFOLDER pDesktopFolder = NULL;
	hr = SHGetDesktopFolder(&pDesktopFolder);
	if ( FAILED(hr) )
	{
		return NULL;
	}

	UnicodeString olestr = StringToUnicode(filePath);
	// �õ��ļ�·����Ӧ��ItemIDList
	LPITEMIDLIST pItemIDList = NULL;
	hr = pDesktopFolder->ParseDisplayName( NULL, NULL, (LPOLESTR)&olestr[0], NULL, &pItemIDList, NULL );
	pDesktopFolder->Release();
	if ( FAILED(hr) )
	{
		return NULL;
	}
	return pItemIDList;
}

LPITEMIDLIST ItemIdListFromPathSimple( String const & filePath )
{
	LPITEMIDLIST pItemIDList = NULL;
	HRESULT hr = ItemIdListFromPathCustom( (LPWSTR)StringToUnicode(filePath).c_str(), NULL, &pItemIDList );
	return pItemIDList;
}

} // namespace winplus