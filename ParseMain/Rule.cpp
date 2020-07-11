#include "StdAfx.h"
#include "Rule.h"
#include "helper.h"

using namespace		ZParser;

void CData::ClearAll()
{
	if( _pData != NULL )
	{
		delete[] _pData;
		_pData = NULL;
	}
}

void CFunction::ClearAll()
{
	if( NULL != _pExp )
		DEL( _pExp );
}

CRule::CRule(void)
: _pMain( NULL )
, m_nCalcIndex( 0 )
{
}

CRule::~CRule(void)
{
	ClearVector< CData >( _apData );
	ClearVector< CFunction >( _apFunc );
}

CData* CRule::FindData( wstring strName )
{
	return FindItem< CData >( _apData, strName );
}

CFunction* CRule::FindFunction( wstring strName )
{
	return FindItem< CFunction >( _apFunc, strName );

}

bool CRule::Parse( wstring str )
{
	// "prime={2,3,5,7,9,11}; main(x)=3.1416*pow(x/2,2)+data_getValue(3);"

	// ȥ���ո�
	CHelper::EraseSpace( str );

	wstringarray		as;
	size_t n = CHelper::String2Array( str, as, L";" );
	if( 0 == n )
	{
		_strError = L"�޷������Ч���ʽ";
		return false;
	}

	int			nDataCount = 0;
	int			nFuncCount = 0;
	int			nMainFunc = 0;

	size_t		i, j;
	for( i = 0; i < n; i++ )
	{
		wstring		strTemp = as[i];
		OUTPUT( L"%d: %s\n", i+1, strTemp.c_str() );

		wstring		strName;
		wstring		strList;
		wstring		strExp;
		if( CHelper::IsData( strTemp, strName, strList ) )
		{
			wstringarray	asData;
			size_t nDataCount = CHelper::String2Array( strList, asData, L"," );
			if( 0 == nDataCount )
			{
				_strError = L"���ݱ��ʽ����Ϊ��";
				return false;				
			}

			for( j = 0; j < nDataCount; j++ )
			{
				wstring		str = asData[j];
				if( 0 != CHelper::IsNumberString( str ) )
				{
					_strError = L"���ݱ��ʽ�д�����Ч�����ֱ��ʽ";
					return false;
				}		
			}

			CData *p = new CData;
			_apData.push_back( p );
			
			p->_strName = strName;
			p->_nSize = (int)nDataCount;
			p->_pData = new Number[nDataCount];
			for( j = 0; j < nDataCount; j++ )
				p->_pData[j] = _wtof( asData[j].c_str() );

			nDataCount++;
		}
		else if( CHelper::IsFunction( strTemp, strName, strList, strExp ) )
		{
			wstringarray		asPara;
			size_t nCount = CHelper::String2Array( strList, asPara, L"," );

			if( !CHelper::IsVarString( strName ) )
				return false;

			for( j = 0; j < nCount; j++ )
			{
				if( !CHelper::IsVarString( asPara[j] ) )
					return false;
			}
			
			CExpress	*pE = new CExpress( this );
			if( !pE->tree3( strExp, pE ) )
			{
				DEL( pE );
				return false;
			}

			CFunction	*p = new CFunction;
			_apFunc.push_back( p );

			p->_strName = strName;
			p->_arPara.insert( p->_arPara.begin(), asPara.begin(), asPara.end() );
			p->_pExp = pE;

			//if( strName.compare( L"main" ) == 0 )
			if( CHelper::CompareNoCase( strName, L"main" ) )
			{
				if( 0 == nMainFunc )
					_pMain = p;

				nMainFunc++;
			}

			nFuncCount++;
		}
		else
		{
			_strError = L"����δ������ʽ";
			return false;			
		}
	}

	if( 1 != nMainFunc )
	{
		_strError = L"���������������ж��������";
		return false;
	}

	return true;
}

bool CRule::ParseVarValues( wstring str )
{
	// ȥ���ո�
	CHelper::EraseSpace( str );

	wstringarray		as;
	size_t	n = CHelper::String2Array( str, as, L";" );
	if( n < 1 )
		return false;

	_mVar2Value.clear();

	size_t	i;
	for( i = 0; i < n; i++ )
	{
		wstring		strT = as[i];
		int			npos = (int)strT.find( L'=' );
		if( -1 == npos )
			return false;

		wstring		strL = strT.substr( 0, npos );
		wstring		strR = strT.substr( npos+1 );

		if( !CHelper::IsVarString( strL ) )
			return false;

		if( 0 != CHelper::IsNumberString( strR ) )
			return false;

		_mVar2Value.insert( pair< wstring, Number >( strL, _wtof( strR.c_str() ) ) );
	}
	
	return true;
}


bool CRule::Calc( wstring str, Number &vResult )
{
	if( NULL == _pMain )
		return false;
	
	if( !ParseVarValues( str ) )
	{
		_strError = L"��������";
		return false;
	}

	m_nCalcIndex = 0;
	if( !_pMain->_pExp->calc4( 0, _mVar2Value ) )
	{
		//_strError = L"�������������";
		return false;
	}

	vResult = _pMain->_pExp->m_value;
	return true;
}

bool CRule::GetValueByVar( wstring str, Number &v )
{
	mapStr2Number::iterator		it;
	it = _mVar2Value.find( str );
	if( it == _mVar2Value.end() )
		return false;

	v = it->second;
	return true;
}

// 0	ok
// -1	û�ж���
// -2	���ô���
int  CRule::CallUserFunction( Number &v, wstring op, const vector< CExpress* > &ap )
{
	CFunction *p = FindFunction( op );
	if( NULL == p )
		return -1;

	mapStr2Number		m;
	
	OUTPUT( L"\nCall Function %s\t",  op.c_str() );
	
	int		i;
	int		n = (int)ap.size();
	for( i = 0; i < n; i++ )
	{
		wstring str = p->_arPara[i];
		
		CExpress *pE = ap[i];
		Number		v = pE->m_value;

		m.insert( pair< wstring, Number >( str, v ) );
		
		OUTPUT( L"%s=%.2f  ", str.c_str(), v );
	}

	OUTPUT( L"\n" );
	//OUTPUT( L"\nCall Function %s\n",  op.c_str() );
	

	if( !p->_pExp->calc4( 0, m ) )
		return -2;

	v = p->_pExp->m_value;
	return 0;
}

// 0 ok
// -1 no function;
// -2 arg error;
// -3 other
int	CRule::CallFunction( Number &v, wstring op, const vector< CExpress* > &ap )
{
	if( CHelper::CompareNoCase( op.substr( 0, 4 ), L"data" ) )
		return DataFunc( v, op, ap );

	Number		v1(0);
	Number		v2(0);
	int		n = (int)ap.size();

	v1 = ap[0]->m_value;
	if( n > 1 )
		v2 = ap[1]->m_value;

#define		IFERROR_RETRUE(x)		{ if( n != x ) return -2; }

	if( CHelper::CompareNoCase( op, L"+" ) )
	{
		IFERROR_RETRUE(2)
		OUTPUT( L"%.2f + %.2f", v1, v2 );
		v = v1 + v2;
	}
	else if( CHelper::CompareNoCase( op, L"-" ) )
	{
		IFERROR_RETRUE(2)
		OUTPUT( L"%.2f - %.2f", v1, v2 );
		v = v1 - v2;
	}
	else if( CHelper::CompareNoCase( op, L"*" ) )
	{
		IFERROR_RETRUE(2)
		OUTPUT( L"%.2f * %.2f", v1, v2 );
		v = v1 * v2;
	}
	else if( CHelper::CompareNoCase( op, L"/" ) )
	{
		IFERROR_RETRUE(2)
		OUTPUT( L"%.2f / %.2ff", v1, v2 );
		if( v2 == 0 )
		{
			OUTPUT( L"\n***[error]��������\n" );
			return -3;
		}
		v = v1 / v2;
	}
	else if( CHelper::CompareNoCase( op, L"%" ) )
	{
		IFERROR_RETRUE(2)
		OUTPUT( L" %.2f %% %.2f", v1, v2 );
		v = (int)v1 % (int)v2;
	}
	else if( CHelper::CompareNoCase( op, L"sin" ) )
	{
		IFERROR_RETRUE(1)
		OUTPUT( L" sin(%.2f)", v1 );
		v = sin( v1 );
	}
	else if( CHelper::CompareNoCase( op, L"cos" ) )
	{
		IFERROR_RETRUE(1)
		OUTPUT( L" cos(%.2f)", v1 );
		v = sin( v1 );
	}
	else if( CHelper::CompareNoCase( op, L"tan" ) )
	{
		IFERROR_RETRUE(1)
		OUTPUT( L" tan(%.2f)", v1 );
		v = tan( v1 );
	}
	else if( CHelper::CompareNoCase( op, L"arcsin" ) )
	{
		IFERROR_RETRUE(1)
		OUTPUT( L" arcsin(%.2f)", v1 );
		v = asin( v1 );
	}
	else if( CHelper::CompareNoCase( op, L"arccos" ) )
	{
		IFERROR_RETRUE(1)
		OUTPUT( L" arccos(%.2f)", v1 );
		v = asin( v1 );
	}
	else if( CHelper::CompareNoCase( op, L"arctan" ) )
	{
		IFERROR_RETRUE(1)
		OUTPUT( L" arctan(%.2f)", v1 );
		v = atan( v1 );
	}
	else if( CHelper::CompareNoCase( op, L"min" ) )
	{
		IFERROR_RETRUE(2)
		OUTPUT( L" min(%.2f,%.2f)", v1, v2 );
		v = min( v1, v2 );
	}
	else if( CHelper::CompareNoCase( op, L"max" ) )
	{
		IFERROR_RETRUE(2)
		OUTPUT( L" max(%.2f,%.2f)", v1, v2 );
		v = max( v1, v2 );
	}
	else if( CHelper::CompareNoCase( op, L"pow" ) )
	{
		IFERROR_RETRUE(2)
		OUTPUT( L" pow(%.2f,%.2f)", v1, v2 );
		v = pow( v1, v2 );		
	}
	else if( CHelper::CompareNoCase( op, L"sqrt" ) )
	{
		IFERROR_RETRUE(1)
		OUTPUT( L" sqrt(%.2f)", v1 );
		v = sqrt( v1 );
	}

	else if( CHelper::CompareNoCase( op, L"if" ) )
	{
		IFERROR_RETRUE(3)

		if( (int)v1 > 0 )
			v = ap[1]->m_value;
		else
			v = ap[2]->m_value;
	}
	else
	{
		return -1;
	}

	return 0;
}

int CRule::DataFunc( Number &v, wstring op, const vector< CExpress* > &ap )
{
	int		n = (int)ap.size();

	wstring	s1 = ap[0]->m_str;
	Number		v2(0);
	if( n > 1 )
		v2 = ap[1]->m_value;

	if( CHelper::CompareNoCase( op, L"data_getvalue" ) )
	{
		int		v3;
		if( 2 == n )
			v3 = 2;
		else if( 3 == n )
			v3 = int( ap[2]->m_value );
		else 
			return -2;

		OUTPUT( L"(%s,%.2f,%.2f)", s1.c_str(), v2, v3 );
		if( 0 != Func_data_getValue( v, s1, v2, v3 ) )
			return -3;
	}
	else if( CHelper::CompareNoCase( op, L"data_getindex" ) )
	{
		int		v3;
		if( 2 == n )
			v3 = 2;
		else if( 3 == n )
			v3 = int( ap[2]->m_value );
		else 
			return -2;

		OUTPUT( L"(%s,%.2f,%.2f)", s1.c_str(), v2, v3 );
		if( 0 != Func_data_getIndex( v, s1, v2, v3 ) )
			return -3;
	}
	else if( CHelper::CompareNoCase( op, L"data2_getvaluebyxy" ) )
	{
		if( n != 5 )
			return -2;

		Number v3 = ap[2]->m_value;
		Number v4 = ap[3]->m_value;
		Number v5 = ap[4]->m_value;
		OUTPUT( L"(%s,%.2f,%.2f,%.2f,%.2f)", s1.c_str(), v2, v3, v4, v5 );
		if( 0 != Func_data2_getValueByXY( v, s1, (int)v2, (int)v3, v4, v5 ) )
			return -3;
	}
	else if( CHelper::CompareNoCase( op, L"data2_getxbyvaluey" ) )
	{
		if( n != 5 )
			return -2;

		Number v3 = ap[2]->m_value;
		Number v4 = ap[3]->m_value;
		Number v5 = ap[4]->m_value;
		OUTPUT( L"(%s,%.2f,%.2f,%.2f,%.2f)", s1.c_str(), v2, v3, v4, v5 );
		if( 0 != Func_data2_getxByValueY( v, s1, (int)v2, (int)v3, v4, v5 ) )
			return -3;
	}
	else if( CHelper::CompareNoCase( op, L"data2_getybyvaluex" ) )
	{
		if( n != 5 )
			return -2;

		Number v3 = ap[2]->m_value;
		Number v4 = ap[3]->m_value;
		Number v5 = ap[4]->m_value;
		OUTPUT( L"(%s,%.2f,%.2f,%.2f,%.2f)", s1.c_str(), v2, v3, v4, v5 );
		if( 0 != Func_data2_getyByValueX( v, s1, (int)v2, (int)v3, v4, v5 ) )
			return -3;
	}
	return 0;
}


int CRule::Func_data_getValue( Number &v, wstring strTable, Number vIndex, int flag )
{
	v = 0.;

	CData *p = FindData( strTable );
	if( NULL == p )
	{
		_strError = L"data_getValue ����1����,�Ҳ������� " + strTable;
		return -1;
	}

	int		i = (int)vIndex;
	int		n = p->_nSize;
	if( i >= n || i < 0 )
	{
		_strError = L"data_getValue ����2�����±�Խ��";
		return -2;
	}

	if( EQUAL( vIndex, i ) || 0 == flag )
	{
		v = p->_pData[i];
		return 0;
	}

	if( i+1 >= n )
	{
		_strError = L"data_getValue ����2�����±�Խ��";
		return -2;
	}
		
	if( 1 == flag )
		v = p->_pData[i+1];
	else// if( 2 == flag )
	{
		//Number v1 = vIndex - i;
		//Number v2 = i+1 - vIndex;
		
		//v = (v1*p->_pData[i+1] + p->_pData[i]*v2 )/(v1+v2);
		v = GetInterValue( i, i+1, vIndex, p->_pData[i], p->_pData[i+1] );
	}

	return 0;
}

// v1 / v2 = (x-p1)/(p2-x)
Number CRule::GetInterValue( Number v1, Number v2, Number v, Number p1, Number p2 )
{
	Number vv1 = v - v1;
	Number vv2 = v2 - v;
	// ( vv1*p2 + vv2*p1 ) / (vv1 + vv2 );
	return ( vv1*p2 + vv2*p1 ) / (v2-v1);
}

// ��� -1
// �Ҳ� 1
// �м� 0
int	CRule::IsInterValue( Number v1, Number v2, Number v )
{
	if( v1 == v2 )
	{
		if( v == v1 )
			return 0;
		if( v < v1 )
			return -1;
		if( v > v1 )
			return 1;
	}

	if( v1 < v2 )
	{
		if( v >= v2 )
			return 1;

		if( v <= v1 )
			return -1;

		return 0;
	}

	//if( v1 > v2 )
	//{
	if( v <= v2 )
		return 1;

	if( v >= v1 )
		return -1;

	return 0;
	//}
}

Number	CRule::GetInterValue( Number v1, Number v2, Number d )
{
	if( EQUAL( d, 0 ) || d < 0 )
		return v1;

	if( EQUAL( d, 1. ) || d > 1. )
		return v2;

	return v1 + (v2-v1)*d;
}

Number CRule::GetInterIndex( Number v1, Number v2, Number v )
{
	//assert( v1 < v2 );
	//if( EQUAL( v1, v ) || v < v1 )
	//	return 0;

	//if( EQUAL( v2, v ) || v > v2 )
	//	return 1.f;

	int nFlag = IsInterValue( v1, v2, v );
	if( v1 == v2 )
	{
		if( -1 == nFlag )
			return 0;

		return 1.f;
	}
	
	if( -1 == nFlag || EQUAL( v1, v ) )
		return 0;

	if( 1 == nFlag || EQUAL( v2, v ) )
		return 1.f;
	
	return (v-v1) / (v2-v1);
}

int CRule::Func_data_getIndex( Number &v, wstring strTable, Number vValue, int flag )
{
	v = 0.;

	CData *p = FindData( strTable );
	if( NULL == p )
	{
		_strError = L"data_getIndex ����1����,�Ҳ������� " + strTable;
		return -1;
	}

	int		n = p->_nSize;
	if( n <= 0 )
	{
		_strError = L"data_getIndex ���ݳ���Ϊ0";
		return -2;
	}

	if( vValue < p->_pData[0] )
	{
		if( 0 == flag )
		{
			_strError = L"data_getIndex ����2С����Сֵ�޷�ȡֵ";
			return -3;
		}
		else
		{
			v = 0;
			return 0;
		}
	}

	if( vValue > p->_pData[n-1] )
	{
		if( 1 == flag )
		{
			_strError = L"data_getIndex ����2�������ֵ�޷�ȡֵ";
			return -4;
		}
		else
		{
			v = n-1;
			return 0;
		}
	}

	int		i;
	for( i = 0; i < n-1; i++ )
	{
		Number		d1 = p->_pData[i];
		Number		d2 = p->_pData[i+1];

		if( vValue >= d1 && vValue <= d2 && d1 != d2 )
		{
			if( 0 == flag )
				v = i;
			else if( 1 == flag )
				v = i+1;
			else
				v = GetInterValue( d1, d2, vValue, i, i+1 );
		
			return 0;
		}
	}

	_strError = L"data_getIndex �޷�ȡ�����ʵ�ֵ";
	return -5;
}

int CRule::Func_data2_Test( Number &v, wstring strTable, int xcount, int ycount, Number vX, Number vY, CData *&p )
{
	v = 0.;
	p = FindData( strTable );

	if( NULL == p )
	{
		_strError = L"data2_getValueByXY ����1����,�Ҳ������� " + strTable;
		return -1;
	}

	if( xcount <= 0 || ycount <= 0 )
	{
		_strError = L"data2_getValueByXY ����2/3����,����x��/y������Ϊ1";
		return -2;
	}

	int		n = p->_nSize;
	if( n <= 0 )
	{
		_strError = L"data2_getValueByXY ���ݳ���Ϊ0";
		return -3;
	}

	if( n < xcount*ycount )
	{
		_strError = L"data2_getValueByXY �����С��һ��";
		return -4;
	}

	if( vX < 0 || vX >= xcount )
	{
		_strError = L"data2_getValueByXY ����4Խ��";
		return -5;
	}

	if( vY < 0 || vY >= ycount )
	{
		_strError = L"data2_getValueByXY ����5Խ��";
		return -6;
	}

	return 0;
}

int CRule::Func_data2_getValueByXY( Number &v, wstring strTable, int xcount, int ycount, Number vX, Number vY )
{
	CData	*p = NULL;
	int		nRet = Func_data2_Test( v, strTable, xcount, ycount, vX, vY, p );
	if( 0 != nRet )
		return nRet;
	
	int		x = int( vX );
	int		y = int( vY );

	Number		v1, v2, v3, v4;
	v1 = p->_pData[ y*xcount+x ];
	if( x+1 >= xcount )
		v2 = v1;
	else
		v2 = p->_pData[ y*xcount+x+1 ];

	if( y+1 >= ycount )
	{
		v3 = v1;
		v4 = v2;
	}
	else
	{
		v3 = p->_pData[ (y+1)*xcount+x ];

		if( x+1 >= xcount )
			v4 = v3;
		else
			v4 = p->_pData[ (y+1)*xcount+x+1 ];
	}

	Number		t1, t2;
	t1 = GetInterValue( v1, v2, vX-x );
	t2 = GetInterValue( v3, v4, vX-x );
	
	v = GetInterValue( t1, t2, vY-y );
	return 0;
}

int CRule::Func_data2_getxByValueY( Number &v, wstring strTable, int xcount, int ycount, Number vValue, Number vY )
{
	CData	*p = NULL;
	int		nRet = Func_data2_Test( v, strTable, xcount, ycount, 0, vY, p );
	if( 0 != nRet )
		return nRet;

	int		i;
	vector< Number >		av;
	
	int		y = int( vY );
	if( EQUAL( y, vY ) || y+1 >= ycount )
	{
		for( i = 0; i < xcount; i++ )
			av.push_back( p->_pData[xcount*y + i] );
	}
	else
	{
		if( y+1 >= ycount )
			return -7;

		for( i = 0; i < xcount; i++ )
		{
			Number	v1 = p->_pData[xcount*y + i];
			Number	v2 = p->_pData[xcount*(y+1) + i];
		
			Number	vT  = GetInterValue( v1, v2, vY - y );
			av.push_back( vT );
		}
	}
	
	for( i = 0; i < xcount-1; i++ )
	{
		//if( av[i] <= vValue && av[i+1] >= vValue )
		if( 0 == IsInterValue( av[i], av[i+1], vValue ) )
		{
			v = i+GetInterIndex( av[i], av[i+1], vValue );
			return 0;
		}
	}

	return -8;
}

int CRule::Func_data2_getyByValueX( Number &v, wstring strTable, int xcount, int ycount, Number vValue, Number vX )
{
	CData	*p = NULL;
	int		nRet = Func_data2_Test( v, strTable, xcount, ycount, vX, 0, p );
	if( 0 != nRet )
		return nRet;

	int		i;
	vector< Number >		av;
	
	int		x = int( vX );
	if( EQUAL( x, vX ) || x+1 >= xcount )
	{
		for( i = 0; i < ycount; i++ )
			av.push_back( p->_pData[xcount*i + x] );
	}
	else
	{
		if( x+1 >= xcount )
			return -7;

		for( i = 0; i < ycount; i++ )
		{
			Number	v1 = p->_pData[xcount*i + x];
			Number	v2 = p->_pData[xcount*i + x+1];
		
			Number	vT  = GetInterValue( v1, v2, vX - x );
			av.push_back( vT );
		}
	}
	
	for( i = 0; i < ycount-1; i++ )
	{
		//if( av[i] <= vValue && av[i+1] >= vValue )
		if( 0 == IsInterValue( av[i], av[i+1], vValue ) )
		{
			v = i+GetInterIndex( av[i], av[i+1], vValue );
			return 0;
		}
	}

	return -8;
}
