#pragma once

class CVar;
class COperator;
class CFunction
{
public:
	CFunction( string str );
	~CFunction(void);

	COperator	*m_op;
	vector
	< CVar* >	m_ip;

	// no err,  return 0
	int		Parse( string str );
};
