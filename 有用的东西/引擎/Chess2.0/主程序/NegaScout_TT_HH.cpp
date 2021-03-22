// NegaScout_TT_HH.cpp: implementation of the CNegaScout_TT_HH class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NegaScout_TT_HH.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNegaScout_TT_HH::CNegaScout_TT_HH()
{

}

CNegaScout_TT_HH::~CNegaScout_TT_HH()
{

}

CNegaScout_TT_HH::SearchAGoodMove(BYTE byChessBoard[][9])
{
	memcpy(byCurChessBoard,byChessBoard,90);//��������

	m_iMaxDepth=m_iSearchDepth;
	CalculateInitHashKey(byCurChessBoard);
	ResetHistoryTable();
	NegaScout(m_iMaxDepth,-20000,20000);
	m_umUndoMove.cmChessMove=m_cmBestMove;
	m_umUndoMove.nChessID=MakeMove(&m_cmBestMove);	

	memcpy(byChessBoard,byCurChessBoard,90);//��������
}

int CNegaScout_TT_HH::NegaScout(int iDepth, int iAlpha, int iBeta)
{
	int iCount,iGameOver;
	BYTE byChess;
	int a,b,t;
	int iSide;
	int iScore;
	int i;
	
	iGameOver=IsGameOver(byCurChessBoard, iDepth);
	if(iGameOver!=0)
		return iGameOver;
	
	iSide=(m_iMaxDepth-iDepth)%2;//���㵱ǰ�ڵ�����ͣ�����0/��С1
	
	iScore=LookUpHashTable(iAlpha,iBeta,iDepth,iSide);
	if(iScore!=66666)
		return iScore;
	
	if(iDepth<=0)//Ҷ�ӽڵ�ȡ��ֵ
	{
		iScore=m_pEval->Eveluate(byCurChessBoard,iSide);
		EnterHashTable(Exact,iScore,iDepth,iSide);//����ֵ�����û���
		return iScore;
	}
	
	iCount=m_pMG->CreatePossibleMove(byCurChessBoard,iDepth,iSide,m_nUserChessColor);
	if(iDepth==m_iMaxDepth)
	{
		//�ڸ��ڵ��趨������
		m_pThinkProgress->SetRange(0,iCount);
		m_pThinkProgress->SetStep(1);
	}

	for(i=0;i<iCount;i++)
		m_pMG->m_MoveList[iDepth][i].iScore=GetHistoryScore(&m_pMG->m_MoveList[iDepth][i]);
	MergeSort(m_pMG->m_MoveList[iDepth],iCount,0);
	
	int bestmove=-1;
	
    a=iAlpha;
    b=iBeta;

    int eval_is_exact=0;

    for(i=0;i<iCount;i++) 
	{
		if(iDepth==m_iMaxDepth)
			m_pThinkProgress->StepIt();//�߽�����

		Hash_MakeMove(&m_pMG->m_MoveList[iDepth][i],byCurChessBoard);
		byChess=MakeMove(&m_pMG->m_MoveList[iDepth][i]);
		
		t=-NegaScout(iDepth-1,-b,-a);
		
		if(t>a && t<iBeta && i>0) 
		{
			//���ڵ�һ����Ľڵ㣬������������failhigh
			a=-NegaScout(iDepth-1,-iBeta,-t);//�ݹ������ӽڵ�
			eval_is_exact=1;//����������Ϊ��ȷֵ
			if(iDepth==m_iMaxDepth)
				m_cmBestMove=m_pMG->m_MoveList[iDepth][i];
			bestmove=i;
		}
		Hash_UnMakeMove(&m_pMG->m_MoveList[iDepth][i],byChess,byCurChessBoard); 
		UnMakeMove(&m_pMG->m_MoveList[iDepth][i],byChess); 
		if(a<t)
		{
			eval_is_exact=1;
			a=t;
			if(iDepth==m_iMaxDepth)
				m_cmBestMove=m_pMG->m_MoveList[iDepth][i];
		}
		if(a>=iBeta)
		{
			EnterHashTable(LowerBound,a,iDepth,iSide);
			EnterHistoryScore(&m_pMG->m_MoveList[iDepth][i],iDepth);
			return a;
		}
		b=a+1;//set new null window
	}
	if(bestmove!=-1)
		EnterHistoryScore(&m_pMG->m_MoveList[iDepth][bestmove],iDepth);
	if(eval_is_exact)
		EnterHashTable(Exact,a,iDepth,iSide);
	else
		EnterHashTable(UpperBound,a,iDepth,iSide);

	return a;
}
