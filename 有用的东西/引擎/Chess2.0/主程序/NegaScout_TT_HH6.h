// NegaScout_TT_HH.h: interface for the CNegaScout_TT_HH6 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADF_H__C05C96CA_6FFA_43EE_B8E0_ED332C525AE7__INCLUDED_)
#define AFX_ADF_H__C05C96CA_6FFA_43EE_B8E0_ED332C525AE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseSearchEngine.h"
#include "TranspositionTable.h"
#include "HistoryHeuristic.h"
#include <algorithm>

extern bool UDgreater(CHESSMOVE cm1, CHESSMOVE cm2);

//ʹ�����û������ʷ������NegaScout��������
class CNegaScout_TT_HH6:public CTranspositionTable,public CHistoryHeuristic,public CBaseSearchEngine 
{
public:
	CNegaScout_TT_HH6();
	virtual ~CNegaScout_TT_HH6();

public:
	virtual  SearchAGoodMove(BYTE byChessBoard[10][9]);

protected:
	void NegaScout(int depth, int alpha, int beta);//NegaScout��������
	int SubNegaScout(int iDepth, int iAlpha, int iBeta);
};

#endif // !defined(AFX_NEGASCOUT_TT_HH_H__DB5C0A53_E715_4037_A42B_B8D5704521B4__INCLUDED_)
