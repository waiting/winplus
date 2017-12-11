//////////////////////////////////////////////////////////////////////////
// purpose: �����ʿ�ͷִʹ���
// author:  WT
// date:    2013-05-31
//////////////////////////////////////////////////////////////////////////

#if !defined(__WINPLUS_WORDSLIB_HPP__)
#define __WINPLUS_WORDSLIB_HPP__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace winplus
{

// ����ʿ��ļ� ----------------------------------------------------------
/*
    �ʿ��ļ�����������ɣ��ļ�ͷ���ʻ����ݡ�
	�ļ�ͷ���ô��ı���ʽ�����ʿ����Ҫ���ԣ��ʿ����ƣ���ע��Ϣ���ʿ����ͣ�ѹ����ʽ�����룬�ʻ����������С
    �ʻ����ݿ�����ѹ���ģ�Ҳ�����Ƿ�ѹ���ģ������ǵȳ��Ҳ�����ǲ��ȳ��ģ���Щ���ļ�ͷָ��

    ���¶���ʿ��ļ���ʽ

        {}     ��ʾ�����ļ�
        ()     ��ʾһ������
        CRLF   ����һ���س����з�
        SPACE  �����CRLF�Ŀհ��ַ�
        +      ����1������
        *      ����0������
        ?      ����0����1��
        |      ����ѡ�������Ǹ�
        //     ����ע��˵��
    
    �ļ�ͷ����������Сд������
    ��ע�⣬���涨���ı��пհ��ַ����Ǳ����Ķ��ģ�ʵ�ʲ����ڣ�
    {
        (name: (SPACE*) �ַ����� CRLF)?
        (desc: (SPACE*) �ַ����� CRLF)?
        type: (SPACE*) (equal | unequal) CRLF // ���������ǵȳ��򲻵ȳ���Ĭ��equal
                                              // �ȳ����������£���������seek()�����ļ��ڴ�ӳ�䶨λ�����Ǵ洢��Ƚ�ռ�ռ�
                                              // ���ȳ�������£���Ҫ���ڴ��й���һ�����Խṹ�洢���дʻ㣬�������������洢���ʡ�ռ�
        itemsize: (SPACE*) ���� CRLF          // �����С(bytes)����type�ǵȳ�����±���ָ�������ȳ�����º��ԡ�
        compress: (SPACE*) (none | gzip) CRLF // �ʻ�����ѹ����ʽ����ѹ������gzipѹ����Ĭ��none
                                              // ��ѹ��������£��ᴴ��һ���µĲ�ѹ���Ĵʿ��ļ������ڲ���
        encoding: (SPACE*) ����ʶ�� CRLF    // Ĭ�ϱ��ر���
        wordscount: (SPACE*) ���� CRLF        // ָʾ�ʿ������ʻ�
        CRLF
        �ʻ�����
    }
 */

/* WordsLibException */
class WordsLibException : public std::exception
{
public:
	WordsLibException( LPCSTR what ) : std::exception(what) {}
};

class WordsLib
{

public:
	WordsLib( String const & filename ) throw(WordsLibException);
	~WordsLib();

	// ��str1����substr(str2, 0, strlen(str1))ʱ����0�����ڷ���1��С�ڷ���-1
	static int StrMatch( String const & str1, String const & str2 );
	// ����һ����(��ȷƥ��), �������ڴʿ��е�����, ���򷵻�-1
	int find( String const & word, int first, int last ) const;
	int find( String const & word ) const
	{
		return this->find( word, 0, wordscount - 1 );
	}
	// ����һ����(ģ��ƥ��), ������ƥ��ʿ��е�һ���ʵ������Լ�ƥ�䵽�Ĵ���, ���򷵻�-1
	int findEx( String const & word, int first, int last, int * count ) const;
	int findEx( String const & word, int * count ) const
	{
		return this->findEx( word, 0, wordscount - 1, count );
	}
	// �ִ�
	int splitWords( String const & text, StringArray * arrWords ) const;

	String prev() const;
	String next() const;
	String at( int i ) const;
	String seek( int index ) const;

	StringMap header;
	String name, desc, type, compress, encoding;
	int itemsize, wordscount;
	uint filesize;
protected:
	String getHeader( String const & key, String const & defval );
private:
	// ��ȡͷ����Ϣ���������洢
	void readHeaders();
	
	HANDLE _file, _fileMapping;
	int _dataOffset; // �ʻ�����ƫ��
	LPBYTE _p, _data;
	uint _dataSize; // ʵ�ʴʻ����ݴ�С
	mutable int _curIndex;
	WordsLib( WordsLib const & );
	WordsLib & operator = ( WordsLib const & );
};

} // namespace winplus

#endif // !defined(__WINPLUS_WORDSLIB_HPP__)
