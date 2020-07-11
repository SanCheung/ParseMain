#include "StdAfx.h"
#include "Express.h"
#include "Helper.h"
#include "FuncBase.h"
#include "Rule.h"

using namespace		ZParser;

CExpress::CExpress( CRule *p )
: m_pRoot( NULL )
, m_pCalc( p )
, m_pL( NULL )
, m_pR( NULL )
, m_value( 0.f )
{
}

CExpress::~CExpress(void)
{
	if( NULL != m_pL )
		DEL( m_pL );

	if( NULL != m_pR )
		DEL( m_pR );

	//int		n = (int)m_ap.size();
	//for( int i = 0; i < n; i++ )
	//{
	//	CExpress *p = m_ap[i];
	//	DEL( p );
	//}

	//m_ap.clear();
	ClearVector< CExpress >( m_ap );
}

bool CExpress::Test( wstring str )
{
	wchar_t		buf[32] = {0};
	//m_str = str;

	wstring		strT = str;

	int			index = 0;
	while( 1 )
	{
		// 获得括号子串
		wstring		strP;
		size_t	i = CHelper::GetSymParent( strT, strP );
		if( -2 == i )
			return false;

		// 没有括号了
		if( -1 == i )
			break;

		//CExpress	*p = new CExpress;
		//p->Test( strP );
		//m_apSub.push_back( p );
	
		swprintf( buf, L"\1%d", index );
		strT.replace( i, strP.size(), buf );

		index++;
	}

	return true;
}

bool CExpress::tree( wstring str, CExpress *pRoot )
{
	// 没有括号的运算符，建立运算树

	// + -      1
	// * / %    2
	// -        3
	// ()       4
	// function 5
	// 1.获取最小优先级的预算符号
	int		nMin = 10;
	int		nPos = -1;

	m_str = str;
	m_pRoot = pRoot;
	//if( str.size() == 1 )
	//	return true;
	int		n = (int)str.size();
	if( 1 == n ||
	    n == wcscspn( str.c_str(), OPCHARLIST ) )
	{
		return true;
	}

	wstring		strT = str;
	wchar		*p0 = (wchar *)strT.c_str();
	wchar		*p = p0;
	wchar		*pMin = NULL;
	while( 0 != *p )
	{
		if( CHelper::IsOpChar( *p ) )
		{
			int		nP = CHelper::GetOpPriority( *p );
			if( nP <= nMin )
			{
				nMin = nP;
				pMin = p;
			}

		}

		p++;
	}

	// 最小运算符没有找到
	if( NULL == pMin || 0 == *pMin )
		return false;

	m_op = *pMin;
	
	// 负操作
	//if( m_op.compare( L"-" && pMin == p0 )
	//{
	//}
	//else
	{
		m_pL = new CExpress( m_pCalc );
		
		int		size = int( pMin-p0 );
		wchar	*pBuf = new wchar[size+1];
		wcsncpy( pBuf, p0, size );
		pBuf[size] = 0;

		wstring		sT = pBuf;
		DEL( pBuf );

		if( !m_pL->tree( sT, pRoot ) )
			return false;
	}

	m_pR = new CExpress( m_pCalc );
	wstring		sT = (pMin+1);
	if( !m_pR->tree( sT, pRoot ) )
		return false;

	return true;
}

void CExpress::LevelOutput( int level )
{
	for( int i = 0; i < level; i++ )
		OUTPUT( L"\t" );
}

Number CExpress::calc( int level )
{
	wchar	*p = (wchar *)m_str.c_str();

	int		n = (int)m_str.size();
	int		npos = (int)wcscspn(p, OPCHARLIST );

	bool	bIsNumber = false;
	if( n == npos )
		bIsNumber = true;

	if( npos == 0 && L'-' == *p )
	{
		int pos2 = (int)wcscspn( m_str.c_str()+1, OPCHARLIST );
		if( pos2 == n-1 )
			bIsNumber = true;
	}
	
	// 负号+数字 直接转
	if( bIsNumber )
	{
		Number		v = _wtof( p );
		LevelOutput( level );
		OUTPUT( L"%s = %.4f\n", m_str.c_str(), v );
		return v;
	}

	Number		vL = 0.f;
	if( m_pL )
		vL = m_pL->calc( level+1 );
	
	Number		vR = m_pR->calc( level+1 );

	OUTPUT( L"%50s = ", m_str.c_str() );

	Number		v = 0;
	if( 0 == m_op.compare( L"+" ) )
	{
		OUTPUT( L"%.4f + %.4f", vL, vR );
		v = vL + vR;
	}

	if( 0 == m_op.compare( L"-" ) )
	{
		OUTPUT( L"%.4f - %.4f", vL, vR );
		v = vL - vR;
	}

	if( 0 == m_op.compare( L"*" ) )
	{
		OUTPUT( L"%.4f * %.4f", vL, vR );
		v = vL * vR;
	}

	if( 0 == m_op.compare( L"/" ) )
	{
		OUTPUT( L"%.4f / %.4f", vL, vR );
		v = vL / vR;
	}

	if( 0 == m_op.compare( L"%" ) )
	{
		OUTPUT( L" %.4f % %.4f", vL, vR );
		v = (int)vL % (int)vR;
	}

	OUTPUT( L" = %.2f\n", v );
	return v;
}


// 加入对()的解析
bool CExpress::tree2( wstring str, CExpress *pRoot )
{
	if( this == pRoot )
	{
		// 去掉空格
		CHelper::EraseSpace( str );

		// 去掉外面多余的括号
		CHelper::EraseSym( str );

		// 测试所有括号是否有效
		if( !CHelper::TestSym( str ) )
			return false;
	}
	else
	{
		// 去掉外面多余的括号
		CHelper::EraseSym( str );	
	}


	// 建立运算树
	// + -      1
	// * / %    2
	// -        3
	// ()       4
	// function 5
	// 1.获取最小优先级的预算符号
	int		nMin = 10;
	int		nPos = -1;

	m_str = str;
	m_pRoot = pRoot;

	int		n = (int)str.size();
	if( 1 == n ||
	    n == wcscspn( str.c_str(), OPCHARLIST ) )
	{
		return true;
	}

	wstring		strT = str;
	wchar		*p0 = (wchar *)strT.c_str();
	wchar		*p = p0;
	wchar		*pMin = NULL;

	int			nBasePr = 0;
	while( 0 != *p )
	{
		if( L'(' == *p )
		{
			nBasePr += 4;
		}
		else if( L')' == *p )
		{
			nBasePr -= 4;
		}
		else if( CHelper::IsOpChar( *p ) )
		{
			int		nP = CHelper::GetOpPriority( *p );
			nP += nBasePr;

			if( nP <= nMin )
			{
				nMin = nP;
				pMin = p;
			}

		}

		p++;
	}

	// 最小运算符没有找到
	if( NULL == pMin || 0 == *pMin )
		return false;

	m_op = *pMin;
	
	// 负操作
	//if( m_op == L'-' && pMin == p0 )
	//{
	//}
	//else
	{
		m_pL = new CExpress( m_pCalc );
		
		int		size = int(pMin-p0);
		wchar	*pBuf = new wchar[size+1];
		wcsncpy( pBuf, p0, size );
		pBuf[size] = 0;

		wstring		sT = pBuf;
		DEL( pBuf );

		if( !m_pL->tree2( sT, pRoot ) )
			return false;
	}

	m_pR = new CExpress( m_pCalc );
	wstring		sT = (pMin+1);
	if( !m_pR->tree2( sT, pRoot ) )
		return false;

	return true;
}


bool CExpress::tree3( wstring str, CExpress *pRoot )
{
	if( this != pRoot )
	{
		// 去掉外面多余的括号
		CHelper::EraseSym( str );	
	}

	m_str = str;
	m_pRoot = pRoot;

	// 单个数字，不用处理
	int		n = (int)str.size();
	if( n <= 1 )
		return true; 

	// 没有操作符号，不用处理
	int		pos = (int)wcscspn( str.c_str(), OPCHARLIST );
	if( pos == n )
		return true;

	// 操作符号-打头，直接加0
	if( pos == 0 && L'-' == str[0] )
	{
		int pos2 = (int)wcscspn( str.c_str()+1, OPCHARLIST );
		if( pos2 == n-1 )
			return true;

		m_str = L"0" + str;
		n++;
	}
	else
	{
		wstring					strFuncName;
		vector< wstring >		asPara;
		if( CHelper::TestFunction( str, strFuncName, asPara ) )
		{
			m_op = strFuncName;
			
			int		i;
			int		n = (int)asPara.size();
			for( i = 0; i < n; i++ )
			{
				CExpress *pE = new CExpress( m_pCalc );
				m_ap.push_back( pE );

				wstring		strP = asPara[i];
				if( !pE->tree3( strP, pRoot ) )
					return false;
			}
			
			return true;
		}
	}


	// 建立运算树
	// + -      1
	// * / %    2
	// ()       3
	// function 4
	// 1.获取最小优先级的预算符号
	int		nMin = 10;
	int		nPos = -1;

	wstring		strT = m_str;
	wchar		*p0 = (wchar *)strT.c_str();
	wchar		*p = p0;
	wchar		*pMin = NULL;

	int			nBasePr = 0;
	while( 0 != *p )
	{
		if( L'(' == *p )
		{
			nBasePr += 3;
		}
		else if( L')' == *p )
		{
			nBasePr -= 3;
		}
		else if( CHelper::IsOpChar( *p ) )
		{
			int		nP = CHelper::GetOpPriority( *p );
			nP += nBasePr;

			if( nP <= nMin )
			{
				nMin = nP;
				pMin = p;
			}
		}

		p++;
	}

	// 最小运算符没有找到
	if( NULL == pMin || 0 == *pMin )
		return false;

	int		size = int(pMin-p0);
	if( size == 0 )
		return false;

	if( size >= n )
		return false;
	
	wstring		sL;
	wstring		sR;

	{
		wchar	*pBuf = new wchar[size+1];
		wcsncpy( pBuf, p0, size );
		pBuf[size] = 0;

		sL = pBuf;
		DEL( pBuf );

		sR = (pMin+1);
	}

	m_op.push_back( *pMin );

	//m_pL = new CExpress;
	//if( !m_pL->tree3( sTL, pRoot ) )
	//	return false;

	//m_pR = new CExpress;
	//if( !m_pR->tree3( sTR, pRoot ) )
	//	return false;

	CExpress	*pL = new CExpress( m_pCalc );
	m_ap.push_back( pL );
	if( !pL->tree3( sL, pRoot ) )
		return false;

	CExpress	*pR = new CExpress( m_pCalc );
	m_ap.push_back( pR );
	if( !pR->tree3( sR, pRoot ) )
		return false;

	return true;
}

// 函数的计算
Number CExpress::calc3( int level )
{
	wchar	*p = (wchar *)m_str.c_str();

	int		n = (int)m_str.size();
	int		npos = (int)wcscspn(p, OPCHARLIST );

	bool	bIsNumber = false;
	if( n == npos )
		bIsNumber = true;

	if( npos == 0 && L'-' == *p )
	{
		int pos2 = (int)wcscspn( m_str.c_str()+1, OPCHARLIST );
		if( pos2 == n-1 )
			bIsNumber = true;
	}
	
	// 负号+数字 直接转
	if( bIsNumber )
	{
		Number		v = _wtof( p );
		//LevelOutput( level );
		//OUTPUT( L"%s = %.4f\n", m_str.c_str(), v );

		m_strVarName = m_str;
		return v;
	}

	//Number		vL = m_pL->calc( level+1 );
	//Number		vR = m_pR->calc( level+1 );

	vector< Number >	av;

	int		i;
	n = (int)m_ap.size();
	for( i = 0; i < n; i++ )
	{
		Number		t = m_ap[i]->calc3( level+1 );
		av.push_back( t );
	}

	OUTPUT( L"%d \t %5s ", m_pCalc->m_nCalcIndex, m_op.c_str() );
	for( i = 0; i < n; i++ )
		OUTPUT( L" %s", m_ap[i]->m_strVarName.c_str() );

	//OUTPUT( L"%d\t %s = ", m_pCalc->m_nCalcIndex, m_str.c_str() );
	OUTPUT( L" = " );
	m_pCalc->m_nCalcIndex++;

	//if( m_pCalc->m_nCalcIndex > 1000 )
	//	return false;
	
	wchar	buf[16] = {0};
	swprintf( buf, L"v%d", m_pCalc->m_nCalcIndex );
	m_strVarName = buf;

	Number		v = 0;
	if( 0 == m_op.compare( L"+" ) )
	{
		OUTPUT( L"%.2f + %.2f", av[0], av[1] );
		v = av[0] + av[1];
	}
	else if( 0 == m_op.compare( L"-" ) )
	{
		OUTPUT( L"%.2f - %.2f", av[0], av[1] );
		v = av[0] - av[1];
	}
	else if( 0 == m_op.compare( L"*" ) )
	{
		OUTPUT( L"%.2f * %.2f", av[0], av[1] );
		v = av[0] * av[1];
	}
	else if( 0 == m_op.compare( L"/" ) )
	{
		OUTPUT( L"%.2f / %.2ff", av[0], av[1] );
		v = av[0] / av[1];
	}
	else if( 0 == m_op.compare( L"%" ) )
	{
		OUTPUT( L" %.2f %% %.2f", av[0], av[1] );
		v = (int)av[0] % (int)av[1];
	}
	else if( 0 == m_op.compare( L"sin" ) )
	{
		OUTPUT( L" sin(%.2f)", av[0] );
		v = sin( av[0] );
	}
	else if( 0 == m_op.compare( L"min" ) )
	{
		OUTPUT( L" min(%.2f,%.2f)", av[0], av[1] );
		v = min( av[0], av[1] );
	}
	else if( 0 == m_op.compare( L"max" ) )
	{
		OUTPUT( L" max(%.2f,%.2f)", av[0], av[1] );
		v = max( av[0], av[1] );
	}

	//OUTPUT( L"\n  %s = %.2f\n\n", m_strVarName.c_str(), v );
	OUTPUT( L"\t\t= %.2f\n", v );
	return v;
}

// 20110514 
bool CExpress::calc4( int level, mapStr2Number &m )
{
	ATLTRACE( L"%s    %f\n", m_str.c_str(), m.begin()->second );

	if( 0 == CHelper::IsNumberString( m_str ) )
	{
		m_strVarName = m_str;
		m_value = _wtof( m_str.c_str() );

		ATLTRACE( L"\t%s = %f            %f\n", m_str.c_str(), m_value, m.begin()->second ); 
		return true;
	}
	else if( CHelper::IsVarString( m_str ) )
	{
		m_strVarName = m_str;

		//if( !m_pCalc->GetValueByVar( m_str, m_value ) &&
		//	NULL == m_pCalc->FindData( m_str ) )
		//	return false;
		if( !CHelper::GetValueByVar( m, m_str, m_value ) &&
			NULL == m_pCalc->FindData( m_str ) )
		{
			m_pCalc->_strError = L"无法找到变量，或无法找到数据" + m_str;
			return false;
		}

		ATLTRACE( L"\t%s = %f            %f\n", m_str.c_str(), m_value, m.begin()->second ); 
		return true;
	}

	int		i;
	int		n = (int)m_ap.size();
	
	if( CHelper::CompareNoCase( m_op, L"if" ) )
	{
		if( n != 3 )
			return false;

		if( !m_ap[0]->calc4( level+1, m ) )
			return false;

		CExpress *pR = m_ap[2];
		if( m_ap[0]->m_value > 0 )
			pR = m_ap[1];

		if( !pR->calc4( level+1, m ) )
			return false;

		m_value = pR->m_value;
	}
	else
	{
		for( i = 0; i < n; i++ )
		{
			if( !m_ap[i]->calc4( level+1, m ) )
				return false;
		}

		OUTPUT( L"%d \t %5s ", m_pCalc->m_nCalcIndex, m_op.c_str() );
		for( i = 0; i < n; i++ )
			OUTPUT( L" %s", m_ap[i]->m_strVarName.c_str() );

		//OUTPUT( L"%d\t %s = ", m_pCalc->m_nCalcIndex, m_str.c_str() );
		OUTPUT( L" = " );
		m_pCalc->m_nCalcIndex++;

		if( m_pCalc->m_nCalcIndex > 1000 ) 
			return false;
		
		wchar	buf[16] = {0};
		swprintf( buf, L"v%d", m_pCalc->m_nCalcIndex );
		m_strVarName = buf;

		Number		v = 0.;

		int		nRet1 = m_pCalc->CallUserFunction( v, m_op, m_ap );
		if(	-2 == nRet1 )
		{
			m_pCalc->_strError = L"用户函数 " + m_op + L" 运算失败";
			return false;
		}
		else if( -1 == nRet1 )
		{
			int		nRet = m_pCalc->CallFunction( v, m_op, m_ap );
			if( 0 != nRet )
			{
				if( -1 == nRet )
					m_pCalc->_strError = L"未知的函数 " + m_op;
				else if( -2 == nRet )
					m_pCalc->_strError = L"参数不符合 " + m_op;
				return false;
			}
		}

		//OUTPUT( L"\n  %s = %.2f\n\n", m_strVarName.c_str(), v );
		OUTPUT( L"\t\t= %.2f  (%s)\n", v, m_strVarName.c_str() );
		m_value = v;
	}

	ATLTRACE( L"\t%s = %f            %f\n", m_str.c_str(), m_value, m.begin()->second );  
	return true;
}
