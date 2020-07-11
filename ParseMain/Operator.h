#pragma once


class CVar;
class COperator
{
public:
	COperator(void);
	virtual ~COperator(void);

	int		m_nPriority;

	virtual bool Calc( CVar *v );


	static	COperator* newOp( int type );
};
