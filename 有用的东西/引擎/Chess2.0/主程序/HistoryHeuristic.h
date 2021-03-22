// HistoryHeuristic.h: interface for the CHistoryHeuristic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HISTORYHEURISTIC_H__BE5BC30B_D7AF_44CD_9A3D_CD9E0AFEE37B__INCLUDED_)
#define AFX_HISTORYHEURISTIC_H__BE5BC30B_D7AF_44CD_9A3D_CD9E0AFEE37B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Define.h"

//��ʷ������
class CHistoryHeuristic  
{
public:
	CHistoryHeuristic();
	virtual ~CHistoryHeuristic();

public:
	void MergeSort(CHESSMOVE* source, int n,BOOL direction);//�Ե�ǰ�߷����н��й鲢����
	void EnterHistoryScore(CHESSMOVE *move,int depth);		//��ĳһ����߷�������ʷ��¼��
	int GetHistoryScore(CHESSMOVE *move);					//ȡĳһ�߷�����ʷ�÷�
	void ResetHistoryTable();								//����ʷ��¼�����

protected:
	void Merge_A(CHESSMOVE* source,CHESSMOVE* target,int l,int m, int r);//��С��������MergePass����
	void MergePass(CHESSMOVE* source, CHESSMOVE* target,const int s, const int n, const BOOL direction);
																		 //���ںϲ�����õ��������ݶΣ�MergeSort����
	void Merge(CHESSMOVE* source,CHESSMOVE* target,int l,int m, int r);  //��С��������MergePass����

protected:
	int m_HistoryTable[90][90]; //��ʷ�÷ֱ�
	CHESSMOVE m_TargetBuff[100];//�����õĻ������
};

#endif // !defined(AFX_HISTORYHEURISTIC_H__BE5BC30B_D7AF_44CD_9A3D_CD9E0AFEE37B__INCLUDED_)
