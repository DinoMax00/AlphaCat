// NegaScout_TT_HH.h: interface for the CNegaScout_TT_HH5 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DFGD_H__48964C19_849F_43A5_BC38_26EFBE7F8D86__INCLUDED_)
#define AFX_DFGD_H__48964C19_849F_43A5_BC38_26EFBE7F8D86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseSearchEngine.h"
#include "TranspositionTable.h"
#include "HistoryHeuristic.h"

//ʹ�����û������ʷ������NegaScout��������
class CNegaScout_TT_HH5:public CTranspositionTable,public CHistoryHeuristic,public CBaseSearchEngine 
{
public:
	CNegaScout_TT_HH5();
	virtual ~CNegaScout_TT_HH5();

public:
	virtual  SearchAGoodMove(BYTE byChessBoard[10][9]);

protected:
	void NegaScout(int depth, int alpha, int beta);//NegaScout��������
	int SubNegaScout(int iDepth, int iAlpha, int iBeta);
};

#endif // !defined(AFX_NEGASCOUT_TT_HH_H__DB5C0A53_E715_4037_A42B_B8D5704521B4__INCLUDED_)
