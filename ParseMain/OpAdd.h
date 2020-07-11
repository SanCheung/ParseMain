#pragma once
#include "operator.h"

// + 
class COpAdd : public COperator
{
public:
	COpAdd(void){ m_nPriority = 3; }
	virtual ~COpAdd(void) {}

	virtual bool Calc( CVar *v );
};

// -
class COpSub : public COperator
{
public:
	COpSub(void){ m_nPriority = 3; }
	virtual ~COpSub(void) {}

	virtual bool Calc( CVar *v );
};


// *
class COpMul : public COperator
{
public:
	COpMul(void){ m_nPriority = 2; }
	virtual ~COpMul(void) {}

	virtual bool Calc( CVar *v );
};

// /
class COpDiv : public COperator
{
public:
	COpDiv(void){ m_nPriority = 2; }
	virtual ~COpDiv(void) {}

	virtual bool Calc( CVar *v );
};

// (
class COpPr : public COperator
{
public:
	COpPr(void){ m_nPriority = 0; }
	virtual ~COpPr(void) {}

	virtual bool Calc( CVar *v );
};

// - ∏∫‘ÀÀ„

class COpNe : public COperator
{
public:
	COpNe(void){ m_nPriority = 1; }
	virtual ~COpNe(void) {}

	virtual bool Calc( CVar *v );
};
