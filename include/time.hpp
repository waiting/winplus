//////////////////////////////////////////////////////////////////////////
// purpose: ʱ�����
// author:  WT
//////////////////////////////////////////////////////////////////////////

#if !defined(__WINPLUS_TIME_HPP__)
#define __WINPLUS_TIME_HPP__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace winplus
{

/* ���[1970-01-01 0:0:0]����ǰ��UTCʱ��,��ȷ������ */
ULONGLONG GetUtcTimeMs( void );
/* ���[1970-01-01 0:0:0]����ǰ��UTCʱ��,��ȷ���� */
UINT GetUtcTime( void );
/* ͨ��һ��[1970-1-1 0:0:0]�����UTC��������ȡһ��UTC SYSTEMTIME */
SYSTEMTIME GetUtcSysTime( ULONGLONG utcTimeMs );
/* ͨ��һ��[1970-1-1 0:0:0]�����UTC��������ȡһ��Local SYSTEMTIME */
SYSTEMTIME GetLocalSysTime( ULONGLONG utcTimeMs );
/* ��ʽ��ʱ��,<yr><mo><dy><h><m><s><ms> */
String FormatTime( String const & fmt, SYSTEMTIME const & time );

} // namespace winplus

#endif // !defined(__WINPLUS_TIME_HPP__)
