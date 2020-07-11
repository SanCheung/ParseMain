#pragma once

#define		BUFFSIZE		4086

class CParseHelper
{
public:
	CParseHelper(void);
	~CParseHelper(void);

	char	_buff[ BUFFSIZE ];

	bool	IsNumChar( char *p );
	Num		GetNumber( char *p, int n );
	bool	GetNewVar( char *&p );
	int		IsOpertorChar( char *p );
};
