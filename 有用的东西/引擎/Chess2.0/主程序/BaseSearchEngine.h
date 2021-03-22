// SearchEngine.h: interface for the CBaseSearchEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEARCHENGINE_H__7A7237B9_0908_45D8_B102_94E342B174A5__INCLUDED_)
#define AFX_SEARCHENGINE_H__7A7237B9_0908_45D8_B102_94E342B174A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseEveluation.h"
#include "Eveluation.h"
#include "MoveGenerator.h"
#include "GradientProgressCtrl.h"
#include <vector>

using namespace std;

//��������Ļ���
class CBaseSearchEngine
{
public:
	CBaseSearchEngine();
	virtual ~CBaseSearchEngine();

public:
	virtual SearchAGoodMove(BYTE position[10][9])=0;        //����һ��
	int GetSearchDepth(){return m_iCanSearchDepth;};        //�õ����������
	CHESSMOVE GetBestMove(){return m_cmBestMove;};			//�õ�����߷�
	UNDOMOVE GetUndoMove(){return m_umUndoMove;};			//�õ������߷�
	int GetEvel();											//�õ���ֵ
	BYTE GetAteChess(){return m_byAteChess;};               //�õ����Ե�������
	void SetSearchDepth(int nDepth){m_iSearchDepth=nDepth;};//�趨����������
	void SetEveluator(CBaseEveluation* pEval){m_pEval=pEval;};//�趨��ֵ����
	void SetMoveGenerator(CMoveGenerator* pMG){m_pMG =pMG;};//�趨�߷�������
	void SetTimeLimit(float fTimeLimit){m_fTimeLimit=fTimeLimit;};//����ʱ������
	void SetThinkProgress(CGradientProgressCtrl* pThinkProgress){m_pThinkProgress=pThinkProgress;};
															//�趨��ʾ˼�����ȵĽ�����
	void SetUserChessColor(int nUserChessColor){m_nUserChessColor=nUserChessColor;};
															//�趨�û�Ϊ�ڷ���췽

	void UndoChessMove(BYTE position[10][9],CHESSMOVE* move,BYTE nChessID);//����
	void RedoChessMove(BYTE position[10][9],CHESSMOVE* move);              //��ԭ

	int IsObeyRule(BYTE byChessBoard[][9],int iDepth,CBANDMOVE cam,bool bIsRed);//�Ƿ���ѭ�������
	bool IsFail(BYTE byChessBoard[][9],bool bIsRed);//�ж�ĳ�����Ѱ�

protected:
	int IsGameOver(BYTE position[10][9],int nDepth);   //�ж��Ƿ��ѷ�ʤ��
	int SelMove(vector<int> vectorScore,int iMaxScore);//�������̷�ѡ��ȽϺõ��߷�
	BYTE MakeMove(CHESSMOVE* move);				       //����ĳһ�߷���������֮�������
	void UnMakeMove(CHESSMOVE* move,BYTE nChessID);    //�ָ�Ϊ�߹�֮ǰ������
	bool BadForMe(BYTE byChessBoard[][9],bool bIsRed); //�Լ�������
	UINT GetChessAge(BYTE byChessBoard[][9]);

public:
	int m_nUserChessColor;

protected:
	CGradientProgressCtrl* m_pThinkProgress;
								 //������ʾ˼�����ȵĽ�����ָ��
	BYTE byCurChessBoard[10][9];//����ʱ���ڼ�¼��ǰ�ڵ�����״̬������
	CHESSMOVE m_cmBestMove;		//��¼����߷�
	UNDOMOVE m_umUndoMove;
	CMoveGenerator* m_pMG;		//�߷�������
	CBaseEveluation* m_pEval;	//��ֵ����
	int m_iSearchDepth;			//����������
	int m_iMaxDepth;			//��ǰ����������������
	int m_iCanSearchDepth;      //�����������
	float m_fTimeLimit;         //����ʱ��
	int m_iTimeCount;			//��ʱ����
	int m_iEvel;
	BYTE m_byAteChess;          //���Ե�������
	vector<int> m_vectorEvel;   //���̹�ֵ

	vector<CBANDMOVE> m_vectorRedCbAndMove;  //���̺��߷�
	vector<CBANDMOVE> m_vectorBlackCbAndMove;//���̺��߷�
};

#endif // !defined(AFX_SEARCHENGINE_H__7A7237B9_0908_45D8_B102_94E342B174A5__INCLUDED_)
