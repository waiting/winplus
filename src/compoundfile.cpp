#include <windows.h>
#include <comdef.h>
#include "../include/definitions.hpp"
#include "../include/compoundfile.hpp"

namespace winplus
{

IStreamPtr CreateNewStream()
{
    HRESULT hr;
    IStreamPtr stream;
    hr = CreateStreamOnHGlobal( NULL, TRUE, &stream ); // ���� IStream
    if ( FAILED(hr) )
    {
        return NULL;
    }
    return stream;
}

IStreamPtr CreateStreamExistingFile( String const & fileName )
{
    IStreamPtr stream;
    HRESULT hr;
    HANDLE file = CreateFile(
        fileName.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if ( INVALID_HANDLE_VALUE == file )
    {
        return NULL;
    }
    DWORD fileSize, tmpSize;
    fileSize = GetFileSize( file, &tmpSize ); // ����ļ���С
    HGLOBAL block = GlobalAlloc( GMEM_MOVEABLE, fileSize );
    if ( block == NULL )
    {
        CloseHandle(file);
        return NULL;
    }
    
    LPVOID buffer = GlobalLock(block);
    ReadFile( file, buffer, fileSize, &tmpSize, NULL );
    GlobalUnlock(block);
    
    CloseHandle(file); // �����ѿ������ڴ�,������File
    
    hr = CreateStreamOnHGlobal( block, TRUE, &stream ); // ���� IStream
    if ( FAILED(hr) )
    {
        GlobalFree(block);
        return NULL;
    }

    return stream;
}

IStreamPtr CreateStreamFromBuffer( LPCVOID buffer, DWORD size )
{
    HRESULT hr;
    IStreamPtr stream;
    HGLOBAL block = GlobalAlloc( GMEM_MOVEABLE, size );
    if ( block == NULL )
    {
        return NULL;
    }
    
    LPVOID data = GlobalLock(block);
    CopyMemory( data, buffer, size );
    GlobalUnlock(block);

    hr = CreateStreamOnHGlobal( block, TRUE, &stream ); // ���� IStream
    if ( FAILED(hr) )
    {
        GlobalFree(block);
        return NULL;
    }
    return stream;
}

IStreamPtr CreateStreamFromResourceEx( HMODULE module, UINT resourceId, LPCTSTR type )
{
    IStreamPtr stream;
    HRSRC resource = FindResource( module, MAKEINTRESOURCE(resourceId), type);
    if ( resource != NULL )
    {
        DWORD size = SizeofResource( module, resource );
        HGLOBAL resBlock = LoadResource( module, resource );
        LPVOID data = LockResource(resBlock);
        stream = CreateStreamFromBuffer( data, size );
        UnlockResource(resBlock);
        FreeResource(resBlock);
    }

    return stream;
}

IStreamPtr CreateStreamFromResource( UINT resourceId, LPCTSTR type )
{
    return CreateStreamFromResourceEx( GetModuleHandle(NULL), resourceId, type );
}

} // namespace winplus
