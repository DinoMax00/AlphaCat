// NegaScout_TT_HH.cpp: implementation of the CNegaScout_TT_HH3 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NegaScout_TT_HH3.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNegaScout_TT_HH3::CNegaScout_TT_HH3()
{
	
}

CNegaScout_TT_HH3::~CNegaScout_TT_HH3()
{

}

CNegaScout_TT_HH3::SearchAGoodMove(BYTE byChessBoard[][9])
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

	CBANDMOVE camTemp;
	memcpy(camTemp.byChessBoard,byCurChessBoard,90);
	camTemp.cm=m_cmBestMove;
	m_vectorRedCbAndMove.push_back(camTemp);

	memcpy(byChessBoard,byCurChessBoard,90);//��������
	m_pThinkProgress->SetPos(0);
}

int CNegaScout_TT_HH3::NegaScout(int iDepth, int iAlpha, int iBeta)
{
	if(GetTickCount()-m_iTimeCount>=m_fTimeLimit)
		return 66666;//������ֹ���

	int iCount;
	BYTE byChess;
	int iSide;
	int iScore;
	int i;
	
	iSide=(m_iMaxDepth-iDepth)%2;//���㵱ǰ�ڵ�����ͣ�����0/��С1

	iCount=m_pMG->CreatePossibleMove(byCurChessBoard,iDepth,iSide,m_nUserChessColor);
	for(i=0;i<iCount;i++)
	{
		Hash_MakeMove(&m_pMG->m_MoveList[iDepth][i],byCurChessBoard);
		byChess=MakeMove(&m_pMG->m_MoveList[iDepth][i]);
		iScore=-SubNegaScout(iDepth-1,-iBeta,-iAlpha);
		if(iScore==-66666)
		{
			Hash_UnMakeMove(&m_pMG->m_MoveList[iDepth][i],byChess,byCurChessBoard);
			UnMakeMove(&m_pMG->m_MoveList[iDepth][i],byChess);
			return 66666;
		}
		m_pMG->m_MoveList[iDepth][i].iScore=iScore;
		Hash_UnMakeMove(&m_pMG->m_MoveList[iDepth][i],byChess,byCurChessBoard);
		UnMakeMove(&m_pMG->m_MoveList[iDepth][i],byChess);
	}

	//���ݵ÷ֶ��߷�����
	sort(m_pMG->m_MoveList[iDepth],m_pMG->m_MoveList[iDepth]+iCount-1,UDgreater);
	
	CBANDMOVE camTemp;
	//ѡ���߷�,���ⳤ��,��׽
	for(i=0;i<iCount;i++)
	{
		byChess=MakeMove(&m_pMG->m_MoveList[iDepth][i]);
		memcpy(camTemp.byChessBoard,byCurChessBoard,90);
		camTemp.cm=m_pMG->m_MoveList[iDepth][i];
		UnMakeMove(&m_pMG->m_MoveList[iDepth][i],byChess);
		if(IsObeyRule(byCurChessBoard,iDepth,camTemp,1)==CR_OBEY && !BadForMe(byCurChessBoard,(m_iMaxDepth-iDepth)%2))
		{
			m_cmBestMove=m_pMG->m_MoveList[iDepth][i];
			m_vectorEvel.push_back(m_iEvel=m_pEval->Eveluate(byCurChessBoard,(m_iMaxDepth-iDepth)%2));
			break;
		}
	}
	
	//������ѡ���߷�,�ұ��ⳤ׽�ͳ���
//	m_cmBestMove=m_pMG->m_MoveList[iDepth][SelMove(vectorScore,iMaxScore)];
}

int CNegaScout_TT_HH3::SubNegaScout(int iDepth, int iAlpha, int iBeta)
{
	int iCount,iGameOverRet;
	BYTE byChess;
	int a,b,t;
	int iSide;
	int score;
	int i;
	
	iGameOverRet=IsGameOver(byCurChessBoard,iDepth);
	if(iGameOverRet)
		return iGameOverRet;
	
	iSide=(m_iMaxDepth-iDepth)%2;//���㵱ǰ�ڵ�����ͣ�����0/��С1
	
	score=LookUpHashTable(iAlpha,iBeta,iDepth,iSide);
	if(score!=66666)
		return score;
	
	if(iDepth<=0)//Ҷ�ӽڵ�ȡ��ֵ
	{
		score=m_pEval->Eveluate(byCurChessBoard,iSide);
		EnterHashTable(Exact,score,iDepth,iSide);//����ֵ�����û���
		return score;
	}
	
	iCount=m_pMG->CreatePossibleMove(byCurChessBoard,iDepth,iSide,m_nUserChessColor);
	//����if���֮�ڴ��룬���ڵ����ײ�ڵ�����˳��
	//���ĳ�߷����ϴε�������BestMove���������ڵ�һλ
	if(iDepth==m_iMaxDepth && m_iMaxDepth>1)
		for(i=1;i<iCount;i++)
		{
			if(m_pMG->m_MoveList[iDepth][i].From.x==m_cmBestMove.From.x
				&& m_pMG->m_MoveList[iDepth][i].From.y==m_cmBestMove.From.y
				&& m_pMG->m_MoveList[iDepth][i].To.x==m_cmBestMove.To.x	
				&& m_pMG->m_MoveList[iDepth][i].To.y==m_cmBestMove.To.y)	
			{
				//���������߷���λ��
				m_pMG->m_MoveList[iDepth][i]=m_pMG->m_MoveList[iDepth][0];
				m_pMG->m_MoveList[iDepth][0]=m_cmBestMove;
			}
		}
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
		byChess=MakeMove(&m_pMG->m_MoveList[iDepth][i]);
		
		t=-SubNegaScout(iDepth-1,-b,-a);
		
		if(t>a && t<iBeta && i>0)
		{
			//���ڵ�һ����Ľڵ㣬������������failhigh
			a=-SubNegaScout(iDepth-1,-iBeta,-t);//�ݹ������ӽڵ�
			eval_is_exact=1;//����������Ϊ��ȷֵ
			iBestMove=i;
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
	if(iBestMove!=-1)
		EnterHistoryScore(&m_pMG->m_MoveList[iDepth][iBestMove],iDepth);
	if(eval_is_exact)
		EnterHashTable(Exact,a,iDepth,iSide);
	else
		EnterHashTable(UpperBound,a,iDepth,iSide);

	return a;
}
