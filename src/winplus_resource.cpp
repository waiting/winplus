#include "winplus_definitions.hpp"
#include "winplus_resource.hpp"

namespace winplus
{

Resource::~Resource()
{
    free();
}

void Resource::free()
{
    if ( _lpData != nullptr && _hResMemBlock != nullptr )
    {
        UnlockResource(_hResMemBlock);
        _lpData = nullptr;
    }
    if ( _hResMemBlock != nullptr )
    {
        FreeResource(_hResMemBlock);
        _hResMemBlock = nullptr;
    }
    _uSize = 0;
}

bool Resource::load( HMODULE hModule, LPCTSTR lpszName, LPCTSTR lpType, WORD wLanguage )
{
    this->free();

    HRSRC hRSRC = FindResourceEx( hModule, lpType, lpszName, wLanguage );
    if ( !hRSRC ) return false;
    _uSize = SizeofResource( hModule, hRSRC );
    _hResMemBlock = LoadResource( hModule, hRSRC );
    if ( !_hResMemBlock ) return false;
    _lpData = LockResource(_hResMemBlock);
    if ( !_lpData ) return false;
    return true;
}

ssize_t Resource::copyTo( LPVOID lpDestination, size_t uDestinationSize ) const
{
    size_t uSize = uDestinationSize < this->getSize() ? uDestinationSize : this->getSize();
    if ( lpDestination != NULL && uSize > 0 )
        CopyMemory( lpDestination, this->getData(), uSize );
    return (ssize_t)( uDestinationSize - this->getSize() );
}

void Resource::_construct( HMODULE hModule, LPCTSTR lpszName, LPCTSTR lpType, WORD wLanguage )
{
    _construct();
    this->load( hModule, lpszName, lpType, wLanguage );
}

void Resource::_construct()
{
    //_hModule = NULL;
    _hResMemBlock = NULL;
    //_hRSRC = NULL;
    _uSize = 0;
    _lpData = NULL;
}

} // namespace winplus
