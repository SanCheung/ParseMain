#pragma once
#include "Express.h"

namespace				ZParser
{

class CData
{
public:
	wstring		_strName;
	Number		*_pData;
	int			_nSize;

	CData(): _pData(NULL), _nSize( 0 ){};
	~CData()	{ ClearAll(); };

	void	ClearAll();
};

class CFunction
{
public:
	wstring			_strName;
	wstringarray	_arPara;
	CExpress		*_pExp;

	CFunction()		{};
	~CFunction()	{ ClearAll(); };

	void	ClearAll();
};

class CRule
{
public:
	CRule(void);
	~CRule(void);

	int					m_nCalcIndex;
	vector< CData* >		_apData;
	vector< CFunction* >	_apFunc;

	CFunction*				_pMain;
	mapStr2Number				_mVar2Value;
	
	wstring					_strError;

	void	ClearAll();

	bool	Parse( wstring str );
	bool	Calc( wstring str, Number &nResult );

	bool	ParseVarValues( wstring str );

	bool	GetValueByVar( wstring str, Number &v );
	int		CallFunction( Number &v, wstring op, const vector< CExpress* > &ap );
	int 	CallUserFunction( Number &v, wstring op, const vector< CExpress* > &ap );

	CData*		FindData( wstring strName );
	CFunction*	FindFunction( wstring strName );

protected:
	int		DataFunc( Number &v, wstring op, const vector< CExpress* > &ap );

	//		flag = 0		ȡ��
	//		1				ȡ��һ��
	//		2				ȡ��ֵ
	int		Func_data_getValue( Number &v, wstring strTable, Number vIndex, int flag );
	int		Func_data_getIndex( Number &v, wstring strTable, Number vValue, int flag );
	int		Func_data2_getValueByXY( Number &v, wstring strTable, int xcount, int ycount, Number vX, Number vY );
	int		Func_data2_getxByValueY( Number &v, wstring strTable, int xcount, int ycount, Number vValue, Number vY );
	int		Func_data2_getyByValueX( Number &v, wstring strTable, int xcount, int ycount, Number vValue, Number vX );

	int		Func_data2_Test( Number &v, wstring strTable, int xcount, int ycount, Number vX, Number vY, CData *&p );


	static Number  GetInterValue( Number v1, Number v2, Number v, Number p1, Number p2 );
	
	static Number	GetInterValue( Number v1, Number v2, Number d );
	static Number	GetInterIndex( Number v1, Number v2, Number v );

	// ��� -1
	// �Ҳ� 1
	// �м� 0
	static int		IsInterValue( Number v1, Number v2, Number v );

};


}; // namespace