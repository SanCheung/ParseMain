// ParseMain.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//#include "Operator.h"
//#include "Var.h"
//
//#include "OpAdd.h"

#include "Express.h"
#include "Helper.h"
#include "rule.h"

#define		OUTPUT2		wprintf		

using namespace ZParser;

void	TestRule()
{
	setlocale( LC_ALL, "chs");
	//OUTPUT( L"123ÖÐ¹ú456\n" );

	//wstring		strRule = L"prime = {2,3,5,7,9,11};\nmain(x)=3.1416*pow(x/2,4)+list_getValue( prime, 3);";
	//wstring		strRule = L"f2(x) = 1/(x-2);\nf(x)=x*10;\nq(x,y)= x+y;\nmain(x)=x*(3+21)-if( x-4, f(5), q(1,2) )+f2(x);";
	wstring		strRule = L"f(x)=if( x, f(x-1)+x, 0 );main(x)= f(x);";

	CRule		rule;
	if( !rule.Parse( strRule ) )
	{
		OUTPUT2( L"Parse Error! %s\n", rule._strError.c_str() );
		return;
	}

	wstring		strParaValue;
	Number			vResult;
	
	strParaValue = L"x=5";
	OUTPUT2( L"%s\n\n", strParaValue.c_str() );
	
	if( !rule.Calc( strParaValue, vResult ) )
		OUTPUT2( L"\nCalc Error! %s\n", rule._strError.c_str() );
	else
		OUTPUT2( L"\nResult: %f\n", vResult );

	//strParaValue = L"x=2";
	//OUTPUT( L"%s\n\n", strParaValue.c_str() );

	//if( !rule.Calc( strParaValue, vResult ) )
	//	OUTPUT( L"\nCalc Error! %s\n", rule._strError.c_str() );
	//else
	//	OUTPUT( L"\nResult: %f\n", vResult );
}

void	NoFunction()
{
	while( 1 )
	{
		OUTPUT( L"Input an express:( q to exit)\n>" );

		wstring			str;
		str.resize( 1024 );
		wscanf( L"%s", (wchar *)str.c_str() );

		if( wcsicmp( str.c_str(), L"q" ) == 0 )
			break;

		//wstring		str = L"190+2*3-4/5+6%7-8";
		//wstring		str = L"-(1+2)*3+(4-5/6)+7+(1+2)";
		CRule		rule;
		CExpress	exp( &rule );
		if( !exp.tree2( str, &exp ) )
		{
			OUTPUT( L"Input string is error!\n" );
			continue;
		}

		Number  value = exp.calc( 0 );
		OUTPUT( L"\n*** Result:  %.4f\n\n", value );
	}
}

void	ParseMain( wstring str )
{
	CRule		rule;
	CExpress	exp( &rule );

	if( !CHelper::Pretreat( str ) )
		OUTPUT( L"Input string is error!\n" );
	else
	{
		OUTPUT( L"%s\n", str.c_str() );
		if( !exp.tree3( str, &exp ) )
		{
			OUTPUT( L"Input string is error!\n" );
		}
		else
		{
			Number  value = exp.calc3( 0 );
			OUTPUT( L"\n*** Result:  %f\n\n", value );
		}
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	//wchar *s = L"0";
	//CHelper::IsIntegerString( s, wcslen(s) );
	//wchar *s = L"0.22";
	//CHelper::IsFloatString( s, wcslen(s) );
	
	//wstring		str = L"-0.3004e2.0";
	//OUTPUT( L"%s is Number %d\n", str.c_str(), CHelper::IsNumberString( str ) );

	//string		str = "-1+(2-3)*4/5-6";
	////string	  ht	str = "11*(12+22)";
	//CVar		v( str );
	//v.Calc();
	//v.Show();


	//v.m_op = new COpMul;
	//v.m_ip.push_back( new CVar( 11. ) );

	//CVar		*pv1 = new CVar;
	//v.m_ip.push_back( pv1 );

	//pv1->m_op = new COpAdd;
	//pv1->m_ip.push_back( new CVar( 12. ) );
	//pv1->m_ip.push_back( new CVar( 22. ) );

	//wstring		str = L"((((1-2)*5+(1+2))*(1+2)))";  // -6

	//wstring		str = L"(100/(((1+2)*3+(-6+2))*(1+5)+7))";  // 2.7027
	//wstring		str = L"(100/((-(1+2)*3+(-6+2))*(1+5)+7))";  // -1.4084507042253521
	//wstring		str = L"-(max( 8,-2.5 )+ 2.3 )*3";
	//CHelper::EraseSpace( str );
	//CHelper::EraseSym( str );
	//bool bRet = CHelper::TestSym( str );

	//wchar		buf[1024] = {0};

	//while( 1 )
	//{
	//	OUTPUT( L"Input an express:( q to exit)\n=" );

	//	wscanf( L"%s", buf );

	//	wstring		str = buf;
	//	if( wcsicmp( str.c_str(), L"q" ) == 0 )
	//		break;

	//	ParseMain( str );
	//}

	//wstring		str = L"sin(3.1416/6)*(3+20)";
	//ParseMain( str );

	TestRule();

	getchar();
	return 0;
}

