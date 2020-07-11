#include "StdAfx.h"
#include "Operator.h"
#include "Var.h"

COperator::COperator(void)
 : m_nPriority( 0 )
{
}

COperator::~COperator(void)
{
}

//bool COperator::Calc( const vector< CVariable* > &av, CVariable *out )
//{
//	return true;
//}

bool COperator::Calc( CVar *v )
{
	int		i;
	int		n = (int)v->m_pFunc->m_ip.size();
	for( i = 0; i < n; i++ )
	{
		CVar *p = v->m_pFunc->m_ip[i];

		if( p->IsValue() )
			continue;

		if( !p->Calc() )
			return false;
	}

	return true;
}

#include "OpAdd.h"
COperator* COperator::newOp( int type )
{
	if( 1 == type )
		return new COpAdd;

	if( 2 == type )
		return new COpSub;
	
	if( 3 == type )
		return new COpMul;

	if( 4 == type )
		return new COpDiv;

	return NULL;
}
