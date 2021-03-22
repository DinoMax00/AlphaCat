// TranspositionTable.cpp: implementation of the CTranspositionTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TranspositionTable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTranspositionTable::CTranspositionTable()
{
	InitializeHashKey();//������ϣ�������������
}

CTranspositionTable::~CTranspositionTable()
{
	//�ͷŹ�ϣ�����ÿռ�
	delete m_pTT[0];
	delete m_pTT[1];
}

void CTranspositionTable::CalculateInitHashKey(BYTE CurPosition[][9])
{
	int j,k,nChessType;

	m_HashKey32=0;
	m_HashKey32=0;
	//���������Ӷ�Ӧ�Ĺ�ϣ������
	for(j=0;j<10;j++)
		for(k=0;k<9;k++)
		{
			nChessType=CurPosition[j][k];
			if(nChessType!=NOCHESS)
			{
				m_HashKey32=m_HashKey32^m_nHashKey32[nChessType][j][k]; 
				m_HashKey64=m_HashKey64^m_ulHashKey64[nChessType][j][k]; 
			}
		}
}

void CTranspositionTable::Hash_MakeMove(CHESSMOVE *move, BYTE CurPosition[][9])
{
	BYTE nToID,nFromID;

	nFromID=CurPosition[move->From.y][move->From.x];
	nToID=CurPosition[move->To.y][move->To.x];

	//��Ҫ�ƶ������Ӵ���ȥ��
	m_HashKey32=m_HashKey32^m_nHashKey32[nFromID][move->From.y][move->From.x]; 
	m_HashKey64=m_HashKey64^m_ulHashKey64[nFromID][move->From.y][move->From.x]; 

	if(nToID!=NOCHESS)//Ŀ��λ�������ӣ�ҲҪ����ȥ��
	{
		m_HashKey32=m_HashKey32^m_nHashKey32[nToID][move->To.y][move->To.x]; 
		m_HashKey64=m_HashKey64^m_ulHashKey64[nToID][move->To.y][move->To.x]; 
	}
	
	//���ƶ���������Ŀ��λ�õ����������
	m_HashKey32=m_HashKey32^m_nHashKey32[nFromID][move->To.y][move->To.x]; 
	m_HashKey64=m_HashKey64^m_ulHashKey64[nFromID][move->To.y][move->To.x];
}

void CTranspositionTable::Hash_UnMakeMove(CHESSMOVE *move, BYTE nChessID, BYTE CurPosition[][9])
{
	BYTE nToID;

	nToID=CurPosition[move->To.y][move->To.x];
	
	//���ƶ��������ƶ�ǰλ���ϵ����������
	m_HashKey32=m_HashKey32^m_nHashKey32[nToID][move->From.y][move->From.x]; 
	m_HashKey64=m_HashKey64^m_ulHashKey64[nToID][move->From.y][move->From.x]; 

	//���ƶ���������λ���ϵ�������ӹ�ϣֵ����ȥ��
	m_HashKey32=m_HashKey32^m_nHashKey32[nToID][move->To.y][move->To.x]; 
	m_HashKey64=m_HashKey64^m_ulHashKey64[nToID][move->To.y][move->To.x]; 
	
	if(nChessID)
	{
		//�����Ե�����������Ӧ��������ָ�����ϣֵ
		m_HashKey32=m_HashKey32^m_nHashKey32[nChessID][move->To.y][move->To.x]; 
		m_HashKey64=m_HashKey64^m_ulHashKey64[nChessID][move->To.y][move->To.x]; 
	}
}

int CTranspositionTable::LookUpHashTable(int alpha, int beta, int depth, int TableNo)
{
	int x;
	HASHITEM* pht;

	//�����ʮλ��ϣ��ַ����������趨�Ĺ�ϣ���С����1M*2��,
	//����TableSize*2��TableSizeΪ�����趨�Ĵ�С
	//����Ҫ�޸���һ��Ϊm_HashKey32%TableSize
	//��һ����������һ��Ҳһ��
	x=m_HashKey32 & 0xFFFFF;
	pht=&m_pTT[TableNo][x];//ȡ������ı���ָ��

    if(pht->depth>=depth && pht->checksum==m_HashKey64)
	{
		switch(pht->entry_type)//�ж���������
		{
		case Exact://ȷ��ֵ
			return pht->eval;

		case LowerBound://�±߽�
			if(pht->eval>=beta)
				return pht->eval;

			break;

		case UpperBound://�ϱ߽�
			if (pht->eval<=alpha)
				return pht->eval;

			break;
        }
	}

	return 66666;
}

void CTranspositionTable::EnterHashTable(ENTRY_TYPE entry_type, short eval, short depth, int TableNo)
{
	int x;
	HASHITEM* pht;

	x=m_HashKey32 & 0xFFFFF;//�����ʮλ��ϣ��ַ
	pht=&m_pTT[TableNo][x]; //ȡ������ı���ָ��

	//������д���ϣ��
	pht->checksum=m_HashKey64; //64λУ����
	pht->entry_type=entry_type;//��������
	pht->eval=eval;			   //Ҫ�����ֵ
	pht->depth=depth;		   //���
}

void CTranspositionTable::InitializeHashKey()
{
	int i,j,k;
	
	srand((unsigned)time(NULL));
	
	//����������
	for(i=0;i<15;i++)
		for(j=0;j<10;j++)
			for(k=0;k<9;k++)
			{
				m_nHashKey32[i][j][k]=Rand32();
				m_ulHashKey64[i][j][k]=Rand64();
			}

	//�����û������ÿռ䡣1M*2����Ŀ������Ҳ��ָ��������С
	m_pTT[0]=new HASHITEM[1024*1024];//���ڴ��ȡ����ֵ�Ľڵ�����
	m_pTT[1]=new HASHITEM[1024*1024];//���ڴ��ȡ��Сֵ�Ľڵ�����
}

//����64λ�����
LONGLONG CTranspositionTable::Rand64()
{
    return rand()^((LONGLONG)rand()<<15)^((LONGLONG)rand()<<30)^
		((LONGLONG)rand()<<45)^((LONGLONG)rand()<<60);
}

//����32λ�����
LONG CTranspositionTable::Rand32()
{
    return rand()^((LONG)rand()<<15)^((LONG)rand()<<30);
}