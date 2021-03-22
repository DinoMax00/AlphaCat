// TranspositionTable.h: interface for the CTranspositionTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSPOSITIONTABLE_H__C6292E32_89E6_46D7_8A85_E335AD8C4BBA__INCLUDED_)
#define AFX_TRANSPOSITIONTABLE_H__C6292E32_89E6_46D7_8A85_E335AD8C4BBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Define.h"

//������ö���͵��������ͣ���ȷ���±߽磬�ϱ߽�
enum ENTRY_TYPE{Exact,LowerBound,UpperBound};

//��ϣ����Ԫ�صĽṹ����
struct HASHITEM
{
	LONGLONG checksum;	  //64λУ����
	ENTRY_TYPE entry_type;//��������
	short depth;		  //ȡ�ô�ֵʱ�Ĳ��
	short eval;			  //�ڵ��ֵ
};

//�û�����
class CTranspositionTable  
{
public:
	CTranspositionTable();
	virtual ~CTranspositionTable();

public:
	void EnterHashTable(ENTRY_TYPE entry_type,short eval,short iDepth,int TableNo);
																    //����ǰ�ڵ��ֵ�����ϣ��
	int LookUpHashTable(int alpha, int beta, int depth,int TableNo);//��ѯ��ϣ���е�ǰ�ڵ�����
	void Hash_UnMakeMove(CHESSMOVE* move,BYTE nChessID,BYTE CurPosition[10][9]);  
																	//���������߷��Ĺ�ϣֵ����ԭ���߹�֮ǰ��
	void Hash_MakeMove(CHESSMOVE* move,BYTE CurPosition[10][9]);	//���������߷������������µĹ�ϣֵ
	void CalculateInitHashKey(BYTE CurPosition[10][9]);				//����������̵Ĺ�ϣֵ
	void InitializeHashKey();										//��ʼ��������飬������ϣ��

protected:
	LONGLONG Rand64();//����64λ�������
	LONG Rand32();	  //����32λ�������

public:	
	UINT m_nHashKey32[15][10][9];	   //32λ������飬��������32λ��ϣֵ
	ULONGLONG m_ulHashKey64[15][10][9];//64λ������飬��������64λ��ϣֵ
	HASHITEM *m_pTT[10];			   //�û���ͷָ��
	UINT m_HashKey32;				   //32λ��ϣֵ
	LONGLONG m_HashKey64;			   //64λ��ϣֵ
};

#endif // !defined(AFX_TRANSPOSITIONTABLE_H__C6292E32_89E6_46D7_8A85_E335AD8C4BBA__INCLUDED_)
