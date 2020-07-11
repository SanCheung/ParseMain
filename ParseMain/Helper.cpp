#include "StdAfx.h"
#include "Helper.h"

CHelper::CHelper(void)
{
}

CHelper::~CHelper(void)
{
}

size_t CHelper::GetSymParent( wstring str, wstring &strSub )
{
	int			nLeft = 0;
	int			nRight= 0;

	int			nFirstStart = -1;
	int			nFirstEnd = -1;
	
	wchar_t		*p0 = (wchar_t *)str.c_str();
	size_t		n = str.length();

	wchar_t		*p = p0;
	while( *p != 0 )
	{
		if( *p == L'(' )
		{
			if( 0 == nLeft )
				nFirstStart = int(p - p0);
			nLeft++;
		}

		if( *p == L')' )
			nRight++;
		
		p++;

		if( -1 == nFirstEnd && nLeft == nRight && nLeft > 0 )
			nFirstEnd = int(p - p0);
	}
	
	// () 数量不一致
	if( nLeft != nRight )
		return -2;

	if( -1 == nFirstStart )
		return -1;

	//assert( nFirstStart != -1 || nFirstEnd - nFirstStart <= 0 );
	strSub = str.substr( nFirstStart, nFirstEnd-nFirstStart );
	return nFirstStart;
}

bool CHelper::IsOpChar( wchar_t ch )
{
	wchar_t		*ch_list = L"+-*/%";
	return ( NULL != wcschr( ch_list, ch ) );
}


// + -      1
// * / %    2
// -        3
// ()       4
// function 5
int CHelper::GetOpPriority( wchar_t ch )
{
	if( L'+' == ch || L'-' == ch )
		return 1;

	if( L'*' == ch || L'/' == ch || L'%' == ch )
		return 2;

	return 0;
}

size_t CHelper::GetOp( wstring str, size_t nFrom )
{
	
	return 0;
}


void CHelper::EraseSym( wstring &str )
{
	stack< int >				sl;
	vector< pair<int,int> >		ap;

	wchar_t		*p0 = (wchar_t *)str.c_str();
	wchar_t		*p = p0;
	while( *p != 0 )
	{
		if( *p == L'(' )
			sl.push( int(p-p0) );
		else if( *p == L')' )
		{
			if( sl.empty() )
				return;

			int	nL = sl.top();
			int	nR = int(p-p0);
			ap.push_back( pair<int,int>( nL, nR ) );

			sl.pop();
		}
		
		p++;
	}

	if( ap.empty() )
		return;
	
	pair<int,int>  pr = ap.back();

	while( 0 == pr.first && str.size()-1 == pr.second )
	{
		str.erase( pr.second );
		str.erase( 0, 1 );
		ap.pop_back();

		int		n = (int)ap.size();
		if( 0 == n )
			break;

		for( int i = 0; i < n; i++ )
		{
			ap[i].first--;
			ap[i].second--;
		}

		pr = ap.back();
	}
}

void CHelper::EraseSpace( wstring &str )
{
	int		n = (int)str.size();

	while( n > 0 )
	{
		n--;
		if( str[n] == L' ' )
			str.erase( n, 1 );
		if( str[n] == L'\n' )
			str.erase( n, 1 );
		if( str[n] == L'\r' )
			str.erase( n, 1 );
		if( str[n] == 9 )
			str.erase( n, 1 );
	}
}


bool CHelper::TestSym( wstring str )
{
	stack< int >	sl;

	wchar_t		*p0 = (wchar_t *)str.c_str();
	wchar_t		*p = p0;
	while( *p != 0 )
	{
		if( *p == L'(' )
			sl.push( int(p-p0) );
		else if( *p == L')' )
		{
			if( sl.empty() )
				return false;

			sl.pop();
		}
		
		p++;
	}

	if( sl.empty() )
		return true;
	
	return false;
}

bool CHelper::TestFunction( wstring str, wstring &strFuncName, vector<wstring> &asPara )
{
	int		nsize = (int)str.size();
	if( nsize < 3 )
		return false;

	int		npos = (int)str.find( L'(' );
	if( npos <= 0 )
		return false;

	if( str[nsize-1] != L')' )
		return false;

	str.erase( nsize-1 );
	strFuncName = _wcslwr( (wchar *)str.substr( 0, npos ).c_str() );
	if( !CHelper::IsVarString( strFuncName ) )
		return false;

	str.erase( 0, npos+1 );

	if( !TestSym( str ) )
		return false;

	//npos = (int)str.find( L"," );
	//while( -1 != npos )
	//{
	//	asPara.push_back( str.substr( 0, npos ) );
	//	str.erase( 0, npos+1 );

	//	npos = (int)str.find( L"," );
	//}

	//if( !str.empty() )
	//	asPara.push_back( str );

	// L"x-4,f(5),q(1,2);
	// 这类情况的处理
	int	npos1 = (int)str.find( L"," );
	int	npos2 = (int)str.find( L"(" );
	int	npos3 = (int)str.find( L")" );
	while( -1 != npos1 )
	{
		bool		bSkip = false;
		if( npos2 == -1 || 
			npos1 < npos2 ||
			( npos3 != -1 && npos1 > npos3 ) )
		{
			asPara.push_back( str.substr( 0, npos1 ) );
			str.erase( 0, npos1+1 );
			npos1 = (int)str.find( L"," );
			npos2 = (int)str.find( L"(" );
			npos3 = (int)str.find( L")" );
		}
		else
		{
			npos1 = (int)str.find( L",", npos3+1 );
			npos2 = (int)str.find( L"(", npos3+1 );
			npos3 = (int)str.find( L")", npos3+1 );
		}
	}

	if( !str.empty() )
		asPara.push_back( str );

	if( asPara.empty() )
		return false;

	return true;
}

// 20110505
// 函数的处理
bool CHelper::Pretreat( wstring &str )
{
	// 去掉空格
	EraseSpace( str );

	// 去掉外面多余的括号
	EraseSym( str );

	// 测试所有括号是否有效匹配
	if( !TestSym( str ) )
		return false;
	
	return true;
}

size_t CHelper::String2Array( wstring str, wstringarray &as, wstring strTok )
{
	if( str.empty() )
		return 0;

	size_t	nTokSize = strTok.size();
	size_t	npos = str.find( strTok );
	while( -1 != npos )
	{
		wstring strT = str.substr( 0, npos );
		as.push_back( strT );

		str.erase( 0, npos+nTokSize );
		npos = str.find( strTok );
	}
	
	if( !str.empty() )
		as.push_back( str );

	return as.size();
}

bool CHelper::IsData( wstring str, wstring &strName, wstring &strDatalist )
{
	// prime={2,3,5,7,9,11}
	int	npos = (int)str.find( L'=' );
	if( -1 == npos || 0 == npos )
		return false;
	
	strName = str.substr( 0, npos );
	str.erase( 0, npos+1 );
	
	if( str.size() < 3 )
		return false;

	if( L'{' != str[0] || L'}' != str[str.size()-1] )
		return false;

	str.erase( 0, 1 );
	str.erase( str.size()-1, 1 );

	strDatalist = str;
	return true;
}

bool CHelper::IsFunction( wstring str, wstring &strName, wstring &strParaList, wstring &strExp )
{
	// main(x)=3.1416*pow(x/2,2)+list_getValue(3)
	int	npos = (int)str.find( L'=' );
	if( -1 == npos || 0 == npos )
		return false;
	
	wstring		strLeft;
	strLeft = str.substr( 0, npos );
	str.erase( 0, npos+1 );

	int	nsize = (int)strLeft.size();
	if( nsize < 4 )
		return false;
	
	if( strLeft[nsize-1] != L')' )
		return false;

	npos = (int)strLeft.find( L'(' );
	if( -1 == npos )
		return false;

	strName = strLeft.substr( 0, npos );
	strLeft.erase( nsize-1, 1 );
	strLeft.erase( 0, npos+1 );

	strParaList = strLeft;
	strExp = str;
	return true;
}

bool CHelper::IsNumberChar( wchar ch )
{
	if( ch >= L'0' && ch <= '9' )
		return true;

	return false;
}

bool CHelper::IsVarString( wstring str )
{
	size_t	n = str.size();
	if( n < 0 )
		return false;

	if( IsNumberChar( str[0] ) )
		return false;

	if( HasOpChar( str ) )
		return false;

	return true;
}


bool CHelper::IsIntegerString( const wchar *p, int n )
{
	int		i = 0;
	bool	bIsNeg = (L'-'==p[0]);
	if( bIsNeg )
		i = 1;

	// 第一位如果是0, 则不是正确的integer;
	if( p[i] == L'0' && n > i+1 )
		return false;

	for( ; i < n; i++ )
	{
		if( !IsNumberChar( p[i] ) )
			return false;
	}

	return (i == n);
}

bool CHelper::IsFloatString( const wchar *p, int n )
{
	bool	bIsNeg = (L'-'==p[0]);
	if( bIsNeg )
	{
		p++;
		n--;
	}
	
	int		i;

	// 小数点位置
	int		nPos = -1;

	for( i = 0; i < n; i++ )
	{
		if( L'.' == p[i] )
		{
			// 多个小数点，不是正确的小数
			if( -1 != nPos )
				return false;

			nPos = i;
		}
	}

	// 没有小数点，自然不是小数
	if( -1 == nPos )
		return false;
	
	bool	bLeft = false;

	// 支持 .11 = 0.11的表示方法
	if( nPos == 0 )
	{
		bLeft = true;
	}
	else
	{
		if( p[0] == L'0' && nPos > 1 )
			return false;

		if( IsIntegerString( p, nPos-1 ) )
			bLeft = true;
	}

	if( !bLeft )
		return false;

	for( int i = nPos+1; i < n; i++ )
	{
		if( !IsNumberChar( p[i] ) )
			return false;
	}		

	return true;
}

// return value
// 0  OK
// <0  ERROR
// -1 空字符串
// -2 含非数字的字符
// -3 普通计数法表示错误
//
// -11 科学计数法左部为空
// -12 科学计数法右部为空
// -13 科学计数法左部只有负号没有数字
// -14 科学计数法左部超出 [1.0, 10)范围
// -15 科学计数法右部只有一个负号没有数字
// -16 科学计数法右部第一位是0
// -17 科学计数法右部不是整数
int CHelper::IsNumberString( wstring str )
{
	size_t	n = str.size();
	
	// -1 空字符串
	if( n <= 0 )
		return -1;

	size_t	i;
	for( i = 0; i < n; i++ )
	{
		wchar	ch = str[i];
		if( !IsNumberChar( ch ) &&
			L'.' != ch && 
			L'-' != ch &&
			L'E' != ch && 
			L'e' != ch  )
		{
			// -2 含非数字的字符
			return -2;
		}
	}

	int		npos = (int)wcscspn( str.c_str(), L"Ee" );
	if( n == npos )
	{
		if( IsIntegerString( str.c_str(), (int)str.size() ) )
			return 0;

		if( IsFloatString( str.c_str(), (int)str.size() ) )
			return 0;
		
		// 普通计数法表示错误
		return -3;
	}

	// 科学记数法
	wstring		strLeft = str.substr( 0, npos );
	wstring		strRight = str.substr( npos+1 );

	// 科学计数法左部为空
	if( strLeft.empty() )
		return -11;

	// 科学计数法右部为空
	if( strRight.empty() )
		return -12;

	if( strLeft[0] == L'-' )
	{
		strLeft.erase( 0, 1 );

		// 科学计数法左部只有负号没有数字
		if( strLeft.empty() )
			return -13;
	}

	wchar	ch = strLeft[0];
	// 科学计数法左部超出 [1.0, 10)范围
	if( ch < L'1' || ch > '9' )
		return -14;

	if( strLeft.size() > 1 )
	{
		if( L'.' != strLeft[1] )
			return -14;
	}

	if( strRight[0] == L'-' )
	{
		// 科学计数法右部只有一个负号没有数字
		strRight.erase( 0, 1 );
		if( strRight.empty() )
			return -15;
	}

	// 科学计数法右部第一位是0
	if( strRight[0] == L'0' )
		return -16;
	
	// 科学计数法右部不是整数
	if( !IsIntegerString( strRight.c_str(), (int)strRight.size() ) )
		return -17;

	return 0;
}


bool CHelper::HasOpChar( wstring str )
{
	int		npos = (int)wcscspn(str.c_str(), OPCHARLIST );
	return ( str.size()!=npos );
}

bool CHelper::GetValueByVar( mapStr2Number &m, wstring str, Number &v )
{
	mapStr2Number::iterator		it;
	it = m.find( str );
	if( it == m.end() )
		return false;

	v = it->second;
	return true;
}

bool CHelper::CompareNoCase( wstring s1, wstring s2 )
{
	if( _wcsicmp( s1.c_str(), s2.c_str() ) == 0 )
		return true;

	return false;
}