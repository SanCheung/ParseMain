#include "StdAfx.h"
#include "FuncBase.h"

CFuncBase::CFuncBase(void)
{
}

CFuncBase::~CFuncBase(void)
{
}

// 是否内置的函数
int CFuncBase::IsKeyFunction( wstring strName )
{
	vector< wstring >		asKeyFunc;
	asKeyFunc.push_back( L"sin" );
	asKeyFunc.push_back( L"cos" );
	asKeyFunc.push_back( L"sqrt" );
	asKeyFunc.push_back( L"pow" );
	asKeyFunc.push_back( L"abs" );
	asKeyFunc.push_back( L"min" );
	asKeyFunc.push_back( L"max" );
	// ...

	int		i;
	int		n = (int)asKeyFunc.size();
	for( i = 0; i < n; i++ )
	{
		if( 0 == _wcsicmp( asKeyFunc[i].c_str(), strName.c_str() ) )
			return i;
	}

	return -1;
}
