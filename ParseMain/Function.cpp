#include "StdAfx.h"
#include "Function.h"
#include "Operator.h"
#include "Var.h"
#include "ParseHelper.h"

CFunction::CFunction( string str )
{
	Parse( str );
}

CFunction::~CFunction(void)
{
	int	n = (int)m_ip.size();
	for( int i = 0; i < n; i++ )
	{
		CVar *p = m_ip[i];
		delete p;
		p = NULL;
	}

	m_ip.clear();
}


int CFunction::Parse( string str )
{
	//if( str.empty() )
	//	return 1;

	//// -1+(2-3)*4/5-6
	//CParseHelper	h;

	//char	*p0 = (char *)str.c_str();
	//char	*p = p0;

	//vector< CFunction* >	af;

	//CVar		*pV = NULL;
	//COperator	*pO = NULL;
	//
	////	-1		none
	////  0       [N] num
	////  1		-   ng 
	////	2       (
	////  3       [F]
	//int			flag = -1;

	//if( h.IsNumChar( p ) )
	//	flag = 0;
	//else if( *p == '-' )
	//	flag = 1;
	//else if( '(' == *p )
	//	flag = 2;
	//else if( h.IsOpertorChar( p ) )
	//	return 2;

	//p++;
	//while( 1 )
	//{
	//	if( *p == 0 )
	//		break;

	//	if( flag == 0 )
	//	{
	//		if( h.IsNumChar( p ) )
	//			p++;
	//	}
	//	else if( flag == 1 )
	//	{
	//		if( flag == 0 )
	//		{
	//			int	n = int( p - p0 );
	//			if( n > 0 )
	//			{
	//				Num d = h.GetNumber( p0, n );
	//				pVar = new CVar( d );
	//				p0 = p;
	//			}
	//		}

	//		int	type = h.IsOpertorChar( p );
	//		if( type > 0)
	//			pOp = COperator::newOp( type );
	//	}

	//	else if( '(' == *p )
	//	{
	//		if( h.GetNewVar( p ) )
	//		{
	//			string	s = h._buff;
	//			pVar = new CVar( s );
	//		}
	//	}
	//
	//
	//}

	return true;
}