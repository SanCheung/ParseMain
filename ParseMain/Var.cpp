#include "StdAfx.h"
#include "Var.h"

CVar::CVar(void)
: m_eType( vt_temp )
, m_dValue( 0. )
{

}

CVar::CVar( Num d )
: m_eType( vt_value )
, m_dValue( d )
{

}

CVar::CVar( string str )
: m_eType( vt_function )
, m_dValue( 0. )
{
	m_pFunc = new CFunction( str );
}

CVar::~CVar(void)
{
	if( m_pFunc != NULL )
	{
		delete m_pFunc;
		m_pFunc = NULL;
	}
}


CVar& CVar::operator = ( Num d )
{
	m_eType = vt_value;
	m_dValue = d;

	return *this;
}

void CVar::Show()
{
	if( IsValue() )
		printf( "%.4g\n", m_dValue );
	else
		printf( "var \n" );
}


bool CVar::Calc()
{
	if( vt_temp == m_eType )
		return false;

	if( vt_value == m_eType )
		return true;

	return m_pFunc->m_op->Calc( this );
}