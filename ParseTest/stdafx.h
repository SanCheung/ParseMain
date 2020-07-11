// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once

// Change these values to use different versions
#define WINVER		0x0500
#define _WIN32_WINNT	0x0501
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0200

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#define	_ATL_USE_CSTRING_FLOAT

#include <atlwin.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>

#include <atlmisc.h>


#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif


#include <assert.h>
#include <math.h>

#pragma warning ( disable:4996 )
#pragma warning ( disable:4530 )

#include <vector>
#include <stack>
#include <string>
#include <map>
using namespace std;

typedef		double		Number;
typedef		wchar_t		wchar;

#define		OPCHARLIST	L"+-*/%()"

#define		DEL(p)		{ delete p; p = NULL; }
#define		EQUAL( m, n )   (abs(m-n) < 1e-3)

typedef		vector< wstring >			wstringarray;
typedef		map< wstring, Number >		mapStr2Number;

#define		OUTPUT		wprintf
//#define		OUTPUT		


template< class T >
void  ClearVector( vector< T* > &v )
{
	int		i;
	int		n = (int)v.size();
	for( i = 0; i < n; i++ )
	{
		T* p = v[i];
		DEL( p );
	}
	v.clear();
}

template< class T >
T*	FindItem( vector< T* > &v, wstring strName )
{
	int		i;
	int		n = (int)v.size();
	for( i = 0; i < n; i++ )
	{
		T* p = v[i];
		if( p->_strName.compare( strName ) == 0 )
			return p;
	}

	return NULL;
}