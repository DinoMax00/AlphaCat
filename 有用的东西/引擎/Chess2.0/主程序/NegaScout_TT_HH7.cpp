// NegaScout_TT_HH.cpp: implementation of the CNegaScout_TT_HH7 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NegaScout_TT_HH7.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNegaScout_TT_HH7::CNegaScout_TT_HH7()
{

}

CNegaScout_TT_HH7::~CNegaScout_TT_HH7()
{

}

CNegaScout_TT_HH7::SearchAGoodMove(BYTE byChessBoard[][9])
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

void CNegaScout_TT_HH7::NegaScout(int iDepth, int iAlpha, int iBeta)
{
	int iCount;
	BYTE byChess;
	int a,b;
	int iSide;
	int iScore;
	int i;
	
	iSide=(m_iMaxDepth-iDepth)%2;//���㵱ǰ�ڵ�����ͣ�����0/��С1
	
	iCount=m_pMG->CreatePossibleMove(byCurChessBoard,iDepth,iSide,m_nUserChessColor);

	m_pThinkProgress->SetRange(0,iCount);
	m_pThinkProgress->SetStep(1);

	int bestmove=-1;
	
    a=iAlpha;
    b=iBeta;

    int eval_is_exact=0;

    for(i=0;i<iCount;i++)
	{
		m_pThinkProgress->StepIt();//�߽�����

		Hash_MakeMove(&m_pMG->m_MoveList[iDepth][i],byCurChessBoard);
		byChess=MakeMove(&m_pMG->m_MoveList[iDepth][i]);
		m_pEval->UpdateCurChess(byChess,iSide);

		iScore=-SubNegaScout(iDepth-1,-b,-a);
		
		if(iScore>a && iScore<iBeta && i>0)
		{
			//���ڵ�һ����Ľڵ㣬������������failhigh
			a=-SubNegaScout(iDepth-1,-iBeta,-iScore);//�ݹ������ӽڵ�
			eval_is_exact=1;//����������Ϊ��ȷֵ
			m_cmBestMove=m_pMG->m_MoveList[iDepth][i];
			bestmove=i;
		}
//		m_pMG->m_MoveList[iDepth][i].iScore=iScore;
		Hash_UnMakeMove(&m_pMG->m_MoveList[iDepth][i],byChess,byCurChessBoard); 
		UnMakeMove(&m_pMG->m_MoveList[iDepth][i],byChess); 
		m_pEval->UnUpdateCurChess(byChess,iSide);

		if(a<iScore)
		{
			eval_is_exact=1;
			a=iScore;
			m_cmBestMove=m_pMG->m_MoveList[iDepth][i];
		}
		b=a+1;//set new null window
	}

	m_iEvel=m_pEval->Eveluate(byCurChessBoard,(m_iMaxDepth-iDepth)%2);
	//���ݵ÷ֶ��߷�����
//	sort(m_pMG->m_MoveList[iDepth],m_pMG->m_MoveList[iDepth]+iCount-1,UDgreater);
//	
//	CBANDMOVE camTemp;
//	//ѡ���߷�,���ⳤ��,��׽
//	for(i=0;i<iCount;i++)
//	{
//		byChess=MakeMove(&m_pMG->m_MoveList[iDepth][i]);
//		memcpy(camTemp.byChessBoard,byCurChessBoard,90);
//		camTemp.cm=m_pMG->m_MoveList[iDepth][i];
//		UnMakeMove(&m_pMG->m_MoveList[iDepth][i],byChess);
//		if(IsObeyRule(byCurChessBoard,iDepth,camTemp,1)==CR_OBEY && !BadForMe(byCurChessBoard,(m_iMaxDepth-iDepth)%2))
//		{
//			m_cmBestMove=m_pMG->m_MoveList[iDepth][i];
//			m_vectorEvel.push_back(m_iEvel=m_pEval->Eveluate(byCurChessBoard,(m_iMaxDepth-iDepth)%2));
//			break;
//		}
//	}
	
	//������ѡ���߷�,�ұ��ⳤ׽�ͳ���
//	m_cmBestMove=m_pMG->m_MoveList[iDepth][SelMove(vectorScore,iMaxScore)];
}

int CNegaScout_TT_HH7::SubNegaScout(int iDepth, int iAlpha, int iBeta)
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

	for(i=0;i<iCount;i++)
		m_pMG->m_MoveList[iDepth][i].iScore=GetHistoryScore(&m_pMG->m_MoveList[iDepth][i]);
	MergeSort(m_pMG->m_MoveList[iDepth],iCount,0);
	
	int bestmove=-1;
	
    a=iAlpha;
    b=iBeta;

    int eval_is_exact=0;

    for(i=0;i<iCount;i++)
	{
		Hash_MakeMove(&m_pMG->m_MoveList[iDepth][i],byCurChessBoard);
		byChess=MakeMove(&m_pMG->m_MoveList[iDepth][i]);
		
		if(m_iMaxDepth-iDepth>=6)//������7��ʱ
		{
		}

		t=-SubNegaScout(iDepth-1,-b,-a);
		
		if(t>a && t<iBeta && i>0)
		{
			//���ڵ�һ����Ľڵ㣬������������failhigh
			a=-SubNegaScout(iDepth-1,-iBeta,-t);//�ݹ������ӽڵ�
			eval_is_exact=1;//����������Ϊ��ȷֵ
			bestmove=i;
		}
		Hash_UnMakeMove(&m_pMG->m_MoveList[iDepth][i],byChess,byCurChessBoard); 
		UnMakeMove(&m_pMG->m_MoveList[iDepth][i],byChess); 
		if(a<t)
		{
			eval_is_exact=1;
			a=t;
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