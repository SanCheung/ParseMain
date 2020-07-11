#pragma once

class CHelper
{
public:
	CHelper(void);
	virtual ~CHelper(void);

	static size_t		GetSymParent( wstring str, wstring &strSub );
	static bool			IsOpChar( wchar_t ch );

	static int			GetOpPriority( wchar_t ch );
	
	static size_t		GetOp( wstring str, size_t nFrom );

	static void			EraseSym( wstring &str );
	static void			EraseSpace( wstring &str );

	// ²»º¬º¯Êý À¨ºÅ²âÊÔ
	static bool			TestSym( wstring str );

	// º¬º¯ÊýµÄÀ¨ºÅ²âÊÔ
	static bool			TestFunction( wstring str, wstring &strFuncName, vector<wstring> &asPara );

	// Ô¤´¦Àí
	static bool			Pretreat( wstring &str );

	// ·Ö¸îstringµ½ stringarray
	static size_t		String2Array( wstring str, wstringarray &as, wstring strTok );

	static bool			IsData( wstring str, wstring &strName, wstring &strDatalist );
	static bool			IsFunction( wstring str, wstring &strName, wstring &strParaList, wstring &strExp );

	static bool			IsNumberChar( wchar ch );
	static bool			HasOpChar( wstring str );
	
	static bool			IsVarString( wstring str );
	static int			IsNumberString( wstring str );

	static bool			IsIntegerString( const wchar *p, int n );
	static bool			IsFloatString( const wchar *p, int n );

	static bool			GetValueByVar( mapStr2Number &m, wstring str, Number &v );
	static bool			CompareNoCase( wstring s1, wstring s2 );

};
