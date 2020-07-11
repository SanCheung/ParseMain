// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <tchar.h>

#include <assert.h>
#include <math.h>

#pragma warning ( disable:4996 )

#include <vector>
#include <stack>
#include <string>
#include <map>
using namespace std;

typedef		double		Number;
typedef		wchar_t		wchar;

#define		OPCHARLIST	L"+-*/%()"

#define		DEL(p)			{ delete p; p = NULL; }
#define		EQUAL( m, n )   (abs(m-n) < 1e-3)

typedef		vector< wstring >		wstringarray;
typedef		map< wstring, Number >	mapStr2Number;



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