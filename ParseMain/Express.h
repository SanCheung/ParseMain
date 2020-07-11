#pragma once

namespace		ZParser
{

class CRule;
class CExpress
{
public:
	CExpress( CRule *p );
	~CExpress(void);

	//wstring			m_str;
	//vector< CExpress* >			m_apSub;

	bool	Test( wstring str );

	bool	tree( wstring str, CExpress *pRoot );

	wstring		m_str;

	wstring		m_op;

	vector< CExpress* >		m_ap;

	// for tree()/tree2()/calc()
	CExpress*	m_pL;
	CExpress*	m_pR;

	CExpress*	m_pRoot;

	Number		calc( int level );

	// 20110504
	bool	tree2( wstring str, CExpress *pRoot );

	// 20110505
	void	LevelOutput( int level );

	bool	tree3( wstring str, CExpress *pRoot );


	// 20110506
	Number		calc3( int level );

	// 20110507
	wstring		m_strVarName;
	Number			m_value;
	
	CRule		*m_pCalc;

	// 20110514
	bool		calc4( int level, mapStr2Number &m );

};

};
