#include "StdAfx.h"
#include "ParseHelper.h"

CParseHelper::CParseHelper(void)
{
	memset( _buff, 0, BUFFSIZE );
}

CParseHelper::~CParseHelper(void)
{
}

bool CParseHelper::IsNumChar( char *p )
{
	if( *p >= '0' && *p <= '9' || *p == '.' )
		return true;
	
	return false;
}

Num CParseHelper::GetNumber( char *p, int n )
{
	assert( n > 0 && n < BUFFSIZE );

	memset( _buff, 0, BUFFSIZE );
	memcpy( _buff, p, n );

	return (Num)::atof( _buff );
}


bool CParseHelper::GetNewVar( char *&p )
{
	if( '(' != *p )
		return false;

	int		b = -1;

	char	*t = p++;
	while( *t == 0  )
	{
		if( *t == '(' )
			b--;
		else if( *t == ')' )
			b++;

		if( b == 0 )
			break;

		t++;
	}

	memset( _buff, 0, BUFFSIZE );
	memcpy( _buff, p, t-p );

	p = t;
	return true;
}

int CParseHelper::IsOpertorChar( char *p )
{
	if( '+' == *p )
		return 1;

	if( '-' == *p )
		return 2;

	if( '*' == *p )
		return 3;

	if( '/' == *p )
		return 4;

	return 0;
}