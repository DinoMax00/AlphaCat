// NegaScout_TT_HH.cpp: implementation of the CNegaScout_TT_HH2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NegaScout_TT_HH2.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNegaScout_TT_HH2::CNegaScout_TT_HH2()
{
	m_iCanSearchDepth=0;
}

CNegaScout_TT_HH2::~CNegaScout_TT_HH2()
{

}

CNegaScout_TT_HH2::SearchAGoodMove(BYTE byChessBoard[][9])
{
	CHESSMOVE cmBackupBM;

	memcpy(byCurChessBoard,byChessBoard,90);//��������

	CalculateInitHashKey(byCurChessBoard);
	ResetHistoryTable();
	m_iTimeCount=GetTickCount();

	m_pThinkProgress->SetRange(0,MAX_SEARCHDEPTH);
	m_pThinkProgress->SetStep(1);

	//���������
	for(m_iMaxDepth=1;m_iMaxDepth<=MAX_SEARCHDEPTH;m_iMaxDepth++)
	{
		if(NegaScout(m_iMaxDepth,-20000,20000)!=66666)//�����������û�б���ֹ����������߷�
			cmBackupBM=m_cmBestMove;
		else
			break;

		m_pThinkProgress->StepIt();//�߽�����
	}
	m_iCanSearchDepth=m_iMaxDepth-1;

	m_cmBestMove=cmBackupBM;
	m_umUndoMove.cmChessMove=m_cmBestMove;
	m_umUndoMove.nChessID=MakeMove(&m_cmBestMove);	

	memcpy(byChessBoard,byCurChessBoard,90);//��������
	m_pThinkProgress->SetPos(0);
}

int CNegaScout_TT_HH2::NegaScout(int iDepth, int iAlpha, int iBeta)
{
	int iCount,iGameOverRet;
	BYTE byType;
	int a,b,t;
	int side;
	int score;
	int i;
	
	iGameOverRet=IsGameOver(byCurChessBoard,iDepth);
	if(iGameOverRet)
		return iGameOverRet;
	
	side=(m_iMaxDepth-iDepth)%2;//���㵱ǰ�ڵ�����ͣ�����0/��С1
	
	score=LookUpHashTable(iAlpha,iBeta,iDepth,side);
	if(score!=66666)
		return score;
	
	if(iDepth<=0)//Ҷ�ӽڵ�ȡ��ֵ
	{
		score=m_pEval->Eveluate(byCurChessBoard,side);
		EnterHashTable(Exact,score,iDepth,side);//����ֵ�����û���
		return score;
	}
	
	iCount=m_pMG->CreatePossibleMove(byCurChessBoard,iDepth,side,m_nUserChessColor);

	for(i=0;i<iCount;i++)
		m_pMG->m_MoveList[iDepth][i].iScore=GetHistoryScore(&m_pMG->m_MoveList[iDepth][i]);
	MergeSort(m_pMG->m_MoveList[iDepth],iCount,0);
	
	int iBestMove=-1;
	
    a=iAlpha;
    b=iBeta;

    int eval_is_exact=0;

    for(i=0;i<iCount;i++)
	{
		if(GetTickCount()-m_iTimeCount>=m_fTimeLimit)
			return 66666;//������ֹ���

		Hash_MakeMove(&m_pMG->m_MoveList[iDepth][i],byCurChessBoard);
		byType=MakeMove(&m_pMG->m_MoveList[iDepth][i]);
		
		t=-NegaScout(iDepth-1,-b,-a);
		
		if(t>a && t<iBeta && i>0) 
		{
			//���ڵ�һ����Ľڵ㣬������������failhigh
			a=-NegaScout(iDepth-1,-iBeta,-t);//�ݹ������ӽڵ�
			eval_is_exact=1;//����������Ϊ��ȷֵ
			if(iDepth==m_iMaxDepth)
				m_cmBestMove=m_pMG->m_MoveList[iDepth][i];
			iBestMove=i;
		}
		Hash_UnMakeMove(&m_pMG->m_MoveList[iDepth][i],byType,byCurChessBoard); 
		UnMakeMove(&m_pMG->m_MoveList[iDepth][i],byType); 
		if(a<t)
		{
			eval_is_exact=1;
			a=t;
			if(iDepth==m_iMaxDepth)
				m_cmBestMove=m_pMG->m_MoveList[iDepth][i];
		}
		if(a>=iBeta)
		{
			EnterHashTable(LowerBound,a,iDepth,side);
			EnterHistoryScore(&m_pMG->m_MoveList[iDepth][i],iDepth);
			return a;
		}
		b=a+1;//set new null window
	}
	if(iBestMove!=-1)
		EnterHistoryScore(&m_pMG->m_MoveList[iDepth][iBestMove],iDepth);
	if(eval_is_exact)
		EnterHashTable(Exact,a,iDepth,side);
	else
		EnterHashTable(UpperBound,a,iDepth,side);

	return a;
}
