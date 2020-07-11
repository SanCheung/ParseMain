#include "StdAfx.h"
#include "OpAdd.h"
#include "Var.h"

bool COpAdd::Calc( CVar *v )
{
	if( !COperator::Calc( v ) )
		return false;

	int	n = (int)v->m_pFunc->m_ip.size();
	if( 2 != n )
		return false;

	CVar *p1 = v->m_pFunc->m_ip[0];
	CVar *p2 = v->m_pFunc->m_ip[1];

	*v = p1->m_dValue + p2->m_dValue;
	return true;
}

bool COpSub::Calc( CVar *v )
{
	if( !COperator::Calc( v ) )
		return false;

	int	n = (int)v->m_pFunc->m_ip.size();
	if( 2 != n )
		return false;

	CVar *p1 = v->m_pFunc->m_ip[0];
	CVar *p2 = v->m_pFunc->m_ip[1];

	*v = p1->m_dValue - p2->m_dValue;
	return true;
}

bool COpMul::Calc( CVar *v )
{
	if( !COperator::Calc( v ) )
		return false;

	int	n = (int)v->m_pFunc->m_ip.size();
	if( 2 != n )
		return false;

	CVar *p1 = v->m_pFunc->m_ip[0];
	CVar *p2 = v->m_pFunc->m_ip[1];

	*v = p1->m_dValue * p2->m_dValue;
	return true;
}

bool COpDiv::Calc( CVar *v )
{
	if( !COperator::Calc( v ) )
		return false;

	int	n = (int)v->m_pFunc->m_ip.size();
	if( 2 != n )
		return false;

	CVar *p1 = v->m_pFunc->m_ip[0];
	CVar *p2 = v->m_pFunc->m_ip[1];

	*v = p1->m_dValue / p2->m_dValue;
	return true;
}

bool COpPr::Calc( CVar *v )
{
	*v = v->m_pFunc->m_ip[0]->m_dValue;
	return true;
}

bool COpNe::Calc( CVar *v )
{
	*v = -v->m_pFunc->m_ip[0]->m_dValue;
	return true;
}
