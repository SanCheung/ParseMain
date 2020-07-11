#pragma once
#include "Operator.h"
#include "Function.h"

class CVar
{
public:
	CVar( void );
	CVar( Num d );
	CVar( string str );

	~CVar(void);

	enum eType
	{
		vt_temp		= -1,
		vt_value	= 0,
		vt_function = 1,
	};

	int			m_eType;
	Num			m_dValue;
	CFunction	*m_pFunc;

	bool	IsValue()	{ return (m_eType==vt_value); }

	CVar& operator = ( Num d );

	bool		Calc();
	void		Show();
};
