// Eveluation.h: interface for the CEveluation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEST_H__4B0D0F69_7DB1_4492_A828_C2DB82294910__INCLUDED_)
#define AFX_TEST_H__4B0D0F69_7DB1_4492_A828_C2DB82294910__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseEveluation.h"

//��ֵ����
class CEveluation:public CBaseEveluation
{
public:
	CEveluation();
	virtual ~CEveluation();

public:
	virtual int Eveluate(BYTE byChessBoard[10][9],BOOL bIsRedTurn);
												 //��ֵ�������Դ�������̴�֣�bIsRedTurn��ʾ�ֵ�˭����
};

#endif // !defined(AFX_EVELUATION_H__8A690769_6BE7_4BE5_ADF3_A857473A5DF4__INCLUDED_)
