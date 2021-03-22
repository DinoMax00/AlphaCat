
/* ============================================================================================ *\
 *	Author		:	ahei																		*
 *	Email		:	ahei0802@126.com															*
 *	Msn			:	ahei080210114@hotmail.com													*
 *	QQ			:	8261525																		*
 *	Version		:	0.2 																		*
 *	Created		:	2005/03/01 9:10:13															*
 *	Modified	:	2005/01/01 17:15:46															*
 *	FileName	: 	SearchEngine.cpp															*
 *	Description	:	This file is using for declaring the class CSearchEngine,and this class is	*
 *					using for generating moves,evaluating and searching.						*
 * 	Warning     : 	Copyright (c) ahei 2004 All rights reserved.  								*
\* ============================================================================================ */

#ifdef _WIN32
//#include <mmsystem.h>
#endif
#include <iostream>
#include <new>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <time.h>
#include <crtdbg.h>

#include "Idle.h"
#include "DispMsg.h"
#include "SearchEngine.h"

using namespace std;

// ���ӻ�����ֵ
FSINT g_nBasicValues[8] = 
{
	BV_NOCHESS,				// ��
	BV_KING,				// ��
	BV_ADVISOR,				// ʿ
	BV_BISHOP,				// ��
	BV_ROOK,				// ��
	BV_KNIGHT_OPEN_GAME,	// ��
	BV_CANNON_OPEN_GAME,	// ��
	BV_PAWN_OPEN_GAME,		// ��
};

// ����CHESSMOVE������
FSbool UDgreater(CHESSMOVE cm1, CHESSMOVE cm2);

/*******************************************************************************************************
������ : CSearchEngine
��  �� : 
��  �� : ��
����ֵ : ��
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��9��28�� 22:24:53
*******************************************************************************************************/
CSearchEngine::CSearchEngine()
{
	// �˴��ı������ܷŵ�InitVar��������,����Ҫ��ǰ��ʼ��
	m_bAllocateMem = FALSE;
	m_EgnProtocol = EgnPUCCI;
	m_nHashTabMask = 0x7ffff;	// hash������λ

	FSINT nLen;

	GetCurrentDirectory(MAXBUFFERLEN, m_szDir);
	m_szDir[nLen = strlen(m_szDir)] = '\\';
	_ASSERT(nLen <= MAXBUFFERLEN - 1);
	m_szDir[nLen + 1] = '\0';
}

/*******************************************************************************************************
������ : ~CSearchEngine
��  �� : 
��  �� : ��
����ֵ : ��
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��9��28�� 22:25:03
*******************************************************************************************************/
CSearchEngine::~CSearchEngine()
{
	DeleteCriticalSection(&g_CriticalSection);
}

/*******************************************************************************************************
������ : AllocateMem
��  �� : �����ڴ�
��  �� : ��
����ֵ : ��
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:45:07
*******************************************************************************************************/
FSVOID CSearchEngine::AllocateMem()
{
	if (CanMemMeet() == FALSE)
	{
		DISPMSG1_ENDL(cout,	"�ڴ治��,�����˳�!");
		exit(EXIT_SUCCESS);
	}

	InitPointer();

	// �߷�Ԥ��������
	// ��
	TRYNEW(m_pcmRookFilePreMoves = new PRECHESSMOVE[91][ROOKFILECHECKSUMMAXCNT][4];);
	TRYNEW(m_pcmRookRankPreMoves = new PRECHESSMOVE[91][ROOKRANKCHECKSUMMAXCNT][4];);

	TRYNEW(m_pcmRookFileCapPreMoves = new PRECHESSMOVE[91][ROOKFILECHECKSUMMAXCNT][4];);
	TRYNEW(m_pcmRookRankCapPreMoves = new PRECHESSMOVE[91][ROOKRANKCHECKSUMMAXCNT][4];);

	TRYNEW(m_pcmRookFileNoCapPreMoves = new PRECHESSMOVE[91][ROOKFILECHECKSUMMAXCNT];);
	TRYNEW(m_pcmRookRankNoCapPreMoves = new PRECHESSMOVE[91][ROOKRANKCHECKSUMMAXCNT];);

	TRYNEW(m_nRookFoeFileRankCheckNum = new FEATCHAR[10][ROOKFRCHECKSUMMAXCNT][1024];);

	// ��
	// �������߷�
	TRYNEW(m_pcmKnightNoCapPreMoves = new PRECHESSMOVE[91][256][16];);
	// �����߷�
	TRYNEW(m_pcmKnightCapPreMoves = new PRECHESSMOVE[91][256][16];);
	
	// ��
	TRYNEW(m_pcmCannonFilePreMoves = new PRECHESSMOVE[91][CANNONFILECHECKSUMMAXCNT][4];);
	TRYNEW(m_pcmCannonRankPreMoves = new PRECHESSMOVE[91][CANNONRANKCHECKSUMMAXCNT][4];);

	// �������߷�[2005/10/23 15:23:58]
	TRYNEW(m_pcmCannonFileNoCapPreMoves = new PRECHESSMOVE[91][CANNONFILECHECKSUMMAXCNT];);
	TRYNEW(m_pcmCannonRankNoCapPreMoves = new PRECHESSMOVE[91][CANNONRANKCHECKSUMMAXCNT];);

	// �����߷�[2005/10/23 15:24:09]
	TRYNEW(m_pcmCannonFileCapPreMoves = new PRECHESSMOVE[91][CANNONFILECHECKSUMMAXCNT][4];);
	TRYNEW(m_pcmCannonRankCapPreMoves = new PRECHESSMOVE[91][CANNONRANKCHECKSUMMAXCNT][4];);	

	TRYNEW(m_nCannonFoeFileRankCheckNum = new FEATCHAR[10][CANNONFRCHECKSUMMAXCNT][1024];);

	// ��
	TRYNEW(m_pcmKingPreMoves = new PRECHESSMOVE[18][16][16];);

	// ʿ
	TRYNEW(m_pcmAdvisorPreMoves = new PRECHESSMOVE[11][16][16];);

	// ��
	TRYNEW(m_pcmBishopPreMoves = new PRECHESSMOVE[15][16][16][16];);
	TRYNEW(m_pcmBishopNoCapPreMoves = new PRECHESSMOVE[15][16][16];);
	TRYNEW(m_pcmBishopCapPreMoves = new PRECHESSMOVE[15][16][16];);

	// ��
	TRYNEW(m_PawnPreMoves = new PRECHESSMOVE[2][56][8][8];);
	TRYNEW(m_PawnNoCapPreMoves = new PRECHESSMOVE[2][56][8];);
	TRYNEW(m_PawnCapPreMoves = new PRECHESSMOVE[2][56][8];);

	InitPreMoveListPointer();

	// ˫�������ж�����
	TRYNEW(m_bIsTwoKingMeet= new FEATBOOL[10][10][1024];);

	// ����Ԥ�ж�����
	// ��
	TRYNEW(m_bIsRookFilePreCheck = new FEATBOOL[2][91][9][512];);
	TRYNEW(m_bIsRookRankPreCheck = new FEATBOOL[2][91][9][1024];);

	// ��
	TRYNEW(m_bIsKnightPreCheck = new FEATBOOL[2][91][9][16];);

	// ��
	TRYNEW(m_bIsCannonFilePreCheck = new FEATBOOL[2][91][9][512];);
	TRYNEW(m_bIsCannonRankPreCheck = new FEATBOOL[2][91][9][1024];);

	// ��
	TRYNEW(m_bIsPawnPreCheck = new FEATBOOL[2][56][9];);

	// �߷���Ч��Ԥ�ж�����
	// ��
//	TRYNEW(m_bIsRookFilePreValidMove = new FEATBOOL[90][90][512];);
//	TRYNEW(m_bIsRookRankPreValidMove = new FEATBOOL[90][90][1024];);
//
//	// ��
//	TRYNEW(m_bIsCannonFilePreValidMove = new FEATBOOL[90][90][512][2];);
//	TRYNEW(m_bIsCannonRankPreValidMove = new FEATBOOL[90][90][1024][2];);

	// ��
	TRYNEW(m_bIsRookPreValidMove = new FEATBOOL[10][10][1024];);

	// ��
	TRYNEW(m_bIsCannonPreValidMove = new FEATBOOL[10][10][1024][2];);

	// hash��
	m_nRepHashTabMask = 0x3ff;	// �ظ������������λ

	TRYNEW(m_pHashTab[BLACK] = new HASHITEM[m_nHashTabMask + 1];);	// 6M
	TRYNEW(m_pHashTab[RED] = new HASHITEM[m_nHashTabMask + 1];);	// 6M

//	TRYNEW(m_pHashTabByDepth[BLACK] = new HASHITEM[m_nHashTabMask + 1];);	//6M
//	TRYNEW(m_pHashTabByDepth[RED] = new HASHITEM[m_nHashTabMask + 1];);	// 6M

	TRYNEW(m_pQSHashTab[BLACK] = new QSHASHITEM[m_nHashTabMask + 1];);	// 6M
	TRYNEW(m_pQSHashTab[RED] = new QSHASHITEM[m_nHashTabMask + 1];);	// 6M

	m_nRepHashTab = new FSINT[m_nRepHashTabMask + 1];
}

/*******************************************************************************************************
������ : FreeMem
��  �� : �ͷ��ڴ�
��  �� : ��
����ֵ : ��
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:45:30
*******************************************************************************************************/
FSVOID CSearchEngine::FreeMem()
{
	// =============================�߷�����=============================
	// �߷�Ԥ���������е��߷�����
	FSINT w, p, i, j, k;
	FSINT nFoeFileCheckNum;
	FSINT nFoeRankCheckNum;

	FSINT tAll = clock();
	
	cout << endl << "�����ͷų���Ԥ�߷��ڴ�..." << endl;
	// ��
	for (p = 0; p < 90; p++)
	{
		i = m_CGN2Pos[p].x;
		j = m_CGN2Pos[p].y;
		
		// ���߷�
		for (k = 0; k < ROOKFILECHECKSUMMAXCNT; k++)
		{
			// ����!!!
			if (m_nRookFileRankCheckSum[i][k] == INVALIDCHECKSUM)
			{
				break;
			}
			
			// �ͷŲ������߷�[2005/10/23 14:49:01]
			VALIDDELETE(m_pcmRookFileNoCapPreMoves[p][k].pMoveList, &m_InvalidMovePointer);

			for (nFoeFileCheckNum = 0; nFoeFileCheckNum < 4; nFoeFileCheckNum++)
			{
				//����!!!
				if (m_nRookFoeFileRankCheckSum[i][k][nFoeFileCheckNum] == INVALIDCHECKSUM)
				{
					break;
				}
				
				VALIDDELETE(m_pcmRookFilePreMoves[p][k][nFoeFileCheckNum].pMoveList, &m_InvalidMovePointer);
				VALIDDELETE(m_pcmRookFileCapPreMoves[p][k][nFoeFileCheckNum].pMoveList, &m_InvalidMovePointer);
			}
		}
		
		//���߷�
		for (k = 0; k < ROOKRANKCHECKSUMMAXCNT; k++)
		{
			//����!!!
			if (m_nRookFileRankCheckSum[j][k] == INVALIDCHECKSUM)
			{
				break;
			}
			
			VALIDDELETE(m_pcmRookRankNoCapPreMoves[p][k].pMoveList, &m_InvalidMovePointer);
			
			for (nFoeRankCheckNum = 0; nFoeRankCheckNum < 4; nFoeRankCheckNum++)
			{
				//����!!!
				if (m_nRookFoeFileRankCheckSum[j][k][nFoeRankCheckNum] == INVALIDCHECKSUM)
				{
					break;
				}
				
				VALIDDELETE(m_pcmRookRankPreMoves[p][k][nFoeRankCheckNum].pMoveList, &m_InvalidMovePointer);
				VALIDDELETE(m_pcmRookRankCapPreMoves[p][k][nFoeRankCheckNum].pMoveList, &m_InvalidMovePointer);
			}
		}
	}
	
	cout << "�����ͷ����Ԥ�߷��ڴ�..." << endl;
	FSINT t = clock();
	//��
	//�������߷�
	for (p = 0; p < 90; p++)
	{
		for (i = 0; i < 256; i++)
		{
			if (m_bIsKnightInvalidArroundCheckSum[p][i] == FALSE)
			{
				continue;
			}

			for (j = 0; j < 16; j++)
			{
				if (m_bIsKnightInvalidLegCheckSum[p][i][j] == FALSE)
				{
					continue;
				}

				VALIDDELETE(m_pcmKnightNoCapPreMoves[p][i][j].pMoveList, &m_InvalidMovePointer);
			}
		}
	}
	//�����߷�
	for (p = 0; p < 90; p++)
	{
		for (i = 0; i < 256; i++)
		{
			if (m_bIsKnightInvalidArroundCheckSum[p][i] == FALSE)
			{
				continue;
			}
			
			for (j = 0; j < 16; j++)
			{
				if (m_bIsKnightInvalidLegCheckSum[p][i][j] == FALSE)
				{
					continue;
				}
				
				VALIDDELETE(m_pcmKnightCapPreMoves[p][i][j].pMoveList, &m_InvalidMovePointer);
			}
		}
	}
	cout << "�ͷ���Ԥ�߷��ڴ�ʱ��:" << clock() - t << "ms" << endl;

	cout << "�����ͷ��ڵ�Ԥ�߷��ڴ�..." << endl;
	//��
	for (p = 0; p < 90; p++)
	{
		i = m_CGN2Pos[p].x;
		j = m_CGN2Pos[p].y;
		
		//���߷�
		for (k = 0; k < CANNONFILECHECKSUMMAXCNT; k++)
		{
			//����!!!
			if (m_nCannonFileRankCheckSum[i][k] == INVALIDCHECKSUM)
			{
				break;
			}
			
			VALIDDELETE(m_pcmCannonFileNoCapPreMoves[p][k].pMoveList, &m_InvalidMovePointer);

			for (nFoeFileCheckNum = 0; nFoeFileCheckNum < 4; nFoeFileCheckNum++)
			{
				//����!!!
				if (m_nCannonFoeFileRankCheckSum[i][k][nFoeFileCheckNum] == INVALIDCHECKSUM)
				{
					break ;
				}
				
				VALIDDELETE(m_pcmCannonFilePreMoves[p][k][nFoeFileCheckNum].pMoveList, &m_InvalidMovePointer);
				VALIDDELETE(m_pcmCannonFileCapPreMoves[p][k][nFoeFileCheckNum].pMoveList, &m_InvalidMovePointer);
			}
		}
		
		//���߷�
		for (k = 0; k < CANNONRANKCHECKSUMMAXCNT; k++)
		{
			//����!!!
			if (m_nCannonFileRankCheckSum[j][k] == INVALIDCHECKSUM)
			{
				break;
			}
			
			VALIDDELETE(m_pcmCannonRankNoCapPreMoves[p][k].pMoveList, &m_InvalidMovePointer);

			for (nFoeRankCheckNum = 0; nFoeRankCheckNum < 4; nFoeRankCheckNum++)
			{
				//����!!!
				if (m_nCannonFoeFileRankCheckSum[j][k][nFoeRankCheckNum] == INVALIDCHECKSUM)
				{
					break;
				}
				
				VALIDDELETE(m_pcmCannonRankPreMoves[p][k][nFoeRankCheckNum].pMoveList, &m_InvalidMovePointer);
				VALIDDELETE(m_pcmCannonRankCapPreMoves[p][k][nFoeRankCheckNum].pMoveList, &m_InvalidMovePointer);
			}
		}
	}

	cout << "�����ͷŽ���Ԥ�߷��ڴ�..." << endl;
	//��
	for (i = 0; i < 18; i++)
	{
		for (j = 0;j < 16; j++)
		{
			for (k = 0; k < 16; k++)
			{
				VALIDDELETE(m_pcmKingPreMoves[i][j][k].pMoveList, &m_InvalidMovePointer);
			}
		}
	}
	//�����߷�
	for (i = 0; i < 18; i++)
	{
		for (k = 0; k < 16; k++)
		{
			VALIDDELETE(m_pcmKingNoCapPreMoves[i][k].pMoveList, &m_InvalidMovePointer);
			VALIDDELETE(m_pcmKingCapPreMoves[i][k].pMoveList, &m_InvalidMovePointer);
		}
	}

	cout << "�����ͷ�ʿ��Ԥ�߷��ڴ�..." << endl;
	//ʿ
	for (i = 0; i < 11; i++)
	{
		for (j = 0; j < 16; j++)
		{
			for (k = 0; k < 16; k++)
			{
				if (m_pcmAdvisorPreMoves[i][j][k].pMoveList != &m_InvalidMovePointer)
				{
					TRYDELETE(SAFEDELETE(m_pcmAdvisorPreMoves[i][j][k].pMoveList));
				}
			}
		}
	}
	for (i = 0; i < 11; i++)
	{
		for (k = 0; k < 16; k++)
		{
			VALIDDELETE(m_pcmAdvisorNoCapPreMoves[i][k].pMoveList, &m_InvalidMovePointer);
			VALIDDELETE(m_pcmAdvisorCapPreMoves[i][k].pMoveList, &m_InvalidMovePointer);
		}
	}

	cout << "�����ͷ����Ԥ�߷��ڴ�..." << endl;
	//��
	for (p = 0; p < 15; p++)
	{
		for (i = 0; i < 16; i++)
		{
			for (j = 0; j < 16; j++)
			{
				for (k = 0; k < 16; k++)
				{
					VALIDDELETE(m_pcmBishopPreMoves[p][i][j][k].pMoveList, &m_InvalidMovePointer);
				}
			}
		}
	}
	for (p = 0; p < 15; p++)
	{
		for (j = 0; j < 16; j++)
		{
			for (k = 0; k < 16; k++)
			{
				VALIDDELETE(m_pcmBishopNoCapPreMoves[p][j][k].pMoveList, &m_InvalidMovePointer);
				VALIDDELETE(m_pcmBishopCapPreMoves[p][j][k].pMoveList, &m_InvalidMovePointer);
			}
		}
	}

	cout << "�����ͷű���Ԥ�߷��ڴ�..." << endl;
	//��
	for (w = 0; w < 2; w++)
	{
		for (p = 0; p < 56; p++)
		{
			for (i = 0; i < 8; i++)
			{
				for (j = 0; j < 8; j++)
				{
					VALIDDELETE(m_PawnPreMoves[w][p][i][j].pMoveList, &m_InvalidMovePointer);
				}
			}
		}
	}
	for (w = 0; w < 2; w++)
	{
		for (p = 0; p < 56; p++)
		{
			for (j = 0; j < 8; j++)
			{
				VALIDDELETE(m_PawnNoCapPreMoves[w][p][j].pMoveList, &m_InvalidMovePointer);
				VALIDDELETE(m_PawnCapPreMoves[w][p][j].pMoveList, &m_InvalidMovePointer);
			}
		}
	}

	cout << "�����ͷ������ڴ�..." << endl;

	//�߷�Ԥ��������
	//��
	TRYDELETE(SAFEDELETE(m_pcmRookFilePreMoves));
	TRYDELETE(SAFEDELETE(m_pcmRookRankPreMoves));

	TRYDELETE(SAFEDELETE(m_pcmRookFileCapPreMoves));
	TRYDELETE(SAFEDELETE(m_pcmRookRankCapPreMoves));

	TRYDELETE(SAFEDELETE(m_pcmRookFileNoCapPreMoves));
	TRYDELETE(SAFEDELETE(m_pcmRookRankNoCapPreMoves));

	TRYDELETE(SAFEDELETE(m_nRookFoeFileRankCheckNum));

	//��
	TRYDELETE(SAFEDELETE(m_pcmKnightNoCapPreMoves));
	TRYDELETE(SAFEDELETE(m_pcmKnightCapPreMoves));

	//��
	TRYDELETE(SAFEDELETE(m_pcmCannonFilePreMoves));
	TRYDELETE(SAFEDELETE(m_pcmCannonRankPreMoves));

	// �������߷�[2005/10/23 15:44:30]
	TRYDELETE(SAFEDELETE(m_pcmCannonFileNoCapPreMoves));
	TRYDELETE(SAFEDELETE(m_pcmCannonRankNoCapPreMoves));

	TRYDELETE(SAFEDELETE(m_pcmCannonFileCapPreMoves));
	TRYDELETE(SAFEDELETE(m_pcmCannonRankCapPreMoves));

	TRYDELETE(SAFEDELETE(m_nCannonFoeFileRankCheckNum));

	//��
	TRYDELETE(SAFEDELETE(m_pcmKingPreMoves));

	//ʿ
	TRYDELETE(SAFEDELETE(m_pcmAdvisorPreMoves));
	
	//��
	TRYDELETE(SAFEDELETE(m_pcmBishopPreMoves));
	TRYDELETE(SAFEDELETE(m_pcmBishopNoCapPreMoves));
	TRYDELETE(SAFEDELETE(m_pcmBishopCapPreMoves));
	
	//��
	TRYDELETE(SAFEDELETE(m_PawnPreMoves));
	TRYDELETE(SAFEDELETE(m_PawnNoCapPreMoves));
	TRYDELETE(SAFEDELETE(m_PawnCapPreMoves));

	//˫�������ж�����
	TRYDELETE(SAFEDELETE(m_bIsTwoKingMeet));
	
	//=============================��������=============================
	
	//=============================��������=============================
	//�����ж�����
	//��
	TRYDELETE(SAFEDELETE(m_bIsRookFilePreCheck));
	TRYDELETE(SAFEDELETE(m_bIsRookRankPreCheck));
	
	//��
	TRYDELETE(SAFEDELETE(m_bIsKnightPreCheck));
	
	//��
	TRYDELETE(SAFEDELETE(m_bIsCannonFilePreCheck));
	TRYDELETE(SAFEDELETE(m_bIsCannonRankPreCheck));
	
	//��
	TRYDELETE(SAFEDELETE(m_bIsPawnPreCheck));

	//hash��
	TRYDELETE(SAFEDELETE(m_pHashTab[BLACK]));
	TRYDELETE(SAFEDELETE(m_pHashTab[RED]));

	TRYDELETE(SAFEDELETE(m_pHashTabByDepth[BLACK]));
	TRYDELETE(SAFEDELETE(m_pHashTabByDepth[RED]));

	TRYDELETE(SAFEDELETE(m_pQSHashTab[BLACK]));
	TRYDELETE(SAFEDELETE(m_pQSHashTab[RED]));

	TRYDELETE(SAFEDELETE(m_nRepHashTab));

	//�߷���Ч��Ԥ�ж�����
	//��
//	TRYDELETE(SAFEDELETE(m_bIsRookFilePreValidMove));
//	TRYDELETE(SAFEDELETE(m_bIsRookRankPreValidMove));
//
//	//��
//	TRYDELETE(SAFEDELETE(m_bIsCannonFilePreValidMove));
//	TRYDELETE(SAFEDELETE(m_bIsCannonRankPreValidMove));

	//��
	TRYDELETE(SAFEDELETE(m_bIsRookPreValidMove));

	//��
	TRYDELETE(SAFEDELETE(m_bIsCannonPreValidMove));

	//=============================��������=============================

	DISPMSG3_ENDL(cout,		"�ͷ�ʱ��:", clock() - t, "ms");

	DISPMSG2_ENDL(cout,		"�ɹ��˳�����", g_szEngineName);
}

/*******************************************************************************************************
������ : AllocateMemErrProc
��  �� : 
��  �� : ��
����ֵ : ��
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:48:16
*******************************************************************************************************/
FSVOID CSearchEngine::AllocateMemErrProc()
{
	cout << "�ڴ治��!" << endl;

	FreeMem();
	exit(EXIT_FAILURE);
}

/*******************************************************************************************************
������ : FreeMemErrProc
��  �� : 
��  �� : ��
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��10��3�� 22:07:25
*******************************************************************************************************/
FSVOID CSearchEngine::FreeMemErrProc()
{
	cout << "�ͷ��ڴ�ʱ��������!" << endl;
}

/*******************************************************************************************************
������ : InitVar
��  �� : ��ʼ������
��  �� : ��
����ֵ : ��
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:37:15
*******************************************************************************************************/
FSVOID CSearchEngine::InitVar()
{
	//=============================�߷�����=============================
	FSINT p;
	FSINT i, j;
	FSINT m, n;
	FSINT k;
	FSINT c;
	FSINT nFoeBit;

	//m_nMask
	c = 1;
	for (i = 0; i < 10; i++)
	{
		m_nMask[i] = c;
		c <<= 1;

		m_nReverseMask[i] = (~m_nMask[i]);
	}

	//m_CGN2Pos
	for (p = 0; p < 91; p++)
	{
		m_CGN2Pos[p].x = (i = p / 10);
		m_CGN2Pos[p].y = (j = p % 10);

		m_nPos2CGN[i][j] = p;
	}

	//ע��:bug�׷���
	//m_nFileRankAroundCheckSum
	i = 0;
	for (k = 0; k < 1024; k++)
	{
		m_nFileRankAroundCheckSum[i][k] = 0;

		if (m_nMask[i + 1] & k)
		{
			m_nFileRankAroundCheckSum[i][k] += 2;
		}
	}
	for (i = 1; i < 9; i++)
	{
		for (k = 0; k < 1024; k++)
		{
			m_nFileRankAroundCheckSum[i][k] = 0;

			if (m_nMask[i-1] & k)
			{
				m_nFileRankAroundCheckSum[i][k]++;
			}
			if (m_nMask[i+1] & k)
			{
				m_nFileRankAroundCheckSum[i][k] += 2;
			}
		}
	}
	i = 9;
	for (k = 0; k < 1024; k++)
	{
		m_nFileRankAroundCheckSum[i][k] = 0;

		if ( m_nMask[i - 1] & k )
		{
			m_nFileRankAroundCheckSum[i][k] ++ ;
		}
	}

	//TODO:bug�׷���
	//m_nFileRankAroundCheckSum2
	for (i = 0; i < 2; i++)
	{
		for (k = 0; k < 1024; k++)
		{
			m_nFileRankAroundCheckSum2[i][k] = 0;

			if (m_nMask[i + 2] & k)
			{
				m_nFileRankAroundCheckSum2[i][k] += 2;
			}
		}
	}
	for ( i = 2 ; i < 8 ; i ++ )
	{
		for (k = 0; k < 1024; k++)
		{
			m_nFileRankAroundCheckSum2[i][k] = 0;

			if (m_nMask[i-2] & k)
			{
				m_nFileRankAroundCheckSum2[i][k]++;
			}
			if (m_nMask[i+2] & k)
			{
				m_nFileRankAroundCheckSum2[i][k] += 2;
			}
		}
	}
	for (i = 8; i < 10; i++)
	{
		for (k = 0; k < 1024; k++)
		{
			m_nFileRankAroundCheckSum2[i][k] = 0;

			if (m_nMask[i - 2] & k)
			{
				m_nFileRankAroundCheckSum2[i][k]++;
			}
		}
	}

	//m_nAroundCheckSum
	for (i = 0;i<4;i++)
	{
		for (j = 0;j<4;j++)
		{
			m_nAroundCheckSum[i][j] = (j << 2) | i;
		}
	}

	//�󳵵�λ�к�λ�е�У�����
	FSINT nNum;
	FSINT nFlag;
	FSINT nRookCheckSum;

	//ע��:����!!!
	for (i = 0; i < 10; i++)
	{
		for ( nNum = 0 ; nNum < ROOKFRCHECKSUMMAXCNT ; nNum ++ )
		{
			m_nRookFileRankCheckSum[i][nNum] = INVALIDCHECKSUM;
		}
	}

	//ע��:bug�׷���
	for ( i = 0 ; i < 10 ; i ++ )
	{
        nNum = 0;

		for ( k = 0 ; k < 1024 ; k ++)
		{
			nRookCheckSum = 0;

			for (m = i-1;m >= 0; m--)
			{
				if (m_nMask[m] & k)
				{
					nRookCheckSum |= m_nMask[m];
					break;
				}
			}

			for (n = i + 1; n < 10; n++)
			{
				if (m_nMask[n] & k)
				{
					nRookCheckSum |= m_nMask[n];
					break;
				}
			}

			nFlag = 0;
			for ( j = 0;j < nNum;j ++ )
			{
				if (m_nRookFileRankCheckSum[i][j] == nRookCheckSum)
				{
					nFlag = 1;
					break;
				}
			}

			if (nFlag)
			{
				m_nRookFileRankCheckNum[i][k] = j;
			}
			else
			{
				m_nRookFileRankCheckNum[i][k] = nNum ;
				m_nRookFileRankCheckSum[i][nNum] = nRookCheckSum ;

				nNum++;
			}
		}
	}

	//m_nRookFoeFileRankCheckNum
	FSINT nRookFoeFileRankCheckSum;

	//ע��:����!!!
	for (i = 0; i < 10; i++)
	{
		for (k = 0; k < ROOKFRCHECKSUMMAXCNT; k++)
		{
			//ע��:����!!!
			if (m_nRookFileRankCheckSum[i][k] == INVALIDCHECKSUM)
			{
				break ;
			}

			for (nNum = 0; nNum < 4; nNum++)
			{
				m_nRookFoeFileRankCheckSum[i][k][nNum] = INVALIDCHECKSUM;
			}
		}
	}

	//ע��:bug�׷���
	for ( i = 0 ; i < 10 ; i ++ )
	{
		for ( k = 0 ; k < ROOKFRCHECKSUMMAXCNT ; k ++ )
		{
			if (m_nRookFileRankCheckSum[i][k] == INVALIDCHECKSUM)
			{
				break ;
			}

            nNum = 0;
            
			for ( nFoeBit = 0 ; nFoeBit < 1024 ; nFoeBit ++ )
			{
				nRookFoeFileRankCheckSum = nFoeBit & m_nRookFileRankCheckSum[i][k];
				
                nFlag = 0;
				for (j = 0;j<nNum;j++)
				{
					if (m_nRookFoeFileRankCheckSum[i][k][j] ==nRookFoeFileRankCheckSum)
					{
						nFlag = 1;
						break;
					}
				}
				if (nFlag)
				{
					m_nRookFoeFileRankCheckNum[i][k][nFoeBit] = j ;
				}
				else
				{
					m_nRookFoeFileRankCheckNum[i][k][nFoeBit] = nNum;
					m_nRookFoeFileRankCheckSum[i][k][nNum] = nRookFoeFileRankCheckSum;

					nNum++;
				}
			}
		}
	}

	//m_nKnightAroundCheckSum
	FSINT nBit1, nBit2;

	for (nBit1 = 0; nBit1 < 16; nBit1++)
	{
		for (nBit2 = 0; nBit2 < 16; nBit2++)
		{
			m_nKnightAroundCheckSum[nBit1][nBit2] = nBit1 | (nBit2 << 4);
		}
	}

	//m_bIsKnightInvalidArroundCheckSum
	FSINT nAroundCheckSum;

	for (p = 0; p < 90; p++)
	{
		i = m_CGN2Pos[p].x;
		j = m_CGN2Pos[p].y;
		
		for (nAroundCheckSum = 0; nAroundCheckSum < 256; nAroundCheckSum++)
		{
			//��
			if (i - 2 < 0)
			{
				//��
				if (((nAroundCheckSum & m_nMask[2]) != 0) && (j - 1 < 0))
				{
					m_bIsKnightInvalidArroundCheckSum[p][nAroundCheckSum] = FALSE;
					continue;
				}
				//��
				if (((nAroundCheckSum & m_nMask[4]) != 0) && (j + 1 >= 10))
				{
					m_bIsKnightInvalidArroundCheckSum[p][nAroundCheckSum] = FALSE;
					continue;
				}
			}
			
			//��
			if (i + 2 >= 9)
			{
				//��
				if (((nAroundCheckSum & m_nMask[3]) != 0) && (j - 1 < 0))
				{
					m_bIsKnightInvalidArroundCheckSum[p][nAroundCheckSum] = FALSE;
					continue;
				}
				//��
				if (((nAroundCheckSum & m_nMask[5]) != 0) && (j + 1 >= 10))
				{
					m_bIsKnightInvalidArroundCheckSum[p][nAroundCheckSum] = FALSE;
					continue;
				}
			}
			
			//��
			if (j - 2 < 0)
			{
				//��
				if (((nAroundCheckSum & m_nMask[0]) != 0) && (i - 1 < 0))
				{
					m_bIsKnightInvalidArroundCheckSum[p][nAroundCheckSum] = FALSE;
					continue;
				}
				//��
				if (((nAroundCheckSum & m_nMask[1]) != 0) && (i + 1 >= 9))
				{
					m_bIsKnightInvalidArroundCheckSum[p][nAroundCheckSum] = FALSE;
					continue;
				}
			}
			
			//��
			if (j + 2 >= 10)
			{
				//��
				if (((nAroundCheckSum & m_nMask[6]) != 0) && (i - 1 < 0))
				{
					m_bIsKnightInvalidArroundCheckSum[p][nAroundCheckSum] = FALSE;
					continue;
				}
				//��
				if (((nAroundCheckSum & m_nMask[7]) != 0) && (i + 1 >= 9))
				{
					m_bIsKnightInvalidArroundCheckSum[p][nAroundCheckSum] = FALSE;
					continue;
				}
			}

			m_bIsKnightInvalidArroundCheckSum[p][nAroundCheckSum] = TRUE;
		}
	}

	//m_bIsKnightInvalidLegCheckSum
	FSINT nKnightLeg;

	for (p = 0; p < 90; p++)
	{
		i = m_CGN2Pos[p].x;
		j = m_CGN2Pos[p].y;
		
		for (nAroundCheckSum = 0; nAroundCheckSum < 256; nAroundCheckSum++)
		{
			if (m_bIsKnightInvalidArroundCheckSum[p][nAroundCheckSum] == FALSE)
			{
				continue;
			}

			for (nKnightLeg = 0; nKnightLeg < 16; nKnightLeg++)
			{
				//��
				if (i - 1 < 0 && (nKnightLeg & m_nMask[0]))
				{
					m_bIsKnightInvalidLegCheckSum[p][nAroundCheckSum][nKnightLeg] = FALSE;
					continue;
				}

				//��
				if (i + 1 >= 9 && (nKnightLeg & m_nMask[1]))
				{
					m_bIsKnightInvalidLegCheckSum[p][nAroundCheckSum][nKnightLeg] = FALSE;
					continue;
				}

				//��
				if (j - 1 < 0 && (nKnightLeg & m_nMask[2]))
				{
					m_bIsKnightInvalidLegCheckSum[p][nAroundCheckSum][nKnightLeg] = FALSE;
					continue;
				}

				//��
				if (j + 1 >= 10 && (nKnightLeg & m_nMask[3]))
				{
					m_bIsKnightInvalidLegCheckSum[p][nAroundCheckSum][nKnightLeg] = FALSE;
					continue;
				}

				m_bIsKnightInvalidLegCheckSum[p][nAroundCheckSum][nKnightLeg] = TRUE;
			}
		}
	}

	//���ڵ�λ�к�λ�е�У�����
	FSINT nCannonCheckSum;

	for (i = 0; i < 10; i++)
	{
		for (nNum = 0 ; nNum < CANNONFRCHECKSUMMAXCNT; nNum++)
		{
			m_nCannonFileRankCheckSum[i][nNum] = INVALIDCHECKSUM;
		}
	}

	//ע��:bug�׷���
	for (i = 0; i < 10; i++)
	{
        nNum = 0 ;

		for (k = 0; k < 1024; k++)
		{
			nCannonCheckSum = 0 ;

			nFlag = 0;
			for (m = i - 1; m >= 0; m--)
			{
				if (m_nMask[m] & k)
				{
					//if����岻�ܵߵ�
					if (nFlag == 1)
					{
						nCannonCheckSum |= m_nMask[m];
						break;
					}
					else
					{
						nCannonCheckSum |= m_nMask[m] ;
						nFlag = 1;
					}
				}
			}

			nFlag = 0;
			for (n = i + 1; n < 10; n++)
			{
				if (m_nMask[n] & k)
				{
					//if����岻�ܵߵ�
					if (nFlag == 1)
					{
						nCannonCheckSum |= m_nMask[n];
						break;
					}
					else
					{
						nCannonCheckSum |= m_nMask[n];
						nFlag = TRUE;
					}
				}
			}

			nFlag = 0;
			for (j = 0; j < nNum; j++)
			{
				if (m_nCannonFileRankCheckSum[i][j] == nCannonCheckSum)
				{
					nFlag = 1;
					break;
				}
			}

			if (nFlag)
			{
				m_nCannonFileRankCheckNum[i][k] = j;
			}
			else
			{
				m_nCannonFileRankCheckNum[i][k] = nNum;
				m_nCannonFileRankCheckSum[i][nNum] = nCannonCheckSum;

				nNum++;
			}
		}
	}

	//m_nCannonFoeFileRankCheckNum
	FSINT nCannonFoeFileRankCheckSum;
	
	for (i = 0; i < 10; i++)
	{
		for (k = 0; k < CANNONFRCHECKSUMMAXCNT; k++)
		{
			if ( m_nCannonFileRankCheckSum[i][k] == INVALIDCHECKSUM )
			{
				break ;
			}

			for (nNum = 0; nNum < 4; nNum++)
			{
				m_nCannonFoeFileRankCheckSum[i][k][nNum] = INVALIDCHECKSUM;
			}
		}
	}

	for (i = 0; i < 10; i++)
	{
		for (k = 0; k < CANNONFRCHECKSUMMAXCNT; k++)
		{
			if (m_nCannonFileRankCheckSum[i][k] == INVALIDCHECKSUM)
			{
				break;
			}

            nNum = 0;
            
			for (nFoeBit = 0;nFoeBit<1024;nFoeBit++)
			{
				//TODO:~��ʹ��
				nCannonFoeFileRankCheckSum = nFoeBit & m_nCannonFileRankCheckSum[i][k] & 
											 (~m_nRookFileRankCheckSum[i][m_nRookFileRankCheckNum[i][m_nCannonFileRankCheckSum[i][k]]]);
				
                nFlag = 0;
				for (j = 0;j<nNum;j++)
				{
					if (m_nCannonFoeFileRankCheckSum[i][k][j] == nCannonFoeFileRankCheckSum)
					{
						nFlag = 1;
						break;
					}
				}
				if (nFlag)
				{
					m_nCannonFoeFileRankCheckNum[i][k][nFoeBit] = j;
				}
				else
				{
					m_nCannonFoeFileRankCheckNum[i][k][nFoeBit] = nNum ;
					m_nCannonFoeFileRankCheckSum[i][k][nNum] = nCannonFoeFileRankCheckSum ;

					nNum ++;
				}
			}
		}
	}

	//m_nKingPosCheckNum
    nNum = 0;
    for (i = 3; i < 6; i++)
    {
        for (j = 0;j<3;j++)
        {
            p = 10 * i + j;

            m_nKingPosCheckNum[p] = nNum;
            m_nKingPosCheckSum[nNum] = p;

            nNum++;
        }

        for (j = 7; j < 10; j++)
        {
            p = 10 * i + j;
            
            m_nKingPosCheckNum[p] = nNum;
            m_nKingPosCheckSum[nNum] = p;
            
            nNum++;
        }
    }

    //m_nAdvisorPosCheckNum
    m_nAdvisorPosCheckNum[30] = 0;
    m_nAdvisorPosCheckNum[50] = 1;
    m_nAdvisorPosCheckNum[41] = 2;
    m_nAdvisorPosCheckNum[32] = 3;
    m_nAdvisorPosCheckNum[52] = 4;

    m_nAdvisorPosCheckNum[37] = 5;
    m_nAdvisorPosCheckNum[57] = 6;
    m_nAdvisorPosCheckNum[48] = 7;
    m_nAdvisorPosCheckNum[39] = 8;
    m_nAdvisorPosCheckNum[59] = 9;

	m_nAdvisorPosCheckNum[90] = 10;
	
    m_nAdvisorPosCheckSum[0] = 30;
    m_nAdvisorPosCheckSum[1] = 50;
    m_nAdvisorPosCheckSum[2] = 41;
    m_nAdvisorPosCheckSum[3] = 32;
    m_nAdvisorPosCheckSum[4] = 52;

    m_nAdvisorPosCheckSum[5] = 37;
    m_nAdvisorPosCheckSum[6] = 57;
    m_nAdvisorPosCheckSum[7] = 48;
    m_nAdvisorPosCheckSum[8] = 39;
    m_nAdvisorPosCheckSum[9] = 59;

	m_nAdvisorPosCheckSum[10] = 90;
	
	//m_nBishopPosCheckNum
	m_nBishopPosCheckNum[20] = 0;
    m_nBishopPosCheckNum[60] = 1;
    m_nBishopPosCheckNum[2] = 2;
    m_nBishopPosCheckNum[42] = 3;
    m_nBishopPosCheckNum[82] = 4;
    m_nBishopPosCheckNum[24] = 5;
    m_nBishopPosCheckNum[64] = 6;
    m_nBishopPosCheckNum[25] = 7;
    m_nBishopPosCheckNum[65] = 8;
    m_nBishopPosCheckNum[7] = 9;
	m_nBishopPosCheckNum[47] = 10;
	m_nBishopPosCheckNum[87] = 11;
    m_nBishopPosCheckNum[29] = 12;
	m_nBishopPosCheckNum[69] = 13;
	m_nBishopPosCheckNum[90] = 14;

    m_nBishopPosCheckSum[0] = 20;
    m_nBishopPosCheckSum[1] = 60;
    m_nBishopPosCheckSum[2] = 2;
    m_nBishopPosCheckSum[3] = 42;
    m_nBishopPosCheckSum[4] = 82;
    m_nBishopPosCheckSum[5] = 24;
    m_nBishopPosCheckSum[6] = 64;
    m_nBishopPosCheckSum[7] = 25;
    m_nBishopPosCheckSum[8] = 65;
    m_nBishopPosCheckSum[9] = 7;
	m_nBishopPosCheckSum[10] = 47;
	m_nBishopPosCheckSum[11] = 87;
    m_nBishopPosCheckSum[12] = 29;
	m_nBishopPosCheckSum[13] = 69;
	m_nBishopPosCheckSum[14] = 90;

	//m_nPawnRankAroundCheckSum
	for (j = 0; j < 10; j++)
	{
		for (k = 0; k < 1024; k++)
		{
			m_nPawnRankAroundCheckSum[0][j][k] = 0;

			if ((j > 0) && (k & m_nMask[j - 1]))
			{
				m_nPawnRankAroundCheckSum[0][j][k] = 1;
			}
		}
	}

	for (j = 0; j < 10; j++)
	{
		for (k = 0; k < 1024; k++)
		{
			m_nPawnRankAroundCheckSum[1][j][k] = 0;

			if ((j < 9) && (k & m_nMask[j + 1]))
			{
				m_nPawnRankAroundCheckSum[1][j][k] = 1;
			}
		}
	}

	//m_nPawnPosCheckNum
	nNum = 0;
	for (i = 0; i < 9; i++)
	{
		for (j = 5; j < 7; j++)
		{
			if ((i % 2) == 0)
			{
				p = 10 * i + j;

				m_nPawnPosCheckNum[BLACK][p] = nNum;
				m_nPawnPosCheckSum[BLACK][nNum] = p;

				nNum++;
			}
		}
	}
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 5; j++)
		{
			p = 10 * i + j;

			m_nPawnPosCheckNum[BLACK][p] = nNum;
			m_nPawnPosCheckSum[BLACK][nNum] = p;

			nNum++;
		}
	}
	m_nPawnPosCheckNum[BLACK][90] = 55;
	m_nPawnPosCheckSum[BLACK][55] = 90;

	nNum = 0;
	for (i = 0; i < 9; i++)
	{
		for (j = 3; j < 5; j++)
		{
			if ((i % 2) == 0)
			{
				p = 10 * i + j;

				m_nPawnPosCheckNum[RED][p] = nNum;
				m_nPawnPosCheckSum[RED][nNum] = p;

				nNum++;
			}
		}
	}
	for (i = 0; i < 9; i++)
	{
		for (j = 5; j < 10; j++)
		{
			p = 10 * i + j;

			m_nPawnPosCheckNum[RED][p] = nNum;
			m_nPawnPosCheckSum[RED][nNum] = p;

			nNum++;
		}
	}
	m_nPawnPosCheckNum[RED][90] = 55;
	m_nPawnPosCheckSum[RED][55] = 90;
	
	//m_nBRKingPosCheckNum[BLACK]
    nNum = 0;
    for (i = 3; i < 6; i++)
    {
        for (j = 7; j < 10; j++)
        {
            p = 10 * i + j;
            
            m_nBRKingPosCheckNum[BLACK][p] = nNum;
            m_nBRKingPosCheckSum[BLACK][nNum] = p;
            
            nNum++;
        }
    }
	m_nBRKingPosCheckNum[BLACK][MAXCHESSGRIDNUM] = nNum;

	//m_nBRKingPosCheckNum[RED]
    nNum = 0;
    for (i = 3; i < 6; i++)
    {
        for (j = 0;j<3;j++)
        {
            p = 10 * i + j;

            m_nBRKingPosCheckNum[RED][p] = nNum;
            m_nBRKingPosCheckSum[RED][nNum] = p;

            nNum++;
        }
    }
	m_nBRKingPosCheckNum[RED][MAXCHESSGRIDNUM] = nNum;
	
	//˫�������ж�����m_bIsTwoKingMeet[3][3][1024]
	FSINT y1, y2;
	FSINT nBKingPosCheckNum;
	FSINT nBKingPos;
	FSINT nRKingPosCheckNum;
	FSINT nRKingPos;
	FSBOOL bFlag;

	for (nBKingPosCheckNum = 0; nBKingPosCheckNum < 9; nBKingPosCheckNum++)
	{
		for (nRKingPosCheckNum = 0; nRKingPosCheckNum < 9; nRKingPosCheckNum++)
		{
			for (k = 0; k < 1024; k++)
			{
				m_bIsTwoKingMeet[nBKingPosCheckNum][nRKingPosCheckNum][k] = FALSE;

				nBKingPos = m_nBRKingPosCheckSum[BLACK][nBKingPosCheckNum];
				nRKingPos = m_nBRKingPosCheckSum[RED][nRKingPosCheckNum];
				
				if (m_CGN2Pos[nBKingPos].x != m_CGN2Pos[nRKingPos].x)
				{
					continue;
				}

				bFlag = TRUE;
				y1 = m_CGN2Pos[nBKingPos].y;
				y2 = m_CGN2Pos[nRKingPos].y;
				for (m = y2 + 1; m < y1; m++)
				{
					if (k & m_nMask[m])
					{
						bFlag = FALSE;
						break;
					}
				}
				m_bIsTwoKingMeet[nBKingPosCheckNum][nRKingPosCheckNum][k] = bFlag;
			}
		}
	}
	for (nBKingPosCheckNum = 0; nBKingPosCheckNum <= 9; nBKingPosCheckNum++)
	{
		for (k = 0; k < 1024; k++)
		{
			m_bIsTwoKingMeet[nBKingPosCheckNum][9][k] = FALSE;
		}
	}
	for (nRKingPosCheckNum = 0; nRKingPosCheckNum < 9; nRKingPosCheckNum++)
	{
		for (k = 0; k < 1024; k++)
		{
			m_bIsTwoKingMeet[9][nRKingPosCheckNum][k] = FALSE;
		}
	}

	//=============================��������=============================
	m_nHave[BLACK] = 0xFFFF;
	m_nHave[RED] = 0;

	m_nSign[BLACK] = 1;
	m_nSign[RED] = -1;

	for (i = 0; i < 17; i++)
	{
		m_nRookFlexValue[i] = (i + 1) * FV_ROOK;
	}
	for (i = 0; i < 8; i++)
	{
		m_nKnightFlexValue[i] = (i + 1) * FV_KNIGHT;
	}
	for (i = 0; i < 17; i++)
	{
		m_nCannonFlexValue[i] = (i + 1) * FV_CANNON;
	}

	m_nEmptyHeartCannonValue[0] = 0;
	m_nEmptyHeartCannonValue[1] = EMPTYHEARTVALUE;

	m_nKnightBasicValue[OPEN_GAME] = BV_KNIGHT_OPEN_GAME;
	m_nKnightBasicValue[MID_GAME] = BV_KNIGHT_MID_GAME;
	m_nKnightBasicValue[END_GAME] = BV_KNIGHT_END_GAME;

	m_nCannonBasicValue[OPEN_GAME] = BV_CANNON_OPEN_GAME;
	m_nCannonBasicValue[MID_GAME] = BV_CANNON_MID_GAME;
	m_nCannonBasicValue[END_GAME] = BV_CANNON_END_GAME;

	m_nPawnBasicValue[OPEN_GAME] = BV_PAWN_OPEN_GAME;
	m_nPawnBasicValue[MID_GAME] = BV_PAWN_MID_GAME;
	m_nPawnBasicValue[END_GAME] = BV_PAWN_END_GAME;

	//=============================��������=============================
	//����������
	srand(time(0));

	for (i = 0; i < 15; i++)
	{
		for (j = 0; j < 9; j++)
		{
			for (k = 0; k < 10; k++)
			{
				m_nHashKey32[i][j][k] = Rand32();
				m_nHashKey64[i][j][k] = Rand64();
			}
		}
	}
	m_nHashKeyPlayer32 = Rand32();
	m_nHashKeyPlayer64 = Rand64();

	//hash���ʼ��
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < m_nHashTabMask + 1; j++)
		{
			m_pHashTab[i][j].nDepth = INVALIDPLY;
			m_pHashTab[i][j].nEval = 0;
			m_pHashTab[i][j].nCBCheckSum = 0;
		}
	}

	m_nDRF_NM = 2;	//��ȼ�������-Null Move(DepthDecreaseFactor)
//	m_nDRF_NM = 6;	//��ȼ�������-Null Move(DepthDecreaseFactor) �ٶȲ���

	m_nHistoryShift = 2;
	
	//��ʼ��m_MoveListÿ�����һ���߷�,��ʷ�÷ֱ����õ�
	//FSINT CSearchEngine::Search(FSINT nDepth, FSINT alpha, FSINT beta)��
	//cm = m_MoveList[nDepth][nBestMoveNum];
	//DEBUGASSERT(m_nHistoryScore[cm.nFrom][cm.nTo] >= 0 && m_nHistoryScore[cm.nFrom][cm.nTo] + (1 << nDepth) <= UINT_MAX);
	//m_nHistoryScore[cm.nFrom][cm.nTo] += (1 << nDepth);
	FSINT nDepth;

	for (nDepth = 0; nDepth < MAXSEARCHDEPTH; nDepth++)
	{
		m_MoveList[nDepth][MAXALLMOVESCNT - 1].nFrom = MAXCHESSGRIDNUM;
		m_MoveList[nDepth][MAXALLMOVESCNT - 1].nTo = MAXCHESSGRIDNUM;
	}

	m_nSearchCnt = 0;
	m_nTotalSearchTime = 0;

	m_cmFight[0].nKind = NORMAL;

	m_cmNullMove.nFrom = INVALIDPIECEPOS;
	m_cmNullMove.nTo = INVALIDPIECEPOS;
	m_cmNullMove.nScore = 0;
	m_cmNullMove.nKind = NORMAL;
	m_cmNullMove.nEatChess = NOCHESS;

	m_bNullMoveOn = TRUE;

	m_bOpenBookOn = TRUE;
	m_bEndBookOn = TRUE;

	m_bOnlyMoveOn = TRUE;

	m_nGetOpenBookMoveErrCnt = 0;

	m_nContemptFactor = g_nContemptFactor_Normal;
	m_ChessStyle = Normal; //������

	m_nDrawMoves = 120;

	m_nCheck[FALSE][NORMAL] = NORMAL;
	m_nCheck[FALSE][ESCAPE] = ESCAPE;
	m_nCheck[FALSE][CAP] = CAP;
	m_nCheck[FALSE][EAT] = EAT;
	m_nCheck[FALSE][CHECK] = CHECK;

	m_nCheck[TRUE][NORMAL] = CHECK;
	m_nCheck[TRUE][ESCAPE] = CHECK;
	m_nCheck[TRUE][CAP] = CHECK;
	m_nCheck[TRUE][EAT] = CHECK;
	m_nCheck[TRUE][CHECK] = CHECK;

//	m_GenMovesFun[ROOK] = GenRookMoves;
//	m_GenMovesFun[KNIGHT] = GenKnightMoves;
//	m_GenMovesFun[CANNON] = GenCannonMoves;
//	m_GenMovesFun[KING] = GenKingMoves;
//	m_GenMovesFun[ADVISOR] = GenAdvisorMoves;
//	m_GenMovesFun[BISHOP] = GenBishopMoves;
//	m_GenMovesFun[PAWN] = GenPawnMoves;
//
//	m_GenMovesFun2[ROOK] = GenRookMoves;
//	m_GenMovesFun2[KNIGHT] = GenKnightMoves;
//	m_GenMovesFun2[CANNON] = GenCannonMoves;
//	m_GenMovesFun2[KING] = GenKingMoves;
//	m_GenMovesFun2[ADVISOR] = GenAdvisorMoves;
//	m_GenMovesFun2[BISHOP] = GenBishopMoves;
//	m_GenMovesFun2[PAWN] = GenPawnMoves;
//
//	m_GenCapMovesFun[ROOK] = GenRookCapMoves;
//	m_GenCapMovesFun[KNIGHT] = GenKnightCapMoves;
//	m_GenCapMovesFun[CANNON] = GenCannonCapMoves;
//	m_GenCapMovesFun[KING] = GenKingCapMoves;
//	m_GenCapMovesFun[ADVISOR] = GenAdvisorCapMoves;
//	m_GenCapMovesFun[BISHOP] = GenBishopCapMoves;
//	m_GenCapMovesFun[PAWN] = GenPawnCapMoves;
	
	m_nCheckExtensionPlies = 1;

	m_bIsHaveCheckMateMove = FALSE;
	m_bIsHaveGoodPosition = FALSE;

	//=============================UCCIЭ��=============================
#ifdef _WIN32
	m_hThinkThread = NULL;

	InitializeCriticalSection(&g_CriticalSection);
#endif

	m_bPonder = FALSE;
	m_bPonderHit = FALSE;

	m_nNoEatMoveCnt_BS = 0;
	m_nNoEatMoveCnt_IS = 0;
	m_nNoEatMoveCnt_IS_Backup = 0;

	m_nBouts = 0;
	m_nCurBout = 0;

	//=============================ǳ��Э��=============================
	m_nQHLevel = 4;
	m_nSearchDepth = g_nQHSchDptByLvl[m_nQHLevel - 1];

	//=============================��������=============================
	//m_nBytes
	for (i = 0; i < MAXSEARCHDEPTH; i++)
	{
		m_nBytes[i] = i * sizeof(CHESSMOVE);
	}

	m_esUCCIEgnStatus = EgnSIdle;

	//m_nPieceID2ChessID
	//��
	m_nPieceID2ChessID[23] = BROOK;
	m_nPieceID2ChessID[31] = BROOK;

	m_nPieceID2ChessID[24] = BKNIGHT;
	m_nPieceID2ChessID[30] = BKNIGHT;

	m_nPieceID2ChessID[21] = BCANNON;
	m_nPieceID2ChessID[22] = BCANNON;

	m_nPieceID2ChessID[27] = BKING;

	m_nPieceID2ChessID[26] = BADVISOR;
	m_nPieceID2ChessID[28] = BADVISOR;

	m_nPieceID2ChessID[25] = BBISHOP;
	m_nPieceID2ChessID[29] = BBISHOP;

	m_nPieceID2ChessID[16] = BPAWN;
	m_nPieceID2ChessID[17] = BPAWN;
	m_nPieceID2ChessID[18] = BPAWN;
	m_nPieceID2ChessID[19] = BPAWN;
	m_nPieceID2ChessID[20] = BPAWN;

	//��
	m_nPieceID2ChessID[0]  = RROOK;
	m_nPieceID2ChessID[8]  = RROOK;

	m_nPieceID2ChessID[1]  = RKNIGHT;
	m_nPieceID2ChessID[7]  = RKNIGHT;

	m_nPieceID2ChessID[9]  = RCANNON;
	m_nPieceID2ChessID[10] = RCANNON;

	m_nPieceID2ChessID[4]  = RKING;

	m_nPieceID2ChessID[3]  = RADVISOR;
	m_nPieceID2ChessID[5]  = RADVISOR;

	m_nPieceID2ChessID[2]  = RBISHOP;
	m_nPieceID2ChessID[6]  = RBISHOP;

	m_nPieceID2ChessID[11] = RPAWN;
	m_nPieceID2ChessID[12] = RPAWN;
	m_nPieceID2ChessID[13] = RPAWN;
	m_nPieceID2ChessID[14] = RPAWN;
	m_nPieceID2ChessID[15] = RPAWN;	

	//���ӷ��ж�����
	for (i = BBEGIN; i <= REND; i++)
	{
		if (i >= BBEGIN && i <= BEND)
		{
			m_bIsBlack_ChessID[i] = TRUE;
			m_bIsRed_ChessID[i] = FALSE;
		}
		else
		{
			m_bIsBlack_ChessID[i] = FALSE;
			m_bIsRed_ChessID[i] = TRUE;
		}
	}
	for (i = 0; i < 32; i++)
	{
		m_bIsBlack_PieceID[i] = m_bIsBlack_ChessID[m_nPieceID2ChessID[i]];
		m_bIsRed_PieceID[i] = m_bIsRed_ChessID[m_nPieceID2ChessID[i]];
	}
	//MakeMove��UnMakeMove�õ�
	m_bIsBlack_PieceID[INVALIDPIECEID] = 2;
	m_bIsRed_PieceID[INVALIDPIECEID] = 2;

	//m_nChessID2PieceID
	//��
	m_nChessID2PieceID[BKING][0] = 27;

	m_nChessID2PieceID[BADVISOR][0] = 26;
	m_nChessID2PieceID[BADVISOR][1] = 28;

	m_nChessID2PieceID[BBISHOP][0] = 25;
	m_nChessID2PieceID[BBISHOP][1] = 29;

	m_nChessID2PieceID[BROOK][0] = 23;
	m_nChessID2PieceID[BROOK][1] = 31;

	m_nChessID2PieceID[BKNIGHT][0] = 24;
	m_nChessID2PieceID[BKNIGHT][1] = 30;

	m_nChessID2PieceID[BCANNON][0] = 21;
	m_nChessID2PieceID[BCANNON][1] = 22;

	m_nChessID2PieceID[BPAWN][0] = 16;
	m_nChessID2PieceID[BPAWN][1] = 17;
	m_nChessID2PieceID[BPAWN][2] = 18;
	m_nChessID2PieceID[BPAWN][3] = 19;
	m_nChessID2PieceID[BPAWN][4] = 20;

	//��
	m_nChessID2PieceID[RKING][0] = 4;

	m_nChessID2PieceID[RADVISOR][0] = 3;
	m_nChessID2PieceID[RADVISOR][1] = 5;

	m_nChessID2PieceID[RBISHOP][0] = 2;
	m_nChessID2PieceID[RBISHOP][1] = 6;

	m_nChessID2PieceID[RROOK][0] = 0;
	m_nChessID2PieceID[RROOK][1] = 8;

	m_nChessID2PieceID[RKNIGHT][0] = 1;
	m_nChessID2PieceID[RKNIGHT][1] = 7;

	m_nChessID2PieceID[RCANNON][0] = 9;
	m_nChessID2PieceID[RCANNON][1] = 10;

	m_nChessID2PieceID[RPAWN][0] = 11;
	m_nChessID2PieceID[RPAWN][1] = 12;
	m_nChessID2PieceID[RPAWN][2] = 13;
	m_nChessID2PieceID[RPAWN][3] = 14;
	m_nChessID2PieceID[RPAWN][4] = 15;

	//��
	m_nInitChessCnt[BKING] = 1;
	for (i=2;i<7;i++)
	{
		m_nInitChessCnt[i] = 2;
	}
	m_nInitChessCnt[BPAWN] = 5;

	//��
	m_nInitChessCnt[RKING] = 1;
	for (i=9;i<14;i++)
	{
		m_nInitChessCnt[i] = 2;
	}
	m_nInitChessCnt[RPAWN] = 5;

	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 3; j++)
		{
			m_nAbs[i][j] = (i - j) < 0 ? 11 : (i - j);
		}
	}

	m_nRook[0] = BROOK;
	m_nRook[1] = RROOK;

	m_nKnight[0] = BKNIGHT;
	m_nKnight[1] = RKNIGHT;

	m_nCannon[0] = BCANNON;
	m_nCannon[1] = RCANNON;

	m_nKing[0] = BKING;
	m_nKing[1] = RKING;

	m_nAdvisor[0] = BADVISOR;
	m_nAdvisor[1] = RADVISOR;

	m_nBishop[0] = BBISHOP;
	m_nBishop[1] = RBISHOP;

	m_nPawn[0] = BPAWN;
	m_nPawn[1] = RPAWN;

	m_nKingID2PieceID[BLACK] = m_nChessID2PieceID[BKING][0];
	m_nKingID2PieceID[RED] = m_nChessID2PieceID[RKING][0];

	//����������ʼ��(��չ)
	for (i = 10; i < 12; i++)
	{
		m_nBitFile[i] = 0;

		m_nBRBitFile[0][i] = 0;
		m_nBRBitFile[1][i] = 0;
	}
	for (i = 9; i < 12; i++)
	{
		m_nBitRank[i] = 0;

		m_nBRBitRank[0][i] = 0;
		m_nBRBitRank[1][i] = 0;
	}

	m_nPiecePos[MAXPIECEID] = INVALIDPIECEPOS;

	memcpy(m_szWhoN2WhoStr[0], "��", 3);
	memcpy(m_szWhoN2WhoStr[1], "��", 3);

	for (i = 0; i < 32; i++)
	{
		m_nPieceID2Bit[i] = 1;
	}
	m_nPieceID2Bit[INVALIDPIECEID] = 0;

	m_nMaxSearchTime = 0; // �����ʱ��[2005/10/22 15:46:05]

	memcpy(m_szBookFile, BOOKFILE, MAXBUFFERLEN);

	m_nConvertChessID[NOCHESS] = NOCHESS;
	for (i = BBEGIN; i <= BEND; i++)
	{
		m_nConvertChessID[i] = i + 7;
	}
	for (i = RBEGIN; i <= REND; i++)
	{
		m_nConvertChessID[i] = i - 7;
	}
}

/*******************************************************************************************************
������ : GenPreMoves
��  �� : �߷�Ԥ����
��  �� : ��
����ֵ : ��
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:42:31
*******************************************************************************************************/
FSVOID CSearchEngine::GenPreMoves()
{
//	cout << "���ڲ�����Ԥ�߷�..." << endl;
	GenRookPreMoves   ();	//��

	FSINT t;

	switch(m_EgnProtocol)
	{
	case EgnPUCCI:
	case EgnPAhei:
		cout << "���ڲ�����Ԥ�߷�..." << endl;
		break;

	case EgnPQianHong:
		STATEMENT_TESTQHP(cout << "���ڲ�����Ԥ�߷�..." << endl;);
		break;

	default:
		break;
	}
	t = clock();
	GenKnightPreMoves ();	//��
	switch(m_EgnProtocol)
	{
	case EgnPUCCI:
	case EgnPAhei:
		cout << "������Ԥ�߷�ʱ��:" << clock() - t << "ms" << endl;
		break;
					
	case EgnPQianHong:
		STATEMENT_TESTQHP(cout << "������Ԥ�߷�ʱ��:" << clock() - t << "ms" << endl;);
		break;
					
	default:
		break;
	}

//	cout << "���ڲ�����Ԥ�߷�..." << endl;
	t = clock();
	GenCannonPreMoves ();	//��
//	cout << "������Ԥ�߷�ʱ��:" << clock() - t << "ms" << endl;

//	cout << "���ڲ�����Ԥ�߷�..." << endl;
	GenKingPreMoves   ();	//��

//	cout << "���ڲ���ʿԤ�߷�..." << endl;
	GenAdvisorPreMoves();	//ʿ

//	cout << "���ڲ�����Ԥ�߷�..." << endl;
	GenBishopPreMoves ();	//��

//	cout << "���ڲ�����Ԥ�߷�..." << endl;
	GenPawnPreMoves   ();	//��
}

/*******************************************************************************************************
������ : GenRookPreMoves
��  �� : �����߷�����
��  �� : ��
����ֵ : ��
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��7��25�� 1:04:29
*******************************************************************************************************/
FSVOID CSearchEngine::GenRookPreMoves()
{
	FSINT p;
	FSINT m, n;
	FSINT i, j, k;
	FSINT nAllCount;
	FSINT nNoCapCount;
	FSINT nCapCount;
	FSINT nCount1, nCount2;
	FSINT nBitFile, nBitRank;//λ��,λ��
	FSINT nFoeBitFile, nFoeBitRank;//�Է���λ��,λ��
	FSINT nFoeFileCheckNum, nFoeRankCheckNum;//�Է���λ�к�λ��У�����
	
#ifdef MEMCALC
	
	FSINT nRookMovesSize = 0;
	FSINT nRookCapMovesSize = 0;
	
#endif
	
	//ע��:bug�׷���,ע��File��Rank
	for (p = 0; p < 90; p++)
	{
		i = m_CGN2Pos[p].x;
		j = m_CGN2Pos[p].y;
		
		//���߷�
		for (k = 0; k < ROOKFILECHECKSUMMAXCNT; k++)
		{
			//����!!!
			if ((nBitFile = m_nRookFileRankCheckSum[i][k]) == INVALIDCHECKSUM)
			{
				break;
			}
			
			nCount1 = 0;
			for (m = i - 1; m >= 0; m--)
			{
				if (m_nMask[m] & nBitFile)
				{
					break;
				}
				
				nCount1++;
			}
			
			nCount2 = 0;
			for (n = i + 1; n < 9; n++)
			{
				if (m_nMask[n] & nBitFile)
				{
					break;
				}
				
				nCount2++;
			}
			
			nNoCapCount = nCount1 + nCount2;

			if (nNoCapCount > 0)
			{
				DEBUGASSERT(nAllCount - nCapCount <= MAXROOKFILENOCAPMOVECNT);
				TRYNEW(m_pcmRookFileNoCapPreMoves[p][k].pMoveList = 
					new CHESSMOVE[nNoCapCount];);

				//�в������߷�
				for (m = i - 1; m > i - 1 - nCount1; m--)
				{
					DEBUGASSERT(m >= 0 && i - 1 - m >= 0 && i - 1 - m < nNoCapCount);
					
					FILLCHESSMOVE(m_pcmRookFileNoCapPreMoves[p][k].pMoveList[i - 1 - m], 
						p, 10 * m + j, BV_ROOK, NOCHESS, NORMAL);
				}
				for (n = i + 1; n < i + 1 + nCount2; n++)
				{
					FILLCHESSMOVE(m_pcmRookFileNoCapPreMoves[p][k].pMoveList[n - i - 1 + nCount1], 
						p, POS2CGN(n, j), BV_ROOK, NOCHESS, NORMAL);
				}
			}

#ifdef MEMCALC
			
			nRookCapMovesSize += m_nBytes[nNoCapCount];
			
#endif
			
			for (nFoeFileCheckNum = 0; nFoeFileCheckNum < 4; nFoeFileCheckNum++)
			{
				//����!!!
				if ((nFoeBitFile = m_nRookFoeFileRankCheckSum[i][k][nFoeFileCheckNum]) == INVALIDCHECKSUM)
				{
					break;
				}
				
				nAllCount = nNoCapCount;
				
				if (nCount1 < i)
				{
					if (m_nMask[i - nCount1 - 1] & nFoeBitFile)//˵��i - nCount1 - 1λ�Է�����
					{
						nAllCount++;
					}
				}
				if ((i + nCount2 + 1 ) < 9)
				{
					if (m_nMask[i + nCount2 + 1] & nFoeBitFile)//˵��i + nCount1 + 1λ�Է�����
					{
						nAllCount++;
					}
				}
				DEBUGASSERT(nAllCount <= MAXROOKFILEMOVECNT);
				
#ifdef MEMCALC
				
				nRookMovesSize += m_nBytes[nAllCount];
				
#endif

				nCapCount = nAllCount - nNoCapCount;

				m_pcmRookFilePreMoves[p][k][nFoeFileCheckNum].nCount = nAllCount;
				m_pcmRookFileNoCapPreMoves[p][k].nCount = nNoCapCount;
				m_pcmRookFileCapPreMoves[p][k][nFoeFileCheckNum].nCount = nCapCount;
				if (nAllCount)
				{
					TRYNEW(m_pcmRookFilePreMoves[p][k][nFoeFileCheckNum].pMoveList = 
						new CHESSMOVE[nAllCount];);

					//�в������߷�
					if (nNoCapCount > 0)
					{
						for (m = i - 1; m > i - 1 - nCount1; m--)
						{
							DEBUGASSERT(m >= 0 && i - 1 - m >= 0 && i - 1 - m < nAllCount);
							
							FILLCHESSMOVE(m_pcmRookFilePreMoves[p][k][nFoeFileCheckNum].pMoveList[i - 1 - m], 
								p, 10 * m + j, BV_ROOK, NOCHESS, NORMAL);
						}
						for (n = i + 1; n < i + 1 + nCount2; n++)
						{
							FILLCHESSMOVE(m_pcmRookFilePreMoves[p][k][nFoeFileCheckNum].pMoveList[n - i - 1 + nCount1], 
								p, POS2CGN(n, j), BV_ROOK, NOCHESS, NORMAL);
						}
					}

					//�г����߷�
					if (nCapCount > 0)
					{
						DEBUGASSERT(nCapCount <= MAXROOKFILECAPMOVECNT);
						TRYNEW(m_pcmRookFileCapPreMoves[p][k][nFoeFileCheckNum].pMoveList = 
							new CHESSMOVE[nCapCount];);

						if (nCount1 < i)
						{
							if (m_nMask[i - nCount1 - 1] & nFoeBitFile)//˵��i-Num1-1λ�Է�����
							{
								DEBUGASSERT(i - nCount1 - 1 >= 0 && i - nCount1 - 1 < 9);							
								DEBUGASSERT(nNoCapCount < nAllCount);
								
								FILLCHESSMOVE(m_pcmRookFilePreMoves[p][k][nFoeFileCheckNum].pMoveList[nCount1 + nCount2], 
									p, POS2CGN(i - nCount1 - 1, j), BV_ROOK, NOCHESS, EAT);
								
								FILLCHESSMOVE(m_pcmRookFileCapPreMoves[p][k][nFoeFileCheckNum].pMoveList[0], 
									p, POS2CGN(i - nCount1 - 1, j), BV_ROOK, NOCHESS, EAT);
							}
						}
						if (i + nCount2 + 1 < 9)
						{
							if (m_nMask[i + nCount2 + 1] & nFoeBitFile)//˵��i+Num2+1λ�Է�����
							{
								DEBUGASSERT(i + nCount2 + 1 >= 0 && i + nCount2 + 1 < 9);							
								DEBUGASSERT(nAllCount - 1 >= 0);
								
								FILLCHESSMOVE(m_pcmRookFilePreMoves[p][k][nFoeFileCheckNum].pMoveList[nAllCount - 1], 
									p, POS2CGN(i + nCount2 + 1, j), BV_ROOK, NOCHESS, EAT);
								
								FILLCHESSMOVE(m_pcmRookFileCapPreMoves[p][k][nFoeFileCheckNum].pMoveList[nCapCount - 1], 
									p, POS2CGN(i + nCount2 + 1, j), BV_ROOK, NOCHESS, EAT);
							}
						}
					}
				}
			}
		}

		//���߷�
		for (k = 0; k < ROOKRANKCHECKSUMMAXCNT; k++)
		{
			//����!!!
			if ((nBitRank = m_nRookFileRankCheckSum[j][k]) == INVALIDCHECKSUM)
			{
				break ;
			}
			
			nCount1 = 0;
			for (m = j - 1; m >= 0; m--)
			{
				if (m_nMask[m] & nBitRank)
				{
					break;
				}
				
				nCount1++;
			}
			
			nCount2 = 0;
			for (n = j + 1; n < 10; n++)
			{
				if (m_nMask[n] & nBitRank)
				{
					break;
				}
				
				nCount2++;
			}
			
			nNoCapCount = nCount1 + nCount2;
			if (nNoCapCount > 0)
			{
				DEBUGASSERT(nCapCount <= MAXROOKRANKNOCAPMOVECNT);

				TRYNEW(m_pcmRookRankNoCapPreMoves[p][k].pMoveList = 
					new CHESSMOVE[nNoCapCount];);
				
				//�в������߷�
				for (m = j - 1; m > j - 1 - nCount1; m--)
				{
					DEBUGASSERT(j - 1 - m >= 0 && j - 1 - m < nNoCapCount);
					
					FILLCHESSMOVE(m_pcmRookRankNoCapPreMoves[p][k].pMoveList[j - 1 - m], 
						p, POS2CGN(i, m), BV_ROOK, NOCHESS, NORMAL);
				}
				
				for (n = j + 1; n < j + 1 + nCount2; n++)
				{
					DEBUGASSERT(n - j - 1 + nCount1 >= 0 && n - j - 1 + nCount1 < nNoCapCount);

					FILLCHESSMOVE(m_pcmRookRankNoCapPreMoves[p][k].pMoveList[n - j - 1 + nCount1], 
						p, POS2CGN(i, n), BV_ROOK, NOCHESS, NORMAL);
				}
			}

#ifdef MEMCALC
			
			nRookCapMovesSize += m_nBytes[nNoCapCount];
			
#endif
			
			for (nFoeRankCheckNum = 0; nFoeRankCheckNum < 4; nFoeRankCheckNum++)
			{
				//����!!!
				if ((nFoeBitRank = m_nRookFoeFileRankCheckSum[j][k][nFoeRankCheckNum]) == INVALIDCHECKSUM)
				{
					break;
				}
				
				nAllCount = nNoCapCount;
				
				if (nCount1 < j)
				{
					if (m_nMask[j - nCount1 - 1] & nFoeBitRank)//˵��j - nCount1 - 1λ�Է�����
					{
						nAllCount++;
					}
				}
				if ((j + nCount2 + 1) < 10)
				{
					if (m_nMask[j + nCount2 + 1] & nFoeBitRank)//˵��j + nCount2 + 1λ�Է�����
					{
						nAllCount++;
					}
				}
				DEBUGASSERT(nAllCount <= MAXROOKRANKMOVECNT);
				
#ifdef MEMCALC
				
				nRookMovesSize += m_nBytes[nAllCount];
				
#endif

				nCapCount = nAllCount - nNoCapCount;
				m_pcmRookRankPreMoves[p][k][nFoeRankCheckNum].nCount = nAllCount;
				m_pcmRookRankNoCapPreMoves[p][k].nCount = nNoCapCount;
				m_pcmRookRankCapPreMoves[p][k][nFoeRankCheckNum].nCount = nCapCount;
				if (nAllCount)
				{
					TRYNEW(m_pcmRookRankPreMoves[p][k][nFoeRankCheckNum].pMoveList = 
						new CHESSMOVE[nAllCount];);

					//�в������߷�
					if (nNoCapCount > 0)
					{
						DEBUGASSERT(nCapCount <= MAXROOKRANKNOCAPMOVECNT);

						for (m = j - 1; m > j - 1 - nCount1; m--)
						{
							DEBUGASSERT(j - 1 - m >= 0 && j - 1 - m < nAllCount);
							
							FILLCHESSMOVE(m_pcmRookRankPreMoves[p][k][nFoeRankCheckNum].pMoveList[j - 1 - m], 
								p, POS2CGN(i, m), BV_ROOK, NOCHESS, NORMAL);
						}
						
						for (n = j + 1; n < j + 1 + nCount2; n++)
						{
							DEBUGASSERT(n - j - 1 + nCount1 >= 0 && n - j - 1 + nCount1 < nAllCount);
							
							FILLCHESSMOVE(m_pcmRookRankPreMoves[p][k][nFoeRankCheckNum].pMoveList[n - j - 1 + nCount1], 
								p, POS2CGN(i, n), BV_ROOK, NOCHESS, NORMAL);
						}
					}

					//�г����߷�
					if (nCapCount > 0)
					{
						DEBUGASSERT(nCapCount <= MAXROOKRANKCAPMOVECNT);
						TRYNEW(m_pcmRookRankCapPreMoves[p][k][nFoeRankCheckNum].pMoveList = 
							new CHESSMOVE[nCapCount];);
						
						if (nCount1 < j)
						{
							if (m_nMask[j - nCount1 - 1] & nFoeBitRank)//˵��j-Num1-1λ�Է�����
							{
								FILLCHESSMOVE(m_pcmRookRankPreMoves[p][k][nFoeRankCheckNum].pMoveList[nCount1 + nCount2], 
									p, POS2CGN(i, j - nCount1 - 1), BV_ROOK, NOCHESS, EAT);
								
								FILLCHESSMOVE(m_pcmRookRankCapPreMoves[p][k][nFoeRankCheckNum].pMoveList[0], 
									p, POS2CGN(i, j - nCount1 - 1), BV_ROOK, NOCHESS, EAT);
							}
						}
						if ((j + nCount2 + 1) < 10)
						{
							if (m_nMask[j+nCount2+1] & nFoeBitRank)//˵��i+Num2+1λ�Է�����
							{
								FILLCHESSMOVE(m_pcmRookRankPreMoves[p][k][nFoeRankCheckNum].pMoveList[nAllCount - 1], 
									p, POS2CGN(i, j + nCount2 + 1), BV_ROOK, NOCHESS, EAT);
								
								FILLCHESSMOVE(m_pcmRookRankCapPreMoves[p][k][nFoeRankCheckNum].pMoveList[nCapCount - 1], 
									p, POS2CGN(i, j + nCount2 + 1), BV_ROOK, NOCHESS, EAT);
							}
						}
					}
				}
			}
		}
	}

#ifdef MEMCALC
		
		cout << "�������߷��ڴ�:" << setprecision(3) << B2M(nRookMovesSize) << "M" << endl;
		cout << "�����г����߷��ڴ�:" << setprecision(3) << B2M(nRookMovesSize - nRookCapMovesSize) << "M" << endl;
		
#endif
}

/*******************************************************************************************************
������ : GenKnightPreMoves
��  �� : ���߷�Ԥ����
��  �� : ��
����ֵ : ��
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:37:41
*******************************************************************************************************/
FSVOID CSearchEngine::GenKnightPreMoves()
{
	FSINT p;
	FSINT i, j;
	FSINT nCount;
	FSINT nCapCount;
	FSINT nKnightLeg;
	FSINT nAroundCheckSum;//����ߵ��У���

#ifdef MEMCALC

	FSINT nKnightMovesSize = 0;
	FSINT nKnightCapMovesSize = 0;

#endif

	// ������Ԥ�߷��ɹ�[2005/10/21 22:31:51]
//	if (LoadKnightPreMove() == TRUE)
//	{
//		return;
//	}

	//�������߷�
	for (p = 0; p < 90; p++)
	{
		i = m_CGN2Pos[p].x;
		j = m_CGN2Pos[p].y;

		//����ߵ��У���
		for (nAroundCheckSum = 0; nAroundCheckSum < 256; nAroundCheckSum++)
		{
			DEBUGASSERT(m_bIsKnightInvalidArroundCheckSum[p][nAroundCheckSum] == TRUE || 
				   m_bIsKnightInvalidArroundCheckSum[p][nAroundCheckSum] == FALSE);

			//�жϴ�У����Ƿ���Ч
			if (m_bIsKnightInvalidArroundCheckSum[p][nAroundCheckSum] == FALSE)
			{
				continue;
			}
			
			//����У���
			for (nKnightLeg = 0; nKnightLeg < 16; nKnightLeg++)
			{
				DEBUGASSERT(m_bIsKnightInvalidLegCheckSum[p][nAroundCheckSum][nKnightLeg] == TRUE ||
					   m_bIsKnightInvalidLegCheckSum[p][nAroundCheckSum][nKnightLeg] == FALSE);

				//�жϴ�����У����Ƿ���Ч
				if (m_bIsKnightInvalidLegCheckSum[p][nAroundCheckSum][nKnightLeg] == FALSE)
				{
					continue;
				}

				nCount = 0;

				//��
				if (((nKnightLeg & m_nMask[0]) == 0) && (i - 2 >= 0))
				{
					//��
					if (((nAroundCheckSum & m_nMask[2]) == 0) && (j - 1 >= 0))
					{
						nCount++;
					}
					//��
					if (((nAroundCheckSum & m_nMask[4]) == 0) && (j + 1 < 10))
					{
						nCount++;
					}
				}
				
				//��
				if (((nKnightLeg & m_nMask[1]) == 0) && (i + 2 < 9))
				{
					//��
					if (((nAroundCheckSum & m_nMask[3]) == 0) && (j - 1 >= 0))
					{
						nCount++;
					}
					//��
					if (((nAroundCheckSum & m_nMask[5]) == 0) && (j + 1 < 10))
					{
						nCount++;
					}
				}
				
				//��
				if (((nKnightLeg & m_nMask[2]) == 0) && (j - 2 >= 0))
				{
					//��
					if (((nAroundCheckSum & m_nMask[0]) == 0) && (i - 1 >= 0))
					{
						nCount++;
					}
					//��
					if (((nAroundCheckSum & m_nMask[1]) == 0) && (i + 1 < 9))
					{
						nCount++;
					}
				}

				//��
				if (((nKnightLeg & m_nMask[3]) == 0) && j + 2 < 10)
				{
					//��
					if (((nAroundCheckSum & m_nMask[6]) == 0) && (i - 1 >= 0))
					{
						nCount++;
					}
					//��
					if (((nAroundCheckSum & m_nMask[7]) == 0) && (i + 1 < 9))
					{
						nCount++;
					}
				}
				DEBUGASSERT(nCount >= 0 && nCount <= 8);

				m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].nCount = nCount;
				if (nCount)
				{
					TRYNEW(m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList = new CHESSMOVE[nCount];);

#ifdef MEMCALC

					nKnightMovesSize += m_nBytes[nCount];

#endif

					nCount = 0;

					//��
					if (((nKnightLeg & m_nMask[0]) == 0) && (i - 2 >= 0))
					{
						//��
						if (((nAroundCheckSum & m_nMask[2]) == 0) && (j - 1 >= 0))
						{
//							m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount].nFrom = p;
//							m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount].nTo = p - 21;
//							m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount].nKind = NORMAL;

							FILLCHESSMOVE(m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount], 
								p, p - 21, BV_KNIGHT_OPEN_GAME, NOCHESS, NORMAL);

							nCount++;
						}
						
						//��
						if (((nAroundCheckSum & m_nMask[4]) == 0) && (j + 1 < 10))
						{
//							m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount].nFrom = p;
//							m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount].nTo = p - 19;
//							m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount].nKind = NORMAL;
							
							FILLCHESSMOVE(m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount], 
								p, p - 19, BV_KNIGHT_OPEN_GAME, NOCHESS, NORMAL);

							nCount++;
						}
					}
					
					//��
					if (((nKnightLeg & m_nMask[1]) == 0) && (i + 2 < 9))
					{
						//��
						if (((nAroundCheckSum & m_nMask[3]) == 0) && (j - 1 >= 0))
						{
//							m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount].nFrom = p;
//							m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount].nTo = p + 19;
//							m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount].nKind = NORMAL;
							
							FILLCHESSMOVE(m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount], 
								p, p + 19, BV_KNIGHT_OPEN_GAME, NOCHESS, NORMAL);

							nCount++;
						}
						
						//��
						if (((nAroundCheckSum & m_nMask[5]) == 0) && (j + 1 < 10))
						{
//							m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount].nFrom = p;
//							m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount].nTo = p + 21;
//							m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount].nKind = NORMAL;
							
							FILLCHESSMOVE(m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount], 
								p, p + 21, BV_KNIGHT_OPEN_GAME, NOCHESS, NORMAL);

							nCount++;
						}
					}
					
					//��
					if (((nKnightLeg & m_nMask[2]) == 0) && (j - 2 >= 0))
					{
						//��
						if (((nAroundCheckSum & m_nMask[0]) == 0) && (i - 1 >= 0))
						{
//							m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount].nFrom = p;
//							m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount].nTo = p - 12;
//							m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount].nKind = NORMAL;
							
							FILLCHESSMOVE(m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount], 
								p, p - 12, BV_KNIGHT_OPEN_GAME, NOCHESS, NORMAL);

							nCount++;
						}

						//��
						if (((nAroundCheckSum & m_nMask[1]) == 0) && (i + 1 < 9))
						{
//							m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount].nFrom = p;
//							m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount].nTo = p + 8;
//							m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount].nKind = NORMAL;
							
							FILLCHESSMOVE(m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount], 
								p, p + 8, BV_KNIGHT_OPEN_GAME, NOCHESS, NORMAL);

							nCount++;
						}
					}

					//��
					if (((nKnightLeg & m_nMask[3]) == 0) && j + 2 < 10)
					{
						//��
						if (((nAroundCheckSum & m_nMask[6]) == 0) && (i - 1 >= 0))
						{
//							m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount].nFrom = p;
//							m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount].nTo = p - 8;
//							m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount].nKind = NORMAL;
//							
							FILLCHESSMOVE(m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount], 
								p, p - 8, BV_KNIGHT_OPEN_GAME, NOCHESS, NORMAL);

							nCount++;
						}

						//��
						if (((nAroundCheckSum & m_nMask[7]) == 0) && (i + 1 < 9))
						{
//							m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount].nFrom = p;
//							m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount].nTo = p + 12;
//							m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount].nKind = NORMAL;
							
							FILLCHESSMOVE(m_pcmKnightNoCapPreMoves[p][nAroundCheckSum][nKnightLeg].pMoveList[nCount], 
								p, p + 12, BV_KNIGHT_OPEN_GAME, NOCHESS, NORMAL);

							nCount++;
						}
					}
				}
			}
		}
	}

	//�����߷�
	FSINT nFoeBitBoardCheckSum;//�Է���λ����У���

	for (p = 0; p < 90; p++)
	{
		i = m_CGN2Pos[p].x;
		j = m_CGN2Pos[p].y;
		
		//����ߵ�Ϊ�Է��ӵ�У���
		for (nFoeBitBoardCheckSum = 0; nFoeBitBoardCheckSum < 256; nFoeBitBoardCheckSum++)
		{
			DEBUGASSERT(m_bIsKnightInvalidArroundCheckSum[p][nFoeBitBoardCheckSum] == TRUE || 
				m_bIsKnightInvalidArroundCheckSum[p][nFoeBitBoardCheckSum] == FALSE);

			//����Ƿ�Ϊ��Ч��У���
			if (m_bIsKnightInvalidArroundCheckSum[p][nFoeBitBoardCheckSum] == FALSE)
			{
				continue;
			}
			
			//����У���
			for (nKnightLeg = 0; nKnightLeg < 16; nKnightLeg++)
			{
				DEBUGASSERT(m_bIsKnightInvalidLegCheckSum[p][nFoeBitBoardCheckSum][nKnightLeg] == TRUE || 
					m_bIsKnightInvalidLegCheckSum[p][nFoeBitBoardCheckSum][nKnightLeg] == FALSE);

				//����Ƿ�Ϊ��Ч������У���
				if (m_bIsKnightInvalidLegCheckSum[p][nFoeBitBoardCheckSum][nKnightLeg] == FALSE)
				{
					continue;
				}
				
				nCapCount = 0;
				
				//��
				if (((nKnightLeg & m_nMask[0]) == 0) && (i - 2 >= 0))
				{
					//��
					if ((nFoeBitBoardCheckSum & m_nMask[2]) && (j - 1 >= 0))
					{
						nCapCount++;
					}
					//��
					if ((nFoeBitBoardCheckSum & m_nMask[4]) && (j + 1 < 10))
					{
						nCapCount++;
					}
				}
				
				//��
				if (((nKnightLeg & m_nMask[1]) == 0) && (i + 2 < 9))
				{
					//��
					if ((nFoeBitBoardCheckSum & m_nMask[3]) && (j - 1 >= 0))
					{
						nCapCount++;
					}
					//��
					if ((nFoeBitBoardCheckSum & m_nMask[5]) && (j + 1 < 10))
					{
						nCapCount++;
					}
				}
				
				//��
				if (((nKnightLeg & m_nMask[2]) == 0) && (j - 2 >= 0))
				{
					//��
					if ((nFoeBitBoardCheckSum & m_nMask[0]) && (i - 1 >= 0))
					{
						nCapCount++;
					}
					//��
					if ((nFoeBitBoardCheckSum & m_nMask[1]) && (i + 1 < 9))
					{
						nCapCount++;
					}
				}
				
				//��
				if (((nKnightLeg & m_nMask[3]) == 0) && j + 2 < 10)
				{
					//��
					if ((nFoeBitBoardCheckSum & m_nMask[6]) && (i - 1 >= 0))
					{
						nCapCount++;
					}
					//��
					if ((nFoeBitBoardCheckSum & m_nMask[7]) && (i + 1 < 9))
					{
						nCapCount++;
					}
				}
				DEBUGASSERT(nCapCount <= 8);
				
				m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].nCount = nCapCount;
				if (nCapCount)
				{
					TRYNEW(m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList = new CHESSMOVE[nCapCount];);
					
#ifdef MEMCALC

					nKnightMovesSize += m_nBytes[nCapCount];
					nKnightCapMovesSize += m_nBytes[nCapCount];
					
#endif
					
					nCapCount = 0;
					
					//��
					if (((nKnightLeg & m_nMask[0]) == 0) && (i - 2 >= 0))
					{
						//��
						if ((nFoeBitBoardCheckSum & m_nMask[2]) && (j - 1 >= 0))
						{
//							m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount].nFrom = p;
//							m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount].nTo = p - 21;
//			
//							m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount].nKind = EAT;
							
							FILLCHESSMOVE(m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount], 
								p, p - 21, BV_KNIGHT_OPEN_GAME, NOCHESS, EAT);

							nCapCount++;
						}
						
						//��
						if ((nFoeBitBoardCheckSum & m_nMask[4]) && (j + 1 < 10))
						{
//							m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount].nFrom = p;
//							m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount].nTo = p - 19;
//
//							m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount].nKind = EAT;
							
							FILLCHESSMOVE(m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount], 
								p, p - 19, BV_KNIGHT_OPEN_GAME, NOCHESS, EAT);

							nCapCount++;
						}
					}
					
					//��
					if (((nKnightLeg & m_nMask[1]) == 0) && (i + 2 < 9))
					{
						//��
						if ((nFoeBitBoardCheckSum & m_nMask[3]) && (j - 1 >= 0))
						{
//							m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount].nFrom = p;
//							m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount].nTo = p + 19;
//
//							m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount].nKind = EAT;							
							
							FILLCHESSMOVE(m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount], 
								p, p + 19, BV_KNIGHT_OPEN_GAME, NOCHESS, EAT);

							nCapCount++;
						}
						
						//��
						if ((nFoeBitBoardCheckSum & m_nMask[5]) && (j + 1 < 10))
						{
//							m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount].nFrom = p;
//							m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount].nTo = p + 21;
//
//							m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount].nKind = EAT;							
							
							FILLCHESSMOVE(m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount], 
								p, p + 21, BV_KNIGHT_OPEN_GAME, NOCHESS, EAT);

							nCapCount++;
						}
					}
					
					//��
					if (((nKnightLeg & m_nMask[2]) == 0) && (j - 2 >= 0))
					{
						//��
						if ((nFoeBitBoardCheckSum & m_nMask[0]) && (i - 1 >= 0))
						{
//							m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount].nFrom = p;
//							m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount].nTo = p - 12;
//
//							m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount].nKind = EAT;							
							
							FILLCHESSMOVE(m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount], 
								p, p - 12, BV_KNIGHT_OPEN_GAME, NOCHESS, EAT);

							nCapCount++;
						}
						
						//��
						if ((nFoeBitBoardCheckSum & m_nMask[1]) && (i + 1 < 9))
						{
//							m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount].nFrom = p;
//							m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount].nTo = p + 8;
//
//							m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount].nKind = EAT;							
							
							FILLCHESSMOVE(m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount], 
								p, p + 8, BV_KNIGHT_OPEN_GAME, NOCHESS, EAT);

							nCapCount++;
						}
					}
					
					//��
					if (((nKnightLeg & m_nMask[3]) == 0) && j + 2 < 10)
					{
						//��
						if ((nFoeBitBoardCheckSum & m_nMask[6]) && (i - 1 >= 0))
						{
//							m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount].nFrom = p;
//							m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount].nTo = p - 8;
//
//							m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount].nKind = EAT;							
							
							FILLCHESSMOVE(m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount], 
								p, p - 8, BV_KNIGHT_OPEN_GAME, NOCHESS, EAT);

							nCapCount++;
						}
						
						//��
						if ((nFoeBitBoardCheckSum & m_nMask[7]) && (i + 1 < 9))
						{
//							m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount].nFrom = p;
//							m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount].nTo = p + 12;
//
//							m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount].nKind = EAT;							
							
							FILLCHESSMOVE(m_pcmKnightCapPreMoves[p][nFoeBitBoardCheckSum][nKnightLeg].pMoveList[nCapCount], 
								p, p + 12, BV_KNIGHT_OPEN_GAME, NOCHESS, EAT);

							nCapCount++;
						}
					}
				}
			}
		}
	}

#ifdef MEMCALC

	cout << "�������߷��ڴ�:" << nKnightMovesSize * 1.0 / (1 << 20) << "M" << endl;
	cout << "�����г����߷��ڴ�:" << nKnightCapMovesSize * 1.0 / (1 << 20) << "M" << endl;

#endif
}

/*******************************************************************************************************
������ : GenCannonPreMoves
��  �� : ���߷�Ԥ����
��  �� : ��
����ֵ : ��
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:38:01
*******************************************************************************************************/
FSVOID CSearchEngine::GenCannonPreMoves()
{
    FSINT p;
	FSINT m, n;
	FSINT nAllCount;
	FSINT nNoCapCount;
	FSINT nCapCount;
	FSINT p1, p2;
	FSINT i, j, k;
	FSINT nFlag;
	FSINT nCount1, nCount2;
	FSINT nBitFile, nBitRank;//λ��,λ��
	FSINT nFoeBitFile, nFoeBitRank;//�Է���λ��,λ��
	FSINT nFoeFileCheckNum, nFoeRankCheckNum;//�Է���λ�к�λ��У�����

#ifdef MEMCALC

	FSINT nCannonMovesSize = 0;
	FSINT nCannonCapMovesSize = 0;

#endif

	for (p = 0; p < 90; p++)
	{
		i = m_CGN2Pos[p].x;
		j = m_CGN2Pos[p].y;
		
		//���߷�
		for (k = 0; k < CANNONFILECHECKSUMMAXCNT; k++)
		{
			//����!!!
			if ((nBitFile = m_nCannonFileRankCheckSum[i][k]) == INVALIDCHECKSUM)
			{
				break;
			}
			
			nCount1 = 0;
			nFlag = 0;
			p1 = -1;
			for (m = i - 1; m >= 0; m--)
			{
				if (m_nMask[m] & nBitFile)
				{
					//if����岻�ܵߵ�
					if (nFlag == 1)
					{
						p1 = m;
						break;
					}
					else
					{
						nFlag = TRUE;
					}
				}
				
				//����!!!
				if (nFlag == 0)
				{
					nCount1++;
				}
			}
			
			nCount2 = 0;
			nFlag = 0;
			p2 = -1;
			for (n = i + 1; n < 9; n++)
			{
				if (m_nMask[n] & nBitFile)
				{
					if (nFlag == 1)
					{
						p2 = n;
						break;
					}
					else
					{
						nFlag = TRUE;
					}
				}
				
				//����!!!
				if (nFlag == 0)
				{
					nCount2++;
				}
			}
			
			nNoCapCount = nCount1 + nCount2;

			// �в������߷�[2005/10/23 15:34:40]
			m_pcmCannonFileNoCapPreMoves[p][k].nCount = nNoCapCount;
			if (nNoCapCount > 0)
			{
				DEBUGASSERT(nNoCapCount <= MAXCANNONFILENOCAPMOVECNT);
				TRYNEW(m_pcmCannonFileNoCapPreMoves[p][k].pMoveList = 
					new CHESSMOVE[nNoCapCount];);
				
				//�в������߷�
				for (m = i - 1; m > i - 1 - nCount1; m--)
				{
					DEBUGASSERT(i - 1 - m >= 0  && i - 1 - m < nNoCapCount);
					
					FILLCHESSMOVE(m_pcmCannonFileNoCapPreMoves[p][k].pMoveList[i - 1 - m], 
						p, m_nPos2CGN[m][j], BV_CANNON_OPEN_GAME, NOCHESS, NORMAL);
				}
				for (n = i + 1; n < i + 1 + nCount2; n++)
				{
					DEBUGASSERT(n - i - 1 + nCount1 >= 0 && n - i - 1 + nCount1 < nNoCapCount);
					
					FILLCHESSMOVE(m_pcmCannonFileNoCapPreMoves[p][k].pMoveList[n - i - 1 + nCount1], 
						p, m_nPos2CGN[n][j], BV_CANNON_OPEN_GAME, NOCHESS, NORMAL);
				}
			}

#ifdef MEMCALC

			nCannonCapMovesSize += m_nBytes[nNoCapCount];

#endif

			for (nFoeFileCheckNum = 0; nFoeFileCheckNum < 4; nFoeFileCheckNum++)
			{
				//����!!!
				if ((nFoeBitFile = m_nCannonFoeFileRankCheckSum[i][k][nFoeFileCheckNum]) == INVALIDCHECKSUM)
				{
					break;
				}

				nAllCount = nNoCapCount;

				if (p1 != -1 && (m_nMask[p1] & nFoeBitFile))//˵��p1λ�Է�����
				{
					DEBUGASSERT(nCount1 < i);
					nAllCount++;
				}
				if (p2!=-1 && (m_nMask[p2] & nFoeBitFile))//˵��p2λ�Է�����
				{
					nAllCount++;
				}
				DEBUGASSERT(nAllCount < 9);
				
				nCapCount = nAllCount - nNoCapCount;
				m_pcmCannonFilePreMoves[p][k][nFoeFileCheckNum].nCount = nAllCount;	
				m_pcmCannonFileCapPreMoves[p][k][nFoeFileCheckNum].nCount = nCapCount;
				if (nAllCount)
				{
					TRYNEW(m_pcmCannonFilePreMoves[p][k][nFoeFileCheckNum].pMoveList = new CHESSMOVE[nAllCount];);

					// �в������߷�[2005/10/23 15:34:40]
					if (nNoCapCount > 0)
					{
						//�в������߷�
						for (m = i - 1; m > i - 1 - nCount1; m--)
						{
							DEBUGASSERT(i - 1 - m >= 0  && i - 1 - m < nAllCount);
							
							FILLCHESSMOVE(m_pcmCannonFilePreMoves[p][k][nFoeFileCheckNum].pMoveList[i - 1 - m], 
								p, m_nPos2CGN[m][j], BV_CANNON_OPEN_GAME, NOCHESS, NORMAL);

							FILLCHESSMOVE(m_pcmCannonFileNoCapPreMoves[p][k].pMoveList[i - 1 - m], 
								p, m_nPos2CGN[m][j], BV_CANNON_OPEN_GAME, NOCHESS, NORMAL);
						}
						for (n = i + 1; n < i + 1 + nCount2; n++)
						{
							DEBUGASSERT(n - i - 1 + nCount1 >= 0 && n - i - 1 + nCount1 < nAllCount);
							
							FILLCHESSMOVE(m_pcmCannonFilePreMoves[p][k][nFoeFileCheckNum].pMoveList[n - i - 1 + nCount1], 
								p, m_nPos2CGN[n][j], BV_CANNON_OPEN_GAME, NOCHESS, NORMAL);

							FILLCHESSMOVE(m_pcmCannonFileNoCapPreMoves[p][k].pMoveList[n - i - 1 + nCount1], 
								p, m_nPos2CGN[n][j], BV_CANNON_OPEN_GAME, NOCHESS, NORMAL);
						}
					}

					// �г����߷�[2005/10/23 15:33:06]
					if (nCapCount > 0)
					{
						DEBUGASSERT(nCapCount <= MAXCANNONFILECAPMOVECNT);
						TRYNEW(m_pcmCannonFileCapPreMoves[p][k][nFoeFileCheckNum].pMoveList = 
							new CHESSMOVE[nCapCount];);

						if (p1 != -1 && (m_nMask[p1] & nFoeBitFile))//˵��p1Ϊ�Է�����
						{
							FILLCHESSMOVE(m_pcmCannonFilePreMoves[p][k][nFoeFileCheckNum].pMoveList[nCount1 + nCount2], 
								p, m_nPos2CGN[p1][j], BV_CANNON_OPEN_GAME, NOCHESS, EAT);
							
							FILLCHESSMOVE(m_pcmCannonFileCapPreMoves[p][k][nFoeFileCheckNum].pMoveList[0], 
								p, m_nPos2CGN[p1][j], BV_CANNON_OPEN_GAME, NOCHESS, EAT);
						}
						if (p2!=-1 && (m_nMask[p2] & nFoeBitFile))//˵��p2λ�Է�����
						{
							FILLCHESSMOVE(m_pcmCannonFilePreMoves[p][k][nFoeFileCheckNum].pMoveList[nAllCount - 1], 
								p, m_nPos2CGN[p2][j], BV_CANNON_OPEN_GAME, NOCHESS, EAT);
							
							FILLCHESSMOVE(m_pcmCannonFileCapPreMoves[p][k][nFoeFileCheckNum].pMoveList[nCapCount - 1], 
								p, m_nPos2CGN[p2][j], BV_CANNON_OPEN_GAME, NOCHESS, EAT);
						}
					}
#ifdef MEMCALC
					
					nCannonMovesSize += m_nBytes[nAllCount];
					
#endif
				}
			}
		}

		//���߷�
		for (k = 0; k < CANNONRANKCHECKSUMMAXCNT; k ++)
		{
			//����!!!
			if ((nBitRank = m_nCannonFileRankCheckSum[j][k]) == INVALIDCHECKSUM)
			{
				break;
			}
			
			nCount1 = 0;
			nFlag = 0;
			p1=-1;
			for (m = j - 1; m >= 0; m--)
			{
				if (m_nMask[m] & nBitRank)
				{
					if (nFlag == 1)
					{
						p1 = m;
						break;
					}
					else
					{
						nFlag = TRUE;
					}
				}
				
				//����!!!
				if (nFlag == 0)
				{
					nCount1++;
				}
			}
			
			nCount2 = 0;
			nFlag = 0;
			p2 = -1;
			for (n = j + 1; n < 10; n++)
			{
				if (m_nMask[n] & nBitRank)
				{
					if (nFlag == 1)
					{
						p2 = n;
						break;
					}
					else
					{
						nFlag = TRUE;
					}
				}
				
				//����!!!
				if (nFlag == 0)
				{
					nCount2++;
				}
			}
			
			nNoCapCount = nCount1 + nCount2;

			// �в������߷�[2005/10/23 15:39:36]
			m_pcmCannonRankNoCapPreMoves[p][k].nCount = nNoCapCount;
			if (nNoCapCount > 0)
			{
				DEBUGASSERT(nNoCapCount <= MAXCANNONRANKNOCAPMOVECNT);

				TRYNEW(m_pcmCannonRankNoCapPreMoves[p][k].pMoveList = 
					new CHESSMOVE[nNoCapCount];);
				
				for (m = j - 1; m > j - 1 - nCount1; m--)
				{
					DEBUGASSERT(j - 1 - m >= 0 && j - 1 - m < nNoCapCount);
								
					FILLCHESSMOVE(m_pcmCannonRankNoCapPreMoves[p][k].pMoveList[j - 1 - m], 
						p, m_nPos2CGN[i][m], BV_CANNON_OPEN_GAME, NOCHESS, NORMAL);
				}
				
				for (n = j + 1; n < j + 1 + nCount2; n++)
				{
					DEBUGASSERT(n - j - 1 + nCount1 >= 0 && n - j - 1 + nCount1 < nNoCapCount);
		
					FILLCHESSMOVE(m_pcmCannonRankNoCapPreMoves[p][k].pMoveList[n - j - 1 + nCount1], 
						p, m_nPos2CGN[i][n], BV_CANNON_OPEN_GAME, NOCHESS, NORMAL);
				}
			}

#ifdef MEMCALC

			nCannonCapMovesSize += m_nBytes[nCapCount];

#endif

			for (nFoeRankCheckNum = 0; nFoeRankCheckNum < 4; nFoeRankCheckNum++)
			{
				//����!!!
				if ((nFoeBitRank = m_nCannonFoeFileRankCheckSum[j][k][nFoeRankCheckNum]) == INVALIDCHECKSUM)
				{
					break;
				}
				
				nAllCount = nNoCapCount;

				if (p1 != -1 && (m_nMask[p1] & nFoeBitRank))//˵��p1Ϊ�Է�����
				{
					nAllCount++;
				}
				if (p2 != -1 && (m_nMask[p2] & nFoeBitRank))//˵��p2Ϊ�Է�����
				{
					nAllCount++;
				}
				DEBUGASSERT(nAllCount < 10);
				
				nCapCount = nAllCount - nNoCapCount;
				m_pcmCannonRankPreMoves[p][k][nFoeRankCheckNum].nCount = nAllCount;
				m_pcmCannonRankCapPreMoves[p][k][nFoeRankCheckNum].nCount = nCapCount;
				if (nAllCount > 0)
				{
					TRYNEW(m_pcmCannonRankPreMoves[p][k][nFoeRankCheckNum].pMoveList = 
						new CHESSMOVE[nAllCount];);

					// �в������߷�[2005/10/23 15:39:36]
					if (nNoCapCount > 0)
					{
						for (m = j - 1; m > j - 1 - nCount1; m--)
						{
							DEBUGASSERT(j - 1 - m >= 0 && j - 1 - m < nAllCount);
							
							FILLCHESSMOVE(m_pcmCannonRankPreMoves[p][k][nFoeRankCheckNum].pMoveList[j - 1 - m], 
								p, m_nPos2CGN[i][m], BV_CANNON_OPEN_GAME, NOCHESS, NORMAL);

							FILLCHESSMOVE(m_pcmCannonRankNoCapPreMoves[p][k].pMoveList[j - 1 - m], 
								p, m_nPos2CGN[i][m], BV_CANNON_OPEN_GAME, NOCHESS, NORMAL);
						}
						
						for (n = j + 1; n < j + 1 + nCount2; n++)
						{
							DEBUGASSERT(n - j - 1 + nCount1 >= 0 && n - j - 1 + nCount1 < nAllCount);
							
							FILLCHESSMOVE(m_pcmCannonRankPreMoves[p][k][nFoeRankCheckNum].pMoveList[n - j - 1 + nCount1], 
								p, m_nPos2CGN[i][n], BV_CANNON_OPEN_GAME, NOCHESS, NORMAL);

							FILLCHESSMOVE(m_pcmCannonRankNoCapPreMoves[p][k].pMoveList[n - j - 1 + nCount1], 
								p, m_nPos2CGN[i][n], BV_CANNON_OPEN_GAME, NOCHESS, NORMAL);
						}
					}

					// �г����߷�[2005/10/23 15:38:19]
					if (nCapCount > 0)
					{
						DEBUGASSERT(nCapCount <= MAXCANNONRANKCAPMOVECNT);
						TRYNEW(m_pcmCannonRankCapPreMoves[p][k][nFoeRankCheckNum].pMoveList = 
							new CHESSMOVE[nCapCount];);

						if (p1 != -1 && (m_nMask[p1] & nFoeBitRank))//˵��p1λ�Է�����
						{
							FILLCHESSMOVE(m_pcmCannonRankPreMoves[p][k][nFoeRankCheckNum].pMoveList[nCount1 + nCount2], 
								p, m_nPos2CGN[i][p1], BV_CANNON_OPEN_GAME, NOCHESS, EAT);
							
							FILLCHESSMOVE(m_pcmCannonRankCapPreMoves[p][k][nFoeRankCheckNum].pMoveList[0], 
								p, m_nPos2CGN[i][p1], BV_CANNON_OPEN_GAME, NOCHESS, EAT);
						}
						if (p2 != -1 && (m_nMask[p2] & nFoeBitRank))//˵��p2λ�Է�����
						{
							FILLCHESSMOVE(m_pcmCannonRankPreMoves[p][k][nFoeRankCheckNum].pMoveList[nAllCount - 1], 
								p, m_nPos2CGN[i][p2], BV_CANNON_OPEN_GAME, NOCHESS, EAT);
							
							FILLCHESSMOVE(m_pcmCannonRankCapPreMoves[p][k][nFoeRankCheckNum].pMoveList[nCapCount - 1], 
								p, m_nPos2CGN[i][p2], BV_CANNON_OPEN_GAME, NOCHESS, EAT);
						}
					}
#ifdef MEMCALC

					nCannonMovesSize += m_nBytes[nAllCount];
					
#endif
				}
			}
		}
	}

#ifdef MEMCALC

	cout << "�������߷��ڴ�:" << nCannonMovesSize * 1.0 / (1 << 20) << "M" << endl;
	cout << "�����г����߷��ڴ�:" << (nCannonMovesSize - nCannonCapMovesSize) * 1.0 / (1 << 20) << "M" << endl;

#endif
}

/*******************************************************************************************************
������ : GenKingPreMoves
��  �� : ���߷�Ԥ����
��  �� : ��
����ֵ : ��
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:38:17
*******************************************************************************************************/
FSVOID CSearchEngine::GenKingPreMoves()
{
    FSINT i, j;
    FSINT nCount;
	int nNoCapCount;
	FSINT nCapCount;
    FSINT nAroundCheckSum;
	FSINT nPosCheckNum;
	FSINT nPosCheckSum;
	FSINT nFoeAroundCheckSum;

#ifdef MEMCALC

	FSINT nKingMovesSize = 0;
	FSINT nKingCapMovesSize = 0;

#endif

	//��λ��У�����
	for (nPosCheckNum = 0; nPosCheckNum < 18; nPosCheckNum++)
	{
		nPosCheckSum = m_nKingPosCheckSum[nPosCheckNum];
		i = m_CGN2Pos[nPosCheckSum].x;
		j = m_CGN2Pos[nPosCheckSum].y;

		//����Χ���У���
		for (nAroundCheckSum = 0;nAroundCheckSum<16;nAroundCheckSum++)
		{
			//����Χ�Է����У���
			for (nFoeAroundCheckSum = 0; nFoeAroundCheckSum < 16; nFoeAroundCheckSum++)
			{
                nCount = 0;

                //��
                if (i>=4)
                {
                    if (nAroundCheckSum & m_nMask[0])//�������
                    {
                        if (nFoeAroundCheckSum & m_nMask[0])//���Ϊ�Է���
						{
                            nCount++;
						}
                    }
                    else
					{
                        nCount++;
					}
                }
                //��
                if (i <= 4)
                {
                    if (nAroundCheckSum & m_nMask[1])//�ұ�����
                    {
                        if (nFoeAroundCheckSum & m_nMask[1])//�ұ�Ϊ�Է���
						{
                            nCount++;
						}
                    }
                    else
					{
                        nCount++;
					}
                }
				//��
                if ((j>=1 && j<=2) || (j>=8 && j<=9))
                {
                    if (nAroundCheckSum & m_nMask[2])//�±�����
                    {
                        if (nFoeAroundCheckSum & m_nMask[2])//�±�Ϊ�Է���
						{
                            nCount++;
						}
                    }
                    else
					{
                        nCount++;
					}
                }
                //��
                if ((j>= 0 && j<=1) || (j>=7 && j<=8))
                {
                    if (nAroundCheckSum & m_nMask[3])//�ϱ�����
                    {
                        if (nFoeAroundCheckSum & m_nMask[3])//�ϱ�Ϊ�Է���
						{
                            nCount++;
						}
                    }
                    else
					{
                        nCount++;
					}
                }
				DEBUGASSERT(nCount >= 0 && nCount <= 4);
                
                m_pcmKingPreMoves[nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].nCount = nCount;
                if (nCount)
                {
					TRYNEW(m_pcmKingPreMoves[nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList = new CHESSMOVE[nCount];);

#ifdef MEMCALC

					nKingMovesSize += m_nBytes[nCount];

#endif

                    nCount = 0;

                    //��
                    if (i>=4)
                    {
                        if (nAroundCheckSum & m_nMask[0])//�������
                        {
                            if (nFoeAroundCheckSum & m_nMask[0])//���Ϊ�Է���
                            {
								FILLCHESSMOVE(m_pcmKingPreMoves[nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
									nPosCheckSum, nPosCheckSum - 10, BV_KING, NOCHESS, EAT);

                                nCount++;
                            }
                        }
                        else
                        {
							FILLCHESSMOVE(m_pcmKingPreMoves[nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
								nPosCheckSum, nPosCheckSum - 10, BV_KING, NOCHESS, NORMAL);

                            nCount++;
                        }
                    }
                    //��
                    if (i <= 4)
                    {
                        if (nAroundCheckSum & m_nMask[1])//�ұ�����
                        {
                            if (nFoeAroundCheckSum & m_nMask[1])//�ұ�Ϊ�Է���
                            {
								FILLCHESSMOVE(m_pcmKingPreMoves[nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
									nPosCheckSum, nPosCheckSum + 10, BV_KING, NOCHESS, EAT);

                                nCount++;
                            }
                        }
                        else
                        {
							FILLCHESSMOVE(m_pcmKingPreMoves[nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
								nPosCheckSum, nPosCheckSum + 10, BV_KING, NOCHESS, NORMAL);

                            nCount++;
                        }
                    }
					//��
                    if ((j>=1 && j<=2) || (j>=8 && j<=9))
                    {
                        if (nAroundCheckSum & m_nMask[2])//�±�����
                        {
                            if (nFoeAroundCheckSum & m_nMask[2])//�±�Ϊ�Է���
                            {
								FILLCHESSMOVE(m_pcmKingPreMoves[nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
									nPosCheckSum, nPosCheckSum - 1, BV_KING, NOCHESS, EAT);

                                nCount++;
                            }
                        }
                        else
                        {
							FILLCHESSMOVE(m_pcmKingPreMoves[nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
									nPosCheckSum, nPosCheckSum - 1, BV_KING, NOCHESS, NORMAL);

                            nCount++;
                        }
                    }
                    //��
                    if ((j>= 0 && j<=1) || (j>=7 && j<=8))
                    {
                        if (nAroundCheckSum & m_nMask[3])//�ϱ�����
                        {
                            if (nFoeAroundCheckSum & m_nMask[3])//�ϱ�Ϊ�Է���
                            {
								FILLCHESSMOVE(m_pcmKingPreMoves[nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
									nPosCheckSum, nPosCheckSum + 1, BV_KING, NOCHESS, EAT);

                                nCount++;
                            }
                        }
                        else
                        {
							FILLCHESSMOVE(m_pcmKingPreMoves[nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
									nPosCheckSum, nPosCheckSum + 1, BV_KING, NOCHESS, NORMAL);

                            nCount++;
                        }
                    }
                }
			}
		}
	}

	//��λ��У�����
	for (nPosCheckNum = 0; nPosCheckNum < 18; nPosCheckNum++)
	{
		nPosCheckSum = m_nKingPosCheckSum[nPosCheckNum];
		i = m_CGN2Pos[nPosCheckSum].x;
		j = m_CGN2Pos[nPosCheckSum].y;
		
		//�������߷�
		//����Χ���У���
		for (nAroundCheckSum = 0; nAroundCheckSum < 16; nAroundCheckSum++)
		{
			nNoCapCount = 0;
			
			//��,�������
			if (i >= 4 && (nAroundCheckSum & m_nMask[0]) == 0)
			{
				nNoCapCount++;
			}
			//��,�ұ�����
			if (i <= 4 && (nAroundCheckSum & m_nMask[1]) == 0)
			{
				nNoCapCount++;
			}
			//��,�±�����
			if (((j>=1 && j<=2) || (j>=8 && j<=9)) && (nAroundCheckSum & m_nMask[2]) == 0)
			{
				nNoCapCount++;
			}
			//��,�ϱ�����
			if (((j>= 0 && j<=1) || (j>=7 && j<=8)) && (nAroundCheckSum & m_nMask[3]) == 0)
			{
				nNoCapCount++;
			}
			DEBUGASSERT(nNoCapCount <= MAXKINGNOCAPMOVECNT);
			
			m_pcmKingNoCapPreMoves[nPosCheckNum][nAroundCheckSum].nCount = nNoCapCount;
			if (nNoCapCount)
			{
				TRYNEW(m_pcmKingNoCapPreMoves[nPosCheckNum][nAroundCheckSum].pMoveList = 
					new CHESSMOVE[nNoCapCount];);

#ifdef MEMCALC
				
				nKingCapMovesSize += m_nBytes[nNoCapCount];
				
#endif
				
				nNoCapCount = 0;
				
				//��,�������
				if (i >= 4 && (nAroundCheckSum & m_nMask[0]) == 0)
				{
					FILLCHESSMOVE(m_pcmKingNoCapPreMoves[nPosCheckNum][nAroundCheckSum].pMoveList[nNoCapCount], 
						nPosCheckSum, nPosCheckSum - 10, BV_KING, NOCHESS, EAT);

					nNoCapCount++;
				}
				//��,�ұ�����
				if (i <= 4 && (nAroundCheckSum & m_nMask[1]) == 0)
				{
					FILLCHESSMOVE(m_pcmKingNoCapPreMoves[nPosCheckNum][nAroundCheckSum].pMoveList[nNoCapCount], 
						nPosCheckSum, nPosCheckSum + 10, BV_KING, NOCHESS, EAT);

					nNoCapCount++;
				}
				//��,�±�����
				if (((j>=1 && j<=2) || (j>=8 && j<=9)) && (nAroundCheckSum & m_nMask[2]) == 0)
				{
					FILLCHESSMOVE(m_pcmKingNoCapPreMoves[nPosCheckNum][nAroundCheckSum].pMoveList[nNoCapCount], 
						nPosCheckSum, nPosCheckSum - 1, BV_KING, NOCHESS, EAT);

					nNoCapCount++;
				}
				//��,�ϱ�����
				if (((j>= 0 && j<=1) || (j>=7 && j<=8)) && (nAroundCheckSum & m_nMask[3]) == 0)
				{
					FILLCHESSMOVE(m_pcmKingNoCapPreMoves[nPosCheckNum][nAroundCheckSum].pMoveList[nNoCapCount], 
						nPosCheckSum, nPosCheckSum + 1, BV_KING, NOCHESS, EAT);

					nNoCapCount++;
				}
			}
		}

		//�����߷�
		//����Χ�Է����У���
		for (nFoeAroundCheckSum = 0; nFoeAroundCheckSum < 16; nFoeAroundCheckSum++)
		{
			nCapCount = 0;
			
			//��,���Ϊ�Է���
			if (i >= 4 && (nFoeAroundCheckSum & m_nMask[0]))
			{
				nCapCount++;
			}
			//��,�ұ�Ϊ�Է���
			if (i <= 4 && (nFoeAroundCheckSum & m_nMask[1]))
			{
				nCapCount++;
			}
			//��,�±�Ϊ�Է���
			if (((j>=1 && j<=2) || (j>=8 && j<=9)) && (nFoeAroundCheckSum & m_nMask[2]))
			{
				nCapCount++;
			}
			//��,�ϱ�Ϊ�Է���
			if (((j>= 0 && j<=1) || (j>=7 && j<=8)) && (nFoeAroundCheckSum & m_nMask[3]))
			{
				nCapCount++;
			}
			DEBUGASSERT(nCapCount <= 4);
			
			m_pcmKingCapPreMoves[nPosCheckNum][nFoeAroundCheckSum].nCount = nCapCount;
			if (nCapCount)
			{
				TRYNEW(m_pcmKingCapPreMoves[nPosCheckNum][nFoeAroundCheckSum].pMoveList = new CHESSMOVE[nCapCount];);

#ifdef MEMCALC
				
				nKingCapMovesSize += m_nBytes[nCapCount];
				
#endif
				
				nCapCount = 0;
				
				//��,���Ϊ�Է���
				if (i >= 4 && (nFoeAroundCheckSum & m_nMask[0]))
				{
					FILLCHESSMOVE(m_pcmKingCapPreMoves[nPosCheckNum][nFoeAroundCheckSum].pMoveList[nCapCount], 
						nPosCheckSum, nPosCheckSum - 10, BV_KING, NOCHESS, EAT);

					nCapCount++;
				}
				//��,�ұ�Ϊ�Է���
				if (i <= 4 && (nFoeAroundCheckSum & m_nMask[1]))
				{
					FILLCHESSMOVE(m_pcmKingCapPreMoves[nPosCheckNum][nFoeAroundCheckSum].pMoveList[nCapCount], 
						nPosCheckSum, nPosCheckSum + 10, BV_KING, NOCHESS, EAT);

					nCapCount++;
				}
				//��,�±�Ϊ�Է���
				if (((j>=1 && j<=2) || (j>=8 && j<=9)) && (nFoeAroundCheckSum & m_nMask[2]))
				{
					FILLCHESSMOVE(m_pcmKingCapPreMoves[nPosCheckNum][nFoeAroundCheckSum].pMoveList[nCapCount], 
						nPosCheckSum, nPosCheckSum - 1, BV_KING, NOCHESS, EAT);

					nCapCount++;
				}
				//��,�ϱ�Ϊ�Է���
				if (((j>= 0 && j<=1) || (j>=7 && j<=8)) && (nFoeAroundCheckSum & m_nMask[3]))
				{
					FILLCHESSMOVE(m_pcmKingCapPreMoves[nPosCheckNum][nFoeAroundCheckSum].pMoveList[nCapCount], 
						nPosCheckSum, nPosCheckSum + 1, BV_KING, NOCHESS, EAT);

					nCapCount++;
				}
			}
		}
	}

#ifdef MEMCALC

	cout << "�������߷��ڴ�:" << nKingMovesSize * 1.0 / (1 << 20) << "M" << endl;
	cout << "�����г����߷��ڴ�:" << nKingCapMovesSize * 1.0 / (1 << 20) << "M" << endl;

#endif
}

/*******************************************************************************************************
������ : GenAdvisorPreMoves
��  �� : ʿ���߷�Ԥ����
��  �� : ��
����ֵ : ��
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:38:36
*******************************************************************************************************/
FSVOID CSearchEngine::GenAdvisorPreMoves()
{
    FSINT i, j;
    FSINT nCount;
	FSINT nNoCapCount;
	FSINT nCapCount;
    FSINT nAroundCheckSum;
    FSINT nPosCheckNum;
    FSINT nPosCheckSum;
    FSINT nFoeAroundCheckSum;

#ifdef MEMCALC

	FSINT nAdvisorMovesSize = 0;
	FSINT nAdvisorCapMovesSize = 0;

#endif

	//ʿλ��У�����
    for (nPosCheckNum = 0; nPosCheckNum < 10; nPosCheckNum++)
    {
		nPosCheckSum = m_nAdvisorPosCheckSum[nPosCheckNum];
		i = m_CGN2Pos[nPosCheckSum].x;
		j = m_CGN2Pos[nPosCheckSum].y;

		//ʿ��Χ�Է������У���
		for (nFoeAroundCheckSum = 0; nFoeAroundCheckSum < 16; nFoeAroundCheckSum++)
		{
			//�������߷�
			//ʿ��Χ���У���
			for (nAroundCheckSum = 0;nAroundCheckSum<16;nAroundCheckSum++)
			{
                nCount = 0;
                
                //���Ͻ�
                if (nPosCheckSum==50 || nPosCheckSum==41 || nPosCheckSum==57 || nPosCheckSum==48)
                {
                    if (nAroundCheckSum & m_nMask[2])//���Ͻ�����
                    {
                        if (nFoeAroundCheckSum & m_nMask[2])//���Ͻ�Ϊ�Է���
						{
                            nCount++;
						}
                    }
                    else
					{
                        nCount++;
					}
                }
                //���Ͻ�
                if (nPosCheckSum == 30 || nPosCheckSum == 41 || 
					nPosCheckSum == 37 || nPosCheckSum == 48)
                {
                    if (nAroundCheckSum & m_nMask[3])//���Ͻ�����
                    {
                        if (nFoeAroundCheckSum & m_nMask[3])//���Ͻ�Ϊ�Է���
						{
                            nCount++;
						}
                    }
                    else
					{
                        nCount++;
					}
                }
                //���½�
                if (nPosCheckSum == 52 || nPosCheckSum == 41 || 
					nPosCheckSum == 59 || nPosCheckSum == 48)
                {
                    if (nAroundCheckSum & m_nMask[0])//���½�����
                    {
                        if (nFoeAroundCheckSum & m_nMask[0])//���½�Ϊ�Է���
						{
                            nCount++;
						}
                    }
                    else
					{
                        nCount++;
					}
                }
                //���½�
                if (nPosCheckSum==32 || nPosCheckSum==41 || nPosCheckSum==39 || nPosCheckSum==48)
                {
                    if (nAroundCheckSum & m_nMask[1])//���½�����
                    {
                        if (nFoeAroundCheckSum & m_nMask[1])//���½�Ϊ�Է���
						{
                            nCount++;
						}
                    }
                    else
					{
                        nCount++;
					}
                }
				DEBUGASSERT(nCount >= 0 && nCount <= 4);

                m_pcmAdvisorPreMoves[nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].nCount = nCount;
                if (nCount)
                {
					TRYNEW(m_pcmAdvisorPreMoves[nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList = new CHESSMOVE[nCount];);

#ifdef MEMCALC

					nAdvisorMovesSize += m_nBytes[nCount];

#endif

                    nCount = 0;

                    //���Ͻ�
                    if (nPosCheckSum==50 || nPosCheckSum==41 || nPosCheckSum==57 || nPosCheckSum==48)
                    {
                        if (nAroundCheckSum & m_nMask[2])//���Ͻ�����
                        {
                            if (nFoeAroundCheckSum & m_nMask[2])//���Ͻ�Ϊ�Է���
                            {
								FILLCHESSMOVE(m_pcmAdvisorPreMoves[nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
									nPosCheckSum, nPosCheckSum - 9, BV_ADVISOR, NOCHESS, EAT);

                                nCount++;
                            }
                        }
                        else
                        {
							FILLCHESSMOVE(m_pcmAdvisorPreMoves[nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
								nPosCheckSum, nPosCheckSum - 9, BV_ADVISOR, NOCHESS, NORMAL);

                            nCount++;
                        }
                    }
                    //���Ͻ�
                    if (nPosCheckSum==30 || nPosCheckSum==41 || nPosCheckSum==37 || nPosCheckSum==48)
                    {
                        if (nAroundCheckSum & m_nMask[3])//���Ͻ�����
                        {
                            if (nFoeAroundCheckSum & m_nMask[3])//���Ͻ�Ϊ�Է���
                            {
								FILLCHESSMOVE(m_pcmAdvisorPreMoves[nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
									nPosCheckSum, nPosCheckSum + 11, BV_ADVISOR, NOCHESS, EAT);

                                nCount++;
                            }
                        }
                        else
                        {
							FILLCHESSMOVE(m_pcmAdvisorPreMoves[nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
									nPosCheckSum, nPosCheckSum + 11, BV_ADVISOR, NOCHESS, NORMAL);

                            nCount++;
                        }
                    }
                    //���½�
                    if (nPosCheckSum==52 || nPosCheckSum==41 || nPosCheckSum==59 || nPosCheckSum==48)
                    {
                        if (nAroundCheckSum & m_nMask[0])//���½�����
                        {
                            if (nFoeAroundCheckSum & m_nMask[0])//���½�Ϊ�Է���
                            {
								FILLCHESSMOVE(m_pcmAdvisorPreMoves[nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
									nPosCheckSum, nPosCheckSum - 11, BV_ADVISOR, NOCHESS, EAT);

                                nCount++;
                            }
                        }
                        else
                        {
							FILLCHESSMOVE(m_pcmAdvisorPreMoves[nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
									nPosCheckSum, nPosCheckSum - 11, BV_ADVISOR, NOCHESS, NORMAL);

                            nCount++;
                        }
                    }
                    //���½�
                    if (nPosCheckSum==32 || nPosCheckSum==41 || nPosCheckSum==39 || nPosCheckSum==48)
                    {
                        if (nAroundCheckSum & m_nMask[1])//���½�����
                        {
                            if (nFoeAroundCheckSum & m_nMask[1])//���½�Ϊ�Է���
                            {
								FILLCHESSMOVE(m_pcmAdvisorPreMoves[nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
									nPosCheckSum, nPosCheckSum + 9, BV_ADVISOR, NOCHESS, EAT);

                                nCount++;
                            }
                        }
                        else
                        {
							FILLCHESSMOVE(m_pcmAdvisorPreMoves[nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
									nPosCheckSum, nPosCheckSum + 9, BV_ADVISOR, NOCHESS, NORMAL);
                          
							nCount++;
                        }
                    }
                }
            }
		}
	}

	//ʿλ��У�����
    for (nPosCheckNum = 0; nPosCheckNum < 10; nPosCheckNum++)
    {
		nPosCheckSum = m_nAdvisorPosCheckSum[nPosCheckNum];
		i = m_CGN2Pos[nPosCheckSum].x;
		j = m_CGN2Pos[nPosCheckSum].y;

		//�������߷�
		//ʿ��Χ�Է������У���
		for (nAroundCheckSum = 0; nAroundCheckSum < 16; nAroundCheckSum++)
		{
			nNoCapCount = 0;
			
			//���Ͻ�, ���Ͻ�Ϊ�Է���
			if ((nPosCheckSum == 50 || nPosCheckSum == 41 || 
				nPosCheckSum == 57 || nPosCheckSum == 48) && 
				(nAroundCheckSum & m_nMask[2]) == 0)
			{
				nNoCapCount++;
			}
			//���Ͻ�, ���Ͻ�Ϊ�Է���
			if ((nPosCheckSum == 30 || nPosCheckSum == 41 || 
				nPosCheckSum == 37 || nPosCheckSum == 48) && 
				(nAroundCheckSum & m_nMask[3]) == 0)
			{
				nNoCapCount++;
			}
			//���½�, ���½�Ϊ�Է���
			if ((nPosCheckSum == 52 || nPosCheckSum == 41 || 
				nPosCheckSum == 59 || nPosCheckSum == 48) && 
				(nAroundCheckSum & m_nMask[0]) == 0)
			{
				nNoCapCount++;
			}
			//���½�, ���½�Ϊ�Է���
			if ((nPosCheckSum == 32 || nPosCheckSum == 41 || 
				nPosCheckSum == 39 || nPosCheckSum == 48) && 
				(nAroundCheckSum & m_nMask[1]) == 0)
			{
				nNoCapCount++;
			}
			DEBUGASSERT(nNoCapCount <= 4);
			
			m_pcmAdvisorNoCapPreMoves[nPosCheckNum][nAroundCheckSum].nCount = nNoCapCount;
			if (nNoCapCount)
			{
				TRYNEW(m_pcmAdvisorNoCapPreMoves[nPosCheckNum][nAroundCheckSum].pMoveList = 
					new CHESSMOVE[nNoCapCount];);
				
#ifdef MEMCALC
				
				nAdvisorCapMovesSize += m_nBytes[nNoCapCount];
				
#endif
				
				nNoCapCount = 0;
				
				//���Ͻ�, ���Ͻ�Ϊ�Է���
				if ((nPosCheckSum == 50 || nPosCheckSum == 41 || 
					nPosCheckSum == 57 || nPosCheckSum == 48) && 
					(nAroundCheckSum & m_nMask[2]) == 0)
				{
					FILLCHESSMOVE(m_pcmAdvisorNoCapPreMoves[nPosCheckNum][nAroundCheckSum].pMoveList[nNoCapCount], 
						nPosCheckSum, nPosCheckSum - 9, BV_ADVISOR, NOCHESS, EAT);

					nNoCapCount++;
				}
				//���Ͻ�, ���Ͻ�Ϊ�Է���
				if ((nPosCheckSum == 30 || nPosCheckSum == 41 || 
					nPosCheckSum == 37 || nPosCheckSum == 48) && 
					(nAroundCheckSum & m_nMask[3]) == 0)
				{
					FILLCHESSMOVE(m_pcmAdvisorNoCapPreMoves[nPosCheckNum][nAroundCheckSum].pMoveList[nNoCapCount], 
						nPosCheckSum, nPosCheckSum + 11, BV_ADVISOR, NOCHESS, EAT);

					nNoCapCount++;
				}
				//���½�, ���½�Ϊ�Է���
				if ((nPosCheckSum == 52 || nPosCheckSum == 41 || 
					nPosCheckSum == 59 || nPosCheckSum == 48) && 
					(nAroundCheckSum & m_nMask[0]) == 0)
				{
					FILLCHESSMOVE(m_pcmAdvisorNoCapPreMoves[nPosCheckNum][nAroundCheckSum].pMoveList[nNoCapCount], 
						nPosCheckSum, nPosCheckSum - 11, BV_ADVISOR, NOCHESS, EAT);

					nNoCapCount++;
				}
				//���½�, ���½�Ϊ�Է���
				if ((nPosCheckSum == 32 || nPosCheckSum == 41 || 
					nPosCheckSum == 39 || nPosCheckSum == 48) && 
					(nAroundCheckSum & m_nMask[1]) == 0)
				{
					FILLCHESSMOVE(m_pcmAdvisorNoCapPreMoves[nPosCheckNum][nAroundCheckSum].pMoveList[nNoCapCount], 
						nPosCheckSum, nPosCheckSum + 9, BV_ADVISOR, NOCHESS, EAT);

					nNoCapCount++;
				}
			}
        }

		// �����߷�[2005/10/23 16:18:49]
		//ʿ��Χ�Է������У���
		for (nFoeAroundCheckSum = 0; nFoeAroundCheckSum < 16; nFoeAroundCheckSum++)
		{
			nCapCount = 0;
			
			//���Ͻ�, ���Ͻ�Ϊ�Է���
			if ((nPosCheckSum == 50 || nPosCheckSum == 41 || 
				nPosCheckSum == 57 || nPosCheckSum == 48) && 
				(nFoeAroundCheckSum & m_nMask[2]))
			{
				nCapCount++;
			}
			//���Ͻ�, ���Ͻ�Ϊ�Է���
			if ((nPosCheckSum == 30 || nPosCheckSum == 41 || 
				nPosCheckSum == 37 || nPosCheckSum == 48) && 
				(nFoeAroundCheckSum & m_nMask[3]))
			{
				nCapCount++;
			}
			//���½�, ���½�Ϊ�Է���
			if ((nPosCheckSum == 52 || nPosCheckSum == 41 || 
				nPosCheckSum == 59 || nPosCheckSum == 48) && 
				(nFoeAroundCheckSum & m_nMask[0]))
			{
				nCapCount++;
			}
			//���½�, ���½�Ϊ�Է���
			if ((nPosCheckSum == 32 || nPosCheckSum == 41 || 
				nPosCheckSum == 39 || nPosCheckSum == 48) && 
				(nFoeAroundCheckSum & m_nMask[1]))
			{
				nCapCount++;
			}
			DEBUGASSERT(nCapCount <= 4);
			
			m_pcmAdvisorCapPreMoves[nPosCheckNum][nFoeAroundCheckSum].nCount = nCapCount;
			if (nCapCount)
			{
				TRYNEW(m_pcmAdvisorCapPreMoves[nPosCheckNum][nFoeAroundCheckSum].pMoveList = new CHESSMOVE[nCapCount];);
				
#ifdef MEMCALC
				
				nAdvisorCapMovesSize += m_nBytes[nCapCount];
				
#endif
				
				nCapCount = 0;
				
				//���Ͻ�, ���Ͻ�Ϊ�Է���
				if ((nPosCheckSum == 50 || nPosCheckSum == 41 || 
					nPosCheckSum == 57 || nPosCheckSum == 48) && 
					(nFoeAroundCheckSum & m_nMask[2]))
				{
					FILLCHESSMOVE(m_pcmAdvisorCapPreMoves[nPosCheckNum][nFoeAroundCheckSum].pMoveList[nCapCount], 
						nPosCheckSum, nPosCheckSum - 9, BV_ADVISOR, NOCHESS, EAT);

					nCapCount++;
				}
				//���Ͻ�, ���Ͻ�Ϊ�Է���
				if ((nPosCheckSum == 30 || nPosCheckSum == 41 || 
					nPosCheckSum == 37 || nPosCheckSum == 48) && 
					(nFoeAroundCheckSum & m_nMask[3]))
				{
					FILLCHESSMOVE(m_pcmAdvisorCapPreMoves[nPosCheckNum][nFoeAroundCheckSum].pMoveList[nCapCount], 
						nPosCheckSum, nPosCheckSum + 11, BV_ADVISOR, NOCHESS, EAT);

					nCapCount++;
				}
				//���½�, ���½�Ϊ�Է���
				if ((nPosCheckSum == 52 || nPosCheckSum == 41 || 
					nPosCheckSum == 59 || nPosCheckSum == 48) && 
					(nFoeAroundCheckSum & m_nMask[0]))
				{
					FILLCHESSMOVE(m_pcmAdvisorCapPreMoves[nPosCheckNum][nFoeAroundCheckSum].pMoveList[nCapCount], 
						nPosCheckSum, nPosCheckSum - 11, BV_ADVISOR, NOCHESS, EAT);

					nCapCount++;
				}
				//���½�, ���½�Ϊ�Է���
				if ((nPosCheckSum == 32 || nPosCheckSum == 41 || 
					nPosCheckSum == 39 || nPosCheckSum == 48) && 
					(nFoeAroundCheckSum & m_nMask[1]))
				{
					FILLCHESSMOVE(m_pcmAdvisorCapPreMoves[nPosCheckNum][nFoeAroundCheckSum].pMoveList[nCapCount], 
						nPosCheckSum, nPosCheckSum + 9, BV_ADVISOR, NOCHESS, EAT);

					nCapCount++;
				}
			}
        }
    }

#ifdef MEMCALC

	cout << "ʿ�����߷��ڴ�:" << nAdvisorMovesSize * 1.0 / (1 << 20) << "M" << endl;
	cout << "ʿ���г����߷��ڴ�:" << nAdvisorCapMovesSize * 1.0 / (1 << 20) << "M" << endl;

#endif
}

/*******************************************************************************************************
������ : GenBishopPreMoves
��  �� : ����߷�Ԥ����
��  �� : ��
����ֵ : ��
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:38:52
*******************************************************************************************************/
FSVOID CSearchEngine::GenBishopPreMoves()
{
    FSINT i, j;
    FSINT nCount;
	FSINT nNoCapCount;
	FSINT nCapCount;
    FSINT nAroundCheckSum;
    FSINT nPosCheckNum;
    FSINT nPosCheckSum;
    FSINT nFoeAroundCheckSum;
    FSINT nBishopEyeCheckSum;
    
#ifdef MEMCALC

	FSINT nBishopMovesSize = 0;
	FSINT nBishopCapMovesSize = 0;

#endif

	//��λ��У�����
    for (nPosCheckNum = 0; nPosCheckNum < 14; nPosCheckNum++)
    {
		nPosCheckSum = m_nBishopPosCheckSum[nPosCheckNum];
		i = m_CGN2Pos[nPosCheckSum].x;
		j = m_CGN2Pos[nPosCheckSum].y;
		
		//����У���
		for (nBishopEyeCheckSum = 0; nBishopEyeCheckSum < 16; nBishopEyeCheckSum++)
        {
			//����Χ�Է�����У���
            for (nFoeAroundCheckSum = 0; nFoeAroundCheckSum < 16; nFoeAroundCheckSum++)
            {
				//����Χ����У���
				for (nAroundCheckSum = 0; nAroundCheckSum < 16; nAroundCheckSum++)
                {
                    nCount = 0;
                    
                    //���Ͻ�
					//�����ж����Ͻ����ӵ������Ƿ����
                    if ((IsInRBoard(nPosCheckSum) && IsInRBoard(nPosCheckSum - 18)) ||
					   (IsInBBoard(nPosCheckSum) && IsInBBoard(nPosCheckSum - 18)))
                    {
						if ((nBishopEyeCheckSum & m_nMask[2]) == 0)//������
						{
							if (nAroundCheckSum & m_nMask[2])//���Ͻ�����
							{
								if (nFoeAroundCheckSum & m_nMask[2])//���Ͻ�Ϊ�Է���
								{
									nCount++;
								}
							}
							else//���Ͻ�����
							{
								nCount++;
							}
						}
                    }
                    //���Ͻ�
					//�����ж����Ͻ����ӵ������Ƿ����
                    if ((IsInRBoard(nPosCheckSum) && IsInRBoard(nPosCheckSum + 22)) ||
					   (IsInBBoard(nPosCheckSum) && IsInBBoard(nPosCheckSum + 22)))
                    {
						if (!(nBishopEyeCheckSum & m_nMask[3]))//������
						{
							if (nAroundCheckSum & m_nMask[3])//���Ͻ�����
							{
								if (nFoeAroundCheckSum & m_nMask[3])//���Ͻ�Ϊ�Է���
								{
									nCount++;
								}
							}
							else
							{
								nCount++;
							}
						}
                    }
                    //���½�
					//�����ж����½����ӵ������Ƿ����
                    if ((IsInRBoard(nPosCheckSum) && IsInRBoard(nPosCheckSum - 22)) ||
					   (IsInBBoard(nPosCheckSum) && IsInBBoard(nPosCheckSum - 22)))
                    {
						if (!(nBishopEyeCheckSum & m_nMask[0]))//������
						{
							if (nAroundCheckSum & m_nMask[0])//���½�����
							{
								if (nFoeAroundCheckSum & m_nMask[0])//���½�Ϊ�Է���
								{
									nCount++;
								}
							}
							else
							{
								nCount++;
							}
						}
                    }
                    //���½�
					//�����ж����½����ӵ������Ƿ����
                    if ((IsInRBoard(nPosCheckSum) && IsInRBoard(nPosCheckSum + 18)) || 
					   (IsInBBoard(nPosCheckSum) && IsInBBoard(nPosCheckSum + 18)))
                    {
						if (!(nBishopEyeCheckSum & m_nMask[1]))//������
						{
							if (nAroundCheckSum & m_nMask[1])//���½�����
							{
								if (nFoeAroundCheckSum & m_nMask[1])//���½�Ϊ�Է���
								{
									nCount++;
								}
							}
							else
							{
								nCount++;
							}
						}
                    }
					DEBUGASSERT(nCount >= 0 && nCount <= 4);

					m_pcmBishopPreMoves[nPosCheckNum][nBishopEyeCheckSum][nAroundCheckSum][nFoeAroundCheckSum].nCount = nCount;
					if (nCount)
					{
						TRYNEW(m_pcmBishopPreMoves[nPosCheckNum][nBishopEyeCheckSum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList = new CHESSMOVE[nCount];);

#ifdef MEMCALC

						nBishopMovesSize += m_nBytes[nCount];

#endif

						nCount = 0;
						
						//���Ͻ�
						if ((IsInRBoard(nPosCheckSum) && IsInRBoard(nPosCheckSum - 18)) || 
						   (IsInBBoard(nPosCheckSum) && IsInBBoard(nPosCheckSum - 18)))
						{
							if (!(nBishopEyeCheckSum & m_nMask[2]))//������
							{
								if (nAroundCheckSum & m_nMask[2])//���Ͻ�����
								{
									if (nFoeAroundCheckSum & m_nMask[2])//���Ͻ�Ϊ�Է���
									{
										FILLCHESSMOVE(m_pcmBishopPreMoves[nPosCheckNum][nBishopEyeCheckSum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
											nPosCheckSum, nPosCheckSum - 18, BV_BISHOP, NOCHESS, EAT);

										nCount++;
									}
								}
								else
								{
									FILLCHESSMOVE(m_pcmBishopPreMoves[nPosCheckNum][nBishopEyeCheckSum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
											nPosCheckSum, nPosCheckSum - 18, BV_BISHOP, NOCHESS, NORMAL);

									nCount++;
								}
							}
						}

						//���Ͻ�
						if ((IsInRBoard(nPosCheckSum) && IsInRBoard(nPosCheckSum + 22)) ||
						  (IsInBBoard(nPosCheckSum) && IsInBBoard(nPosCheckSum + 22)))
						{
							if (!(nBishopEyeCheckSum & m_nMask[3]))//������
							{
								if (nAroundCheckSum & m_nMask[3])//���Ͻ�����
								{
									if (nFoeAroundCheckSum & m_nMask[3])//���Ͻ�Ϊ�Է���
									{
										FILLCHESSMOVE(m_pcmBishopPreMoves[nPosCheckNum][nBishopEyeCheckSum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
											nPosCheckSum, nPosCheckSum + 22, BV_BISHOP, NOCHESS, EAT);

										nCount++;
									}
								}
								else
								{
									FILLCHESSMOVE(m_pcmBishopPreMoves[nPosCheckNum][nBishopEyeCheckSum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
											nPosCheckSum, nPosCheckSum + 22, BV_BISHOP, NOCHESS, NORMAL);

									nCount++;
								}
							}
						}

						//���½�
						if ((IsInRBoard(nPosCheckSum) && IsInRBoard(nPosCheckSum - 22)) ||
						   (IsInBBoard(nPosCheckSum) && IsInBBoard(nPosCheckSum - 22)))
						{
							if (!(nBishopEyeCheckSum & m_nMask[0]))//������
							{
								if (nAroundCheckSum & m_nMask[0])//���½�����
								{
									if (nFoeAroundCheckSum & m_nMask[0])//���½�Ϊ�Է���
									{
										FILLCHESSMOVE(m_pcmBishopPreMoves[nPosCheckNum][nBishopEyeCheckSum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
											nPosCheckSum, nPosCheckSum - 22, BV_BISHOP, NOCHESS, EAT);

										nCount++;
									}
								}
								else
								{
									FILLCHESSMOVE(m_pcmBishopPreMoves[nPosCheckNum][nBishopEyeCheckSum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
											nPosCheckSum, nPosCheckSum - 22, BV_BISHOP, NOCHESS, NORMAL);

									nCount++;
								}
							}
						}
						
						//���½�
				        if ((IsInRBoard(nPosCheckSum) && IsInRBoard(nPosCheckSum + 18)) ||
						   (IsInBBoard(nPosCheckSum) && IsInBBoard(nPosCheckSum + 18)))
						{
							if (!(nBishopEyeCheckSum & m_nMask[1]))//������
							{
								if (nAroundCheckSum & m_nMask[1])//���½�����
								{
									if (nFoeAroundCheckSum & m_nMask[1])//���½�Ϊ�Է���
									{
										FILLCHESSMOVE(m_pcmBishopPreMoves[nPosCheckNum][nBishopEyeCheckSum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
											nPosCheckSum, nPosCheckSum + 18, BV_BISHOP, NOCHESS, EAT);

										nCount++;
									}
								}
								else
								{
									FILLCHESSMOVE(m_pcmBishopPreMoves[nPosCheckNum][nBishopEyeCheckSum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
											nPosCheckSum, nPosCheckSum + 18, BV_BISHOP, NOCHESS, NORMAL);

									nCount++;
								}
							}
						}
					}
                }
			}

			//�������߷�
			//����Χ�Է�����У���
            for (nAroundCheckSum = 0; nAroundCheckSum < 16; nAroundCheckSum++)
            {
				nNoCapCount = 0;
				
				//���Ͻ�
				//�����ж����Ͻ����ӵ������Ƿ����, ������, ���Ͻ�Ϊ�Է���
				if (((IsInRBoard(nPosCheckSum) && IsInRBoard(nPosCheckSum - 18)) ||
					(IsInBBoard(nPosCheckSum) && IsInBBoard(nPosCheckSum - 18))) && 
					((nBishopEyeCheckSum & m_nMask[2]) == 0) && 
					(nAroundCheckSum & m_nMask[2]) == 0)
				{
					nNoCapCount++;
				}
				//���Ͻ�
				//�����ж����Ͻ����ӵ������Ƿ����, ������, ���Ͻ�Ϊ�Է���
				if (((IsInRBoard(nPosCheckSum) && IsInRBoard(nPosCheckSum + 22)) ||
					(IsInBBoard(nPosCheckSum) && IsInBBoard(nPosCheckSum + 22))) && 
					(!(nBishopEyeCheckSum & m_nMask[3])) && 
					(nAroundCheckSum & m_nMask[3]) == 0)
				{
					nNoCapCount++;
				}
				//���½�
				//�����ж����½����ӵ������Ƿ����, ������, ���½�Ϊ�Է���
				if (((IsInRBoard(nPosCheckSum) && IsInRBoard(nPosCheckSum - 22)) ||
					(IsInBBoard(nPosCheckSum) && IsInBBoard(nPosCheckSum - 22))) && 
					(!(nBishopEyeCheckSum & m_nMask[0])) && 
					(nAroundCheckSum & m_nMask[0]) == 0)
				{
					nNoCapCount++;
				}
				//���½�
				//�����ж����½����ӵ������Ƿ����, ������, ���½�Ϊ�Է���
				if (((IsInRBoard(nPosCheckSum) && IsInRBoard(nPosCheckSum + 18)) || 
					(IsInBBoard(nPosCheckSum) && IsInBBoard(nPosCheckSum + 18))) && 
					(!(nBishopEyeCheckSum & m_nMask[1])) && 
					(nAroundCheckSum & m_nMask[1]) == 0)
				{
					nNoCapCount++;
				}
				DEBUGASSERT(nNoCapCount <= 4);
				
				m_pcmBishopNoCapPreMoves[nPosCheckNum][nBishopEyeCheckSum][nAroundCheckSum].nCount = 
					nNoCapCount;
				if (nNoCapCount)
				{
					TRYNEW(m_pcmBishopNoCapPreMoves[nPosCheckNum][nBishopEyeCheckSum][nAroundCheckSum].pMoveList = 
						new CHESSMOVE[nNoCapCount];);

#ifdef MEMCALC
					
					nBishopCapMovesSize += m_nBytes[nNoCapCount];
					
#endif
					
					nNoCapCount = 0;
					
					//���Ͻ�
					//�����ж����Ͻ����ӵ������Ƿ����, ������, ���Ͻ�Ϊ�Է���
					if (((IsInRBoard(nPosCheckSum) && IsInRBoard(nPosCheckSum - 18)) ||
						(IsInBBoard(nPosCheckSum) && IsInBBoard(nPosCheckSum - 18))) && 
						((nBishopEyeCheckSum & m_nMask[2]) == 0) && 
						(nAroundCheckSum & m_nMask[2]) == 0)
					{
						FILLCHESSMOVE(m_pcmBishopNoCapPreMoves[nPosCheckNum][nBishopEyeCheckSum][nAroundCheckSum].pMoveList[nNoCapCount], 
							nPosCheckSum, nPosCheckSum - 18, BV_BISHOP, NOCHESS, EAT);

						nNoCapCount++;
					}
					//���Ͻ�
					//�����ж����Ͻ����ӵ������Ƿ����, ������, ���Ͻ�Ϊ�Է���
					if (((IsInRBoard(nPosCheckSum) && IsInRBoard(nPosCheckSum + 22)) ||
						(IsInBBoard(nPosCheckSum) && IsInBBoard(nPosCheckSum + 22))) && 
						(!(nBishopEyeCheckSum & m_nMask[3])) && 
						(nAroundCheckSum & m_nMask[3]) == 0)
					{
						FILLCHESSMOVE(m_pcmBishopNoCapPreMoves[nPosCheckNum][nBishopEyeCheckSum][nAroundCheckSum].pMoveList[nNoCapCount], 
							nPosCheckSum, nPosCheckSum + 22, BV_BISHOP, NOCHESS, EAT);

						nNoCapCount++;
					}
					//���½�
					//�����ж����½����ӵ������Ƿ����, ������, ���½�Ϊ�Է���
					if (((IsInRBoard(nPosCheckSum) && IsInRBoard(nPosCheckSum - 22)) ||
						(IsInBBoard(nPosCheckSum) && IsInBBoard(nPosCheckSum - 22))) && 
						(!(nBishopEyeCheckSum & m_nMask[0])) && 
						(nAroundCheckSum & m_nMask[0]) == 0)
					{
						FILLCHESSMOVE(m_pcmBishopNoCapPreMoves[nPosCheckNum][nBishopEyeCheckSum][nAroundCheckSum].pMoveList[nNoCapCount], 
							nPosCheckSum, nPosCheckSum - 22, BV_BISHOP, NOCHESS, EAT);

						nNoCapCount++;
					}
					//���½�
					//�����ж����½����ӵ������Ƿ����, ������, ���½�Ϊ�Է���
					if (((IsInRBoard(nPosCheckSum) && IsInRBoard(nPosCheckSum + 18)) || 
						(IsInBBoard(nPosCheckSum) && IsInBBoard(nPosCheckSum + 18))) && 
						(!(nBishopEyeCheckSum & m_nMask[1])) && 
						(nAroundCheckSum & m_nMask[1]) == 0)
					{
						FILLCHESSMOVE(m_pcmBishopNoCapPreMoves[nPosCheckNum][nBishopEyeCheckSum][nAroundCheckSum].pMoveList[nNoCapCount], 
							nPosCheckSum, nPosCheckSum + 18, BV_BISHOP, NOCHESS, EAT);

						nNoCapCount++;
					}
                }
            }

			//�����߷�
			//����Χ�Է�����У���
            for (nFoeAroundCheckSum = 0; nFoeAroundCheckSum < 16; nFoeAroundCheckSum++)
            {
				nCapCount = 0;
				
				//���Ͻ�
				//�����ж����Ͻ����ӵ������Ƿ����, ������, ���Ͻ�Ϊ�Է���
				if (((IsInRBoard(nPosCheckSum) && IsInRBoard(nPosCheckSum - 18)) ||
					(IsInBBoard(nPosCheckSum) && IsInBBoard(nPosCheckSum - 18))) && 
					((nBishopEyeCheckSum & m_nMask[2]) == 0) && 
					(nFoeAroundCheckSum & m_nMask[2]))
				{
					nCapCount++;
				}
				//���Ͻ�
				//�����ж����Ͻ����ӵ������Ƿ����, ������, ���Ͻ�Ϊ�Է���
				if (((IsInRBoard(nPosCheckSum) && IsInRBoard(nPosCheckSum + 22)) ||
					(IsInBBoard(nPosCheckSum) && IsInBBoard(nPosCheckSum + 22))) && 
					(!(nBishopEyeCheckSum & m_nMask[3])) && 
					(nFoeAroundCheckSum & m_nMask[3]))
				{
					nCapCount++;
				}
				//���½�
				//�����ж����½����ӵ������Ƿ����, ������, ���½�Ϊ�Է���
				if (((IsInRBoard(nPosCheckSum) && IsInRBoard(nPosCheckSum - 22)) ||
					(IsInBBoard(nPosCheckSum) && IsInBBoard(nPosCheckSum - 22))) && 
					(!(nBishopEyeCheckSum & m_nMask[0])) && 
					(nFoeAroundCheckSum & m_nMask[0]))
				{
					nCapCount++;
				}
				//���½�
				//�����ж����½����ӵ������Ƿ����, ������, ���½�Ϊ�Է���
				if (((IsInRBoard(nPosCheckSum) && IsInRBoard(nPosCheckSum + 18)) || 
					(IsInBBoard(nPosCheckSum) && IsInBBoard(nPosCheckSum + 18))) && 
					(!(nBishopEyeCheckSum & m_nMask[1])) && 
					(nFoeAroundCheckSum & m_nMask[1]))
				{
					nCapCount++;
				}
				DEBUGASSERT(nCapCount <= 4);
				
				m_pcmBishopCapPreMoves[nPosCheckNum][nBishopEyeCheckSum][nFoeAroundCheckSum].nCount = 
					nCapCount;
				if (nCapCount)
				{
					TRYNEW(m_pcmBishopCapPreMoves[nPosCheckNum][nBishopEyeCheckSum][nFoeAroundCheckSum].pMoveList = 
						new CHESSMOVE[nCapCount];);

#ifdef MEMCALC
					
					nBishopCapMovesSize += m_nBytes[nCapCount];
					
#endif
					
					nCapCount = 0;
					
					//���Ͻ�
					//�����ж����Ͻ����ӵ������Ƿ����, ������, ���Ͻ�Ϊ�Է���
					if (((IsInRBoard(nPosCheckSum) && IsInRBoard(nPosCheckSum - 18)) ||
						(IsInBBoard(nPosCheckSum) && IsInBBoard(nPosCheckSum - 18))) && 
						((nBishopEyeCheckSum & m_nMask[2]) == 0) && 
						(nFoeAroundCheckSum & m_nMask[2]))
					{
						FILLCHESSMOVE(m_pcmBishopCapPreMoves[nPosCheckNum][nBishopEyeCheckSum][nFoeAroundCheckSum].pMoveList[nCapCount], 
							nPosCheckSum, nPosCheckSum - 18, BV_BISHOP, NOCHESS, EAT);

						nCapCount++;
					}
					//���Ͻ�
					//�����ж����Ͻ����ӵ������Ƿ����, ������, ���Ͻ�Ϊ�Է���
					if (((IsInRBoard(nPosCheckSum) && IsInRBoard(nPosCheckSum + 22)) ||
						(IsInBBoard(nPosCheckSum) && IsInBBoard(nPosCheckSum + 22))) && 
						(!(nBishopEyeCheckSum & m_nMask[3])) && 
						(nFoeAroundCheckSum & m_nMask[3]))
					{
						FILLCHESSMOVE(m_pcmBishopCapPreMoves[nPosCheckNum][nBishopEyeCheckSum][nFoeAroundCheckSum].pMoveList[nCapCount], 
							nPosCheckSum, nPosCheckSum + 22, BV_BISHOP, NOCHESS, EAT);

						nCapCount++;
					}
					//���½�
					//�����ж����½����ӵ������Ƿ����, ������, ���½�Ϊ�Է���
					if (((IsInRBoard(nPosCheckSum) && IsInRBoard(nPosCheckSum - 22)) ||
						(IsInBBoard(nPosCheckSum) && IsInBBoard(nPosCheckSum - 22))) && 
						(!(nBishopEyeCheckSum & m_nMask[0])) && 
						(nFoeAroundCheckSum & m_nMask[0]))
					{
						FILLCHESSMOVE(m_pcmBishopCapPreMoves[nPosCheckNum][nBishopEyeCheckSum][nFoeAroundCheckSum].pMoveList[nCapCount], 
							nPosCheckSum, nPosCheckSum - 22, BV_BISHOP, NOCHESS, EAT);

						nCapCount++;
					}
					//���½�
					//�����ж����½����ӵ������Ƿ����, ������, ���½�Ϊ�Է���
					if (((IsInRBoard(nPosCheckSum) && IsInRBoard(nPosCheckSum + 18)) || 
						(IsInBBoard(nPosCheckSum) && IsInBBoard(nPosCheckSum + 18))) && 
						(!(nBishopEyeCheckSum & m_nMask[1])) && 
						(nFoeAroundCheckSum & m_nMask[1]))
					{
						FILLCHESSMOVE(m_pcmBishopCapPreMoves[nPosCheckNum][nBishopEyeCheckSum][nFoeAroundCheckSum].pMoveList[nCapCount], 
							nPosCheckSum, nPosCheckSum + 18, BV_BISHOP, NOCHESS, EAT);

						nCapCount++;
					}
                }
            }
        }
    }

#ifdef MEMCALC

	cout << "�������߷��ڴ�:" << B2M(nBishopMovesSize) << "M" << endl;
	cout << "�����г����߷��ڴ�:" << B2M(nBishopCapMovesSize) << "M" << endl;

#endif
}

/*******************************************************************************************************
������ : GenPawnPreMoves
��  �� : �����߷�Ԥ����
��  �� : ��
����ֵ : ��
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:39:22
*******************************************************************************************************/
FSVOID CSearchEngine::GenPawnPreMoves()
{
	FSINT i, j;
    FSINT nCount;
	FSINT nNoCapCount;
	FSINT nCapCount;
    FSINT nAroundCheckSum;
	FSINT nPosCheckNum;			//��λ��У�����
	FSINT nPosCheckSum;			//��λ��У���
	FSINT nFoeAroundCheckSum;	//����Χ���У���

#ifdef MEMCALC

	FSINT nPawnMovesSize = 0;
	FSINT nPawnCapMovesSize = 0;

#endif

	//��λ��У�����
	for (nPosCheckNum = 0; nPosCheckNum < 55; nPosCheckNum++)
	{
		//����Χ�Է�����У���
		for (nFoeAroundCheckSum = 0; nFoeAroundCheckSum < 8; nFoeAroundCheckSum++)
		{
			//����Χ����У���
			for (nAroundCheckSum = 0; nAroundCheckSum < 8; nAroundCheckSum++)
			{
				//����
				nPosCheckSum = m_nPawnPosCheckSum[BLACK][nPosCheckNum];
				
				i = m_CGN2Pos[nPosCheckSum].x;
				j = m_CGN2Pos[nPosCheckSum].y;
				
				nCount = 0;
				
				//��
				if (j <= 4 && i >= 1)
				{
					if (nAroundCheckSum & m_nMask[0])//�������
					{
						if (nFoeAroundCheckSum & m_nMask[0])//���Ϊ�Է���
						{
							nCount++;
						}
					}
					else
					{
						nCount++;
					}
				}
				
				//��
				if (j <= 4 && i <= 7)
				{
					if (nAroundCheckSum & m_nMask[1])//�ұ�����
					{
						if (nFoeAroundCheckSum & m_nMask[1])//�ұ�Ϊ�Է���
						{
							nCount++;
						}
					}
					else
					{
						nCount++;
					}
				}
				
				//��
				if (j>=1)
				{
					if (nAroundCheckSum & m_nMask[2])//�±�����
					{
						if (nFoeAroundCheckSum & m_nMask[2])//�±�Ϊ�Է���
						{
							nCount++;
						}
					}
					else
					{
						nCount++;
					}
				}
				DEBUGASSERT(nCount >= 0 && nCount <= 3);
				
				m_PawnPreMoves[BLACK][nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].nCount = nCount;
				if (nCount)
				{
					TRYNEW(m_PawnPreMoves[BLACK][nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList = new CHESSMOVE[nCount];);

#ifdef MEMCALC
					
					nPawnMovesSize += m_nBytes[nCount];
					
#endif
					nCount = 0;
					
					//��
					if (j <= 4 && i >= 1)
					{
						if (nAroundCheckSum & m_nMask[0])//�������
						{
							if (nFoeAroundCheckSum & m_nMask[0])//���Ϊ�Է���
							{
								FILLCHESSMOVE(m_PawnPreMoves[BLACK][nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
									nPosCheckSum, nPosCheckSum - 10, BV_PAWN_OPEN_GAME, NOCHESS, EAT);

								nCount++;
							}
						}
						else
						{
							FILLCHESSMOVE(m_PawnPreMoves[BLACK][nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
								nPosCheckSum, nPosCheckSum - 10, BV_PAWN_OPEN_GAME, NOCHESS, NORMAL);

							nCount++;
						}
					}
					
					//��
					if (j <= 4 && i <= 7)
					{
						if (nAroundCheckSum & m_nMask[1])//�ұ�����
						{
							if (nFoeAroundCheckSum & m_nMask[1])//�ұ�Ϊ�Է���
							{
								FILLCHESSMOVE(m_PawnPreMoves[BLACK][nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
									nPosCheckSum, nPosCheckSum + 10, BV_PAWN_OPEN_GAME, NOCHESS, EAT);

								nCount++;
							}
						}
						else
						{
							FILLCHESSMOVE(m_PawnPreMoves[BLACK][nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
								nPosCheckSum, nPosCheckSum + 10, BV_PAWN_OPEN_GAME, NOCHESS, NORMAL);

							nCount++;
						}
					}
					
					//��
					if (j>=1)
					{
						if (nAroundCheckSum & m_nMask[2])//�±�����
						{
							if (nFoeAroundCheckSum & m_nMask[2])//�±�Ϊ�Է���
							{
								FILLCHESSMOVE(m_PawnPreMoves[BLACK][nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
									nPosCheckSum, nPosCheckSum - 1, BV_PAWN_OPEN_GAME, NOCHESS, EAT);

								nCount++;
							}
						}
						else
						{
							FILLCHESSMOVE(m_PawnPreMoves[BLACK][nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
								nPosCheckSum, nPosCheckSum - 1, BV_PAWN_OPEN_GAME, NOCHESS, NORMAL);

							nCount++;
						}
					}
				}
				
				//���
				nPosCheckSum = m_nPawnPosCheckSum[RED][nPosCheckNum];
				
				i = m_CGN2Pos[nPosCheckSum].x;
				j = m_CGN2Pos[nPosCheckSum].y;
				
				nCount = 0;
				
				//��
				if (j>=5 && i>=1)
				{
					if (nAroundCheckSum & m_nMask[0])//�������
					{
						if (nFoeAroundCheckSum & m_nMask[0])//���Ϊ�Է���
						{
							nCount++;
						}
					}
					else
					{
						nCount++;
					}
				}
				
				//��
				if (j>=5 && i<=7)
				{
					if (nAroundCheckSum & m_nMask[1])//�ұ�����
					{
						if (nFoeAroundCheckSum & m_nMask[1])//�ұ�Ϊ�Է���
						{
							nCount++;
						}
					}
					else
					{
						nCount++;
					}
				}
				
				//��
				if (j <= 8)
				{
					if (nAroundCheckSum & m_nMask[2])//�±�����
					{
						if (nFoeAroundCheckSum & m_nMask[2])//�±�Ϊ�Է���
						{
							nCount++;
						}
					}
					else
					{
						nCount++;
					}
				}
				DEBUGASSERT(nCount >= 0 && nCount <= 3);
				
				m_PawnPreMoves[RED][nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].nCount = nCount;
				if (nCount)
				{
					TRYNEW(m_PawnPreMoves[RED][nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList = new CHESSMOVE[nCount];);
					
#ifdef MEMCALC
					
					nPawnMovesSize += m_nBytes[nCount];
					
#endif
					
					nCount = 0;
					
					//��
					if (j>=5 && i>=1)
					{
						if (nAroundCheckSum & m_nMask[0])//�������
						{
							if (nFoeAroundCheckSum & m_nMask[0])//���Ϊ�Է���
							{
								FILLCHESSMOVE(m_PawnPreMoves[RED][nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
									nPosCheckSum, nPosCheckSum - 10, BV_PAWN_OPEN_GAME, NOCHESS, EAT);

								nCount++;
							}
						}
						else
						{
							FILLCHESSMOVE(m_PawnPreMoves[RED][nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
								nPosCheckSum, nPosCheckSum - 10, BV_PAWN_OPEN_GAME, NOCHESS, NORMAL);

							nCount++;
						}
					}
					
					//��
					if (j>=5 && i<=7)
					{
						if (nAroundCheckSum & m_nMask[1])//�ұ�����
						{
							if (nFoeAroundCheckSum & m_nMask[1])//�ұ�Ϊ�Է���
							{
								FILLCHESSMOVE(m_PawnPreMoves[RED][nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
									nPosCheckSum, nPosCheckSum + 10, BV_PAWN_OPEN_GAME, NOCHESS, EAT);

								nCount++;
							}
						}
						else
						{
							FILLCHESSMOVE(m_PawnPreMoves[RED][nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
								nPosCheckSum, nPosCheckSum + 10, BV_PAWN_OPEN_GAME, NOCHESS, NORMAL);

							nCount++;
						}
					}
					
					//��
					if (j <= 8)
					{
						if (nAroundCheckSum & m_nMask[2])//�ϱ�����
						{
							if (nFoeAroundCheckSum & m_nMask[2])//�ϱ�Ϊ�Է���
							{
								FILLCHESSMOVE(m_PawnPreMoves[RED][nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
									nPosCheckSum, nPosCheckSum + 1, BV_PAWN_OPEN_GAME, NOCHESS, EAT);

								nCount++;
							}
						}
						else
						{
							FILLCHESSMOVE(m_PawnPreMoves[RED][nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].pMoveList[nCount], 
									nPosCheckSum, nPosCheckSum + 1, BV_PAWN_OPEN_GAME, NOCHESS, NORMAL);

							nCount++;
						}
					}
				}
			}
		}

		//�������߷�
		//����Χ�Է�����У���
		for (nAroundCheckSum = 0; nAroundCheckSum < 8; nAroundCheckSum++)
		{
			//����
			nPosCheckSum = m_nPawnPosCheckSum[BLACK][nPosCheckNum];
			
			i = m_CGN2Pos[nPosCheckSum].x;
			j = m_CGN2Pos[nPosCheckSum].y;
			
			nNoCapCount = 0;
			
			//��, ���Ϊ�Է���
			if ((j <= 4 && i >= 1) && (nAroundCheckSum & m_nMask[0]) == 0)
			{
				nNoCapCount++;
			}
			
			//��, �ұ�Ϊ�Է���
			if ((j <= 4 && i <= 7) && (nAroundCheckSum & m_nMask[1]) == 0)
			{
				nNoCapCount++;
			}
			
			//��, �±�Ϊ�Է���
			if ((j>=1) && (nAroundCheckSum & m_nMask[2]) == 0)
			{
				nNoCapCount++;
			}
			DEBUGASSERT(nNoCapCount <= 3);
			
			m_PawnNoCapPreMoves[BLACK][nPosCheckNum][nAroundCheckSum].nCount = nNoCapCount;
			if (nNoCapCount)
			{
				TRYNEW(m_PawnNoCapPreMoves[BLACK][nPosCheckNum][nAroundCheckSum].pMoveList = new CHESSMOVE[nNoCapCount];);
				
#ifdef MEMCALC
				
				nPawnCapMovesSize += m_nBytes[nNoCapCount];
				
#endif
				nNoCapCount = 0;
				
				//��, ���Ϊ�Է���
				if ((j <= 4 && i >= 1) && (nAroundCheckSum & m_nMask[0]) == 0)
				{
					FILLCHESSMOVE(m_PawnNoCapPreMoves[BLACK][nPosCheckNum][nAroundCheckSum].pMoveList[nNoCapCount], 
						nPosCheckSum, nPosCheckSum - 10, BV_PAWN_OPEN_GAME, NOCHESS, EAT);

					nNoCapCount++;
				}
				
				//��, �ұ�Ϊ�Է���
				if ((j <= 4 && i <= 7) && (nAroundCheckSum & m_nMask[1]) == 0)
				{
					FILLCHESSMOVE(m_PawnNoCapPreMoves[BLACK][nPosCheckNum][nAroundCheckSum].pMoveList[nNoCapCount], 
						nPosCheckSum, nPosCheckSum + 10, BV_PAWN_OPEN_GAME, NOCHESS, EAT);

					nNoCapCount++;
				}
				
				//��, �±�Ϊ�Է���
				if ((j>=1) && (nAroundCheckSum & m_nMask[2]) == 0)
				{
					FILLCHESSMOVE(m_PawnNoCapPreMoves[BLACK][nPosCheckNum][nAroundCheckSum].pMoveList[nNoCapCount], 
						nPosCheckSum, nPosCheckSum - 1, BV_PAWN_OPEN_GAME, NOCHESS, EAT);

					nNoCapCount++;
				}
			}
			
			//���
			nPosCheckSum = m_nPawnPosCheckSum[RED][nPosCheckNum];
			
			i = m_CGN2Pos[nPosCheckSum].x;
			j = m_CGN2Pos[nPosCheckSum].y;
			
			nNoCapCount = 0;
			
			//��, ���Ϊ�Է���
			if ((j>=5 && i>=1) && (nAroundCheckSum & m_nMask[0]) == 0)
			{
				nNoCapCount++;
			}
			
			//��, �ұ�Ϊ�Է���
			if ((j>=5 && i<=7) && (nAroundCheckSum & m_nMask[1]) == 0)
			{
				nNoCapCount++;
			}
			
			//��, �±�Ϊ�Է���
			if ((j <= 8) && (nAroundCheckSum & m_nMask[2]) == 0)
			{
				nNoCapCount++;
			}
			DEBUGASSERT(nNoCapCount <= 3);
			
			m_PawnNoCapPreMoves[RED][nPosCheckNum][nAroundCheckSum].nCount = nNoCapCount;
			if (nNoCapCount)
			{
				TRYNEW(m_PawnNoCapPreMoves[RED][nPosCheckNum][nAroundCheckSum].pMoveList = new CHESSMOVE[nNoCapCount];);
				
#ifdef MEMCALC
				
				nPawnMovesSize += m_nBytes[nNoCapCount];
				
#endif
				
				nNoCapCount = 0;
				
				//��, ���Ϊ�Է���
				if ((j>=5 && i>=1) && (nAroundCheckSum & m_nMask[0]) == 0)
				{
					FILLCHESSMOVE(m_PawnNoCapPreMoves[RED][nPosCheckNum][nAroundCheckSum].pMoveList[nNoCapCount], 
						nPosCheckSum, nPosCheckSum - 10, BV_PAWN_OPEN_GAME, NOCHESS, EAT);

					nNoCapCount++;
				}
				
				//��, �ұ�Ϊ�Է���
				if ((j>=5 && i<=7) && (nAroundCheckSum & m_nMask[1]) == 0)
				{
					FILLCHESSMOVE(m_PawnNoCapPreMoves[RED][nPosCheckNum][nAroundCheckSum].pMoveList[nNoCapCount], 
						nPosCheckSum, nPosCheckSum + 10, BV_PAWN_OPEN_GAME, NOCHESS, EAT);

					nNoCapCount++;
				}
				
				//��, �±�Ϊ�Է���
				if ((j <= 8) && (nAroundCheckSum & m_nMask[2]) == 0)
				{
					FILLCHESSMOVE(m_PawnNoCapPreMoves[RED][nPosCheckNum][nAroundCheckSum].pMoveList[nNoCapCount], 
						nPosCheckSum, nPosCheckSum + 1, BV_PAWN_OPEN_GAME, NOCHESS, EAT);

					nNoCapCount++;
				}
			}
		}

		//�����߷�
		//����Χ�Է�����У���
		for (nFoeAroundCheckSum = 0; nFoeAroundCheckSum < 8; nFoeAroundCheckSum++)
		{
			//����
			nPosCheckSum = m_nPawnPosCheckSum[BLACK][nPosCheckNum];
			
			i = m_CGN2Pos[nPosCheckSum].x;
			j = m_CGN2Pos[nPosCheckSum].y;
			
			nCapCount = 0;
			
			//��, ���Ϊ�Է���
			if ((j <= 4 && i >= 1) && (nFoeAroundCheckSum & m_nMask[0]))
			{
				nCapCount++;
			}
			
			//��, �ұ�Ϊ�Է���
			if ((j <= 4 && i <= 7) && (nFoeAroundCheckSum & m_nMask[1]))
			{
				nCapCount++;
			}
			
			//��, �±�Ϊ�Է���
			if ((j>=1) && (nFoeAroundCheckSum & m_nMask[2]))
			{
				nCapCount++;
			}
			DEBUGASSERT(nCapCount <= 3);
			
			m_PawnCapPreMoves[BLACK][nPosCheckNum][nFoeAroundCheckSum].nCount = nCapCount;
			if (nCapCount)
			{
				TRYNEW(m_PawnCapPreMoves[BLACK][nPosCheckNum][nFoeAroundCheckSum].pMoveList = new CHESSMOVE[nCapCount];);
				
#ifdef MEMCALC
				
				nPawnCapMovesSize += m_nBytes[nCapCount];
				
#endif
				nCapCount = 0;
				
				//��, ���Ϊ�Է���
				if ((j <= 4 && i >= 1) && (nFoeAroundCheckSum & m_nMask[0]))
				{
					FILLCHESSMOVE(m_PawnCapPreMoves[BLACK][nPosCheckNum][nFoeAroundCheckSum].pMoveList[nCapCount], 
						nPosCheckSum, nPosCheckSum - 10, BV_PAWN_OPEN_GAME, NOCHESS, EAT);

					nCapCount++;
				}
				
				//��, �ұ�Ϊ�Է���
				if ((j <= 4 && i <= 7) && (nFoeAroundCheckSum & m_nMask[1]))
				{
					FILLCHESSMOVE(m_PawnCapPreMoves[BLACK][nPosCheckNum][nFoeAroundCheckSum].pMoveList[nCapCount], 
						nPosCheckSum, nPosCheckSum + 10, BV_PAWN_OPEN_GAME, NOCHESS, EAT);

					nCapCount++;
				}
				
				//��, �±�Ϊ�Է���
				if ((j>=1) && (nFoeAroundCheckSum & m_nMask[2]))
				{
					FILLCHESSMOVE(m_PawnCapPreMoves[BLACK][nPosCheckNum][nFoeAroundCheckSum].pMoveList[nCapCount], 
						nPosCheckSum, nPosCheckSum - 1, BV_PAWN_OPEN_GAME, NOCHESS, EAT);

					nCapCount++;
				}
			}
			
			//���
			nPosCheckSum = m_nPawnPosCheckSum[RED][nPosCheckNum];
			
			i = m_CGN2Pos[nPosCheckSum].x;
			j = m_CGN2Pos[nPosCheckSum].y;
			
			nCapCount = 0;
			
			//��, ���Ϊ�Է���
			if ((j>=5 && i>=1) && (nFoeAroundCheckSum & m_nMask[0]))
			{
				nCapCount++;
			}
			
			//��, �ұ�Ϊ�Է���
			if ((j>=5 && i<=7) && (nFoeAroundCheckSum & m_nMask[1]))
			{
				nCapCount++;
			}
			
			//��, �±�Ϊ�Է���
			if ((j <= 8) && (nFoeAroundCheckSum & m_nMask[2]))
			{
				nCapCount++;
			}
			DEBUGASSERT(nCapCount <= 3);
			
			m_PawnCapPreMoves[RED][nPosCheckNum][nFoeAroundCheckSum].nCount = nCapCount;
			if (nCapCount)
			{
				TRYNEW(m_PawnCapPreMoves[RED][nPosCheckNum][nFoeAroundCheckSum].pMoveList = new CHESSMOVE[nCapCount];);
				
#ifdef MEMCALC
				
				nPawnMovesSize += m_nBytes[nCapCount];
				
#endif
				
				nCapCount = 0;
				
				//��, ���Ϊ�Է���
				if ((j>=5 && i>=1) && (nFoeAroundCheckSum & m_nMask[0]))
				{
					FILLCHESSMOVE(m_PawnCapPreMoves[RED][nPosCheckNum][nFoeAroundCheckSum].pMoveList[nCapCount], 
						nPosCheckSum, nPosCheckSum - 10, BV_PAWN_OPEN_GAME, NOCHESS, EAT);

					nCapCount++;
				}
				
				//��, �ұ�Ϊ�Է���
				if ((j>=5 && i<=7) && (nFoeAroundCheckSum & m_nMask[1]))
				{
					FILLCHESSMOVE(m_PawnCapPreMoves[RED][nPosCheckNum][nFoeAroundCheckSum].pMoveList[nCapCount], 
						nPosCheckSum, nPosCheckSum + 10, BV_PAWN_OPEN_GAME, NOCHESS, EAT);

					nCapCount++;
				}
				
				//��, �±�Ϊ�Է���
				if ((j <= 8) && (nFoeAroundCheckSum & m_nMask[2]))
				{
					FILLCHESSMOVE(m_PawnCapPreMoves[RED][nPosCheckNum][nFoeAroundCheckSum].pMoveList[nCapCount], 
						nPosCheckSum, nPosCheckSum + 1, BV_PAWN_OPEN_GAME, NOCHESS, EAT);

					nCapCount++;
				}
			}
		}
	}

#ifdef MEMCALC

	cout << "�������߷��ڴ�:" << nPawnMovesSize * 1.0 / (1 << 20) << "M" << endl;
	cout << "�����г����߷��ڴ�:" << nPawnCapMovesSize * 1.0 / (1 << 20) << "M" << endl;

#endif
}

/*******************************************************************************************************
������ : LoadKnightPreMove
��  �� : 
��  �� : ��
����ֵ : ����Ϊ FSBOOL
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��10��21�� 22:28:37
*******************************************************************************************************/
/*FSBOOL CSearchEngine::LoadKnightPreMove()
{
#ifdef CFILE
	FILE *fp = fopen(KNIGHTPREMOVESFILE, "rb");

	if (!fp)
	{
		return FALSE;
	}
#else
	// �ļ�����
	ifstream ifs(KNIGHTPREMOVESFILE);

	if (!ifs)
	{
		return FALSE;
	}
#endif

	FSINT p, i, j;
	FSINT nCount;
	FSINT nSizeOfFRINT = sizeof(FSINT);

	// ���ز������߷�[2005/10/21 21:53:40]
	for (p = 0; p < 90; p++)
	{
		for (i = 0; i < 256; i++)
		{
			if (m_bIsKnightInvalidArroundCheckSum[p][i] == FALSE)
			{
				continue;
			}

			for (j = 0; j < 16; j++)
			{
				if (m_bIsKnightInvalidLegCheckSum[p][i][j] == FALSE)
				{
					continue;
				}

			#ifdef CFILE
				fread(&nCount, nSizeOfFRINT, 1, fp);
			#else
				ifs.read((char*)&nCount, nSizeOfFRINT);
			#endif

				if (nCount < 0)
				{
					return FALSE;
				}

				m_pcmKnightNoCapPreMoves[p][i][j].nCount = nCount;
				if (nCount == 0)
				{
					continue;
				}

				TRYNEW(m_pcmKnightNoCapPreMoves[p][i][j].pMoveList = new CHESSMOVE[nCount];);

			#ifdef CFILE
				fread(m_pcmKnightNoCapPreMoves[p][i][j].pMoveList, sizeof(FSCHAR), m_nBytes[nCount], fp);
			#else
				ifs.read((FSCHAR*)m_pcmKnightNoCapPreMoves[p][i][j].pMoveList, m_nBytes[nCount]);
			#endif
			}
		}
	}

	// ���س����߷�[2005/10/21 21:53:40]
	for (p = 0; p < 90; p++)
	{
		for (i = 0; i < 256; i++)
		{
			if (m_bIsKnightInvalidArroundCheckSum[p][i] == FALSE)
			{
				continue;
			}

			for (j = 0; j < 16; j++)
			{
				if (m_bIsKnightInvalidLegCheckSum[p][i][j] == FALSE)
				{
					continue;
				}

			#ifdef CFILE
				fread(&nCount, nSizeOfFRINT, 1, fp);
			#else
				ifs.read((char*)&nCount, nSizeOfFRINT);
			#endif

				if (nCount < 0)
				{
					return FALSE;
				}

				m_pcmKnightCapPreMoves[p][i][j].nCount = nCount;
				if (nCount == 0)
				{
					continue;
				}

				TRYNEW(m_pcmKnightCapPreMoves[p][i][j].pMoveList = new CHESSMOVE[nCount];);

			#ifdef CFILE
				fread(m_pcmKnightCapPreMoves[p][i][j].pMoveList, sizeof(FSCHAR), m_nBytes[nCount], fp);
			#else
				ifs.read((FSCHAR*)m_pcmKnightCapPreMoves[p][i][j].pMoveList, m_nBytes[nCount]);
			#endif
			}
		}
	}

	return TRUE;
}*/

/*******************************************************************************************************
������ : SaveKnightPreMoves
��  �� : 
��  �� : ��
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��10��21�� 22:33:06
*******************************************************************************************************/
/*FSVOID CSearchEngine::SaveKnightPreMoves()
{
	//�ļ����� [2005/10/21 21:53:35]
	if (GetFileAttributes(KNIGHTPREMOVESFILE) != -1)
	{
		return;
	}

#ifdef CFILE
	FILE *fp = fopen(KNIGHTPREMOVESFILE, "wb");

	if (!fp)
	{
		return;
	}
#else
	ofstream ofs(KNIGHTPREMOVESFILE);

	if (!ofs)
	{
		return;
	}
#endif
	
	FSINT p, i, j;
	FSINT nCount;
	FSINT nSizeOfFRINT = sizeof(FSINT);

	// ���治�����߷�[2005/10/21 21:53:40]
	for (p = 0; p < 90; p++)
	{
		for (i = 0; i < 256; i++)
		{
			if (m_bIsKnightInvalidArroundCheckSum[p][i] == FALSE)
			{
				continue;
			}
			
			for (j = 0; j < 16; j++)
			{
				if (m_bIsKnightInvalidLegCheckSum[p][i][j] == FALSE)
				{
					continue;
				}
				
				nCount = m_pcmKnightNoCapPreMoves[p][i][j].nCount;
				DEBUGASSERT(nCount >= 0 && nCount <= MAXKNIGHTNOCAPMOVECNT);

			#ifdef CFILE
				fwrite(&nCount, nSizeOfFRINT, 1, fp);
			#else
				ofs.write((char*)&nCount, nSizeOfFRINT);
			#endif

				if (nCount == 0)
				{
					continue;
				}

			#ifdef CFILE
				fwrite(m_pcmKnightNoCapPreMoves[p][i][j].pMoveList, sizeof(FSCHAR), m_nBytes[nCount], fp);
			#else
				ofs.write((FSCHAR*)m_pcmKnightNoCapPreMoves[p][i][j].pMoveList, m_nBytes[nCount]);
			#endif
			}
		}
	}

	// ��������߷�[2005/10/21 21:53:40]
	for (p = 0; p < 90; p++)
	{
		for (i = 0; i < 256; i++)
		{
			if (m_bIsKnightInvalidArroundCheckSum[p][i] == FALSE)
			{
				continue;
			}

			for (j = 0; j < 16; j++)
			{
				if (m_bIsKnightInvalidLegCheckSum[p][i][j] == FALSE)
				{
					continue;
				}
				
				nCount = m_pcmKnightNoCapPreMoves[p][i][j].nCount;
				DEBUGASSERT(nCount >= 0 && nCount <= MAXKNIGHTCAPMOVECNT);
				
			#ifdef CFILE
				fwrite(&nCount, nSizeOfFRINT, 1, fp);
			#else
				ofs.write((FSCHAR*)&nCount, nSizeOfFRINT);
			#endif

				if (nCount == 0)
				{
					continue;
				}

			#ifdef CFILE
				fwrite(m_pcmKnightCapPreMoves[p][i][j].pMoveList, sizeof(FSCHAR), m_nBytes[nCount], fp);
			#else
				ofs.write((FSCHAR*)m_pcmKnightCapPreMoves[p][i][j].pMoveList, m_nBytes[nCount]);
			#endif
			}
		}
	}
}*/

/*******************************************************************************************************
������ : GenPreCheck
��  �� : ��������Ԥ�ж�����
��  �� : ��
����ֵ : ��
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:45:43
*******************************************************************************************************/
FSVOID CSearchEngine::GenPreCheck()
{
	GenRookPreCheck  ();	//��
	GenKnightPreCheck();	//��
	GenCannonPreCheck();	//��
	GenPawnPreCheck  ();	//��
}

/*******************************************************************************************************
������ : GenRookPreCheck
��  �� : ����������Ԥ�ж�����
��  �� : ��
����ֵ : ��
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:46:09
*******************************************************************************************************/
FSVOID CSearchEngine::GenRookPreCheck()
{
	FSINT p;
	FSINT i,j,k;
	FSINT nWho;					//�η�
	FSINT nFoeKingPosCheckNum;	//�Է���У���
	FSINT nFoeKingPosCheckSum;	//�Է���λ��
	FSINT nBitFile;
	FSINT nBitRank;
	FSINT nFoeKingI;
	FSINT nFoeKingJ;
	FSBOOL bFlag;
	
	for (nWho = 0; nWho < 2; nWho++)
	{
		for (p = 0; p < 90; p++)
		{
			i = m_CGN2Pos[p].x;
			j = m_CGN2Pos[p].y;
			
			for (nFoeKingPosCheckNum = 0; nFoeKingPosCheckNum < 9; nFoeKingPosCheckNum++)
			{
				nFoeKingPosCheckSum=m_nBRKingPosCheckSum[1-nWho][nFoeKingPosCheckNum];
				
				nFoeKingI = m_CGN2Pos[nFoeKingPosCheckSum].x;
				nFoeKingJ = m_CGN2Pos[nFoeKingPosCheckSum].y;

				//m_bIsRookFilePreCheck
				for (nBitFile = 0; nBitFile < 512; nBitFile++)
				{
					m_bIsRookFilePreCheck[nWho][p][nFoeKingPosCheckNum][nBitFile] = FALSE;
					
					if (j != m_CGN2Pos[nFoeKingPosCheckSum].y)
					{
						continue;
					}
					
					bFlag = FALSE;
					if (i < nFoeKingI)
					{
						for (k = i + 1; k < nFoeKingI; k++)
						{
							if (nBitFile & m_nMask[k])
							{
								bFlag = TRUE;
								break;
							}
						}
						
						if (bFlag == FALSE)
						{
							m_bIsRookFilePreCheck[nWho][p][nFoeKingPosCheckNum][nBitFile] = TRUE;
						}
					}
					else
					{
						for (k = i - 1; k > nFoeKingI; k--)
						{
							if (nBitFile & m_nMask[k])
							{
								bFlag = TRUE;
								break;
							}
						}
						
						if (bFlag == FALSE)
						{
							m_bIsRookFilePreCheck[nWho][p][nFoeKingPosCheckNum][nBitFile] = TRUE;
						}
					}
				}
				
				//m_bIsRookRankPreCheck
				for (nBitRank = 0; nBitRank < 1024; nBitRank++)
				{
					m_bIsRookRankPreCheck[nWho][p][nFoeKingPosCheckNum][nBitRank] = FALSE;
					
					if (i != m_CGN2Pos[nFoeKingPosCheckSum].x)
					{
						continue;
					}
					
					bFlag = FALSE;
					if (j < nFoeKingJ)
					{
						for (k = j + 1; k < nFoeKingJ; k++)
						{
							if (nBitRank & m_nMask[k])
							{
								bFlag = TRUE;
								break;
							}
						}
						
						if (bFlag == FALSE)
						{
							m_bIsRookRankPreCheck[nWho][p][nFoeKingPosCheckNum][nBitRank] = TRUE;
						}
					}
					else
					{
						for (k=j-1;k>nFoeKingJ;k--)
						{
							if (nBitRank & m_nMask[k])
							{
								bFlag = TRUE;
								break;
							}
						}
						
						if (bFlag == FALSE)
						{
							m_bIsRookRankPreCheck[nWho][p][nFoeKingPosCheckNum][nBitRank] = TRUE;
						}
					}
				}
			}
		}

		//p=90
		p = 90;
		for (nFoeKingPosCheckNum = 0; nFoeKingPosCheckNum < 9; nFoeKingPosCheckNum++)
		{
			//��
			for (nBitFile = 0; nBitFile < 512; nBitFile++)
			{
				m_bIsRookFilePreCheck[nWho][p][nFoeKingPosCheckNum][nBitFile] = FALSE;
			}
			//��
			for (nBitRank = 0; nBitRank < 1024; nBitRank++)
			{
				m_bIsRookRankPreCheck[nWho][p][nFoeKingPosCheckNum][nBitRank] = FALSE;
			}
		}
	}
}

/*******************************************************************************************************
������ : GenKnightPreCheck
��  �� : �����ж�
��  �� : ��
����ֵ : ��
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:46:45
*******************************************************************************************************/
FSVOID CSearchEngine::GenKnightPreCheck()
{
	FSINT p;
	FSINT i, j;
	FSINT nWho;//�η�
	FSINT nFoeKingPosCheckNum;//�Է���У���
	FSINT nFoeKingPosCheckSum;//�Է���λ��
	FSINT nKnightLeg;//����
	FSINT nFoeKingI;
	FSINT nFoeKingJ;

	for (nWho = 0; nWho < 2; nWho++)
	{
		for (p = 0; p < 90; p++)
		{
			i = m_CGN2Pos[p].x;
			j = m_CGN2Pos[p].y;
			
			for (nFoeKingPosCheckNum = 0; nFoeKingPosCheckNum < 9; nFoeKingPosCheckNum++)
			{
				nFoeKingPosCheckSum=m_nBRKingPosCheckSum[1-nWho][nFoeKingPosCheckNum];
				
				nFoeKingI = m_CGN2Pos[nFoeKingPosCheckSum].x;
				nFoeKingJ = m_CGN2Pos[nFoeKingPosCheckSum].y;

				for (nKnightLeg = 0; nKnightLeg < 16; nKnightLeg++)
				{
					m_bIsKnightPreCheck[nWho][p][nFoeKingPosCheckNum][nKnightLeg] = 
						(!(nKnightLeg & m_nMask[0]) && 
						(i == nFoeKingI + 2 && (j == nFoeKingJ + 1 || j == nFoeKingJ - 1))) ||//��������
					    (!(nKnightLeg & m_nMask[1]) && 
					    (i == nFoeKingI - 2 && (j == nFoeKingJ + 1 || j == nFoeKingJ - 1))) ||//��������
					    (!(nKnightLeg & m_nMask[2]) && 
					    (j == nFoeKingJ + 2 && (i == nFoeKingI + 1 || i == nFoeKingI - 1))) ||//��������
					    (!(nKnightLeg & m_nMask[3]) && 
					    (j == nFoeKingJ - 2 && (i == nFoeKingI + 1 || i == nFoeKingI - 1)));  //��������
				}
			}
		}

		//p = 90
		p = 90;
		for (nFoeKingPosCheckNum = 0; nFoeKingPosCheckNum < 9; nFoeKingPosCheckNum++)
		{
			for (nKnightLeg = 0; nKnightLeg < 16; nKnightLeg++)
			{
				m_bIsKnightPreCheck[nWho][p][nFoeKingPosCheckNum][nKnightLeg] = FALSE;
			}
		}
	}
}

/*******************************************************************************************************
������ : GenCannonPreCheck
��  �� : �ڽ����ж�
��  �� : ��
����ֵ : ��
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:46:59
*******************************************************************************************************/
FSVOID CSearchEngine::GenCannonPreCheck()
{
	FSINT p;
	FSINT i,j,k;
	FSINT nWho;//�η�
	FSINT nFoeKingPosCheckNum;//�Է���У���
	FSINT nFoeKingPosCheckSum;//�Է���λ��
	FSINT nBitFile;
	FSINT nBitRank;
	FSINT nFoeKingI;
	FSINT nFoeKingJ;
	FSINT nCnt;

	for (nWho = 0; nWho < 2; nWho++)
	{
		for (p = 0; p < 90; p++)
		{
			i = m_CGN2Pos[p].x;
			j = m_CGN2Pos[p].y;

			for (nFoeKingPosCheckNum = 0; nFoeKingPosCheckNum < 9; nFoeKingPosCheckNum++)
			{
				nFoeKingPosCheckSum = m_nBRKingPosCheckSum[1-nWho][nFoeKingPosCheckNum];
				
				nFoeKingI = m_CGN2Pos[nFoeKingPosCheckSum].x;
				nFoeKingJ = m_CGN2Pos[nFoeKingPosCheckSum].y;

				//m_bIsCannonFilePreCheck
				for (nBitFile = 0; nBitFile < 512; nBitFile++)
				{
					m_bIsCannonFilePreCheck[nWho][p][nFoeKingPosCheckNum][nBitFile] = FALSE;

					if (j != m_CGN2Pos[nFoeKingPosCheckSum].y)
					{
						continue;
					}

					nCnt = 0;
					if (i < nFoeKingI)
					{
						for (k = i + 1; k < nFoeKingI; k++)
						{
							if (nBitFile & m_nMask[k])
							{
								nCnt++;
							}
						}

						if (nCnt == 1)
						{
							m_bIsCannonFilePreCheck[nWho][p][nFoeKingPosCheckNum][nBitFile] = TRUE;
						}
					}
					else
					{
						for (k=i-1;k>nFoeKingI;k--)
						{
							if (nBitFile & m_nMask[k])
							{
								nCnt++;
							}
						}

						if (nCnt == 1)
						{
							m_bIsCannonFilePreCheck[nWho][p][nFoeKingPosCheckNum][nBitFile] = TRUE;
						}
					}
				}

				//m_bIsCannonRankPreCheck
				for (nBitRank = 0; nBitRank < 1024; nBitRank++)
				{
					m_bIsCannonRankPreCheck[nWho][p][nFoeKingPosCheckNum][nBitRank] = FALSE;
					
					if (i != m_CGN2Pos[nFoeKingPosCheckSum].x)
					{
						continue;
					}
					
					nCnt = 0;
					if (j<nFoeKingJ)
					{
						for (k=j+1;k<nFoeKingJ;k++)
						{
							if (nBitRank & m_nMask[k])
							{
								nCnt++;
							}
						}
						
						if (nCnt == 1)
						{
							m_bIsCannonRankPreCheck[nWho][p][nFoeKingPosCheckNum][nBitRank] = TRUE;
						}
					}
					else
					{
						for (k=j-1;k>nFoeKingJ;k--)
						{
							if (nBitRank & m_nMask[k])
							{
								nCnt++;
							}
						}
						
						if (nCnt == 1)
						{
							m_bIsCannonRankPreCheck[nWho][p][nFoeKingPosCheckNum][nBitRank] = TRUE;
						}
					}
				}
			}
		}

		//p=90
		p = 90;
		for (nFoeKingPosCheckNum = 0; nFoeKingPosCheckNum < 9; nFoeKingPosCheckNum++)
		{
			//��
			for (nBitFile = 0; nBitFile < 512; nBitFile++)
			{
				m_bIsCannonFilePreCheck[nWho][p][nFoeKingPosCheckNum][nBitFile] = FALSE;
			}
			//��
			for (nBitRank = 0; nBitRank < 1024; nBitRank++)
			{
				m_bIsCannonRankPreCheck[nWho][p][nFoeKingPosCheckNum][nBitRank] = FALSE;
			}
		}
	}
}

/*******************************************************************************************************
������ : GenPawnPreCheck
��  �� : �������ж�
��  �� : ��
����ֵ : ��
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:47:13
*******************************************************************************************************/
FSVOID CSearchEngine::GenPawnPreCheck()
{
	FSINT i, j;
	FSINT nWho;	//�η�

	FSINT nPosCheckNum;	//��λ��У�����
	FSINT nPosCheckSum;	//��λ��У���

	FSINT nFoeKingPosCheckNum;	//�Է���У���
	FSINT nFoeKingPosCheckSum;	//�Է���λ��

	for (nWho = 0; nWho < 2; nWho++)
	{
		//��λ��У�����
		for (nPosCheckNum = 0;nPosCheckNum<55;nPosCheckNum++)
		{
			nPosCheckSum=m_nPawnPosCheckSum[nWho][nPosCheckNum];

			i=m_CGN2Pos[nPosCheckSum].x;
			j=m_CGN2Pos[nPosCheckSum].y;

			for (nFoeKingPosCheckNum = 0; nFoeKingPosCheckNum < 9; nFoeKingPosCheckNum++)
			{
				nFoeKingPosCheckSum=m_nBRKingPosCheckSum[1 - nWho][nFoeKingPosCheckNum];
				
				m_bIsPawnPreCheck[nWho][nPosCheckNum][nFoeKingPosCheckNum] = FALSE;
				
				//��
				if (nWho)
				{
					if (nFoeKingPosCheckSum == nPosCheckSum - 10 ||
					   nFoeKingPosCheckSum == nPosCheckSum + 10 ||
					   nFoeKingPosCheckSum == nPosCheckSum + 1)
					{
						m_bIsPawnPreCheck[nWho][nPosCheckNum][nFoeKingPosCheckNum] = TRUE;
					}
				}
				else
				{
					if (nFoeKingPosCheckSum == nPosCheckSum - 10 ||
					   nFoeKingPosCheckSum == nPosCheckSum + 10 ||
				       nFoeKingPosCheckSum == nPosCheckSum - 1)
					{
						m_bIsPawnPreCheck[nWho][nPosCheckNum][nFoeKingPosCheckNum] = TRUE;
					}
				}
			}
		}

		//nPosCheckNum = 55
		nPosCheckNum = 55;
		for (nFoeKingPosCheckNum = 0; nFoeKingPosCheckNum < 9; nFoeKingPosCheckNum++)
		{
			m_bIsPawnPreCheck[nWho][nPosCheckNum][nFoeKingPosCheckNum] = FALSE;
		}
	}
}

/*******************************************************************************************************
������ : GenPreValidMove
��  �� : �����߷���Ч��Ԥ�ж�����
��  �� : ��
����ֵ : ����Ϊ FSVOID
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��10��2�� 20:44:58
*******************************************************************************************************/
FSVOID CSearchEngine::GenPreValidMove()
{
	GenRookPreValidMove();		//��
//	GenKnightPreValidMove();	//��
	GenCannonPreValidMove();	//��
//	GenKingPreValidMove();		//��
//	GenAdvisorPreValidMove();	//ʿ
//	GenBishopPreValidMove();	//��
//	GenPawnPreValidMove();		//��
}

/*******************************************************************************************************
������ : GenRookPreValidMove
��  �� : �������߷���Ч��Ԥ�ж�����
��  �� : ��
����ֵ : ����Ϊ FSVOID
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��10��2�� 20:44:53
*******************************************************************************************************/
FSVOID CSearchEngine::GenRookPreValidMove()
{
//	FSINT i, j;
//	FSINT nMinI, nMaxI;
//	FSINT nMinJ, nMaxJ;
//	FSINT nSrc, nDst;
//	FSINT nSrcI, nSrcJ;
//	FSINT nDstI, nDstJ;
//	FSINT nBitFile;
//	FSINT nBitRank;
//	FSBOOL bFlag;
//	FSINT nFEATBOOLSize = sizeof(FEATBOOL);

#ifdef TIMECALC

	FSINT nTimeStart = clock();

#endif

//	for (nSrc = 0; nSrc < 90; nSrc++)
//	{
//		nSrcI = m_CGN2Pos[nSrc].x;
//		nSrcJ = m_CGN2Pos[nSrc].y;
//
//		for (nDst = 0; nDst < 90; nDst++)
//		{
//			nDstI = m_CGN2Pos[nDst].x;
//			nDstJ = m_CGN2Pos[nDst].y;
//
//			//��
//			if (nSrcJ == nDstJ)
//			{
//				for (nBitFile = 0; nBitFile < 512; nBitFile++)
//				{
//					nMinI = min(nSrcI, nDstI);
//					nMaxI = max(nSrcI, nDstI);
//					
//					bFlag = TRUE;
//					for (i = nMinI + 1; i < nMaxI; i++)
//					{
//						if (m_nMask[i] & nBitFile)
//						{
//							bFlag = FALSE;
//							break;
//						}
//					}
//					m_bIsRookFilePreValidMove[nSrc][nDst][nBitFile] = bFlag;
//				}
//			}
//			else
//			{
//				memset(m_bIsRookFilePreValidMove[nSrc][nDst], FALSE, nFEATBOOLSize << 9);
//			}
//
//			//��
//			if (nSrcI != nDstI)
//			{
//				memset(m_bIsRookRankPreValidMove[nSrc][nDst], FALSE, nFEATBOOLSize << 10);
//				continue;
//			}
//
//			//nSrcJ == nDstJ
//			for (nBitRank = 0; nBitRank < 1024; nBitRank++)
//			{
//				nMinJ = min(nSrcJ, nDstJ);
//				nMaxJ = max(nSrcJ, nDstJ);
//
//				bFlag = TRUE;
//				for (j = nMinJ + 1; j < nMaxJ; j++)
//				{
//					if (m_nMask[j] & nBitRank)
//					{
//						bFlag = FALSE;
//						break;
//					}
//				}
//				m_bIsRookRankPreValidMove[nSrc][nDst][nBitRank] = bFlag;
//			}
//		}
//	}

	FSINT i;
	FSINT nMin, nMax;
	FSINT nSrc, nDst;
	FSINT nBit;
	FSBOOL bFlag;
	
	for (nSrc = 0; nSrc < 10; nSrc++)
	{
		for (nDst = 0; nDst < 10; nDst++)
		{
			for (nBit = 0; nBit < 1024; nBit++)
			{
				nMin = min(nSrc, nDst);
				nMax = max(nSrc, nDst);
				
				bFlag = TRUE;
				for (i = nMin + 1; i < nMax; i++)
				{
					if (m_nMask[i] & nBit)
					{
						bFlag = FALSE;
						break;
					}
				}
				m_bIsRookPreValidMove[nSrc][nDst][nBit] = bFlag;
			}
		}
	}

#ifdef TIMECALC

	cout << "�������߷���Ч��Ԥ�жϵ�ʱ��:" << clock() - nTimeStart << endl;

#endif
}

/*******************************************************************************************************
������ : GenKnightPreValidMove
��  �� : �������߷���Ч��Ԥ�ж�����
��  �� : ��
����ֵ : ����Ϊ FSVOID
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��10��2�� 20:44:46
*******************************************************************************************************/
//FSVOID CSearchEngine::GenKnightPreValidMove()
//{
//	FSINT nSrc, nDst;
//	FSINT nSrcI, nSrcJ;
//	FSINT nDstI, nDstJ;
//	FSINT nKnightLeg;//����
//
//	for (nSrc = 0; nSrc < 90; nSrc++)
//	{
//		nSrcI = m_CGN2Pos[nSrc].x;
//		nSrcJ = m_CGN2Pos[nSrc].y;
//		
//		for (nDst = 0; nDst < 90; nDst++)
//		{
//			nDstI = m_CGN2Pos[nDst].x;
//			nDstJ = m_CGN2Pos[nDst].y;
//			
//			for (nKnightLeg = 0; nKnightLeg < 16; nKnightLeg++)
//			{
//				m_bIsKnightPreValidMove[nSrc][nDst][nKnightLeg] = 
//					(!(nKnightLeg & m_nMask[0]) && 
//					(nSrcI == nDstI + 2 && (nSrcJ == nDstJ + 1 || nSrcJ == nDstJ - 1))) ||//��������
//					(!(nKnightLeg & m_nMask[1]) && 
//					(nSrcI == nDstI - 2 && (nSrcJ == nDstJ + 1 || nSrcJ == nDstJ - 1))) ||//��������
//					(!(nKnightLeg & m_nMask[2]) && 
//					(nSrcJ == nDstJ + 2 && (nSrcI == nDstI + 1 || nSrcI == nDstI - 1))) ||//��������
//					(!(nKnightLeg & m_nMask[3]) && 
//					(nSrcJ == nDstJ - 2 && (nSrcI == nDstI + 1 || nSrcI == nDstI - 1)));  //��������
//			}
//		}
//	}
//}

/*******************************************************************************************************
������ : GenCannonPreValidMove
��  �� : �������߷���Ч��Ԥ�ж�����
��  �� : ��
����ֵ : ����Ϊ FSVOID
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��10��2�� 20:44:40
*******************************************************************************************************/
FSVOID CSearchEngine::GenCannonPreValidMove()
{
//	FSINT i, j;
//	FSINT nMinI, nMaxI;
//	FSINT nMinJ, nMaxJ;
//	FSINT nSrc, nDst;
//	FSINT nSrcI, nSrcJ;
//	FSINT nDstI, nDstJ;
//	FSINT nBitFile;
//	FSINT nBitRank;
//	FSINT nCnt;
//	FSINT nFEATBOOLSize = sizeof(FEATBOOL);
//
//	//��
//	for (nSrc = 0; nSrc < 90; nSrc++)
//	{
//		nSrcI = m_CGN2Pos[nSrc].x;
//		nSrcJ = m_CGN2Pos[nSrc].y;
//
//		for (nDst = 0; nDst < 90; nDst++)
//		{
//			nDstI = m_CGN2Pos[nDst].x;
//			nDstJ = m_CGN2Pos[nDst].y;
//			
//			//��
//			if (nSrcJ == nDstJ)
//			{
//				nMinI = min(nSrcI, nDstI);
//				nMaxI = max(nSrcI, nDstI);
//				
//				for (nBitFile = 0; nBitFile < 512; nBitFile++)
//				{
//					nCnt = 0;
//					for (i = nMinI + 1; i < nMaxI; i++)
//					{
//						if ((nBitFile & m_nMask[i]) != 0)
//						{
//							nCnt++;
//						}
//					}
//					if (nCnt == 0)
//					{
//						m_bIsCannonFilePreValidMove[nSrc][nDst][nBitFile][FALSE] = TRUE;
//						m_bIsCannonFilePreValidMove[nSrc][nDst][nBitFile][TRUE] = FALSE;
//					}
//					else if (nCnt == 1)
//					{
//						m_bIsCannonFilePreValidMove[nSrc][nDst][nBitFile][FALSE] = FALSE;
//						m_bIsCannonFilePreValidMove[nSrc][nDst][nBitFile][TRUE] = TRUE;
//					}
//					else
//					{
//						m_bIsCannonFilePreValidMove[nSrc][nDst][nBitFile][FALSE] = FALSE;
//						m_bIsCannonFilePreValidMove[nSrc][nDst][nBitFile][TRUE] = FALSE;
//					}
//				}
//			}
//			else
//			{
//				for (nBitFile = 0; nBitFile < 512; nBitFile++)
//				{
//					m_bIsCannonFilePreValidMove[nSrc][nDst][nBitFile][FALSE] = FALSE;
//					m_bIsCannonFilePreValidMove[nSrc][nDst][nBitFile][TRUE] = FALSE;
//				}
//			}
//
//			//��
//			if (nSrcI != nDstI)
//			{
//				for (nBitRank = 0; nBitRank < 1024; nBitRank++)
//				{
//					m_bIsCannonRankPreValidMove[nSrc][nDst][nBitRank][FALSE] = FALSE;
//					m_bIsCannonRankPreValidMove[nSrc][nDst][nBitRank][TRUE] = FALSE;
//				}
//
//				continue;
//			}
//
//			//nSrcI == nDstI
//			nMinJ = min(nSrcJ, nDstJ);
//			nMaxJ = max(nSrcJ, nDstJ);
//			
//			for (nBitRank = 0; nBitRank < 1024; nBitRank++)
//			{
//				nCnt = 0;
//				for (j = nMinJ + 1; j < nMaxJ; j++)
//				{
//					if ((nBitRank & m_nMask[j]) != 0)
//					{
//						nCnt++;
//					}
//				}
//				if (nCnt == 0)
//				{
//					m_bIsCannonRankPreValidMove[nSrc][nDst][nBitRank][FALSE] = TRUE;
//					m_bIsCannonRankPreValidMove[nSrc][nDst][nBitRank][TRUE] = FALSE;
//				}
//				else if (nCnt == 1)
//				{
//					m_bIsCannonRankPreValidMove[nSrc][nDst][nBitRank][FALSE] = FALSE;
//					m_bIsCannonRankPreValidMove[nSrc][nDst][nBitRank][TRUE] = TRUE;
//				}
//				else
//				{
//					m_bIsCannonRankPreValidMove[nSrc][nDst][nBitRank][FALSE] = FALSE;
//					m_bIsCannonRankPreValidMove[nSrc][nDst][nBitRank][TRUE] = FALSE;
//				}
//			}
//		}
//	}

	FSINT i;
	FSINT nMin, nMax;
	FSINT nSrc, nDst;
	FSINT nBit;
	FSINT nCnt;

	for (nSrc = 0; nSrc < 10; nSrc++)
	{
		for (nDst = 0; nDst < 10; nDst++)
		{
			nMin = min(nSrc, nDst);
			nMax = max(nSrc, nDst);
			
			for (nBit = 0; nBit < 1024; nBit++)
			{
				nCnt = 0;
				for (i = nMin + 1; i < nMax; i++)
				{
					if ((nBit & m_nMask[i]) != 0)
					{
						nCnt++;
					}
				}
				if (nCnt == 0)
				{
					m_bIsCannonPreValidMove[nSrc][nDst][nBit][FALSE] = TRUE;
					m_bIsCannonPreValidMove[nSrc][nDst][nBit][TRUE] = FALSE;			
				}
				else if (nCnt == 1)
				{
					m_bIsCannonPreValidMove[nSrc][nDst][nBit][FALSE] = FALSE;
					m_bIsCannonPreValidMove[nSrc][nDst][nBit][TRUE] = TRUE;
				}
				else
				{
					m_bIsCannonPreValidMove[nSrc][nDst][nBit][FALSE] = FALSE;
					m_bIsCannonPreValidMove[nSrc][nDst][nBit][TRUE] = FALSE;
				}
			}
		}
	}
}

/*******************************************************************************************************
������ : GenKingPreValidMove
��  �� : �������߷���Ч��Ԥ�ж�����
��  �� : ��
����ֵ : ����Ϊ FSVOID
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��10��2�� 20:44:18
*******************************************************************************************************/
//FSVOID CSearchEngine::GenKingPreValidMove()
//{
//	FSINT i;
//	FSINT nSrc, nDst;
//
//	for (nSrc = 0; nSrc < 90; nSrc++)
//	{
//		for (nDst = 0; nDst < 90; nDst++)
//		{
//			for (i = 0; i < MAXKINGMOVECNT; i++)
//			{
//				if (nDst - nSrc == g_nKingMoveDelta[i])
//				{
//					m_bIsKingPreValidMove[nSrc][nDst] = TRUE;
//					continue;
//				}
//				m_bIsKingPreValidMove[nSrc][nDst] = FALSE;
//			}
//		}
//	}
//}

/*******************************************************************************************************
������ : GenAdvisorPreValidMove
��  �� : ����ʿ�߷���Ч��Ԥ�ж�����
��  �� : ��
����ֵ : ����Ϊ FSVOID
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��10��2�� 20:44:14
*******************************************************************************************************/
//FSVOID CSearchEngine::GenAdvisorPreValidMove()
//{
//
//}

/*******************************************************************************************************
������ : GenBishopPreValidMove
��  �� : �������߷���Ч��Ԥ�ж�����
��  �� : ��
����ֵ : ����Ϊ FSVOID
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��10��2�� 20:44:06
*******************************************************************************************************/
//FSVOID CSearchEngine::GenBishopPreValidMove()
//{
//
//}

/*******************************************************************************************************
������ : GenPawnPreValidMove
��  �� : �������߷���Ч��Ԥ�ж�����
��  �� : ��
����ֵ : ����Ϊ FSVOID 
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��10��2�� 20:43:59
*******************************************************************************************************/
//FSVOID CSearchEngine::GenPawnPreValidMove()
//{
//
//}

/*******************************************************************************************************
������ : GenMoves
��  �� : �������߷�������m_pMoveList��
��  �� : 
        FSINT nDepth : ��ʱ״̬�ڲ������еĲ���
����ֵ : ��
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:31:00
*******************************************************************************************************/
inline FSVOID CSearchEngine::GenMoves(FSINT nDepth)
{
	DEBUGASSERT(nDepth >= 0 && nDepth < MAXSEARCHDEPTH);

	FSINT nPiecePos;

	FSINT nRook;
	FSINT nKnight;
	FSINT nCannon;
	FSINT nAdvisor;
	FSINT nBishop;
	FSINT nPawn;

	nRook = m_nRook[m_bIsRed];
	nKnight = m_nKnight[m_bIsRed];
	nCannon = m_nCannon[m_bIsRed];
	nAdvisor = m_nAdvisor[m_bIsRed];
	nBishop = m_nBishop[m_bIsRed];
	nPawn = m_nPawn[m_bIsRed];

	m_nMovesCnt = 0;//�߷�������

	//��
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nRook][0]]) != INVALIDPIECEPOS)
	{
		GenRookMoves(nPiecePos, nDepth);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nRook][1]]) != INVALIDPIECEPOS)
	{
		GenRookMoves(nPiecePos, nDepth);
	}
	
	//��
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nKnight][0]]) != INVALIDPIECEPOS)
	{
		GenKnightMoves(nPiecePos, nDepth);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nKnight][1]]) != INVALIDPIECEPOS)
	{
		GenKnightMoves(nPiecePos, nDepth);
	}
	
	//��
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nCannon][0]]) != INVALIDPIECEPOS)
	{
		GenCannonMoves(nPiecePos, nDepth);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nCannon][1]]) != INVALIDPIECEPOS)
	{
		GenCannonMoves(nPiecePos, nDepth);
	}
	
	//��
	GenKingMoves(m_nPiecePos[m_nKingID2PieceID[m_bIsRed]], nDepth);
	
	//ʿ
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nAdvisor][0]]) != INVALIDPIECEPOS)
	{
		GenAdvisorMoves(nPiecePos, nDepth);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nAdvisor][1]]) != INVALIDPIECEPOS)
	{
		GenAdvisorMoves(nPiecePos, nDepth);
	}
	
	//��
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nBishop][0]]) != INVALIDPIECEPOS)
	{
		GenBishopMoves(nPiecePos, nDepth);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nBishop][1]]) != INVALIDPIECEPOS)
	{
		GenBishopMoves(nPiecePos, nDepth);
	}
	
	//��
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nPawn][0]]) != INVALIDPIECEPOS)
	{
		GenPawnMoves(nPiecePos, nDepth);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nPawn][1]]) != INVALIDPIECEPOS)
	{
		GenPawnMoves(nPiecePos, nDepth);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nPawn][2]]) != INVALIDPIECEPOS)
	{
		GenPawnMoves(nPiecePos, nDepth);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nPawn][3]]) != INVALIDPIECEPOS)
	{
		GenPawnMoves(nPiecePos, nDepth);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nPawn][4]]) != INVALIDPIECEPOS)
	{
		GenPawnMoves(nPiecePos, nDepth);
	}
}

/*******************************************************************************************************
������ : GenRookMoves
��  �� : �����߷�����
��  �� : 
        FSINT p         : 
		FSINT nDepth    : 
����ֵ : FSVOID
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:32:47
*******************************************************************************************************/
inline FSVOID CSearchEngine::GenRookMoves(FSINT p, FSINT nDepth)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);
	DEBUGASSERT(nDepth >= 0 && nDepth < MAXSEARCHDEPTH);

	FSINT i, j;
	FSINT nCount;
	FSINT nRookCheckNum;//��У�����
	FSINT nRookFoeCheckNum;
	PRECHESSMOVE pcmFile;
	PRECHESSMOVE pcmRank;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;
	
	//��
	nRookCheckNum = m_nRookFileRankCheckNum[i][m_nBitFile[j]];
	nRookFoeCheckNum = m_nRookFoeFileRankCheckNum[i][nRookCheckNum][m_nBRBitFile[1 - m_bIsRed][j]];
	pcmFile = m_pcmRookFilePreMoves[p][nRookCheckNum][nRookFoeCheckNum];
	nCount = pcmFile.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 8);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcmFile.pMoveList, m_nBytes[nCount]);
	m_nMovesCnt += nCount;

	//��
	nRookCheckNum = m_nRookFileRankCheckNum[j][m_nBitRank[i]];
	nRookFoeCheckNum = m_nRookFoeFileRankCheckNum[j][nRookCheckNum][m_nBRBitRank[1 - m_bIsRed][i]];
	pcmRank = m_pcmRookRankPreMoves[p][nRookCheckNum][nRookFoeCheckNum];
	nCount = pcmRank.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 9);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcmRank.pMoveList, m_nBytes[nCount]);
	m_nMovesCnt += nCount;
}

/*******************************************************************************************************
������ : GenKnightMoves
��  �� : ����߷�����
��  �� : 
        FSINT p			  : 
		FSINT nDepth      : 
		FSBOOL m_bIsRed   : 
����ֵ : FSVOID
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:33:33
*******************************************************************************************************/
inline FSVOID CSearchEngine::GenKnightMoves(FSINT p, FSINT nDepth)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);
	DEBUGASSERT(nDepth >= 0 && nDepth < MAXSEARCHDEPTH);

	FSINT i, j;
	FSINT nCount;
	FSINT nKnightLegCheckSum;
	FSINT nKnightFile0AroundCheckSum;
	FSINT nKnightFile1AroundCheckSum;
	FSINT nKnightFile2AroundCheckSum;
	FSINT nKnightFile3AroundCheckSum;
	FSINT nKnightFileAroundCheckSum;

	FSINT nKnightFoeFile0AroundCheckSum;
	FSINT nKnightFoeFile1AroundCheckSum;
	FSINT nKnightFoeFile2AroundCheckSum;
	FSINT nKnightFoeFile3AroundCheckSum;
	FSINT nKnightFoeFileAroundCheckSum;

	PRECHESSMOVE pcm;
	
	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;
	
	nKnightFile0AroundCheckSum = m_nFileRankAroundCheckSum[i][m_nBitFile[m_nAbs[j][2]]];
	nKnightFile1AroundCheckSum = m_nFileRankAroundCheckSum2[i][m_nBitFile[m_nAbs[j][1]]];
	nKnightFile2AroundCheckSum = m_nFileRankAroundCheckSum2[i][m_nBitFile[j + 1]];
	nKnightFile3AroundCheckSum = m_nFileRankAroundCheckSum[i][m_nBitFile[j + 2]];

	nKnightFoeFile0AroundCheckSum = m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][m_nAbs[j][2]]];
	nKnightFoeFile1AroundCheckSum = m_nFileRankAroundCheckSum2[i][m_nBRBitFile[1 - m_bIsRed][m_nAbs[j][1]]];
	nKnightFoeFile2AroundCheckSum = m_nFileRankAroundCheckSum2[i][m_nBRBitFile[1 - m_bIsRed][j + 1]];
	nKnightFoeFile3AroundCheckSum = m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][j + 2]];

	nKnightFileAroundCheckSum = 
		m_nKnightAroundCheckSum[m_nAroundCheckSum[nKnightFile0AroundCheckSum][nKnightFile1AroundCheckSum]][m_nAroundCheckSum[nKnightFile2AroundCheckSum][nKnightFile3AroundCheckSum]];
	nKnightFoeFileAroundCheckSum = 
		m_nKnightAroundCheckSum[m_nAroundCheckSum[nKnightFoeFile0AroundCheckSum][nKnightFoeFile1AroundCheckSum]][m_nAroundCheckSum[nKnightFoeFile2AroundCheckSum][nKnightFoeFile3AroundCheckSum]];
	nKnightLegCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBitFile[j]]][m_nFileRankAroundCheckSum[j][m_nBitRank[i]]];
	
	//�������߷�
	pcm = m_pcmKnightNoCapPreMoves[p][nKnightFileAroundCheckSum][nKnightLegCheckSum];
	nCount = pcm.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 8);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcm.pMoveList, m_nBytes[nCount]);
	m_nMovesCnt += nCount;

	//�����߷�
	pcm = m_pcmKnightCapPreMoves[p][nKnightFoeFileAroundCheckSum][nKnightLegCheckSum];
	nCount = pcm.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 8);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcm.pMoveList, m_nBytes[nCount]);
	m_nMovesCnt += nCount;
}

/*******************************************************************************************************
������ : GenCannonMoves
��  �� : �ڵ��߷�����
��  �� : 
        FSINT p			: 
		FSINT nDepth    : 
����ֵ : inline
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:34:39
*******************************************************************************************************/
inline FSVOID CSearchEngine::GenCannonMoves(FSINT p, FSINT nDepth)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);
	DEBUGASSERT(nDepth >= 0 && nDepth < MAXSEARCHDEPTH);	

	FSINT i, j;
	FSINT nCount;
	FSINT nCannonCheckNum;
	FSINT nCannonFoeCheckNum;
	PRECHESSMOVE pcmFile;
	PRECHESSMOVE pcmRank;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	//��
	nCannonCheckNum = m_nCannonFileRankCheckNum[i][m_nBitFile[j]];
	nCannonFoeCheckNum =
		m_nCannonFoeFileRankCheckNum[i][nCannonCheckNum][m_nBRBitFile[1 - m_bIsRed][j]];
	pcmFile = m_pcmCannonFilePreMoves[p][nCannonCheckNum][nCannonFoeCheckNum];
	nCount = pcmFile.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 8);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcmFile.pMoveList, m_nBytes[nCount]);
	m_nMovesCnt += nCount;

	//��
	nCannonCheckNum = m_nCannonFileRankCheckNum[j][m_nBitRank[i]];
	nCannonFoeCheckNum = 
		m_nCannonFoeFileRankCheckNum[j][nCannonCheckNum][m_nBRBitRank[1 - m_bIsRed][i]];
	pcmRank = m_pcmCannonRankPreMoves[p][nCannonCheckNum][nCannonFoeCheckNum];
	nCount = pcmRank.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 9);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcmRank.pMoveList, m_nBytes[nCount]);
	m_nMovesCnt += nCount;
}

/*******************************************************************************************************
������ : GenKingMoves
��  �� : �����߷�����
��  �� : 
        FSINT p		: 
		FSINT nDepth  : 
����ֵ : inline
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:35:11
*******************************************************************************************************/
inline FSVOID CSearchEngine::GenKingMoves(FSINT p,FSINT nDepth)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);
	DEBUGASSERT(nDepth >= 0 && nDepth < MAXSEARCHDEPTH);

	FSINT i, j;
	FSINT nCount;
	FSINT nPosCheckNum;
	FSINT nAroundCheckSum;
	FSINT nFoeAroundCheckSum;
	PRECHESSMOVE pcm;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	nPosCheckNum = m_nKingPosCheckNum[p];
	DEBUGASSERT(nPosCheckNum >= MINKINGPOSCHECKNUM && nPosCheckNum <= MAXKINGPOSCHECKNUM);
	nAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBitFile[j]]][m_nFileRankAroundCheckSum[j][m_nBitRank[i]]];
	nFoeAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][j]]][m_nFileRankAroundCheckSum[j][m_nBRBitRank[1 - m_bIsRed][i]]];
	
	pcm = m_pcmKingPreMoves[nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum];
	nCount = pcm.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 4);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcm.pMoveList, m_nBytes[nCount]);
	m_nMovesCnt += nCount;
}

/*******************************************************************************************************
������ : GenAdvisorMoves
��  �� : ʿ���߷�����
��  �� : 
        FSINT p		    : 
		FSINT nDepth    : 
		FSBOOL m_bIsRed : 
����ֵ : inline
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:35:42
*******************************************************************************************************/
inline FSVOID CSearchEngine::GenAdvisorMoves(FSINT p,FSINT nDepth)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);
	DEBUGASSERT(nDepth >= 0 && nDepth < MAXSEARCHDEPTH);	

	FSINT i, j;
	FSINT nCount;
	FSINT nPosCheckNum;
	FSINT nAroundCheckSum;
	FSINT nFoeAroundCheckSum;
	PRECHESSMOVE pcm;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	nPosCheckNum = m_nAdvisorPosCheckNum[p];
	nAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBitFile[m_nAbs[j][1]]]][m_nFileRankAroundCheckSum[i][m_nBitFile[j + 1]]];
	nFoeAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][m_nAbs[j][1]]]][m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][j + 1]]];
	
	pcm = m_pcmAdvisorPreMoves[nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum];
	nCount = pcm.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 4);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcm.pMoveList, m_nBytes[nCount]);
	m_nMovesCnt += nCount;
}

/*******************************************************************************************************
������ : GenBishopMoves
��  �� : ����߷�����
��  �� : 
        FSINT p       : 
		FSINT nDepth  : 
����ֵ : inline
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:36:14
*******************************************************************************************************/
inline FSVOID CSearchEngine::GenBishopMoves(FSINT p,FSINT nDepth)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);
	DEBUGASSERT(nDepth >= 0 && nDepth < MAXSEARCHDEPTH);

	FSINT i, j;
	FSINT nCount;
	FSINT nPosCheckNum;
	FSINT nElephantEyeCheckSum;
	FSINT nAroundCheckSum;
	FSINT nFoeAroundCheckSum;
	PRECHESSMOVE pcm;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	nPosCheckNum = m_nBishopPosCheckNum[p];
	nAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum2[i][m_nBitFile[m_nAbs[j][2]]]][m_nFileRankAroundCheckSum2[i][m_nBitFile[j + 2]]];
	nFoeAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum2[i][m_nBRBitFile[1 - m_bIsRed][m_nAbs[j][2]]]][m_nFileRankAroundCheckSum2[i][m_nBRBitFile[1 - m_bIsRed][j + 2]]];
	nElephantEyeCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBitFile[m_nAbs[j][1]]]][m_nFileRankAroundCheckSum[i][m_nBitFile[j + 1]]];

	pcm = 
		m_pcmBishopPreMoves[nPosCheckNum][nElephantEyeCheckSum][nAroundCheckSum][nFoeAroundCheckSum];
	nCount = pcm.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 4);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcm.pMoveList, m_nBytes[nCount]);
	m_nMovesCnt += nCount;
}

/*******************************************************************************************************
������ : GenPawnMoves
��  �� : �����߷�����
��  �� : 
        FSINT p         : 
		FSINT nDepth    : 
����ֵ : ��
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:36:43
*******************************************************************************************************/
inline FSVOID CSearchEngine::GenPawnMoves(FSINT p, FSINT nDepth)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);
	DEBUGASSERT(nDepth >= 0 && nDepth < MAXSEARCHDEPTH);

	FSINT i, j;
	FSINT nCount;
	FSINT nPosCheckNum;
	FSINT nAroundCheckSum;
	FSINT nFoeAroundCheckSum;
	PRECHESSMOVE pcm;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	nPosCheckNum = m_nPawnPosCheckNum[m_bIsRed][p];
	nAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBitFile[j]]][m_nPawnRankAroundCheckSum[m_bIsRed][j][m_nBitRank[i]]];
	nFoeAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][j]]][m_nPawnRankAroundCheckSum[m_bIsRed][j][m_nBRBitRank[1 - m_bIsRed][i]]];

	pcm = m_PawnPreMoves[m_bIsRed][nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum];
	nCount = pcm.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= MAXPAWNCAPMOVECNT);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcm.pMoveList, m_nBytes[nCount]);
	m_nMovesCnt += nCount;
}

/*******************************************************************************************************
������ : GenNoCapMoves
��  �� : 
��  �� : 
        FSINT nDepth : 
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��10��23�� 17:15:46
*******************************************************************************************************/
FSVOID CSearchEngine::GenNoCapMoves(FSINT nDepth)
{
	DEBUGASSERT(nDepth >= 0 && nDepth < MAXSEARCHDEPTH);

	FSINT nPiecePos;

	FSINT nRook;
	FSINT nKnight;
	FSINT nCannon;
	FSINT nAdvisor;
	FSINT nBishop;
	FSINT nPawn;

	nRook = m_nRook[m_bIsRed];
	nKnight = m_nKnight[m_bIsRed];
	nCannon = m_nCannon[m_bIsRed];
	nAdvisor = m_nAdvisor[m_bIsRed];
	nBishop = m_nBishop[m_bIsRed];
	nPawn = m_nPawn[m_bIsRed];

	m_nMovesCnt = 0;	//�߷�������

	//��
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nRook][0]]) != INVALIDPIECEPOS)
	{
		GenRookNoCapMoves(nPiecePos, nDepth);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nRook][1]]) != INVALIDPIECEPOS)
	{
		GenRookNoCapMoves(nPiecePos, nDepth);
	}
	
	//��
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nKnight][0]]) != INVALIDPIECEPOS)
	{
		GenKnightNoCapMoves(nPiecePos, nDepth);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nKnight][1]]) != INVALIDPIECEPOS)
	{
		GenKnightNoCapMoves(nPiecePos, nDepth);
	}
	
	//��
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nCannon][0]]) != INVALIDPIECEPOS)
	{
		GenCannonNoCapMoves(nPiecePos, nDepth);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nCannon][1]]) != INVALIDPIECEPOS)
	{
		GenCannonNoCapMoves(nPiecePos, nDepth);
	}
	
	//��
	GenKingNoCapMoves(m_nPiecePos[m_nKingID2PieceID[m_bIsRed]], nDepth);
	
	//ʿ
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nAdvisor][0]]) != INVALIDPIECEPOS)
	{
		GenAdvisorNoCapMoves(nPiecePos, nDepth);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nAdvisor][1]]) != INVALIDPIECEPOS)
	{
		GenAdvisorNoCapMoves(nPiecePos, nDepth);
	}
	
	//��
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nBishop][0]]) != INVALIDPIECEPOS)
	{
		GenBishopNoCapMoves(nPiecePos, nDepth);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nBishop][1]]) != INVALIDPIECEPOS)
	{
		GenBishopNoCapMoves(nPiecePos, nDepth);
	}
	
	//��
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nPawn][0]]) != INVALIDPIECEPOS)
	{
		GenPawnNoCapMoves(nPiecePos, nDepth);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nPawn][1]]) != INVALIDPIECEPOS)
	{
		GenPawnNoCapMoves(nPiecePos, nDepth);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nPawn][2]]) != INVALIDPIECEPOS)
	{
		GenPawnNoCapMoves(nPiecePos, nDepth);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nPawn][3]]) != INVALIDPIECEPOS)
	{
		GenPawnNoCapMoves(nPiecePos, nDepth);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nPawn][4]]) != INVALIDPIECEPOS)
	{
		GenPawnNoCapMoves(nPiecePos, nDepth);
	}
}

/*******************************************************************************************************
������ : GenRookNoCapMoves
��  �� : 
��  �� : 
        FSINT p      : 
		FSINT nDepth : 
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��10��23�� 17:15:55
*******************************************************************************************************/
FSVOID CSearchEngine::GenRookNoCapMoves(FSINT p, FSINT nDepth)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);
	DEBUGASSERT(nDepth >= 0 && nDepth < MAXSEARCHDEPTH);

	FSINT i, j;
	FSINT nCount;
	FSINT nRookCheckNum;//��У�����
	PRECHESSMOVE pcmFile;
	PRECHESSMOVE pcmRank;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;
	
	//��
	nRookCheckNum = m_nRookFileRankCheckNum[i][m_nBitFile[j]];
	pcmFile = m_pcmRookFileNoCapPreMoves[p][nRookCheckNum];
	nCount = pcmFile.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= MAXROOKFILENOCAPMOVECNT);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcmFile.pMoveList, m_nBytes[nCount]);
	m_nMovesCnt += nCount;

	//��
	nRookCheckNum = m_nRookFileRankCheckNum[j][m_nBitRank[i]];
	pcmRank = m_pcmRookRankNoCapPreMoves[p][nRookCheckNum];
	nCount = pcmRank.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= MAXROOKRANKNOCAPMOVECNT);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcmRank.pMoveList, m_nBytes[nCount]);
	m_nMovesCnt += nCount;
}

/*******************************************************************************************************
������ : GenKnightNoCapMoves
��  �� : 
��  �� : 
        FSINT p      : 
		FSINT nDepth : 
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��10��23�� 17:16:03
*******************************************************************************************************/
FSVOID CSearchEngine::GenKnightNoCapMoves(FSINT p, FSINT nDepth)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);
	DEBUGASSERT(nDepth >= 0 && nDepth < MAXSEARCHDEPTH);

	FSINT i, j;
	FSINT nCount;
	FSINT nKnightLegCheckSum;
	FSINT nKnightFile0AroundCheckSum;
	FSINT nKnightFile1AroundCheckSum;
	FSINT nKnightFile2AroundCheckSum;
	FSINT nKnightFile3AroundCheckSum;
	FSINT nKnightFileAroundCheckSum;

	PRECHESSMOVE pcm;
	
	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;
	
	nKnightFile0AroundCheckSum = m_nFileRankAroundCheckSum[i][m_nBitFile[m_nAbs[j][2]]];
	nKnightFile1AroundCheckSum = m_nFileRankAroundCheckSum2[i][m_nBitFile[m_nAbs[j][1]]];
	nKnightFile2AroundCheckSum = m_nFileRankAroundCheckSum2[i][m_nBitFile[j + 1]];
	nKnightFile3AroundCheckSum = m_nFileRankAroundCheckSum[i][m_nBitFile[j + 2]];

	nKnightFileAroundCheckSum = 
		m_nKnightAroundCheckSum[m_nAroundCheckSum[nKnightFile0AroundCheckSum][nKnightFile1AroundCheckSum]][m_nAroundCheckSum[nKnightFile2AroundCheckSum][nKnightFile3AroundCheckSum]];
	nKnightLegCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBitFile[j]]][m_nFileRankAroundCheckSum[j][m_nBitRank[i]]];
	
	//�������߷�
	pcm = m_pcmKnightNoCapPreMoves[p][nKnightFileAroundCheckSum][nKnightLegCheckSum];
	nCount = pcm.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= MAXKNIGHTNOCAPMOVECNT);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcm.pMoveList, m_nBytes[nCount]);
	m_nMovesCnt += nCount;
}

/*******************************************************************************************************
������ : GenCannonNoCapMoves
��  �� : 
��  �� : 
        FSINT p      : 
		FSINT nDepth : 
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��10��23�� 17:16:09
*******************************************************************************************************/
FSVOID CSearchEngine::GenCannonNoCapMoves(FSINT p, FSINT nDepth)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);
	DEBUGASSERT(nDepth >= 0 && nDepth < MAXSEARCHDEPTH);	

	FSINT i, j;
	FSINT nCount;
	FSINT nCannonCheckNum;
	PRECHESSMOVE pcmFile;
	PRECHESSMOVE pcmRank;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	//��
	nCannonCheckNum = m_nCannonFileRankCheckNum[i][m_nBitFile[j]];
	pcmFile = m_pcmCannonFileNoCapPreMoves[p][nCannonCheckNum];
	nCount = pcmFile.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= MAXCANNONFILENOCAPMOVECNT);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcmFile.pMoveList, m_nBytes[nCount]);
	m_nMovesCnt += nCount;

	//��
	nCannonCheckNum = m_nCannonFileRankCheckNum[j][m_nBitRank[i]];
	pcmRank = m_pcmCannonRankNoCapPreMoves[p][nCannonCheckNum];
	nCount = pcmRank.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= MAXCANNONRANKNOCAPMOVECNT);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcmRank.pMoveList, m_nBytes[nCount]);
	m_nMovesCnt += nCount;
}

/*******************************************************************************************************
������ : GenKingNoCapMoves
��  �� : 
��  �� : 
        FSINT p      : 
		FSINT nDepth : 
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��10��23�� 17:16:15
*******************************************************************************************************/
FSVOID CSearchEngine::GenKingNoCapMoves(FSINT p, FSINT nDepth)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);
	DEBUGASSERT(nDepth >= 0 && nDepth < MAXSEARCHDEPTH);

	FSINT i, j;
	FSINT nCount;
	FSINT nPosCheckNum;
	FSINT nAroundCheckSum;
	PRECHESSMOVE pcm;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	nPosCheckNum = m_nKingPosCheckNum[p];
	nAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBitFile[j]]][m_nFileRankAroundCheckSum[j][m_nBitRank[i]]];
	
	pcm = m_pcmKingNoCapPreMoves[nPosCheckNum][nAroundCheckSum];
	nCount = pcm.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= MAXKINGNOCAPMOVECNT);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcm.pMoveList, m_nBytes[nCount]);
	m_nMovesCnt += nCount;
}

/*******************************************************************************************************
������ : GenAdvisorNoCapMoves
��  �� : 
��  �� : 
        FSINT p      : 
		FSINT nDepth : 
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��10��23�� 17:16:21
*******************************************************************************************************/
FSVOID CSearchEngine::GenAdvisorNoCapMoves(FSINT p, FSINT nDepth)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);
	DEBUGASSERT(nDepth >= 0 && nDepth < MAXSEARCHDEPTH);	

	FSINT i, j;
	FSINT nCount;
	FSINT nPosCheckNum;
	FSINT nAroundCheckSum;
	PRECHESSMOVE pcm;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	nPosCheckNum = m_nAdvisorPosCheckNum[p];
	nAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBitFile[m_nAbs[j][1]]]][m_nFileRankAroundCheckSum[i][m_nBitFile[j + 1]]];
	
	pcm = m_pcmAdvisorNoCapPreMoves[nPosCheckNum][nAroundCheckSum];
	nCount = pcm.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 4);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcm.pMoveList, m_nBytes[nCount]);
	m_nMovesCnt += nCount;
}

/*******************************************************************************************************
������ : GenBishopNoCapMoves
��  �� : 
��  �� : 
        FSINT p      : 
		FSINT nDepth : 
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��10��23�� 17:16:26
*******************************************************************************************************/
FSVOID CSearchEngine::GenBishopNoCapMoves(FSINT p, FSINT nDepth)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);
	DEBUGASSERT(nDepth >= 0 && nDepth < MAXSEARCHDEPTH);

	FSINT i, j;
	FSINT nCount;
	FSINT nPosCheckNum;
	FSINT nElephantEyeCheckSum;
	FSINT nAroundCheckSum;
	PRECHESSMOVE pcm;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	nPosCheckNum = m_nBishopPosCheckNum[p];
	nAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum2[i][m_nBitFile[m_nAbs[j][2]]]][m_nFileRankAroundCheckSum2[i][m_nBitFile[j + 2]]];
	nElephantEyeCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBitFile[m_nAbs[j][1]]]][m_nFileRankAroundCheckSum[i][m_nBitFile[j + 1]]];

	pcm = m_pcmBishopNoCapPreMoves[nPosCheckNum][nElephantEyeCheckSum][nAroundCheckSum];
	nCount = pcm.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 4);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcm.pMoveList, m_nBytes[nCount]);
	m_nMovesCnt += nCount;
}

/*******************************************************************************************************
������ : GenPawnNoCapMoves
��  �� : 
��  �� : 
        FSINT p      : 
		FSINT nDepth : 
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��10��23�� 17:16:31
*******************************************************************************************************/
FSVOID CSearchEngine::GenPawnNoCapMoves(FSINT p, FSINT nDepth)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);
	DEBUGASSERT(nDepth >= 0 && nDepth < MAXSEARCHDEPTH);

	FSINT i, j;
	FSINT nCount;
	FSINT nPosCheckNum;
	FSINT nAroundCheckSum;
	PRECHESSMOVE pcm;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	nPosCheckNum = m_nPawnPosCheckNum[m_bIsRed][p];
	nAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBitFile[j]]][m_nPawnRankAroundCheckSum[m_bIsRed][j][m_nBitRank[i]]];

	pcm = m_PawnNoCapPreMoves[m_bIsRed][nPosCheckNum][nAroundCheckSum];
	nCount = pcm.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= MAXPAWNNOCAPMOVECNT);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcm.pMoveList, m_nBytes[nCount]);
	m_nMovesCnt += nCount;
}

/*******************************************************************************************************
������ : GenCapMoves
��  �� : 
��  �� : 
        FSINT nDepth : 
����ֵ : ����Ϊ FSVOID
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��10��4�� 22:27:51
*******************************************************************************************************/
FSVOID CSearchEngine::GenCapMoves(FSINT nDepth)
{
	DEBUGASSERT(nDepth >= 0 && nDepth < MAXSEARCHDEPTH);

	FSINT nPiecePos;

	FSINT nRook;
	FSINT nKnight;
	FSINT nCannon;
	FSINT nAdvisor;
	FSINT nBishop;
	FSINT nPawn;

	nRook = m_nRook[m_bIsRed];
	nKnight = m_nKnight[m_bIsRed];
	nCannon = m_nCannon[m_bIsRed];
	nAdvisor = m_nAdvisor[m_bIsRed];
	nBishop = m_nBishop[m_bIsRed];
	nPawn = m_nPawn[m_bIsRed];

	m_nMovesCnt = 0;	//�߷�������

	//��
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nRook][0]]) != INVALIDPIECEPOS)
	{
		GenRookCapMoves(nPiecePos, nDepth);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nRook][1]]) != INVALIDPIECEPOS)
	{
		GenRookCapMoves(nPiecePos, nDepth);
	}
	
	//��
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nKnight][0]]) != INVALIDPIECEPOS)
	{
		GenKnightCapMoves(nPiecePos, nDepth);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nKnight][1]]) != INVALIDPIECEPOS)
	{
		GenKnightCapMoves(nPiecePos, nDepth);
	}
	
	//��
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nCannon][0]]) != INVALIDPIECEPOS)
	{
		GenCannonCapMoves(nPiecePos, nDepth);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nCannon][1]]) != INVALIDPIECEPOS)
	{
		GenCannonCapMoves(nPiecePos, nDepth);
	}
	
	//��
	GenKingCapMoves(m_nPiecePos[m_nKingID2PieceID[m_bIsRed]], nDepth);
	
	//ʿ
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nAdvisor][0]]) != INVALIDPIECEPOS)
	{
		GenAdvisorCapMoves(nPiecePos, nDepth);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nAdvisor][1]]) != INVALIDPIECEPOS)
	{
		GenAdvisorCapMoves(nPiecePos, nDepth);
	}
	
	//��
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nBishop][0]]) != INVALIDPIECEPOS)
	{
		GenBishopCapMoves(nPiecePos, nDepth);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nBishop][1]]) != INVALIDPIECEPOS)
	{
		GenBishopCapMoves(nPiecePos, nDepth);
	}
	
	//��
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nPawn][0]]) != INVALIDPIECEPOS)
	{
		GenPawnCapMoves(nPiecePos, nDepth);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nPawn][1]]) != INVALIDPIECEPOS)
	{
		GenPawnCapMoves(nPiecePos, nDepth);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nPawn][2]]) != INVALIDPIECEPOS)
	{
		GenPawnCapMoves(nPiecePos, nDepth);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nPawn][3]]) != INVALIDPIECEPOS)
	{
		GenPawnCapMoves(nPiecePos, nDepth);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nPawn][4]]) != INVALIDPIECEPOS)
	{
		GenPawnCapMoves(nPiecePos, nDepth);
	}
}

/*******************************************************************************************************
������ : GenRookCapMoves
��  �� : 
��  �� : 
        FSINT p      : 
		FSINT nDepth : 
����ֵ : ����Ϊ FSVOID
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��10��4�� 22:27:55
*******************************************************************************************************/
inline FSVOID CSearchEngine::GenRookCapMoves(FSINT p, FSINT nDepth)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);
	DEBUGASSERT(nDepth >= 0 && nDepth < MAXSEARCHDEPTH);

	FSINT i, j;
	FSINT nCount;
	FSINT nRookCheckNum;//��У�����
	FSINT nRookFoeCheckNum;
	PRECHESSMOVE pcmFile;
	PRECHESSMOVE pcmRank;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;
	
	//��
	nRookCheckNum = m_nRookFileRankCheckNum[i][m_nBitFile[j]];
	nRookFoeCheckNum = m_nRookFoeFileRankCheckNum[i][nRookCheckNum][m_nBRBitFile[1 - m_bIsRed][j]];
	pcmFile = m_pcmRookFileCapPreMoves[p][nRookCheckNum][nRookFoeCheckNum];
	nCount = pcmFile.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= MAXROOKFILECAPMOVECNT);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcmFile.pMoveList, m_nBytes[nCount]);
	m_nMovesCnt += nCount;

	//��
	nRookCheckNum = m_nRookFileRankCheckNum[j][m_nBitRank[i]];
	nRookFoeCheckNum = m_nRookFoeFileRankCheckNum[j][nRookCheckNum][m_nBRBitRank[1 - m_bIsRed][i]];
	pcmRank = m_pcmRookRankCapPreMoves[p][nRookCheckNum][nRookFoeCheckNum];
	nCount = pcmRank.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= MAXROOKRANKCAPMOVECNT);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcmRank.pMoveList, m_nBytes[nCount]);
	m_nMovesCnt += nCount;
}

/*******************************************************************************************************
������ : GenKnightCapMoves
��  �� : 
��  �� : 
        FSINT p      : 
		FSINT nDepth : 
����ֵ : ����Ϊ FSVOID
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��10��4�� 22:28:01
*******************************************************************************************************/
inline FSVOID CSearchEngine::GenKnightCapMoves(FSINT p, FSINT nDepth)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);
	DEBUGASSERT(nDepth >= 0 && nDepth < MAXSEARCHDEPTH);

	FSINT i, j;
	FSINT nCount;
	PRECHESSMOVE pcm;
	FSINT nKnightLegCheckSum;

	FSINT nKnightFoeFile0AroundCheckSum;
	FSINT nKnightFoeFile1AroundCheckSum;
	FSINT nKnightFoeFile2AroundCheckSum;
	FSINT nKnightFoeFile3AroundCheckSum;
	FSINT nKnightFoeFileAroundCheckSum;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	nKnightFoeFile0AroundCheckSum = m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][m_nAbs[j][2]]];
	nKnightFoeFile1AroundCheckSum = m_nFileRankAroundCheckSum2[i][m_nBRBitFile[1 - m_bIsRed][m_nAbs[j][1]]];
	nKnightFoeFile2AroundCheckSum = m_nFileRankAroundCheckSum2[i][m_nBRBitFile[1 - m_bIsRed][j + 1]];
	nKnightFoeFile3AroundCheckSum = m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][j + 2]];

	nKnightFoeFileAroundCheckSum = 
		m_nKnightAroundCheckSum[m_nAroundCheckSum[nKnightFoeFile0AroundCheckSum][nKnightFoeFile1AroundCheckSum]][m_nAroundCheckSum[nKnightFoeFile2AroundCheckSum][nKnightFoeFile3AroundCheckSum]];
	nKnightLegCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBitFile[j]]][m_nFileRankAroundCheckSum[j][m_nBitRank[i]]];

	//�����߷�
	pcm = m_pcmKnightCapPreMoves[p][nKnightFoeFileAroundCheckSum][nKnightLegCheckSum];
	nCount = pcm.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= MAXKNIGHTCAPMOVECNT);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcm.pMoveList, m_nBytes[nCount]);
	m_nMovesCnt += nCount;
}

/*******************************************************************************************************
������ : GenCannonCapMoves
��  �� : 
��  �� : 
        FSINT p      : 
		FSINT nDepth : 
����ֵ : ����Ϊ FSVOID
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��10��4�� 22:28:05
*******************************************************************************************************/
inline FSVOID CSearchEngine::GenCannonCapMoves(FSINT p, FSINT nDepth)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);
	DEBUGASSERT(nDepth >= 0 && nDepth < MAXSEARCHDEPTH);	

	FSINT i, j;
	FSINT nCount;
	FSINT nCannonCheckNum;
	FSINT nCannonFoeCheckNum;
	PRECHESSMOVE pcmFile;
	PRECHESSMOVE pcmRank;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	//��
	nCannonCheckNum = m_nCannonFileRankCheckNum[i][m_nBitFile[j]];
	nCannonFoeCheckNum =
		m_nCannonFoeFileRankCheckNum[i][nCannonCheckNum][m_nBRBitFile[1 - m_bIsRed][j]];
	pcmFile = m_pcmCannonFileCapPreMoves[p][nCannonCheckNum][nCannonFoeCheckNum];
	nCount = pcmFile.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 2);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcmFile.pMoveList, m_nBytes[nCount]);
	m_nMovesCnt += nCount;

	//��
	nCannonCheckNum = m_nCannonFileRankCheckNum[j][m_nBitRank[i]];
	nCannonFoeCheckNum = 
		m_nCannonFoeFileRankCheckNum[j][nCannonCheckNum][m_nBRBitRank[1 - m_bIsRed][i]];
	pcmRank = m_pcmCannonRankCapPreMoves[p][nCannonCheckNum][nCannonFoeCheckNum];
	nCount = pcmRank.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 2);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcmRank.pMoveList, m_nBytes[nCount]);
	m_nMovesCnt += nCount;
}

/*******************************************************************************************************
������ : GenKingCapMoves
��  �� : 
��  �� : 
        FSINT p      : 
		FSINT nDepth : 
����ֵ : ����Ϊ FSVOID
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��10��4�� 22:28:11
*******************************************************************************************************/
inline FSVOID CSearchEngine::GenKingCapMoves(FSINT p, FSINT nDepth)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);
	DEBUGASSERT(nDepth >= 0 && nDepth < MAXSEARCHDEPTH);

	FSINT i, j;
	FSINT nCount;
	FSINT nPosCheckNum;
	FSINT nFoeAroundCheckSum;
	PRECHESSMOVE pcm;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	nPosCheckNum = m_nKingPosCheckNum[p];
	nFoeAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][j]]][m_nFileRankAroundCheckSum[j][m_nBRBitRank[1 - m_bIsRed][i]]];
	
	pcm = m_pcmKingCapPreMoves[nPosCheckNum][nFoeAroundCheckSum];
	nCount = pcm.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= MAXKINGCAPMOVECNT);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcm.pMoveList, m_nBytes[nCount]);
	m_nMovesCnt += nCount;
}

/*******************************************************************************************************
������ : GenAdvisorCapMoves
��  �� : 
��  �� : 
        FSINT p      : 
		FSINT nDepth : 
����ֵ : ����Ϊ FSVOID
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��10��4�� 22:28:17
*******************************************************************************************************/
inline FSVOID CSearchEngine::GenAdvisorCapMoves(FSINT p, FSINT nDepth)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);
	DEBUGASSERT(nDepth >= 0 && nDepth < MAXSEARCHDEPTH);	

	FSINT i, j;
	FSINT nCount;
	FSINT nPosCheckNum;
	FSINT nFoeAroundCheckSum;
	PRECHESSMOVE pcm;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	nPosCheckNum = m_nAdvisorPosCheckNum[p];
	DEBUGASSERT(nPosCheckNum >= 0);
	nFoeAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][m_nAbs[j][1]]]][m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][j + 1]]];
	
	pcm = m_pcmAdvisorCapPreMoves[nPosCheckNum][nFoeAroundCheckSum];
	nCount = pcm.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= MAXADVISORCAPMOVECNT);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcm.pMoveList, m_nBytes[nCount]);
	m_nMovesCnt += nCount;
}

/*******************************************************************************************************
������ : GenBishopCapMoves
��  �� : 
��  �� : 
        FSINT p      : 
		FSINT nDepth : 
����ֵ : ����Ϊ FSVOID
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��10��4�� 22:28:22
*******************************************************************************************************/
inline FSVOID CSearchEngine::GenBishopCapMoves(FSINT p, FSINT nDepth)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);
	DEBUGASSERT(nDepth >= 0 && nDepth < MAXSEARCHDEPTH);

	FSINT i, j;
	FSINT nCount;
	FSINT nPosCheckNum;
	FSINT nElephantEyeCheckSum;
	FSINT nFoeAroundCheckSum;
	PRECHESSMOVE pcm;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	nPosCheckNum = m_nBishopPosCheckNum[p];
	nFoeAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum2[i][m_nBRBitFile[1 - m_bIsRed][m_nAbs[j][2]]]][m_nFileRankAroundCheckSum2[i][m_nBRBitFile[1 - m_bIsRed][j + 2]]];
	nElephantEyeCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBitFile[m_nAbs[j][1]]]][m_nFileRankAroundCheckSum[i][m_nBitFile[j + 1]]];

	pcm = 
		m_pcmBishopCapPreMoves[nPosCheckNum][nElephantEyeCheckSum][nFoeAroundCheckSum];
	nCount = pcm.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= MAXBISHOPCAPMOVECNT);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcm.pMoveList,
		   m_nBytes[nCount]);
	m_nMovesCnt += nCount;
}

/*******************************************************************************************************
������ : GenPawnCapMoves
��  �� : 
��  �� : 
        FSINT p      : 
		FSINT nDepth : 
����ֵ : ����Ϊ FSVOID
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��10��4�� 22:28:27
*******************************************************************************************************/
inline FSVOID CSearchEngine::GenPawnCapMoves(FSINT p, FSINT nDepth)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);
	DEBUGASSERT(nDepth >= 0 && nDepth < MAXSEARCHDEPTH);

	FSINT i, j;
	FSINT nCount;
	FSINT nPosCheckNum;
	FSINT nFoeAroundCheckSum;
	PRECHESSMOVE pcm;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	nPosCheckNum = m_nPawnPosCheckNum[m_bIsRed][p];
	nFoeAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][j]]][m_nPawnRankAroundCheckSum[m_bIsRed][j][m_nBRBitRank[1 - m_bIsRed][i]]];

	pcm = m_PawnCapPreMoves[m_bIsRed][nPosCheckNum][nFoeAroundCheckSum];
	nCount = pcm.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= MAXPAWNCAPMOVECNT);
	memcpy(m_MoveList[nDepth] + m_nMovesCnt, pcm.pMoveList, m_nBytes[nCount]);
	m_nMovesCnt += nCount;
}

/*******************************************************************************************************
������ : IsInCity
��  �� : 
��  �� : 
        FSINT p : 
����ֵ : FSBOOL
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:39:38
*******************************************************************************************************/
inline FSBOOL CSearchEngine::IsInCity(FSINT p)
{
	if (p < 0 || p > 89)
	{
		return FALSE;
	}

	FSINT i, j;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	return IsInCity(i,j);
}

/*******************************************************************************************************
������ : IsInCity
��  �� : (i, j)�Ƿ��ھŹ���
��  �� : 
        FSINT i : 
		FSINT j : 
����ֵ : inline
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:39:48
*******************************************************************************************************/
inline FSBOOL CSearchEngine::IsInCity(FSINT i, FSINT j)
{
	return (i > 3) && (i <= 5) && ((j >= 0 && j <= 2) || (j >= 7 && j <= 9));
}

/*******************************************************************************************************
������ : IsInBoard
��  �� : 
��  �� : 
        FSINT p : 
����ֵ : inline
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:39:57
*******************************************************************************************************/
inline FSBOOL CSearchEngine::IsInBoard(FSINT p)
{
	return (p >= 0) && (p < 90);
}

/*******************************************************************************************************
������ : IsInBoard
��  �� : 
��  �� : 
        FSINT i : 
		FSINT j : 
����ֵ : inline
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:40:06
*******************************************************************************************************/
inline FSBOOL CSearchEngine::IsInBoard(FSINT i, FSINT j)
{
	return (i >= 0) && (i < 9) && (j >= 0) && (j < 10);
}

/*******************************************************************************************************
������ : IsInBBoard
��  �� : 
��  �� : 
        FSINT p : 
����ֵ : inline
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:40:14
*******************************************************************************************************/
inline FSBOOL CSearchEngine::IsInBBoard(FSINT p)
{
	if (p < 0 || p > 89)
	{
		return FALSE;
	}

	FSINT i, j;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	return IsInBBoard(i,j);
}

/*******************************************************************************************************
������ : IsInRBoard
��  �� : 
��  �� : 
        FSINT p : 
����ֵ : inline
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:40:23
*******************************************************************************************************/
inline FSBOOL CSearchEngine::IsInRBoard(FSINT p)
{
	if (p < 0 || p > 89)
	{
		return FALSE;
	}

	FSINT i, j;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	return IsInRBoard(i, j);
}

/*******************************************************************************************************
������ : IsInBBoard
��  �� : 
��  �� : 
        FSINT i : 
		FSINT j : 
����ֵ : inline
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:40:31
*******************************************************************************************************/
inline FSBOOL CSearchEngine::IsInBBoard(FSINT i, FSINT j)
{
	return IsInBoard(i, j) && (j <= 4);
}

/*******************************************************************************************************
������ : IsInRBoard
��  �� : 
��  �� : 
        FSINT i : 
		FSINT j : 
����ֵ : inline
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:40:40
*******************************************************************************************************/
inline FSBOOL CSearchEngine::IsInRBoard(FSINT i, FSINT j)
{
	return IsInBoard(i, j) && (j > 4);
}

/*******************************************************************************************************
������ : Evaluate
��  �� : ��ֵ����
��  �� : ��
����ֵ : FSINT
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:40:57
*******************************************************************************************************/
inline FSINT CSearchEngine::Evaluate()
{
	FSINT nRetVal;
//	FSINT i, j, k;
//	
//	nRetVal = 0;
//
//	//�췽
////	for (i = 0; i < 16; i++)
//	{
//		j = m_nPiecePos[i];
//		if (j != INVALIDPIECEPOS)
//		{
//			k = m_nPieceID2ChessID[i] - 7;
//			DEBUGASSERT(k >= 0 && k < 8);	// [2005/10/20 19:52:50]
//
//			nRetVal -= g_nBasicValues[k];
//			nRetVal -= g_nPosValues[k][j];
//		}
//	}
//	//�ڷ�
//	for (i = 16; i < 32; i++)
//	{
//		j = m_nPiecePos[i];
//		if (j != INVALIDPIECEPOS)
//		{
//			k = m_nPieceID2ChessID[i];
//			DEBUGASSERT(k >= 0 && k < 8);	// [2005/10/20 19:52:45]
//
//			nRetVal += g_nBasicValues[k];
//			nRetVal += g_nPosValues[k][89 - j];
//		}
//	}
//	
//	nRetVal = m_nBoardValue + GetFlexValue(m_bIsRed) - GetFlexValue(1 - m_bIsRed); //����Թ�ֵ
//	nRetVal = m_nBoardValue + m_nEmptyHeartCannonValue[IsEmptyHeartCannon(m_bIsRed)] - m_nEmptyHeartCannonValue[IsEmptyHeartCannon(1 - m_bIsRed)];
	nRetVal = m_nBoardValue;

	return m_bIsRed ? -nRetVal : nRetVal;
//	return nRetVal - (nRetVal << 1) * m_bIsRed;
//	return nRetVal - (nRetVal << 1) & m_nHave[m_bIsRed];
//	return nRetVal * m_nSign[m_bIsRed];
}

/*******************************************************************************************************
������ : MakeMove
��  �� : 
��  �� : 
        CHESSMOVE cm      : �߷�
		FSINT& nToChessID : Ŀ��λ�õ��������ͱ��
		FSINT& nToPieceID : Ŀ��λ�����ӱ��
����ֵ : ��
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:41:18
*******************************************************************************************************/
inline FSVOID CSearchEngine::MakeMove(CHESSMOVE& cm, FSINT& nToChessID, FSINT& nToPieceID)
{
#ifdef DEBUG
	m_nTestTimer++;
#endif

	FSINT nFrom = cm.nFrom;
	FSINT nTo = cm.nTo;

	FSINT nFromI = m_CGN2Pos[nFrom].x;
	FSINT nFromJ = m_CGN2Pos[nFrom].y;

	FSINT nToI = m_CGN2Pos[nTo].x;
	FSINT nToJ = m_CGN2Pos[nTo].y;

	FSINT nFromChessID;
	FSINT nFromPieceID;

	//nFromPieceID
	//nToPieceID
	nFromPieceID = m_nCBPieceID[nFrom];
	nToPieceID = m_nCBPieceID[nTo];

	//�߷�����ʼλ�ú�Ŀ��λ�ö���������
	DEBUGASSERT(ISINBOARD(nFrom));
	DEBUGASSERT(ISINBOARD(nTo));
	
	//���ӱ�źϷ�
	DEBUGASSERT(nFromPieceID >= MINVALIDPIECEID && nFromPieceID <= MAXVALIDPIECEID);
	DEBUGASSERT(nToPieceID >= MINVALIDPIECEID && nToPieceID <= MAXPIECEID);
	
	//Դ��Ŀ��������ɫ��ͬ,��������ֵ����Ľ����0, 1, 10b, 11b,ֻ��Ϊ0ʱ��������ɫ����ͬ
	DEBUGASSERT((m_bIsBlack_PieceID[nFromPieceID] ^ m_bIsBlack_PieceID[nToPieceID]) > 0);

	//nFromChessID
	//nToChessID
	nFromChessID = m_nCBChessID[nFrom];
	nToChessID = m_nCBChessID[nTo];

	m_nChessCnt[nToChessID]--;	// [2006/1/2 18:53:17]

	//m_nBoardValue
	if (m_bIsRed == RED)
	{
		DEBUGASSERT(nFromChessID >= RBEGIN && nFromChessID <= REND);
		DEBUGASSERT((nToChessID >= BBEGIN && nToChessID <= BEND) || nToChessID == NOCHESS);

		m_nBoardValue -= g_nBasicValues[nToChessID];	//��ȥĿ��λ�úڷ�

		m_nBoardValue += g_nPosValues[nFromChessID - 7][nFrom];		//����Դλ�ú췽
		m_nBoardValue -= g_nPosValues[nFromChessID - 7][nTo];		//��ȥĿ��λ�ú췽
		m_nBoardValue -= g_nPosValues[nToChessID][89 - nTo];		//��ȥĿ��λ�úڷ�
	}
	else
	{
		DEBUGASSERT((nFromChessID >= BBEGIN && nFromChessID <= BEND) || nFromChessID == NOCHESS);
		DEBUGASSERT((nToChessID >= RBEGIN && nToChessID <= REND) || nToChessID == NOCHESS);

		m_nBoardValue += g_nBasicValues[g_nChessID2Chess[nToChessID]];	//����Ŀ��λ�ú췽

		m_nBoardValue -= g_nPosValues[nFromChessID][89 - nFrom];			//��ȥԴλ�úڷ�
		m_nBoardValue += g_nPosValues[nFromChessID][89 - nTo];				//����Ŀ��λ�úڷ�
		m_nBoardValue += g_nPosValues[g_nChessID2Chess[nToChessID]][nTo];	//����Ŀ��λ�ú췽
	}

	ChangeSide();

	//m_nPiecePos
	m_nPiecePos[nFromPieceID] = nTo;
	m_nPiecePos[nToPieceID] = INVALIDPIECEPOS;

	//m_nCBPieceID
	m_nCBPieceID[nFrom] = INVALIDPIECEID;
	m_nCBPieceID[nTo] = nFromPieceID;

	//m_nBitFile
	m_nBitFile[nFromJ] &= m_nReverseMask[nFromI];
	m_nBitFile[nToJ] |= m_nMask[nToI];

	//m_nBitRank
	m_nBitRank[nFromI] &= m_nReverseMask[nFromJ];
	m_nBitRank[nToI] |= m_nMask[nToJ];

	//m_nCBChessID
	m_nCBChessID[nFrom] = NOCHESS;
	m_nCBChessID[nTo] = nFromChessID;
	
	//m_nBRBitFile:ע��,bug�׷���
	//ȥ��nFromJλ���ϵ�nFromλ���ϵ�����
	m_nBRBitFile[m_bIsRed_PieceID[nFromPieceID]][nFromJ] &= m_nReverseMask[nFromI];
	//���nToJλ���ϵ�nFromλ���ϵ�����
	m_nBRBitFile[m_bIsRed_PieceID[nFromPieceID]][nToJ] |= m_nMask[nToI];
	//ȥ��nToJλ���ϵ�nToλ���ϵ�����,������ڵĻ�
	m_nBRBitFile[m_bIsRed_PieceID[nToPieceID]][nToJ] &= m_nReverseMask[nToI];

	//m_nBRBitRank:ע��,bug�׷���
	//ȥ��nFromIλ���ϵ�nFromλ���ϵ�����
	m_nBRBitRank[m_bIsRed_PieceID[nFromPieceID]][nFromI] &= m_nReverseMask[nFromJ];
	//���nToIλ���ϵ�nFromλ���ϵ�����
	m_nBRBitRank[m_bIsRed_PieceID[nFromPieceID]][nToI] |= m_nMask[nToJ];
	//ȥ��nToIλ���ϵ�nToλ���ϵ�����,������ڵĻ�
	m_nBRBitRank[m_bIsRed_PieceID[nToPieceID]][nToI] &= m_nReverseMask[nToJ];

	//m_nHashTabIndex
	m_nHashTabIndex ^= m_nHashKey32[nFromChessID][nFromI][nFromJ];	//ȥ��ԭ����nFrom
	m_nHashTabIndex ^= m_nHashKey32[NOCHESS][nFromI][nFromJ];		//�����µ�nFrom
	m_nHashTabIndex ^= m_nHashKey32[nToChessID][nToI][nToJ];		//ȥ��ԭ����nTo
	m_nHashTabIndex ^= m_nHashKey32[nFromChessID][nToI][nToJ];		//�����µ�nTo

	//m_nCBCheckSum
	m_nCBCheckSum ^= m_nHashKey64[nFromChessID][nFromI][nFromJ];	//ȥ��ԭ����nFrom
	m_nCBCheckSum ^= m_nHashKey64[NOCHESS][nFromI][nFromJ];			//�����µ�nFrom
	m_nCBCheckSum ^= m_nHashKey64[nToChessID][nToI][nToJ];			//ȥ��ԭ����nTo
	m_nCBCheckSum ^= m_nHashKey64[nFromChessID][nToI][nToJ];		//�����µ�nTo
	
	// ���ڳ������[2006/1/2 19:26:50]
	m_nCurDepth++;
//	if (IsChecked(1 - m_bIsRed) == TRUE)
//	{
//		cm.nKind = CHECK;
//	}
	cm.nKind = m_nCheck[IsChecked(1 - m_bIsRed)][cm.nKind];		// [2006/1/2 19:33:08]

	DEBUGASSERT(m_nCurDepth < MAXSEARCHDEPTH);
	m_cmFight[m_nCurDepth] = cm;
#ifdef GETMAXSCHDPT
	if (m_nCurDepth > m_nMaxSearchDepth)
	{
		m_nMaxSearchDepth = m_nCurDepth;
	}
#endif

	// ���ڼ���ظ�����[2005/11/10 16:18:45]
	if (m_nRepHashTab[m_nHashTabIndex & m_nRepHashTabMask] == -1)
	{
		m_nRepHashTab[m_nHashTabIndex & m_nRepHashTabMask] = m_nCurDepth;
	}

	m_nHistoryCBCheckSum[m_nCurDepth] = m_nCBCheckSum;

	if (nToPieceID == INVALIDPIECEID)
	{
		m_nNoEatMoveCnt_IS++;
	}
	else
	{
		m_nNoEatMoveCnt_IS = 0;
		m_nNoEatMoveCnt_IS_Backup = m_nNoEatMoveCnt_IS;
	}
}

/*******************************************************************************************************
������ : UnMakeMove
��  �� : 
��  �� : 
        CHESSMOVE cm	 : �߷�
		FSINT nToChessID : Ŀ��λ�õ��������ͱ��(�����Ŀ��ָ���ǲ����߷�ʱ��Ŀ��,cm���ǲ����߷�ʱ���߷�)
		FSINT nToPieceID : Ŀ��λ�����ӱ��(�����Ŀ��ָ���ǲ����߷�ʱ��Ŀ��,cm���ǲ����߷�ʱ���߷�)
����ֵ : ��
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:41:28
*******************************************************************************************************/
inline FSVOID CSearchEngine::UnMakeMove(const CHESSMOVE& cm, FSINT nToChessID, FSINT nToPieceID)
{
	DEBUGASSERT(cm.nFrom >= 0 && cm.nFrom < MAXCHESSGRIDNUM);
	DEBUGASSERT(cm.nTo >= 0 && cm.nTo < MAXCHESSGRIDNUM);

	DEBUGASSERT(nToChessID >= MINVALIDCHESSID && nToChessID <= MAXVALIDCHESSID);
	DEBUGASSERT(nToPieceID >= MINVALIDPIECEID && nToPieceID <= MAXPIECEID);

#ifdef DEBUG
//	m_nTestTimer++;
#endif

	FSINT nFrom = cm.nFrom;
	FSINT nTo = cm.nTo;

	FSINT nFromI = m_CGN2Pos[nFrom].x;
	FSINT nFromJ = m_CGN2Pos[nFrom].y;

	FSINT nToI = m_CGN2Pos[nTo].x;
	FSINT nToJ = m_CGN2Pos[nTo].y;

	ChangeSide();

	//m_nCBChessID
	m_nCBChessID[nFrom] = m_nCBChessID[nTo];
	m_nCBChessID[nTo] = nToChessID;

	//ע��:m_nPiecePos��m_nCBPieceID�ĸ��²��ܵߵ�˳��,�����ߵ�˳��,����Ӧ�ĸ��´���ҲҪ�ı�
	//׼ȷ�Ľ�:m_nPiecePos�ĸ���������m_nCBPieceID,��m_nCBPieceID�ĸ��²�������m_nPiecePos
	//m_nPiecePos
	m_nPiecePos[m_nCBPieceID[nTo]] = nFrom;
	m_nPiecePos[nToPieceID] = nTo;

	//m_nCBPieceID
	m_nCBPieceID[nFrom] = m_nCBPieceID[nTo];
	m_nCBPieceID[nTo] = nToPieceID;

	//m_nBitFile
	//ע��:bug�׷���
	m_nBitFile[nToJ] = m_nBitFile[nToJ] & 
		m_nReverseMask[nToI] | (m_nPieceID2Bit[nToPieceID] << nToI);
	m_nBitFile[nFromJ] |= m_nMask[nFromI];

	//m_nBitRank
	//ע��:bug�׷���
	m_nBitRank[nToI] = m_nBitRank[nToI] & 
		m_nReverseMask[nToJ] | (m_nPieceID2Bit[nToPieceID] << nToJ);
	m_nBitRank[nFromI] |= m_nMask[nFromJ];

	m_nChessCnt[nToChessID]++;	// [2006/1/2 18:53:17]

	FSINT nFromChessID = m_nCBChessID[nFrom];
	FSINT nFromPieceID = m_nCBPieceID[nFrom];

	//m_nBoardValue
	if (m_bIsRed == RED)
	{
		DEBUGASSERT(nFromChessID >= RBEGIN && nFromChessID <= REND);
		DEBUGASSERT((nToChessID >= BBEGIN && nToChessID <= BEND) || nToChessID == NOCHESS);

		m_nBoardValue += g_nBasicValues[nToChessID];	//����Ŀ��λ�úڷ�

		m_nBoardValue -= g_nPosValues[nFromChessID - 7][nFrom];		//��ȥԴλ�ú췽
		m_nBoardValue += g_nPosValues[nFromChessID - 7][nTo];		//����Ŀ��λ�ú췽
		m_nBoardValue += g_nPosValues[nToChessID][89 - nTo];		//����Ŀ��λ�úڷ�		
	}
	else
	{
		DEBUGASSERT((nFromChessID >= BBEGIN && nFromChessID <= BEND) || nFromChessID == NOCHESS);
		DEBUGASSERT((nToChessID >= RBEGIN && nToChessID <= REND) || nToChessID == NOCHESS);

		m_nBoardValue -= g_nBasicValues[g_nChessID2Chess[nToChessID]];	//��ȥĿ��λ�ú췽

		m_nBoardValue += g_nPosValues[nFromChessID][89 - nFrom];			//����Դλ�úڷ�
		m_nBoardValue -= g_nPosValues[nFromChessID][89 - nTo];				//��ȥĿ��λ�úڷ�
		m_nBoardValue -= g_nPosValues[g_nChessID2Chess[nToChessID]][nTo];	//��ȥĿ��λ�ú췽		
	}

	//m_nBRBitFile
	//ע��:bug�׷���,��m_nBitFile�Ĵ�������ͬ,�м�!!!
	m_nBRBitFile[m_bIsRed_PieceID[nFromPieceID]][nToJ] = 
		m_nBRBitFile[m_bIsRed_PieceID[nFromPieceID]][nToJ] & m_nReverseMask[nToI];
	m_nBRBitFile[m_bIsRed_PieceID[nFromPieceID]][nFromJ] |= m_nMask[nFromI];
	m_nBRBitFile[m_bIsRed_PieceID[nToPieceID]][nToJ] |= m_nMask[nToI];

	//m_nBRBitRank
	//ע��:bug�׷���,��m_nBitRank�Ĵ�������ͬ,�м�!!!
	m_nBRBitRank[m_bIsRed_PieceID[nFromPieceID]][nToI] = 
		m_nBRBitRank[m_bIsRed_PieceID[nFromPieceID]][nToI] & m_nReverseMask[nToJ];
	m_nBRBitRank[m_bIsRed_PieceID[nFromPieceID]][nFromI] |= m_nMask[nFromJ];
	m_nBRBitRank[m_bIsRed_PieceID[nToPieceID]][nToI] |= m_nMask[nToJ];

	// �ظ����������õ�[2005/11/10 16:33:14]
	if (m_nRepHashTab[m_nHashTabIndex & m_nRepHashTabMask] == m_nCurDepth)
	{
		m_nRepHashTab[m_nHashTabIndex & m_nRepHashTabMask] = -1;
	}

	//m_nHashTabIndex
	m_nHashTabIndex ^= m_nHashKey32[NOCHESS][nFromI][nFromJ];		//ȥ��ԭ����nFrom
	m_nHashTabIndex ^= m_nHashKey32[nFromChessID][nFromI][nFromJ];	//�����µ�nFrom
	m_nHashTabIndex ^= m_nHashKey32[nFromChessID][nToI][nToJ];		//ȥ��ԭ����nTo
	m_nHashTabIndex ^= m_nHashKey32[nToChessID][nToI][nToJ];		//�����µ�nTo

	//m_nCBCheckSum
	m_nCBCheckSum ^= m_nHashKey64[NOCHESS][nFromI][nFromJ];			//ȥ��ԭ����nFrom
	m_nCBCheckSum ^= m_nHashKey64[nFromChessID][nFromI][nFromJ];	//�����µ�nFrom
	m_nCBCheckSum ^= m_nHashKey64[nFromChessID][nToI][nToJ];		//ȥ��ԭ����nTo
	m_nCBCheckSum ^= m_nHashKey64[nToChessID][nToI][nToJ];			//�����µ�nTo

	m_nCurDepth--;

	if (nToPieceID == INVALIDPIECEID)
	{
		m_nNoEatMoveCnt_IS--;
	}
	else
	{
		m_nNoEatMoveCnt_IS = m_nNoEatMoveCnt_IS_Backup;
	}
}

/*******************************************************************************************************
������ : SearchInit
��  �� : 
��  �� : ��
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��10��21�� 21:48:24
*******************************************************************************************************/
FSVOID CSearchEngine::SearchInit()
{
	m_esUCCIEgnStatus = EgnSBusy;	//������æ��־

	//�߷�����ʷ�÷ֱ�
	memset(m_nHistoryScore[BLACK], 0, (MAXCHESSGRIDNUM + 1) * (MAXCHESSGRIDNUM + 1) * sizeof(SCORETYPE));
	memset(m_nHistoryScore[RED], 0, (MAXCHESSGRIDNUM + 1) * (MAXCHESSGRIDNUM + 1) * sizeof(SCORETYPE));

//	HASHITEM *pHashItem = &m_pHashTab[m_bIsRed][m_nHashTabIndex & m_nHashTabMask];
//	if (pHashItem->nCBCheckSum == m_nCBCheckSum)
//	{
//		pHashItem->nDepth = 1;
//	}

	HASHITEM hashItem = m_pHashTab[m_bIsRed][m_nHashTabIndex & m_nHashTabMask];
	if (hashItem.nCBCheckSum == m_nCBCheckSum)
	{
		hashItem.nDepth = 1;
	}

	// ���hash��
	ClearHash();

	if (hashItem.nCBCheckSum == m_nCBCheckSum)
	{
		m_pHashTab[m_bIsRed][m_nHashTabIndex & m_nHashTabMask] = hashItem;
	}

	memset(m_nMoveSearchedCnt, 0, MAXCHESSGRIDNUM * MAXCHESSGRIDNUM * sizeof(FSUINT));
	memset(m_nMoveCutoffCnt, 0, MAXCHESSGRIDNUM * MAXCHESSGRIDNUM * sizeof(FSUINT));

	//m_nKillerMoveCnt
	memset(m_nKillerMoveCnt, 0, MAXSEARCHDEPTH * sizeof(FSINT));

	m_bStopThink = FALSE;

//	if (m_bIsHaveCheckMateMove == TRUE)
	if (m_bIsHaveCheckMateMove == TRUE || m_bIsHaveGoodPosition == TRUE)
	{
		m_nCheckExtensionPlies = 2;
	}
	else
	{
		m_nCheckExtensionPlies = 1;
	}

	// [2006/1/10 17:18:56]
	m_bIsHaveCheckMateMove = FALSE;
	m_bIsHaveGoodPosition = FALSE;

	// [2006/1/4 16:53:18]
	if (m_nCurDepth > 0)
	{
		m_cmFight[0] = m_cmFight[m_nCurDepth];
		m_nCurDepth = 0;
	}
	else
	{
		m_cmFight[0] = m_cmNullMove;
	}

	m_nSearchCnt++;

	STATEMENT_STAT
	(
	m_nProbeHashCnt = 0;
	m_nRecordHashCnt = 0;
	m_nHitHashCnt = 0;

	m_nProbeQSHashCnt = 0;
	m_nRecordQSHashCnt = 0;
	m_nHitQSHashCnt = 0;
	);

	m_nTimeStartThink = clock();
}

/*******************************************************************************************************
������ : SearchMain
��  �� : 
��  �� : ��
����ֵ : DWORD
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:41:40
*******************************************************************************************************/
DWORD WINAPI CSearchEngine::SearchMain()
{
	// �������ľ���Ƿ�
	if (IsValidPosition() == FALSE)
	{
		return 0;
	}

	// ������Ȼ�����к�
	if (m_nCurDepth > m_nDrawMoves)
	{
		DISPMSG1_ENDL(cout,	"nobestmove");
		return 0;
	}

	FSINT	i;
	FSINT	nEval;
	FSBOOL	bPrintEndL = FALSE;
	FSINT	nLegalNum;
	FSINT	nLegalCnt;

	//����֮ǰ��ʼ��
	SearchInit();

	//�޺Ϸ��߷�
	if ((nLegalCnt = LegalMoves(nLegalNum)) == 0)
	{
		DISPMSG1_ENDL(cout, "nobestmove");
		return 0;
	}
	//����һ�ֺϷ��߷���ֱ�ӳԶԷ���
	else if ((nLegalCnt == 1 && m_bOnlyMoveOn == TRUE) || nLegalCnt == -1)
	{
		m_PVLine.nCount = 1;
		m_PVLine.pMoveList[0] = m_MoveList[0][nLegalNum];

		SearchStop();

		return 0;
	}

	//���������ֿ� [2005/12/7 21:29:34]
	DISPMSG1_ENDL(cout, "info Searching OpenBook...");
//	if (m_bOpenBookOn == TRUE && m_nChessAge == OPENGAME)
//	if (m_bOpenBookOn == TRUE && (m_nGetOpenBookMoveErrCnt <= 1) || m_nBigChessCnt == 12)
	if (m_bOpenBookOn == TRUE && m_nGetOpenBookMoveErrCnt <= 1 && m_nChessAge != END_GAME)
	{
		if (m_nGetOpenBookMoveErrCnt > 1)
		{
			m_nGetOpenBookMoveErrCnt = 0;
		}

		CHESSMOVE cmTemp;
		
		if ((cmTemp = GetMoveFromBook()) != m_cmNullMove)
		{
			m_PVLine.nCount = 1;
			m_PVLine.pMoveList[0] = cmTemp;
			
			SearchStop();
			
			SWITCHEGN(
			DISPMSG1_ENDL(cout,	"info move from book"););
			
			return 0;
		}
	}

	//���Ϊ0���ؾ����ֵ
	if (m_nSearchDepth == 0)
	{
		DISPMSG2_ENDL(cout,	"the position score is ", (nEval = Evaluate()));
		return 0;
	}

	m_nTimeStartThink = clock();

	// ��˼����ʽ����˼�� [2005/10/22 20:39:53]
	if (m_ThinkType == TT_DEPTH)
	{
		//��������
		for (i = 1; i <= m_nSearchDepth; i++)
		{
			DEBUGASSERT(m_nCurDepth == 0);

			if (bPrintEndL == FALSE)
			{
				bPrintEndL = TRUE;
				switch(m_EgnProtocol)
				{
				case EgnPUCCI:
				case EgnPAhei:
					cout << endl;
					break;
				
				case EgnPQianHong:
					STATEMENT_TESTQHP(cout << endl;);
					break;

				default:
					break;
				}
			}

			IDSearchInit();
			m_nCurSearchDepth_ID = i;

			switch(m_EgnProtocol)
			{
			case EgnPUCCI:
			case EgnPAhei:
				cout << endl;
				cout << "info depth " << i << endl;
				break;
				
			case EgnPQianHong:
				STATEMENT_TESTQHP
				(
				cout << endl;
				cout << "info depth " << i << endl;
				);
				break;
				
			default:
				break;
			}

			// [2006/1/8 10:44:50]
//			if (m_nCheckExtensionPlies == 1 && i >= 4 && (m_PVLine.pMoveList[0].nKind & EAT) == 0 && IsCheckMove(m_PVLine.pMoveList[0]) == TRUE)
//			if (m_nCheckExtensionPlies == 1 && i >= 4 && ((m_PVLine.pMoveList[0].nKind & EAT) == 0 && IsCheckMove(m_PVLine.pMoveList[0]) == TRUE) || m_bIsHaveGoodPosition == TRUE)
			if (m_nCheckExtensionPlies == 1 && i >= 4 && 
				(g_nChessID2Chess[m_nCBChessID[m_PVLine.pMoveList[0].nFrom]] == ROOK && 
				(m_PVLine.pMoveList[0].nKind & EAT) == 0 && IsCheckMove(m_PVLine.pMoveList[0]) == TRUE) || m_bIsHaveGoodPosition == TRUE)
			{
				m_nCheckExtensionPlies = 2;
			}

			nEval = Search(0, i, g_nPerCheckValue, -g_nPerCheckValue, m_KillerMoveTab);
//			nEval = Search(0, g_nPerCheckValue, -g_nPerCheckValue, m_KillerMoveTab);
//			nEval = SearchTest(0, i, g_nPerCheckValue, -g_nPerCheckValue, m_KillerMoveTab);
			if (abs(nEval) != g_nTimeOutScore)
			{
				DEBUGASSERT(i >= 0);
				DEBUGASSERT(m_nBytes[i] >= 0);

				SWITCHEGN(
				DISPMSG2_ENDL(cout,	"info time ", MS2S(clock() - m_nTimeStartThink));
				);

				if (abs(nEval) + MAXSEARCHDEPTH >= g_nMateValue)
				{
					m_bIsHaveCheckMateMove = TRUE;
					break;
				}

				// [2006/2/24 9:23:53]
				m_bIsHaveGoodPosition = (abs(nEval) >= g_nGoodPositionValue);
			}
			else
			{
				break;
			}
		}
//		m_nCurSearchDepth_ID = m_nSearchDepth;
//		SearchTest(0, g_nPerCheckValue, -g_nPerCheckValue, m_KillerMoveTab);
//		Search(0, g_nPerCheckValue, -g_nPerCheckValue, m_KillerMoveTab);
	}
	else if (m_ThinkType == TT_TIME)
	{
		FSBOOL bStop = FALSE;

		//��������
		for (i = 1; i <= MAXSEARCHDEPTH; i++)
		{
			DEBUGASSERT(m_nCurDepth == 0);

			if (bPrintEndL == FALSE)
			{
				bPrintEndL = TRUE;
				switch(m_EgnProtocol)
				{
				case EgnPUCCI:
				case EgnPAhei:
					cout << endl;
					break;
				
				case EgnPQianHong:
					STATEMENT_TESTQHP(cout << endl;);
					break;

				default:
					break;
				}
			}

			IDSearchInit();
			m_nCurSearchDepth_ID = i;

			switch(m_EgnProtocol)
			{
			case EgnPUCCI:
			case EgnPAhei:
				cout << endl;
				cout << "info depth " << i << endl;
				break;
				
			case EgnPQianHong:
				STATEMENT_TESTQHP
				(
				cout << endl;
				cout << "info depth " << i << endl;
				);
				break;
				
			default:
				break;
			}

			// [2006/1/8 10:44:50]
			//if (m_nCheckExtensionPlies == 1 && i >= 4 && (m_PVLine.pMoveList[0].nKind & EAT) == 0 && IsCheckMove(m_PVLine.pMoveList[0]) == TRUE)
//			if (m_nCheckExtensionPlies == 1 && i >= 4 && ((m_PVLine.pMoveList[0].nKind & EAT) == 0 && IsCheckMove(m_PVLine.pMoveList[0]) == TRUE) || m_bIsHaveGoodPosition == TRUE)
			if (m_nCheckExtensionPlies == 1 && i >= 4 && 
				(g_nChessID2Chess[m_nCBChessID[m_PVLine.pMoveList[0].nFrom]] == ROOK && 
				(m_PVLine.pMoveList[0].nKind & EAT) == 0 && IsCheckMove(m_PVLine.pMoveList[0]) == TRUE) || m_bIsHaveGoodPosition == TRUE)
			{
				m_nCheckExtensionPlies = 2;
			}

			nEval = Search(0, i, g_nPerCheckValue, -g_nPerCheckValue, m_KillerMoveTab);
//			nEval = Search(0, g_nPerCheckValue, -g_nPerCheckValue, m_KillerMoveTab);
			if (abs(nEval) != g_nTimeOutScore)
			{
				DEBUGASSERT(i >= 0);
				DEBUGASSERT(m_nBytes[i] >= 0);

				SWITCHEGN(
				DISPMSG2_ENDL(cout,	"info time ", MS2S(clock() - m_nTimeStartThink));
				);

				if (abs(nEval) + MAXSEARCHDEPTH >= g_nMateValue)
				{
					m_bIsHaveCheckMateMove = TRUE;
					break;
				}

				// [2006/2/24 9:23:47]
				m_bIsHaveGoodPosition = (abs(nEval) >= g_nGoodPositionValue);
			}
			else
			{
				break;
			}
		}

		if (bStop == TRUE)
		{
			m_nSearchDepth = i - 1;
		}
	}

	SearchStop();

	return 0;
}

/*******************************************************************************************************
������ : Search
��  �� : 
��  �� : 
        FSINT nDepth		: ��ǰ���
		FSINT nSearchDepth	: �������
		FSINT alpha			: 
		FSINT beta			: 
����ֵ : FSINT
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:41:48
*******************************************************************************************************/
FSINT CSearchEngine::Search(FSINT nDepth, FSINT nSearchDepth, FSINT alpha, FSINT beta, KILLERMOVETAB& killerMoveTab)
{
	DEBUGASSERT(nDepth >= 0 && nDepth <= MAXSEARCHDEPTH);
//	DEBUGASSERT(nDepth >= 0);
	DEBUGASSERT(beta <= -g_nPerCheckValue);
	DEBUGASSERT(alpha >= g_nPerCheckValue && alpha <= beta);

	//˫������
	DEBUGASSERT(m_nPiecePos[m_nKingID2PieceID[m_bIsRed]] != INVALIDPIECEPOS);
	DEBUGASSERT(m_nPiecePos[m_nKingID2PieceID[1 - m_bIsRed]] != INVALIDPIECEPOS);

	FSINT	i;
	FSINT	nBestEval = m_nCurDepth - g_nMateValue;
	FSINT	nNewDepth;
	FSINT	nCount;							//�߷���
	FSINT	nEval;							//��ֵ
	FSINT	nToChessID;						//���Ե��������ͱ��
	FSINT	nToPieceID;						//���Ե����ӱ��
	FSINT	nHashFlag = HASHALPHA;			//hash�ڵ��־
	FSBOOL	bCutoff = FALSE;				//�Ƿ񱻽ض�
	FSBOOL	bFindPV = FALSE;				//�Ƿ��ҵ�PV�߷�
	FSINT	nBestMoveNum = 0;				//�õ��߷���,���ڼ�¼��ʷ�÷�
	FSINT	bChecked;						//��ǰ�����Ƿ񱻽���,������ʷ�ü��ͽ�������
	FSBOOL	bFirstMove = TRUE;				//���������߷��Ƿ��ǵ�һ���߷�,������ʷ�ü���
	FSBOOL	bMateThreat = FALSE;			//ɱ����в,����ɱ����в����
	FSBOOL	bSingleResponse = FALSE;		//��һӦ��,���ڵ�һӦ������
//	FSBOOL	bDangerPosition = FALSE;		//Σ�վ���,������в����������[2006/1/1 11:24:24]
	FSINT	nCanMoveCnt = 0;				//����Ӧ�ŵ��߷���,���ڵ�һӦ����

	CHESSMOVE		cm = m_cmNullMove;
	CHESSMOVE		cmBestMove = m_cmNullMove;
	KILLERMOVETAB	subKillerMoveTab; //ɱ���߷���

	//��ʱ��˼��ʱ,��ʱ����
	if (m_ThinkType == TT_TIME && (clock() - m_nTimeStartThink) > m_nTimeThinkLimit && 
		(m_bPonder == FALSE || m_bPonderHit == TRUE))
	{
		SearchStop();
#ifdef _WIN32
		ExitThread(0);
#else
#endif
	}

	if (nDepth > 0)
	{
		//ɱ��,ֱ�ӷ���
		if (nBestEval >= beta)
		{
			return nBestEval;
		}
		
		//�ظ����
		//	if ((nEval = IsRep(1)) != FALSE)
		//	if ((nEval = IsRep(3)) != FALSE)
		//	if ((nEval = IsRep(nDepth <= 1 ? 3 : 1)) != FALSE)
		if ((nEval = IsRep(nDepth == 1 ? 3 : 1)) != FALSE)
		{
			return RepValue(nEval);
		}
		
		//����ü�
		if (IsDraw() == TRUE)
		{
			return DrawValue();
		}
		
		// ��ֹ�������[2005/11/2 13:14:45]
		if (nDepth >= MAXSEARCHDEPTH)
		{
			return Evaluate();
		}
		
		//���ڵ㴦���þ�̬����
		if (nDepth >= nSearchDepth)
		{
//			bDangerPosition = IsEmptyHeartCannon(1 - m_bIsRed);
//			if (bDangerPosition == TRUE)
//			{
//				return Search(nDepth + 1, nDepth + 2, alpha, beta, subKillerMoveTab);
//			}
			return QuiesSearch(nDepth + 1, alpha, beta);
			//		return Evaluate();
		}

//		if (nDepth == 1)
//		{
//			if (m_cmFight[1] == m_PVLine.pMoveList[0])
//			{
////				nSearchDepth += 2;
//				nSearchDepth += 1;
//			}
//		}
	}

#ifdef STAT
	DEBUGASSERT(m_nVistNodeCnt >= 0 && m_nVistNodeCnt + 1 <= UINT_MAX);

	// ���ʵĽڵ�����1
	m_nVistNodeCnt++;
#endif

	// ̽��hash��
	if ((nEval = ProbeHash(nDepth, nSearchDepth, alpha, beta, cm)) != g_nInvalidValue)
	{
		// ���ڵ����ظ�[2005/11/14 16:55:27]
		if (nDepth > 0)
		{
			return nEval;
		}

		MakeMove(cm, nToChessID, nToPieceID);
		if (IsRep(3) == FALSE)
		{
			UnMakeMove(cm, nToChessID, nToPieceID);

			m_PVLine.nCount = 0;
			GetPVLineFromHash(m_nCurSearchDepth_ID);
			PrintPVLine();
			
			return nEval;
		}
		// ˵���Ѿ�����ѭ��,������û��������Ͳ���������߷�[2005/12/21 22:02:08]
		cm = m_cmNullMove;
		UnMakeMove(cm, nToChessID, nToPieceID);
	}

/*
 ************************************************************
 *                                                          *
 *   try to null move					                    *
 *                                                          *
 ************************************************************
 */
	if (m_bNullMoveOn == TRUE)
	{
		//Null Move
		MakeNullMove();
		
		if (IsChecked(m_bIsRed) == FALSE)
		{
			UnMakeNullMove();
			
			//˫���ż���
			if (nDepth + 2 + m_nDRF_NM < nSearchDepth)
			{
				MakeNullMove();
				MakeNullMove();
				
				nEval = Search(nDepth + 2 + m_nDRF_NM, nSearchDepth, beta - 1, beta, subKillerMoveTab);
				
				UnMakeNullMove();
				UnMakeNullMove();
			}
			else
			{
				nEval = beta;
			}
			
			if (nEval >= beta)
			{
				MakeNullMove();
				
				nEval = -Search(nDepth + 1 + m_nDRF_NM, nSearchDepth, -beta, 1 - beta, subKillerMoveTab);
				
				UnMakeNullMove();
				
				if (nEval >= beta)
				{
					#ifdef STAT
					m_nNullNodeCnt++;
					#endif
					
					return beta;
				}

				if (nEval == m_nCurDepth + 2 - m_nDRF_NM)
				{
					bMateThreat = TRUE;
				}

				//ProbeHash(nDepth, nSearchDepth, g_nPerCheckValue, -g_nPerCheckValue, cm);
			}
			
			//		nEval = -Search(nDepth + 1 + m_nDRF_NM, -beta, 1 - beta, subKillerMoveTab);
			//		
			//		UnMakeNullMove();
			//
			//		if (nEval >= beta)
			//		{
			//#			ifdef STAT
			//			m_nNullNodeCnt++;
			//#			endif
			
			//			return beta;
			//		}
		}
		else
		{
			UnMakeNullMove();
		}
	}

	bChecked = IsChecked(1 - m_bIsRed);
	
	//��һӦ��
//	GenMoves(nDepth);
//	nCount = m_nMovesCnt;
//	for (i = 0; i < nCount; i++)
//	{
//		cm = m_MoveList[nDepth][i];
//
//		MakeMove(cm, nToChessID, nToPieceID);
//
//		//�жϸ��߷��Ƿ�Ϸ�
//		if (IsLegalMove(cm) == FALSE)
//		{
//			UnMakeMove(cm, nToChessID, nToPieceID);
//			continue;
//		}
//
//		nCanMoveCnt++;
//	}
//	if (nCanMoveCnt == 1)
//	{
//		bSingleResponse = TRUE;
//	}

	// [2006/1/6 16:27:50]
	FSBOOL bExtension = FALSE;

	if (bChecked == TRUE)
	{
//		nNewDepth = nSearchDepth + 2;
//		nNewDepth = nSearchDepth + 1;
		nNewDepth = nSearchDepth + m_nCheckExtensionPlies;
	}
	else if (bMateThreat == TRUE || bSingleResponse == TRUE)
	{
		nNewDepth = nSearchDepth + 1;
	}
	else
	{
		nNewDepth = nSearchDepth;

		bExtension = TRUE;
	}
//	nNewDepth = nSearchDepth;

	// �ڵ�����1:Hash Move [2005/10/25 13:05:50]
	// �ڲ���������[2005/10/25 19:19:24]
	if (cm == m_cmNullMove && nDepth + 2 < m_nCurSearchDepth_ID)
	{
		Search(nDepth + 2, nSearchDepth, alpha, beta, killerMoveTab);
		ProbeHash(nDepth, nSearchDepth, alpha, beta, cm);
//		ProbeHash(nDepth, nSearchDepth, g_nPerCheckValue, -g_nPerCheckValue, cm);
	}
	if (cm != m_cmNullMove)
	{
		MakeMove(cm, nToChessID, nToPieceID);

		DEBUGASSERT(IsLegalMove(cm) == TRUE);

		m_nMoveSearchedCnt[cm.nFrom][cm.nTo]++;
		
		// ��������,ɱ����в����,��һӦ������,��������[2005/11/1 18:49:14]
		DEBUGASSERT(m_nCurDepth >= 1);
		if (bExtension == TRUE)
		{
			nNewDepth = (m_cmFight[m_nCurDepth - 1].nKind == EAT && cm.nKind == EAT && m_cmFight[m_nCurDepth - 1].nTo == cm.nTo) ? (nSearchDepth + 1) : nSearchDepth;
//			nNewDepth = (m_cmFight[m_nCurDepth - 1].nKind == EAT && cm.nKind == EAT) ? (nSearchDepth + 1) : nSearchDepth;
		}
//		if (bExtension == TRUE)
//		{
//			if (m_cmFight[m_nCurDepth - 1].nKind == EAT && cm.nKind == EAT && m_cmFight[m_nCurDepth - 1].nTo == cm.nTo)
//			{
//				nNewDepth = nSearchDepth + 1;
//			}
//			else
//			{
		//ͦ������
//				FSINT nFromJ;
//
//				if (m_bIsRed == RED && m_nCBChessID[cm.nTo] == RPAWN)
//				{
//					if (cm.nFrom == cm.nTo + 1)
//					{
//						nFromJ = m_CGN2Pos[cm.nFrom].y;
//						if (nFromJ == 5 || nFromJ == 6)
//						{
//							nNewDepth = nSearchDepth + 1;
//						}
//					}
//				}
//				else if (m_nCBChessID[cm.nTo] == RPAWN)
//				{
//					if (cm.nFrom == cm.nTo - 1)
//					{
//						nFromJ = m_CGN2Pos[cm.nFrom].y;
//						if (nFromJ == 3 || nFromJ == 4)
//						{
//							nNewDepth = nSearchDepth + 1;
//						}
//					}
//				}
//			}
//		}

		nEval = -Search(nDepth + 1, nNewDepth, -beta, -alpha, subKillerMoveTab);
		
		UnMakeMove(cm, nToChessID, nToPieceID);

		bFirstMove = FALSE;

		if (nEval > nBestEval)
		{
			nBestEval = nEval;
			cmBestMove = cm;
			cmBestMove.nScore = nEval;

			if (nEval > alpha)
			{
				alpha = nEval;
				
				if (nBestEval >= beta)
				{
					nHashFlag = HASHBETA;

					killerMoveTab.cmKiller[killerMoveTab.nPointer] = cm;
					killerMoveTab.nPointer = (killerMoveTab.nPointer + 1) & (MAXKILLERMOVECNT - 1);
					if (killerMoveTab.nKillerCount <= MAXKILLERMOVECNT - 1)
					{
						killerMoveTab.nKillerCount++;
					}
					DEBUGASSERT(killerMoveTab.nPointer >= 0 && killerMoveTab.nPointer <= MAXKILLERMOVECNT - 1);
					DEBUGASSERT(killerMoveTab.nKillerCount >= 1 && killerMoveTab.nKillerCount <= MAXKILLERMOVECNT);

					RecordHash(nDepth, nSearchDepth, alpha, nHashFlag, cmBestMove);

					m_nMoveCutoffCnt[cm.nFrom][cm.nTo]++;

					STATEMENT_STAT(m_nBetaNodeCnt++;);
					STATEMENT_STAT(m_nHashBetaNodeCnt++;);

					return alpha;
				}
				
				bFindPV = TRUE;
				nHashFlag = HASHEXACT;

				if (nDepth == 0)
				{
					PrintBetterPV(nDepth, nSearchDepth, nEval, nHashFlag, cmBestMove);
					if (nEval + MAXSEARCHDEPTH >= g_nMateValue)
					{
						return nEval;
					}
				}
			}
		}
	}

	// �ڵ�����2:Capture Move[2005/10/23 18:44:05]
	// �����������߷�[2005/10/22 23:02:02]
	//�������г����߷�
	GenCapMoves(nDepth);
	nCount = m_nMovesCnt;
	DEBUGASSERT(nCount >= 0 && nCount <= MAXALLMOVESCNT);

	//ȡ��ʷ�÷�
	for (i = 0; i < nCount; i++)
	{
		cm = m_MoveList[nDepth][i];
		m_MoveList[nDepth][i].nScore = m_nHistoryScore[m_bIsRed][cm.nFrom][cm.nTo];
	}

	//���߷�����
	sort(m_MoveList[nDepth], m_MoveList[nDepth] + nCount, ::UDgreater);

#ifdef TESTHH
	if (nDepth == 0)
	{
		cout << "The best move history score is " << m_MoveList[nDepth][0].nScore << endl;
	}
#endif

//	bFirstMove = TRUE;

	//ɨ�������߷�
	for (i = 0; i < nCount; i++)
	{
		cm = m_MoveList[nDepth][i];

		if (IsGoodCapMove(cm) == FALSE)
		{
			continue;
		}

		MakeMove(cm, nToChessID, nToPieceID);

		//�жϸ��߷��Ƿ�Ϸ�
		if (IsLegalMove(cm) == FALSE)
		{
			UnMakeMove(cm, nToChessID, nToPieceID);
			continue;
		}

		m_nMoveSearchedCnt[cm.nFrom][cm.nTo]++;
		
		// ��������,ɱ����в����,��һӦ������,��������[2005/11/1 18:49:14]
		DEBUGASSERT(m_nCurDepth >= 1);
		if (bExtension == TRUE)
		{
			nNewDepth = (m_cmFight[m_nCurDepth - 1].nKind == EAT && cm.nKind == EAT && m_cmFight[m_nCurDepth - 1].nTo == cm.nTo) ? (nSearchDepth + 1) : nSearchDepth;
//			nNewDepth = (m_cmFight[m_nCurDepth - 1].nKind == EAT && cm.nKind == EAT) ? (nSearchDepth + 1) : nSearchDepth;
		}
		
		if (bFindPV == TRUE)
		{
			nEval = -Search(nDepth + 1, nNewDepth, -alpha - 1, -alpha, subKillerMoveTab);
			if (nEval > alpha && nEval < beta)
			{
				nEval = -Search(nDepth + 1, nNewDepth, -beta, -alpha, subKillerMoveTab);
			}
		}
		else
		{
			nEval = -Search(nDepth + 1, nNewDepth, -beta, -alpha, subKillerMoveTab);
		}
		
		UnMakeMove(cm, nToChessID, nToPieceID);

		bFirstMove = FALSE;

		if (nEval > nBestEval)
		{
			nBestEval = nEval;
			nBestMoveNum = i;
			cmBestMove = cm;
			cmBestMove.nScore = nEval;

			if (nEval > alpha)
			{
				alpha = nEval;
				
				if (nBestEval >= beta)
				{
					nHashFlag = HASHBETA;
					
					killerMoveTab.cmKiller[killerMoveTab.nPointer] = cm;
					killerMoveTab.nPointer = (killerMoveTab.nPointer + 1) & (MAXKILLERMOVECNT - 1);
					if (killerMoveTab.nKillerCount <= MAXKILLERMOVECNT - 1)
					{
						killerMoveTab.nKillerCount++;
					}
					DEBUGASSERT(killerMoveTab.nPointer >= 0 && killerMoveTab.nPointer <= MAXKILLERMOVECNT - 1);
					DEBUGASSERT(killerMoveTab.nKillerCount >= 1 && killerMoveTab.nKillerCount <= MAXKILLERMOVECNT);

					RecordHash(nDepth, nSearchDepth, alpha, nHashFlag, cmBestMove);

					m_nMoveCutoffCnt[cm.nFrom][cm.nTo]++;
					
					STATEMENT_STAT(m_nBetaNodeCnt++;);
					STATEMENT_STAT(m_nCapMoveBetaNodeCnt++;);

					return alpha;
				}
				
				bFindPV = TRUE;
				nHashFlag = HASHEXACT;

				if (nDepth == 0)
				{
					PrintBetterPV(nDepth, nSearchDepth, nEval, nHashFlag, cmBestMove);
					if (nEval + MAXSEARCHDEPTH >= g_nMateValue)
					{
						return nEval;
					}
				}
			}
		}
	}

	// �ڵ�����3:Killer Move[2005/10/23 18:44:42]
//	bFirstMove = TRUE;

	//ɨ��ɱ���߷�
	nCount = killerMoveTab.nKillerCount;
	for (i = 0; i < nCount; i++)
	{
		cm = killerMoveTab.cmKiller[i];

		if (IsValidMove(cm) == FALSE)
		{
			continue;
		}

		MakeMove(cm, nToChessID, nToPieceID);

		//�жϸ��߷��Ƿ�Ϸ�
		if (IsLegalMove(cm) == FALSE)
		{
			UnMakeMove(cm, nToChessID, nToPieceID);
			continue;
		}

		m_nMoveSearchedCnt[cm.nFrom][cm.nTo]++;

		// ��������,ɱ����в����,��һӦ������,��������[2005/11/1 18:49:14]
		DEBUGASSERT(m_nCurDepth >= 1);
		if (bExtension == TRUE)
		{
			nNewDepth = (m_cmFight[m_nCurDepth - 1].nKind == EAT && cm.nKind == EAT && m_cmFight[m_nCurDepth - 1].nTo == cm.nTo) ? (nSearchDepth + 1) : nSearchDepth;
//			nNewDepth = (m_cmFight[m_nCurDepth - 1].nKind == EAT && cm.nKind == EAT) ? (nSearchDepth + 1) : nSearchDepth;
		}

		if (bFindPV == TRUE)
		{
			nEval = -Search(nDepth + 1, nNewDepth, -alpha - 1, -alpha, subKillerMoveTab);
			if (nEval > alpha && nEval < beta)
			{
				nEval = -Search(nDepth + 1, nNewDepth, -beta, -alpha, subKillerMoveTab);
			}
		}
		else
		{
			nEval = -Search(nDepth + 1, nNewDepth, -beta, -alpha, subKillerMoveTab);
		}
		
		UnMakeMove(cm, nToChessID, nToPieceID);

		bFirstMove = FALSE;

		m_MoveList[nDepth][i].nScore = nEval;

		if (nEval > nBestEval)
		{
			nBestEval = nEval;
			nBestMoveNum = i;
			cmBestMove = cm;
			cmBestMove.nScore = nEval;

			if (nEval > alpha)
			{
				alpha = nEval;
				
				if (nBestEval >= beta)
				{
					nHashFlag = HASHBETA;
					RecordHash(nDepth, nSearchDepth, alpha, nHashFlag, cmBestMove);

					DEBUGASSERT(m_nHistoryScore[m_bIsRed][cmBestMove.nFrom][cmBestMove.nTo] >= 0 && 
						m_nHistoryScore[m_bIsRed][cmBestMove.nFrom][cmBestMove.nTo] + 
						(1 << (nSearchDepth - nDepth)) <= INT_MAX);
					m_nHistoryScore[m_bIsRed][cmBestMove.nFrom][cmBestMove.nTo] += 
						(1 << (nSearchDepth - nDepth));
					
					m_nMoveCutoffCnt[cm.nFrom][cm.nTo]++;

					STATEMENT_STAT(m_nBetaNodeCnt++;);
					STATEMENT_STAT(m_nKillerMoveBetaNodeCnt++;);

					return alpha;
				}
				
				bFindPV = TRUE;
				nHashFlag = HASHEXACT;

				if (nDepth == 0)
				{
					PrintBetterPV(nDepth, nSearchDepth, nEval, nHashFlag, cmBestMove);
					if (nEval + MAXSEARCHDEPTH >= g_nMateValue)
					{
						return nEval;
					}
				}
			}
		}
	}

	// �ڵ�����4:History Move[2005/10/23 18:45:05]
	//�������в������߷�
//	GenNoCapMoves(nDepth);
	GenMoves(nDepth);
	nCount = m_nMovesCnt;
	DEBUGASSERT(nCount >= 0 && nCount <= MAXALLMOVESCNT);

	//ȡ��ʷ�÷�
	for (i = 0; i < nCount; i++)
	{
		cm = m_MoveList[nDepth][i];
		m_MoveList[nDepth][i].nScore = m_nHistoryScore[m_bIsRed][cm.nFrom][cm.nTo];
	}

	//���߷�����
	sort(m_MoveList[nDepth], m_MoveList[nDepth] + nCount, ::UDgreater);

	bFirstMove = TRUE;

	//ɨ�������߷�
	for (i = 0; i < nCount; i++)
	{
		cm = m_MoveList[nDepth][i];

		// [2006/1/5 21:01:57]
//		if ((cm.nKind & EAT) != 0 && IsGoodCapMove(cm) == TRUE)
//		{
//			continue;
//		}

		MakeMove(cm, nToChessID, nToPieceID);

		//�жϸ��߷��Ƿ�Ϸ�
		if (IsLegalMove(cm) == FALSE)
		{
			UnMakeMove(cm, nToChessID, nToPieceID);
			continue;
		}

		m_nMoveSearchedCnt[cm.nFrom][cm.nTo]++;
		
		// ��������,ɱ����в����,��һӦ������,��������[2005/11/1 18:49:14]
		DEBUGASSERT(m_nCurDepth >= 1);
		if (bExtension == TRUE)
		{
			nNewDepth = (m_cmFight[m_nCurDepth - 1].nKind == EAT && cm.nKind == EAT && m_cmFight[m_nCurDepth - 1].nTo == cm.nTo) ? (nSearchDepth + 1) : nSearchDepth;
//			nNewDepth = (m_cmFight[m_nCurDepth - 1].nKind == EAT && cm.nKind == EAT) ? (nSearchDepth + 1) : nSearchDepth;
		}

		DEBUGASSERT(m_nMoveCutoffCnt[cm.nFrom][cm.nTo] <= m_nMoveSearchedCnt[cm.nFrom][cm.nTo]);
		if (m_nMoveCutoffCnt[cm.nFrom][cm.nTo] < (m_nMoveSearchedCnt[cm.nFrom][cm.nTo] >> m_nHistoryShift) && 
			bChecked == FALSE && m_cmFight[m_nCurDepth - 1].nKind != CHECK && cm.nKind != EAT && 
			alpha + 1 == beta && bFirstMove == FALSE && nNewDepth == nSearchDepth)
		{
			nEval = -Search(nDepth + 2, nNewDepth, -beta, -alpha, subKillerMoveTab);
			//if (nEval >= beta)
			if (nEval > alpha)
			{
				nEval = -Search(nDepth + 1, nNewDepth, -beta, -alpha, subKillerMoveTab);
			}
			else
			{
				//m_nMoveCutoffCnt[cm.nFrom][cm.nTo]++;
			}
		}
		else
		{
			if (bFindPV == TRUE)
			{
				nEval = -Search(nDepth + 1, nNewDepth, -alpha - 1, -alpha, subKillerMoveTab);
				if (nEval > alpha && nEval < beta)
				{
					nEval = -Search(nDepth + 1, nNewDepth, -beta, -alpha, subKillerMoveTab);
				}
			}
			else
			{
				nEval = -Search(nDepth + 1, nNewDepth, -beta, -alpha, subKillerMoveTab);
			}
		}
		
		UnMakeMove(cm, nToChessID, nToPieceID);

		bFirstMove = FALSE;

		m_MoveList[nDepth][i].nScore = nEval;

		if (nEval > nBestEval)
		{
			nBestEval = nEval;
			nBestMoveNum = i;
			cmBestMove = cm;
			cmBestMove.nScore = nEval;

			if (nEval > alpha)
			{
				alpha = nEval;
				
				if (nBestEval >= beta)
				{
					nHashFlag = HASHBETA;
					
					killerMoveTab.cmKiller[killerMoveTab.nPointer] = cm;
					killerMoveTab.nPointer = (killerMoveTab.nPointer + 1) & (MAXKILLERMOVECNT - 1);
					if (killerMoveTab.nKillerCount <= MAXKILLERMOVECNT - 1)
					{
						killerMoveTab.nKillerCount++;
					}
					DEBUGASSERT(killerMoveTab.nPointer >= 0 && killerMoveTab.nPointer <= MAXKILLERMOVECNT - 1);
					DEBUGASSERT(killerMoveTab.nKillerCount >= 1 && killerMoveTab.nKillerCount <= MAXKILLERMOVECNT);

					m_nMoveCutoffCnt[cm.nFrom][cm.nTo]++;

					STATEMENT_STAT(m_nBetaNodeCnt++;);
					STATEMENT_STAT(m_nNoCapMoveBetaNodeCnt++;);

					break;
				}
				
				bFindPV = TRUE;
				nHashFlag = HASHEXACT;

				if (nDepth == 0)
				{
					PrintBetterPV(nDepth, nSearchDepth, nEval, nHashFlag, cmBestMove);
					if (nEval + MAXSEARCHDEPTH >= g_nMateValue)
					{
						return nEval;
					}
				}
			}
		}
	}

	if (nHashFlag != HASHALPHA)
	{
		DEBUGASSERT(m_nHistoryScore[m_bIsRed][cmBestMove.nFrom][cmBestMove.nTo] >= 0 && 
			m_nHistoryScore[m_bIsRed][cmBestMove.nFrom][cmBestMove.nTo] + 
			(1 << (nSearchDepth - nDepth)) <= INT_MAX);
		m_nHistoryScore[m_bIsRed][cmBestMove.nFrom][cmBestMove.nTo] += 
			(1 << (nSearchDepth - nDepth));

		STATEMENT_STAT(m_nAlphaNodeCnt++;);

		//����PVLine
//		if (nHashFlag == HASHEXACT)
//		{
//			m_PVLineThink.pMoveList[m_nCurDepth] = cmBestMove;
//			
//			STATEMENT_STAT(m_nPVNodeCnt++;);
//		}
	}

	//��Ҷ�ӽڵ��ֵ���浽hash��
	// TT�洢��Fail Low�����㷨(ZMBL)[2005/12/22 19:48:02]
//	if (nHashFlag == HASHALPHA)
//	{
//		HASHITEM *pHashItem = &m_pHashTab[m_bIsRed][m_nHashTabIndex & m_nHashTabMask];
//
//		if (pHashItem->nCBCheckSum == m_nCBCheckSum && pHashItem->cmBestMove != m_cmNullMove)
//		{
//			cmBestMove = pHashItem->cmBestMove;
//		}
//	}
	RecordHash(nDepth, nSearchDepth, alpha, nHashFlag, cmBestMove);

	return nBestEval;
}

/*******************************************************************************************************
������ : QuiesSearch
��  �� : ��̬����
��  �� : 
        FSINT nDepth : 
		FSINT alpha  : 
		FSINT beta   : 
����ֵ : ����Ϊ FSINT
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��10��5�� 19:24:28
*******************************************************************************************************/
FSINT CSearchEngine::QuiesSearch(FSINT nDepth, FSINT alpha, FSINT beta)
{
	DEBUGASSERT(nDepth >= 0 && nDepth < MAXSEARCHDEPTH);
	DEBUGASSERT(beta <= -g_nPerCheckValue);
	DEBUGASSERT(alpha >= g_nPerCheckValue && alpha <= beta);

	//˫������
	DEBUGASSERT(m_nPiecePos[m_nKingID2PieceID[m_bIsRed]] != INVALIDPIECEPOS);
	DEBUGASSERT(m_nPiecePos[m_nKingID2PieceID[1 - m_bIsRed]] != INVALIDPIECEPOS);

	//��ʱ��˼��ʱ,��ʱ����
	if (m_ThinkType == TT_TIME && (clock() - m_nTimeStartThink) > m_nTimeThinkLimit && 
		(m_bPonder == FALSE || m_bPonderHit == TRUE))
	{
		SearchStop();
#ifdef _WIN32
		ExitThread(0);
#else
#endif
	}

#ifdef STAT

	DEBUGASSERT(m_nVistNodeCnt >= 0 && m_nVistNodeCnt + 1 <= UINT_MAX);

	//���ʵĽڵ�����1
	m_nVistNodeCnt++;

	m_nQuiesNodeCnt++;

#endif

	FSINT	i;
	FSINT	nEval;
	FSINT	nCount;
	FSINT	nToChessID;
	FSINT	nToPieceID;
	FSINT	nHashFlag = HASHALPHA;	//hash�ڵ��־
	FSINT	nBestMoveNum = 0;
	FSINT	nBestEval = nDepth - g_nMateValue;

	CHESSMOVE cm;
	CHESSMOVE cmBestMove;

	//ɱ��,ֱ�ӷ���
	if (nBestEval >= beta)
	{
		return nBestEval;
	}

	//�ظ����
	if ((nEval = IsRep(1)) != FALSE)
//	if ((nEval = IsRep(3)) != FALSE)
	{
		return RepValue(nEval);
	}

	//����ü�
	if (IsDraw() == TRUE)
	{
		return DrawValue();
	}

	//̽��hash��
	if ((nEval = ProbeQSHash(alpha, beta)) != g_nInvalidValue)
	{
		return nEval;
	}

	// ��ֹ�������[2005/11/2 13:14:45]
	if (nDepth >= MAXSEARCHDEPTH)
	{
		return Evaluate();
	}

	//���������,����Ҫ���������߷��⽫
	if (IsChecked(1 - m_bIsRed) == TRUE)
	{
		//�������г����߷�
		GenCapMoves(nDepth);
		nCount = m_nMovesCnt;
		DEBUGASSERT(nCount >= 0 && nCount <= MAXCAPMOVECNT);
		
		//ȡ�߷��÷� [2005/10/20 23:14:14]
		for (i = 0; i < nCount; i++)
		{
			cm = m_MoveList[nDepth][i];
			
			//�����ӵļ�ֵ - ���ӵļ�ֵ,��MVV - LVA
			//		m_MoveList[nDepth][i].nScore = 
			//			g_nBasicValues[g_nChessID2Chess[m_nCBChessID[cm.nTo]]] - cm.nScore;
			
			//�����ӵļ�ֵ,���ӵļ�ֵ,���ȱȽϱ����ӵļ�ֵ,�ٱȽϳ��ӵļ�ֵ
			m_MoveList[nDepth][i].nScore = 
				(g_nBasicValues[g_nChessID2Chess[m_nCBChessID[cm.nTo]]] << 16) | cm.nScore;
		}
		
		//���߷�����
		sort(m_MoveList[nDepth], m_MoveList[nDepth] + nCount, ::UDgreater);
		
		//ɨ�����в������߷�
		for (i = 0; i < nCount; i++)
		{
			cm = m_MoveList[nDepth][i];
			
			if (IsGoodCapMove(cm) == FALSE)
			{
				continue;
			}
			
			MakeMove(cm, nToChessID, nToPieceID);
			
			//�жϸ��߷��Ƿ�Ϸ�
			if (IsLegalMove(cm) == FALSE)
			{
				UnMakeMove(cm, nToChessID, nToPieceID);
				continue;
			}
			
			nEval = -QuiesSearch(nDepth + 1, -beta, -alpha);
			
			UnMakeMove(cm, nToChessID, nToPieceID);
			
			//��ʱ
			if (nEval == -g_nTimeOutScore)
			{
				return g_nTimeOutScore;
			}
			
			if (nEval > nBestEval)
			{
				nBestEval = nEval;
				if (nEval > alpha)
				{
					alpha = nEval;
					
					nBestMoveNum = i;
					cmBestMove = cm;
					
					if (alpha >= beta)
					{
						nHashFlag = HASHBETA;
						RecordQSHash(alpha, nHashFlag, cmBestMove);

						INCREMENT_STAT(m_nQuiesBetaNodeCnt);
						return alpha;
					}
					
					nHashFlag = HASHEXACT;
				}
			}
		}
		
		//�������в������߷�
//		GenNoCapMoves(nDepth);
		GenMoves(nDepth);
		nCount = m_nMovesCnt;
		DEBUGASSERT(nCount >= 0 && nCount <= MAXCAPMOVECNT);
		
		//ȡ��ʷ�÷�
		for (i = 0; i < nCount; i++)
		{
			cm = m_MoveList[nDepth][i];
			m_MoveList[nDepth][i].nScore = m_nHistoryScore[m_bIsRed][cm.nFrom][cm.nTo];
		}

		//ȡ�߷��÷� [2005/10/20 23:14:14]
//		for (i = 0; i < nCount; i++)
//		{
//			cm = m_MoveList[nDepth][i];
//			
//			//�����ӵļ�ֵ - ���ӵļ�ֵ,��MVV - LVA
//			//		m_MoveList[nDepth][i].nScore = 
//			//			g_nBasicValues[g_nChessID2Chess[m_nCBChessID[cm.nTo]]] - cm.nScore;
//			
//			//�����ӵļ�ֵ,���ӵļ�ֵ,���ȱȽϱ����ӵļ�ֵ,�ٱȽϳ��ӵļ�ֵ
//			m_MoveList[nDepth][i].nScore = 
//				(g_nBasicValues[g_nChessID2Chess[m_nCBChessID[cm.nTo]]] << 16) | cm.nScore;
//		}
		
		//���߷�����
		sort(m_MoveList[nDepth], m_MoveList[nDepth] + nCount, ::UDgreater);
		
		//ɨ�����в������߷�
		for (i = 0; i < nCount; i++)
		{
			cm = m_MoveList[nDepth][i];
			
			MakeMove(cm, nToChessID, nToPieceID);
			
			//�жϸ��߷��Ƿ�Ϸ�
			if (IsLegalMove(cm) == FALSE)
			{
				UnMakeMove(cm, nToChessID, nToPieceID);
				continue;
			}
			
			nEval = -QuiesSearch(nDepth + 1, -beta, -alpha);
			
			UnMakeMove(cm, nToChessID, nToPieceID);
			
			if (nEval > nBestEval)
			{
				nBestEval = nEval;
				if (nEval > alpha)
				{
					alpha = nEval;
					
					nBestMoveNum = i;
					cmBestMove = cm;
					
					if (alpha >= beta)
					{
						nHashFlag = HASHBETA;
						INCREMENT_STAT(m_nQuiesBetaNodeCnt);
						break;
					}
					
					nHashFlag = HASHEXACT;
				}
			}
		}

		INCREMENT_STAT(nHashFlag == HASHALPHA ? m_nQuiesAlphaNodeCnt : m_nQuiesPVNodeCnt);

		RecordQSHash(alpha, nHashFlag, cmBestMove);

		return nBestEval;
	}

	nEval = Evaluate();
	if (nEval > nBestEval)
	{
		nBestEval = nEval;
		if (nEval > alpha)
		{
			alpha = nEval;
			
			if (alpha >= beta)
			{
				//RecordQSHash(beta, HASHBETA, m_cmNullMove);
				INCREMENT_STAT(m_nQuiesNullNodeCnt);

				return beta;
			}
			
			nHashFlag = HASHEXACT;
		}
	}

	//�������г����߷�
	GenCapMoves(nDepth);
	nCount = m_nMovesCnt;
	DEBUGASSERT(nCount >= 0 && nCount <= MAXCAPMOVECNT);

	//ȡ�߷��÷� [2005/10/20 23:14:14]
	for (i = 0; i < nCount; i++)
	{
		cm = m_MoveList[nDepth][i];

		//�����ӵļ�ֵ - ���ӵļ�ֵ,��MVV - LVA
//		m_MoveList[nDepth][i].nScore = 
//			g_nBasicValues[g_nChessID2Chess[m_nCBChessID[cm.nTo]]] - cm.nScore;

		//�����ӵļ�ֵ,���ӵļ�ֵ,���ȱȽϱ����ӵļ�ֵ,�ٱȽϳ��ӵļ�ֵ
		m_MoveList[nDepth][i].nScore = 
			(g_nBasicValues[g_nChessID2Chess[m_nCBChessID[cm.nTo]]] << 16) | cm.nScore;
	}

	//���߷�����
	sort(m_MoveList[nDepth], m_MoveList[nDepth] + nCount, ::UDgreater);

	//ɨ�����г����߷�
	for (i = 0; i < nCount; i++)
	{
		cm = m_MoveList[nDepth][i];

		//���Ǻõĳ����߷�
//		if (IsGoodCapMove(cm) == FALSE)
//		{
//			continue;
//		}

		MakeMove(cm, nToChessID, nToPieceID);

		//�жϸ��߷��Ƿ�Ϸ�
		if (IsLegalMove(cm) == FALSE)
		{
			UnMakeMove(cm, nToChessID, nToPieceID);
			continue;
		}

		nEval = -QuiesSearch(nDepth + 1, -beta, -alpha);

		UnMakeMove(cm, nToChessID, nToPieceID);

		if (nEval > nBestEval)
		{
			nBestEval = nEval;
			if (nEval > alpha)
			{
				alpha = nEval;
				nBestMoveNum = i;
				
				if (alpha >= beta)
				{
					nHashFlag = HASHBETA;
					INCREMENT_STAT(m_nQuiesBetaNodeCnt);
					break;
				}
				
				nHashFlag = HASHEXACT;
			}
		}
	}
	
	cm = m_MoveList[nDepth][nBestMoveNum];
	RecordQSHash(alpha, nHashFlag, cm);
	INCREMENT_STAT(nHashFlag == HASHALPHA ? m_nQuiesAlphaNodeCnt : m_nQuiesPVNodeCnt);

	return nBestEval;
}

/*******************************************************************************************************
������ : IsLegalMove
��  �� : ע��:���������MakeMove����ִ�к�ŵ���!
		 �߷�cm�Ƿ�Ϸ�,�������߷�cm���Ƿ��������,��׽,����������
��  �� : 
        CHESSMOVE cm : �߷�
����ֵ : FSINT
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:42:05
*******************************************************************************************************/
inline FSBOOL CSearchEngine::IsLegalMove(CHESSMOVE cm)
{
	DEBUGASSERT(cm.nFrom >= 0 && cm.nFrom < MAXCHESSGRIDNUM);
	DEBUGASSERT(cm.nTo >= 0 && cm.nTo < MAXCHESSGRIDNUM);
	
	FSINT nBKingPos;
	FSINT nRKingPos;

	nBKingPos = m_nPiecePos[m_nChessID2PieceID[BKING][0]];
	nRKingPos = m_nPiecePos[m_nChessID2PieceID[RKING][0]];

	DEBUGASSERT(nBKingPos != INVALIDPIECEPOS);
	DEBUGASSERT(nRKingPos != INVALIDPIECEPOS);

	//˫���Ƿ�����,1 - m_bIsRed���Ƿ񱻽���,�Ƿ��������,��׽
	return 1 - (m_bIsTwoKingMeet[m_nBRKingPosCheckNum[BLACK][nBKingPos]][m_nBRKingPosCheckNum[RED][nRKingPos]][m_nBitRank[m_CGN2Pos[nBKingPos].x]] | 
		IsChecked(m_bIsRed));
}

/*******************************************************************************************************
������ : IsValidMove
��  �� : �ж��߷�cm�Ƿ�����Ч���߷�,������ɱ���ŷ�������,�ں���MakeMove֮ǰ����
��  �� : 
        CHESSMOVE cm : ɱ���߷�
����ֵ : ����Ϊ FSBOOL
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��10��1�� 20:05:51
*******************************************************************************************************/
inline FSBOOL CSearchEngine::IsValidMove(CHESSMOVE cm)
{
	FSINT nFrom;
	FSINT nTo;

	nFrom = cm.nFrom;
	nTo = cm.nTo;

	//�߷�����ʼλ�ú�Ŀ��λ�ö���������
	DEBUGASSERT(ISINBOARD(nFrom));
	DEBUGASSERT(ISINBOARD(nTo));

	FSINT nFromPieceID;

	//Դ�ޱ�������,����Ҫ��m_bIsRed_PieceID[INVALIDPIECEID] != 0,1
	if (m_bIsRed_PieceID[nFromPieceID = m_nCBPieceID[nFrom]] != m_bIsRed)
	{
		return FALSE;
	}

	FSINT nToPieceID = m_nCBPieceID[nTo];
	
	//Դ��Ŀ��������ɫ��ͬ
	if ((m_bIsBlack_PieceID[nFromPieceID] ^ m_bIsBlack_PieceID[nToPieceID]) == 0)
	{
		return FALSE;
	}

//	FSINT nFromI, nFromJ;
//
//	switch(g_nChessID2Chess[m_nCBChessID[nFrom]])
//	{
//	case ROOK:
//		nFromI = m_CGN2Pos[nFrom].x;
//		nFromJ = m_CGN2Pos[nFrom].y;
//		DEBUGASSERT(m_bIsRookFilePreValidMove[nFrom][nTo][m_nBitFile[nFromJ]] == FALSE || 
//					m_bIsRookFilePreValidMove[nFrom][nTo][m_nBitFile[nFromJ]] == TRUE);
//		DEBUGASSERT(m_bIsRookRankPreValidMove[nFrom][nTo][m_nBitRank[nFromI]] == FALSE || 
//					m_bIsRookRankPreValidMove[nFrom][nTo][m_nBitRank[nFromI]] == TRUE);
//		return m_bIsRookFilePreValidMove[nFrom][nTo][m_nBitFile[nFromJ]] || 
//			   m_bIsRookRankPreValidMove[nFrom][nTo][m_nBitRank[nFromI]];
//
//	case KNIGHT:
//		DEBUGASSERT(nTo - nFrom + 21 >= 0 && nTo - nFrom + 21 < 43);
//		return m_nCBPieceID[nFrom + g_nKnightLegPos[nTo - nFrom + 21]] == INVALIDPIECEID;
//
//	case CANNON:
//		nFromI = m_CGN2Pos[nFrom].x;
//		nFromJ = m_CGN2Pos[nFrom].y;
//		DEBUGASSERT(m_bIsCannonFilePreValidMove[nFrom][nTo][m_nBitFile[nFromJ]][m_nPieceID2Bit[nToPieceID]] == FALSE || 
//					m_bIsCannonFilePreValidMove[nFrom][nTo][m_nBitFile[nFromJ]][m_nPieceID2Bit[nToPieceID]] == TRUE);
//		DEBUGASSERT(m_bIsCannonRankPreValidMove[nFrom][nTo][m_nBitRank[nFromI]][m_nPieceID2Bit[nToPieceID]] == FALSE || 
//					m_bIsCannonRankPreValidMove[nFrom][nTo][m_nBitRank[nFromI]][m_nPieceID2Bit[nToPieceID]] == TRUE);
//		return m_bIsCannonFilePreValidMove[nFrom][nTo][m_nBitFile[nFromJ]][m_nPieceID2Bit[nToPieceID]] || 
//			   m_bIsCannonRankPreValidMove[nFrom][nTo][m_nBitRank[nFromI]][m_nPieceID2Bit[nToPieceID]];
//
//	case KING:
//	case ADVISOR:
//	case PAWN:
//		return TRUE;
//
//	case BBISHOP:
//		return m_nCBPieceID[(nFrom + nTo) >> 1] == INVALIDPIECEID;
//	
//	default:
//		DEBUGASSERT(0 > 1);
//		return FALSE;
//	}

	FSINT nFromI, nFromJ;
	FSINT nToI, nToJ;

	switch(g_nChessID2Chess[m_nCBChessID[nFrom]])
	{
	case ROOK:
		nFromI = m_CGN2Pos[nFrom].x;
		nFromJ = m_CGN2Pos[nFrom].y;
		nToI = m_CGN2Pos[nTo].x;
		nToJ = m_CGN2Pos[nTo].y;
		DEBUGASSERT(m_bIsRookPreValidMove[nFromI][nToI][m_nBitFile[nFromJ]] == FALSE || 
					m_bIsRookPreValidMove[nFromI][nToI][m_nBitFile[nFromJ]] == TRUE);
		DEBUGASSERT(m_bIsRookPreValidMove[nFromJ][nToJ][m_nBitRank[nFromI]] == FALSE || 
					m_bIsRookPreValidMove[nFromJ][nToJ][m_nBitRank[nFromI]] == TRUE);
		return (nFromJ == nToJ && m_bIsRookPreValidMove[nFromI][nToI][m_nBitFile[nFromJ]]) || 
			   (nFromI == nToI && m_bIsRookPreValidMove[nFromJ][nToJ][m_nBitRank[nFromI]]);

	case KNIGHT:
		DEBUGASSERT(nTo - nFrom + 21 >= 0 && nTo - nFrom + 21 < 43);
		return m_nCBPieceID[nFrom + g_nKnightLegPos[nTo - nFrom + 21]] == INVALIDPIECEID;

	case CANNON:
		nFromI = m_CGN2Pos[nFrom].x;
		nFromJ = m_CGN2Pos[nFrom].y;
		nToI = m_CGN2Pos[nTo].x;
		nToJ = m_CGN2Pos[nTo].y;
		DEBUGASSERT(m_bIsCannonPreValidMove[nFromI][nToI][m_nBitFile[nFromJ]][m_nPieceID2Bit[nToPieceID]] == FALSE || 
					m_bIsCannonPreValidMove[nFromI][nToI][m_nBitFile[nFromJ]][m_nPieceID2Bit[nToPieceID]] == TRUE);
		DEBUGASSERT(m_bIsCannonPreValidMove[nFromJ][nToJ][m_nBitRank[nFromI]][m_nPieceID2Bit[nToPieceID]] == FALSE || 
					m_bIsCannonPreValidMove[nFromJ][nToJ][m_nBitRank[nFromI]][m_nPieceID2Bit[nToPieceID]] == TRUE);
		return (nFromJ == nToJ && m_bIsCannonPreValidMove[nFromI][nToI][m_nBitFile[nFromJ]][m_nPieceID2Bit[nToPieceID]]) || 
			   (nFromI == nToI && m_bIsCannonPreValidMove[nFromJ][nToJ][m_nBitRank[nFromI]][m_nPieceID2Bit[nToPieceID]]);

	case KING:
	case ADVISOR:
	case PAWN:
		return TRUE;

	case BBISHOP:
		return m_nCBPieceID[(nFrom + nTo) >> 1] == INVALIDPIECEID;
	
	default:
		DEBUGASSERT(0 > 1);
		return FALSE;
	}
}

/*******************************************************************************************************
������ : IsGoodCapMove
��  �� : �˺���Ҫ��MakeMove��������֮ǰ����,������IsLegalMove�����Ĳ�֮ͬ��
��  �� : 
        CHESSMOVE cm : 
����ֵ : ����Ϊ FSBOOL
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��10��24�� 22:20:10
*******************************************************************************************************/
FSBOOL CSearchEngine::IsGoodCapMove(const CHESSMOVE& cm)
{
	FSINT nFrom = cm.nFrom;
	FSINT nTo = cm.nTo;
	FSINT nToChess;

	nToChess = g_nChessID2Chess[m_nCBChessID[nTo]];

	//�߷�����ʼλ�ú�Ŀ��λ�ö���������
	DEBUGASSERT(ISINBOARD(nFrom));
	DEBUGASSERT(ISINBOARD(nTo));

	//���ܳԽ�
	DEBUGASSERT(nToChess != KING);

	//ʿ��
	if (nToChess == ADVISOR || nToChess == BISHOP)
	{
		return FALSE;
	}

	//������
	if (nToChess == ROOK || nToChess == KNIGHT || nToChess == CANNON)
	{
		return TRUE;
	}

	// ��[2005/10/25 10:39:59]
	return m_bIsRed ? (m_CGN2Pos[nTo].y <= 4) : (m_CGN2Pos[nTo].y >= 5);
}

/*******************************************************************************************************
������ : ProbeHash
��  �� : ̽��hash��
��  �� : 
        FSINT nDepth : 
		FSINT alpha  : 
		FSINT beta   : 
����ֵ : ����Ϊ inline
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��10��18�� 16:42:10
*******************************************************************************************************/
inline FSINT CSearchEngine::ProbeHash(FSINT nDepth, FSINT nSearchDepth, FSINT alpha, FSINT beta, CHESSMOVE& cmBestMove)
{
	HASHITEM hashItem;

	DEBUGASSERT((m_nHashTabIndex & m_nHashTabMask) >= 0 && (m_nHashTabIndex & m_nHashTabMask) <= m_nHashTabMask);
	
	STATEMENT_STAT(m_nProbeHashCnt++;);

	hashItem = m_pHashTab[m_bIsRed][m_nHashTabIndex & m_nHashTabMask];
	if (hashItem.nCBCheckSum == m_nCBCheckSum)
	{
		FSINT nHashVal = hashItem.nEval;

		// ˵����hash���ǳ���ʱ�ľ���,����Ҫ[2005/12/22 18:23:51]
		if (nHashVal > g_nMateValue)
		{
			return g_nInvalidValue;
		}

		//��������߷�
		cmBestMove = hashItem.cmBestMove;
//		if (hashItem.nDepth >= 5)
//		{
//			cmBestMove = hashItem.cmBestMove;
//		}

		if (hashItem.nDepth < nSearchDepth - nDepth)
		{
			return g_nInvalidValue;
		}

		if (hashItem.nHashFlag == HASHEXACT)
		{
			m_PVLineThink.pMoveList[nDepth] = hashItem.cmBestMove;
			STATEMENT_STAT(m_nHitHashCnt++;);
			return nHashVal;
		}

		//hashItem.nHashFlag != HASHEXACT
		if (hashItem.nHashFlag == HASHALPHA && nHashVal <= alpha)
		{
			STATEMENT_STAT(m_nHitHashCnt++;);
			return alpha;
		}

		if (hashItem.nHashFlag == HASHBETA && nHashVal >= beta)
		{
			STATEMENT_STAT(m_nHitHashCnt++;);
			return beta;
		}

		return g_nInvalidValue;
	}

	return g_nInvalidValue;

//	FSINT nRet = GetHash(m_pHashTabByDepth, nDepth, nSearchDepth, alpha, beta, cmBestMove);
//
//	if (nRet != g_nInvalidValue)
//	//if (nRet != g_nInvalidValue || cmBestMove == m_cmNullMove)
//	{
//		return nRet;
//	}
//
//	return GetHash(m_pHashTab, nDepth, nSearchDepth, alpha, beta, cmBestMove);
}

/*******************************************************************************************************
������ : RecordHash
��  �� : ¼��hash��
��  �� : 
        FSINT nDepth    : 
		FSINT nVal      : 
		FSINT nHashFlag : 
����ֵ : ����Ϊ inline
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��10��18�� 16:42:01
*******************************************************************************************************/
inline FSVOID CSearchEngine::RecordHash(FSINT nDepth, FSINT nSearchDepth, FSINT nEval, FSINT nHashFlag, CHESSMOVE cmBestMove)
{
	DEBUGASSERT(cmBestMove == m_cmNullMove || 
		(ISINBOARD(cmBestMove.nFrom) && ISINBOARD(cmBestMove.nTo)));

	STATEMENT_STAT(m_nRecordHashCnt++;);

	// ����滻����[2005/10/31 18:48:11]
//	HASHITEM *pHashItem = &m_pHashTabByDepth[m_bIsRed][m_nHashTabIndex & m_nHashTabMask];
//	
//	if (pHashItem->nDepth <= m_nCurSearchDepth_ID - nDepth)
//	{
//		pHashItem->nDepth = m_nCurSearchDepth_ID - nDepth;
//		pHashItem->nEval = nEval;
//		pHashItem->nHashFlag = nHashFlag;
//		pHashItem->cmBestMove = cmBestMove;
//		pHashItem->nCBCheckSum = m_nCBCheckSum;
//
//		return;
//	}

	// �Ľ�������滻����[2005/10/31 18:48:23]
//	HASHITEM *pHashItem = &m_pHashTab[m_bIsRed][m_nHashTabIndex & m_nHashTabMask];
//
//	if (pHashItem->nCBCheckSum == m_nCBCheckSum && 
//		pHashItem->nDepth >= m_nCurSearchDepth_ID - nDepth)
//	{
//		return;
//	}
//
//	pHashItem->nDepth = m_nCurSearchDepth_ID - nDepth;
//	pHashItem->nEval = nEval;
//	pHashItem->nHashFlag = nHashFlag;
//	pHashItem->cmBestMove = cmBestMove;
//	pHashItem->nCBCheckSum = m_nCBCheckSum;

	HASHITEM hashItem;

	hashItem.nDepth = nSearchDepth - nDepth;
	hashItem.nEval = nEval;
	hashItem.nHashFlag = nHashFlag;
	hashItem.cmBestMove = cmBestMove;
	hashItem.nCBCheckSum = m_nCBCheckSum;
	
	m_pHashTab[m_bIsRed][m_nHashTabIndex & m_nHashTabMask] = hashItem;
}

/*******************************************************************************************************
������ : ProbeQSHash
��  �� : 
��  �� : 
        FSINT alpha : 
		FSINT beta  : 
����ֵ : ����Ϊ FSINT
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��10��24�� 16:04:11
*******************************************************************************************************/
inline FSINT CSearchEngine::ProbeQSHash(FSINT alpha, FSINT beta)
{
	QSHASHITEM hashItem;

	DEBUGASSERT((m_nHashTabIndex & m_nHashTabMask) >= 0 && (m_nHashTabIndex & m_nHashTabMask) <= m_nHashTabMask);
	
	STATEMENT_STAT(m_nProbeQSHashCnt++;);

	hashItem = m_pQSHashTab[m_bIsRed][m_nHashTabIndex & m_nHashTabMask];
	if (hashItem.nCBCheckSum == m_nCBCheckSum)
	{
		FSINT nHashVal = hashItem.nEval;

		// ˵����hash���ǳ���ʱ�ľ���,����Ҫ[2005/12/22 18:23:51]
		if (nHashVal > g_nMateValue)
		{
			return g_nInvalidValue;
		}

		if (hashItem.nHashFlag == HASHEXACT)
		{
			STATEMENT_STAT(m_nHitQSHashCnt++;);
			return nHashVal;
		}

		//hashItem.nHashFlag != HASHEXACT
		if (hashItem.nHashFlag == HASHALPHA && nHashVal <= alpha)
		{
			STATEMENT_STAT(m_nHitQSHashCnt++;);
			return alpha;
		}

		if (hashItem.nHashFlag == HASHBETA && nHashVal >= beta)
		{
			STATEMENT_STAT(m_nHitQSHashCnt++;);
			return beta;
		}
	}

	return g_nInvalidValue;
}

/*******************************************************************************************************
������ : RecordQSHash
��  �� : 
��  �� : 
        FSINT nVal           : 
		FSINT nHashFlag      : 
		CHESSMOVE cmBestMove : 
����ֵ : ����Ϊ inline
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��10��24�� 16:04:03
*******************************************************************************************************/
inline FSVOID CSearchEngine::RecordQSHash(FSINT nVal, FSINT nHashFlag, CHESSMOVE cmBestMove)
{
	STATEMENT_STAT(m_nRecordQSHashCnt++;);

	QSHASHITEM hashItem;

	hashItem.nEval = nVal;
	hashItem.nHashFlag = nHashFlag;
	hashItem.cmBestMove = cmBestMove;
	hashItem.nCBCheckSum = m_nCBCheckSum;
	
	m_pQSHashTab[m_bIsRed][m_nHashTabIndex & m_nHashTabMask] = hashItem;
}

/*******************************************************************************************************
������ : UCCIProc
��  �� : 
��  �� : ��
����ֵ : FSINT
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:42:44
*******************************************************************************************************/
FSVOID CSearchEngine::UCCIProc()
{
	UCCICmd	ucciCmd;
	FSCHAR	*szUCCICmd = new FSCHAR[MAXBUFFERLEN];
	FSINT	nHashItemSize = sizeof(HASHITEM);
	FSCHAR	*szCmdCpy = szUCCICmd;	//szUCCICmd�Ŀ���,�����ͷ�szUCCICmd���ڴ�
	FSINT	nLimitMovesCnt;			//������
	FSINT	nAddTime;				//��ʱ
	FSINT	nTemp;
#ifdef _WIN32
	FSDWORD	(WINAPI CSearchEngine::*p1)() = SearchMain;
	FSDWORD	(WINAPI *p2)(LPVOID);
	FSVOID (CALLBACK CSearchEngine::*pInternalCallBack)
		(FSUINT uID, FSUINT uMsg, FSDWORD dwUser, FSDWORD dw1, FSDWORD dw2) = ThinkTimeOut;
//	LPTIMECALLBACK pTimeOutCallBack;

	memcpy(&p2, &p1, sizeof(p1));
//	memcpy(&pTimeOutCallBack, &pInternalCallBack, sizeof(pInternalCallBack));

#else
#endif
	
	//��ʾ����İ汾��,��Ȩ,���ߺ���Ȩ�û�
	DISPMSG0_ENDL(cout);
	DISPMSG1_ENDL(cout, "id name "ENGINENAME" "ENGINEVERSION);
	DISPMSG1_ENDL(cout, "id copyright 2004 - 2006");
	DISPMSG1_ENDL(cout, "id author "ENGINEAUTHOR);
	DISPMSG1_ENDL(cout, "id user Unknown User");

	//��ʾ������֧�ֵ�ѡ��
	DISPMSG7_ENDL(cout, "option hashsize type spin", 
		" min ", 2 * MINHASHTABSIZE * nHashItemSize / (1 << 20), 
		" max ", 2 * MAXHASHTABSIZE * nHashItemSize / (1 << 20), 
		" default ", 2 * (m_nHashTabMask + 1) * nHashItemSize / (1 << 20));
	DISPMSG3_ENDL(cout, "option bookfiles type string default ", m_szDir, BOOKFILE);

	//��ʾ��Ȩ�����Ϣ
	cout << "copyprotection OK" << endl;
	cout << "ucciok" << endl;

	if (m_bAllocateMem == FALSE)
	{
		Initial();
	}

	LoadFEN((FSCHAR*)g_szMidGamePos);

GetUCCICmd:
	szUCCICmd = szCmdCpy;
	*szUCCICmd = '\0';
	gets(szUCCICmd);
	if (strncmp(szUCCICmd, "", 1))
	{
		STATEMENT_TEST
		(
		CString str;

		str.Format("UCCIЭ����������:%s", szUCCICmd);
		MessageBox(NULL, str, "������ʾ", MB_OK);
		);
	}

	szUCCICmd = JumpNoMatterChar(szUCCICmd);
	ucciCmd = GetUCCICmd(szUCCICmd);
	szUCCICmd = JumpNoMatterChar(szUCCICmd);

	switch(ucciCmd)
	{
	case UCCICIsReady:
		if (m_esUCCIEgnStatus == EgnSBusy || m_esUCCIEgnStatus == EgnSSuspend)
		{
			UCCIBusy();
			break;
		}

		cout << "readyok" << endl;
		break;

	case UCCICPosition:
		if (m_esUCCIEgnStatus == EgnSBusy || m_esUCCIEgnStatus == EgnSSuspend)
		{
			UCCIBusy();
			break;
		}

		//�������,���⴦��
		if (IsSpecialPos(szUCCICmd) == TRUE)
		{
			break;
		}

		if (LoadFEN(szUCCICmd) == FALSE)
		{
			cout <<	"FEN��"	<< szUCCICmd << "�﷨����!" << endl;
			
			LoadFEN((FSCHAR*)g_szOpenGamePos);
		}
		break;

	case UCCICGo:
		if (m_esUCCIEgnStatus == EgnSBusy || m_esUCCIEgnStatus == EgnSSuspend)
		{
			UCCIBusy();
			break;
		}

		//��̨˼��
		if (!strncmp(szUCCICmd, "ponder", 6))
		{
			m_bPonder = TRUE;
			szUCCICmd += 6;
			szUCCICmd = JumpNoMatterChar(szUCCICmd);
		}
		else
		{
			m_bPonder = FALSE;
		}

		if (!strncmp(szUCCICmd, "depth", 5))
		{
			m_ThinkType = TT_DEPTH;		//�����˼��

			szUCCICmd += 5;		//����"depth"
			szUCCICmd = JumpNoMatterChar(szUCCICmd);
			m_nSearchDepth = atoi(szUCCICmd);	//������������
			if (m_nSearchDepth > MAXSEARCHDEPTH)
			{
				cout <<	"������Ȳ��ܳ���" << MAXSEARCHDEPTH << endl;
				break;
			}
			else if (m_nSearchDepth < 0)
			{
				cout <<	"������Ȳ���С��0" << endl;
				break;
			}
		}
		else if (!strncmp(szUCCICmd, "time", 4))
		{
			m_ThinkType = TT_TIME;		//��ʱ��˼��

			szUCCICmd += 4;		//����"time"
			szUCCICmd = JumpNoMatterChar(szUCCICmd);
			m_nTimeThinkLimit = atoi(szUCCICmd);	//����˼��ʱ��
			m_nTimeThinkLimit *= 1000;
			szUCCICmd = JumpDigit(szUCCICmd);
			szUCCICmd = JumpNoMatterChar(szUCCICmd);
			if (szUCCICmd[0] == '\0')
			{
				nLimitMovesCnt = 1;
				m_nTimeThinkLimit /= nLimitMovesCnt;
			}
			else if(strncmp(szUCCICmd, "movestogo", 9) == 0)
			{
				szUCCICmd += 9;	//����"movestogo"
				szUCCICmd = JumpNoMatterChar(szUCCICmd);
				nLimitMovesCnt = atoi(szUCCICmd);
				if (nLimitMovesCnt <= 0)
				{
					nLimitMovesCnt = 1;

					cout << "����������С��0,�����潫��������Ϊ1����" << endl;
				}
				m_nTimeThinkLimit /= nLimitMovesCnt;
			}
			else if (strncmp(szUCCICmd, "increment", 9) == 0)
			{
				szUCCICmd += 9;	//����"increment"
				szUCCICmd = JumpNoMatterChar(szUCCICmd);
				nAddTime = atoi(szUCCICmd);
				m_nTimeThinkLimit = (m_nTimeThinkLimit + nAddTime * 1000) / 20;
			}

//			#ifdef _WIN32
//			if (timeSetEvent(m_nTimeThinkLimit, 1, pTimeOutCallBack, (FSDWORD)this, 
//				TIME_PERIODIC | TIME_CALLBACK_FUNCTION) == NULL)
//			if (timeSetEvent(m_nTimeThinkLimit, 1, pTimeOutCallBack, 0, 
//				TIME_PERIODIC | TIME_CALLBACK_FUNCTION) == NULL)
//			{
//				DISPMSG1_ENDL(cout,	"������ʱ��ʧ��!");
//				break;
//			}
//			#else
//			#endif
		}
		else if (!strncmp(szUCCICmd, "", 1) || !strncmp(szUCCICmd, "infinite", 8))
		{
			m_ThinkType = TT_DEPTH;		//�����˼��
			m_nSearchDepth = MAXSEARCHDEPTH;
		}
		else
		{
			cout << "����ʶ���UCCIЭ��ָ��:" << szCmdCpy << endl;
			break;
		}

		SearchGo();
		break;

	case UCCICSetOption:
		if (m_esUCCIEgnStatus == EgnSBusy || m_esUCCIEgnStatus == EgnSSuspend)
		{
			UCCIBusy();
			break;
		}

		if (strncmp(szUCCICmd, "bookfiles", 9) == 0)
		{
			strcpy(m_szBookFile, JumpNoMatterChar(szUCCICmd + 9));
			break;
		}

		if (strncmp(szUCCICmd, "loadbook", 8) == 0)
		{
			m_nGetOpenBookMoveErrCnt = 0;
			m_bIsHaveCheckMateMove = FALSE;
			break;
		}

		if (strncmp(szUCCICmd, "style", 5) == 0)
		{
			SetChessStyle(szUCCICmd + 5);
			break;
		}

		if (strncmp(szUCCICmd, "drawmoves", 9) == 0)
		{
			nTemp = m_nDrawMoves;

			m_nDrawMoves = atoi(JumpNoMatterChar(szUCCICmd + 9));
			if (m_nDrawMoves <= 0)
			{
				m_nDrawMoves = nTemp;

				DISPMSG1_ENDL(cout,	"��Ȼ�������������0!");
			}
			break;
		}
		
		DISPMSG1_ENDL(cout,	"setoption�����﷨����!");
		break;

	case UCCICPonderHit:
		m_bPonderHit = TRUE;
		break;

	case UCCICDebug:
		if (m_esUCCIEgnStatus == EgnSBusy || m_esUCCIEgnStatus == EgnSSuspend)
		{
			UCCIBusy();
			break;
		}
		break;

	case UCCICAddPosition:
		if (m_esUCCIEgnStatus == EgnSBusy || m_esUCCIEgnStatus == EgnSSuspend)
		{
			UCCIBusy();
			break;
		}
		break;
	
	case UCCICStop://ֹͣ˼��
		UCCIStopThink();
		break;

	case UCCIPause://��ͣ˼��
		#ifdef _WIN32
		if (SuspendThread(m_hThinkThread) == 0xFFFFFFFF)
		{
			cout << "��ͣ˼��ʧ��,���Ժ�����!" << endl;
		}
		#else
		#endif
		break;

	case UCCIResume://����˼��
		STRIDEPLATFORM
		(
		if (ResumeThread(m_hThinkThread) == 0xFFFFFFFF)
		{
			cout << "����˼��ʧ��,���Ժ�����!" << endl;
		}, 
		;
		);
		break;

	case UCCICQuitUCCI://�˳�UCCIЭ��
		//���������߳�
		KillThread(m_hThinkThread);

		TRYDELETE(SAFEDELETE(szCmdCpy));
		return;

	case UCCICQuitEgn://�˳�����
		//���������߳�
		KillThread(m_hThinkThread);

		cout << "bye" << endl;

		TRYDELETE(SAFEDELETE(szCmdCpy));

		ExitEgn();	//�˳�����
		exit(EXIT_SUCCESS);
		break;

	case UCCIStatus:
		PrintEgnStatus();
		break;

	case UCCINullMove:
		if (szUCCICmd[0] == '\0')
		{
			if (m_bNullMoveOn == TRUE)
			{
				DISPMSG1_ENDL(cout,	"Null Move is on");
			}
			else
			{
				DISPMSG1_ENDL(cout,	"Null Move is off");
			}

			break;
		}

		if (!strncmp(szUCCICmd, "on", 3))
		{
			m_bNullMoveOn = TRUE;

			DISPMSG1_ENDL(cout,	"OK - Null Move On Now");
		}
		else if (!strncmp(szUCCICmd, "off", 4))
		{
			m_bNullMoveOn = FALSE;

			DISPMSG1_ENDL(cout,	"OK - Null Move Off Now");
		}
		else
		{
			DISPMSG2_ENDL(cout,	"�����Null Moveָ��", szCmdCpy);
		}
		break;

	case UCCIOpenBook:
		if (szUCCICmd[0] == '\0')
		{
			if (m_bOpenBookOn == TRUE)
			{
				DISPMSG1_ENDL(cout,	"OpenBook is on");
			}
			else
			{
				DISPMSG1_ENDL(cout,	"OpenBook is off");
			}

			break;
		}

		if (!strncmp(szUCCICmd, "on", 3))
		{
			m_bOpenBookOn = TRUE;

			DISPMSG1_ENDL(cout,	"OK - OpenBook On Now");
		}
		else if (!strncmp(szUCCICmd, "off", 4))
		{
			m_bOpenBookOn = FALSE;

			DISPMSG1_ENDL(cout,	"OK - OpenBook Off Now");
		}
		else
		{
			DISPMSG2_ENDL(cout,	"�����OpenBookָ��", szCmdCpy);
		}
		break;

	case UCCILoadBook:
		m_nGetOpenBookMoveErrCnt = 0;
		break;

	case UCCIClearHash:
		ClearHash();
		
		DISPMSG1_ENDL(cout,	"OK - Clear Hash Success!");
		break;

	case UCCIHistroyShit:
		if (szUCCICmd[0] == '\0')
		{
			DISPMSG2_ENDL(cout,	"HistoryShift is ", m_nHistoryShift);

			break;
		}

		m_nHistoryShift = atoi(JumpNoMatterChar(szUCCICmd));

		DISPMSG1_ENDL(cout,	"OK");
		break;

	case UCCIDrawValue:
		if (szUCCICmd[0] == '\0')
		{
			DISPMSG2_ENDL(cout,	"DrawValue is ", m_nContemptFactor);

			break;
		}

		m_nContemptFactor = atoi(JumpNoMatterChar(szUCCICmd));

		DISPMSG1_ENDL(cout,	"OK");
		break;

	case UCCIDrawMoves:
		if (szUCCICmd[0] == '\0')
		{
			DISPMSG2_ENDL(cout,	"DrawMoves is ", m_nDrawMoves);

			break;
		}

		nTemp = m_nDrawMoves;

		m_nDrawMoves = atoi(JumpNoMatterChar(szUCCICmd));
		if (m_nDrawMoves <= 0)
		{
			m_nDrawMoves = nTemp;
			
			DISPMSG1_ENDL(cout,	"��Ȼ�������������0!");
			break;
		}

		DISPMSG1_ENDL(cout,	"OK");
		break;

	case UCCICEPlies:
//		if (szUCCICmd[0] == '\0')
		{
			DISPMSG2_ENDL(cout,	"CE Plies is ", m_nCheckExtensionPlies);

			break;
		}

//		nTemp = m_nCheckExtensionPlies;
//
//		m_nCheckExtensionPlies = atoi(JumpNoMatterChar(szUCCICmd));
//		if (m_nCheckExtensionPlies <= 0)
//		{
//			m_nCheckExtensionPlies = nTemp;
//			
//			DISPMSG1_ENDL(cout,	"������������������0!");
//			break;
//		}
//
//		DISPMSG1_ENDL(cout,	"OK");
		break;

	case UCCIOnlyMove:
		if (szUCCICmd[0] == '\0')
		{
			if (m_bOnlyMoveOn == TRUE)
			{
				DISPMSG1_ENDL(cout,	"OnlyMove is on");
			}
			else
			{
				DISPMSG1_ENDL(cout,	"OnlyMove is off");
			}

			break;
		}

		if (!strncmp(szUCCICmd, "on", 3))
		{
			m_bOnlyMoveOn = TRUE;

			DISPMSG1_ENDL(cout,	"OK - OnlyMove On Now");
		}
		else if (!strncmp(szUCCICmd, "off", 4))
		{
			m_bOnlyMoveOn = FALSE;

			DISPMSG1_ENDL(cout,	"OK - OnlyMove Off Now");
		}
		else
		{
			DISPMSG2_ENDL(cout,	"�����OnlyMoveָ��", szCmdCpy);
		}
		break;

	case UCCICInvalid:
		cout << "����ʶ���UCCIЭ��ָ��:" << szCmdCpy << endl;
		break;

	case UCCITime:
		DISPMSG3_ENDL(cout,	"Time Searched is ", MS2S(clock() - m_nTimeStartThink), "s");
		break;

	case UCCIGetSpecialFEN:
		DISPMSG1_ENDL(cout,	GetSpecialFEN(szUCCICmd));
		break;

	default:
		break;
	}

	Idle();

	goto GetUCCICmd;
}

/*******************************************************************************************************
������ : GetUCCICmd
��  �� : 
��  �� : 
        const FSCHAR *szUCCICmd : 
����ֵ : UCCICmd
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:42:58
*******************************************************************************************************/
UCCICmd CSearchEngine::GetUCCICmd(FSCHAR * &szUCCICmd)
{
	if (strlen(szUCCICmd) >= 8)
	{
		FSCHAR	szTemp[MAXBUFFERLEN];
		
		szUCCICmd = JumpNoMatterChar(szUCCICmd);
		
		memcpy(szTemp, szUCCICmd, 8);
		szTemp[8] = '\0';
		strlwr(szTemp);
		
		if (!strncmp(szTemp,"position", 8))
		{
			szUCCICmd += 8;
			return UCCICPosition;
		}
	}

	strlwr(szUCCICmd);

	if (!strncmp(szUCCICmd,"ponderhit", 9))
	{
		szUCCICmd += 9;
		return UCCICPonderHit;
	}

	if (!strncmp(szUCCICmd,"stop", 4))
	{
		szUCCICmd += 4;
		return UCCICStop;
	}

	if (!strncmp(szUCCICmd,"pause", 5))
	{
		szUCCICmd += 5;
		return UCCIPause;
	}

	if (!strncmp(szUCCICmd,"resume", 6))
	{
		szUCCICmd += 6;
		return UCCIResume;
	}

	if (!strncmp(szUCCICmd,"isready", 7))
	{
		szUCCICmd += 7;
		return UCCICIsReady;
	}

	if (!strncmp(szUCCICmd,"setoption", 9))
	{
		szUCCICmd += 9;
		return UCCICSetOption;
	}

	if (!strncmp(szUCCICmd,"go", 2))
	{
		szUCCICmd += 2;
		return UCCICGo;
	}

	//������Ҫ����quit����ǰ�洦��
	if (!strncmp(szUCCICmd,"quitucci", 8))
	{
		szUCCICmd += 8;
		return UCCICQuitUCCI;
	}

	if (!strncmp(szUCCICmd,"quit", 4))
	{
		szUCCICmd += 4;
		return UCCICQuitEgn;
	}

	if (!strncmp(szUCCICmd,"debug",5))
	{
		szUCCICmd += 5;
		return UCCICDebug;
	}

	if (!strncmp(szUCCICmd,"addposition", 11))
	{
		szUCCICmd += 11;
		return UCCICAddPosition;
	}

	if (!strncmp(szUCCICmd, "status", 7))
	{
		return UCCIStatus;
	}

	if (!strncmp(szUCCICmd, "nullmove", 8))
	{
		szUCCICmd += 8;
		return UCCINullMove;
	}

	if (!strncmp(szUCCICmd, "openbook", 8))
	{
		szUCCICmd += 8;
		return UCCIOpenBook;
	}

	if (!strncmp(szUCCICmd, "endbook", 7))
	{
		szUCCICmd += 7;
		return UCCIEndBook;
	}

	if (!strncmp(szUCCICmd, "loadbook", 9))
	{
		szUCCICmd += 8;
		return UCCILoadBook;
	}

	if (!strncmp(szUCCICmd, "clearhash", 10))
	{
		szUCCICmd += 9;
		return UCCIClearHash;
	}

	if (!strncmp(szUCCICmd, "historyshift", 12))
	{
		szUCCICmd += 12;
		return UCCIHistroyShit;
	}
	
	if (!strncmp(szUCCICmd, "onlymove", 8))
	{
		szUCCICmd += 8;
		return UCCIOnlyMove;
	}
	
	if (!strncmp(szUCCICmd, "drawvalue", 9))
	{
		szUCCICmd += 9;
		return UCCIDrawValue;
	}

	if (strncmp(szUCCICmd, "time", 4) == 0)
	{
		szUCCICmd += 4;
		return UCCITime;
	}

	if (strncmp(szUCCICmd, "drawmoves", 9) == 0)
	{
		szUCCICmd += 9;
		return UCCIDrawMoves;
	}
	
	if (strncmp(szUCCICmd, "ceplies", 7) == 0)
	{
		szUCCICmd += 7;
		return UCCICEPlies;
	}

	if (strncmp(szUCCICmd, "getspecialfen", 13) == 0)
	{
		szUCCICmd += 13;
		return UCCIGetSpecialFEN;
	}

	if (strncmp(szUCCICmd, "", 1))
	{
		return UCCICInvalid;
	}

	return UCCICNone;
}

/*******************************************************************************************************
������ : LoadFEN
��  �� : 
��  �� : 
        FSCHAR *szStr : 
����ֵ : ��
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:43:11
*******************************************************************************************************/
FSBOOL CSearchEngine::LoadFEN(FSCHAR *szFEN)
{
	if (szFEN == 0)
	{
		DISPMSG1_ENDL(cout,	"FEN������Ϊ��");

		return FALSE;
	}

	FSINT p;
	FSINT i, j;
	FSINT nCuri = 0;
	FSINT nCurj = 9;
	FSINT nPos;

	//��0ԭ����������
	// m_nBitFile
	// m_nBitRank
	memset(m_nBitFile, 0, MAXYSCALENUM * sizeof(FSINT));
	memset(m_nBitRank, 0, MAXXSCALENUM * sizeof(FSINT));

	// m_nBRBitFile
	memset(m_nBRBitFile[BLACK], 0, MAXYSCALENUM * sizeof(FSINT));
	memset(m_nBRBitFile[RED], 0, MAXYSCALENUM * sizeof(FSINT));

	// m_nBRBitRank
	memset(m_nBRBitRank[BLACK], 0, MAXXSCALENUM * sizeof(FSINT));
	memset(m_nBRBitRank[RED], 0, MAXXSCALENUM * sizeof(FSINT));

	// m_nCBChessID
	// m_nCBPieceID
	memset(m_nCBChessID, NOCHESS, MAXCHESSGRIDNUM * sizeof(FSINT));
	for (i = 0; i < MAXCHESSGRIDNUM; i++)
	{
		m_nCBPieceID[i] = INVALIDPIECEID;
	}

	// m_nPiecePos
	for (i = 0; i < MAXPIECEID; i++)
	{
		m_nPiecePos[i] = INVALIDPIECEPOS;
	}

	for (i = 0; i < m_nRepHashTabMask + 1; i++)
	{
		m_nRepHashTab[i] = -1;
	}

	m_nCurDepth = 0;

	m_nAllChessCnt = 0;
	m_nBigChessCnt = 0;

	//m_nChessCnt
	memset(m_nChessCnt, 0, 15 * sizeof(FSINT));

	//����FEN�ַ�������m_nCBChessID
	//����ǰ��Ŀո�,Tab��
	szFEN = JumpNoMatterChar(szFEN);

	FSCHAR szTemp[4];

	memcpy(szTemp, szFEN, 3);
	strlwr(szTemp);
	if (strncmp(szTemp, "fen", 3) != 0)
	{
		DISPMSG1_ENDL(cout,	"FEN���﷨����,ȱ�ٹؼ���fen");

		return FALSE;
	}

	szFEN = JumpNoMatterChar(szFEN + 3);
	nPos = 0;
	while(szFEN[nPos] != ' ')
	{
		if (isdigit(szFEN[nPos]))
		{
			nCuri += szFEN[nPos] - '0';
		}
		else if (szFEN[nPos] == '/')
		{
			nCuri = 0;
			nCurj--;
		}
		else if (szFEN[nPos] >= 'a' && szFEN[nPos] <= 'z')//ΪСд��ĸ
		{
			m_nCBChessID[10 * nCuri + nCurj] = g_nAlpha2Chess[szFEN[nPos] -'a'];
			nCuri++;
		}
		else if (szFEN[nPos] >= 'A' && szFEN[nPos] <= 'Z')//Ϊ��д��ĸ
		{
			m_nCBChessID[10 * nCuri + nCurj] = g_nAlpha2Chess[szFEN[nPos] -'A'] + 7;
			nCuri++;
		}
		else//������
		{
			return FALSE;
		}

		nPos++;
	}

	//������������
	FSINT nCnt[15] = {0};
	FSINT nChessID;
	FSINT nChess;
	FSINT nPieceID;

	m_nHashTabIndex = 0;
	m_nCBCheckSum = 0;
	for (p = 0; p < 90; p++)
	{
		i = m_CGN2Pos[p].x;
		j = m_CGN2Pos[p].y;

		nChessID = m_nCBChessID[p];

		m_nHashTabIndex ^= m_nHashKey32[nChessID][i][j];
		m_nCBCheckSum ^= m_nHashKey64[nChessID][i][j];

		m_nChessCnt[nChessID]++;	// [2006/1/2 18:52:11]

		if (nChessID == NOCHESS)
		{
			m_nCBPieceID[p] = INVALIDPIECEID;
			continue;
		}

		//������λ��λ��
		m_nBitFile[j] |= m_nMask[i];
		m_nBitRank[i] |= m_nMask[j];

		//������λ��λ��
		m_nBRBitFile[m_bIsRed_ChessID[nChessID]][j] |= m_nMask[i];
		m_nBRBitRank[m_bIsRed_ChessID[nChessID]][i] |= m_nMask[j];

		nPieceID = m_nChessID2PieceID[nChessID][nCnt[nChessID]];

		m_nPiecePos[nPieceID] = p;
		m_nCBPieceID[p] = nPieceID;

		nCnt[nChessID]++;

		m_nAllChessCnt++;

		if ((nChess = g_nChessID2Chess[nChessID]) == ROOK || nChess == KNIGHT || nChess == CANNON)
		{
			m_nBigChessCnt++;
		}
	}

	if (m_nBigChessCnt >= 11)
	{
		m_nChessAge = OPEN_GAME;
	}
	else if (m_nBigChessCnt <= 4)
	{
		m_nChessAge = END_GAME;
	}
	else
	{
		m_nChessAge = MID_GAME;
	}

//	g_nBasicValues[KNIGHT] = m_nKnightBasicValue[m_nChessAge];
//	g_nBasicValues[CANNON] = m_nCannonBasicValue[m_nChessAge];
//	g_nBasicValues[PAWN] = m_nPawnBasicValue[m_nChessAge];

	if (m_nBigChessCnt <= 1)
	{
		m_bNullMoveOn = FALSE;
	}
	else
	{
		m_bNullMoveOn = TRUE;
	}
	
	for (i = 1; i < 15; i++)
	{
		for (j = nCnt[i]; j < m_nInitChessCnt[i]; j++)
		{
			m_nPiecePos[m_nChessID2PieceID[i][j]] = INVALIDPIECEPOS;
		}
	}

	FSCHAR		szMove[5];	//�߷�
	CHESSMOVE	cm;
	UNDOMOVE	undoMove;
	
	szFEN = JumpNoMatterChar(szFEN + nPos);
	if (*szFEN == '\0')
	{
		return FALSE;
	}
	//������߷�
	if (*szFEN  == 'b')
	{
		m_bIsRed = BLACK;
	}
	else
	{
		m_bIsRed = RED;

		// [2005/11/10 11:21:57]
		m_nHashTabIndex ^= m_nHashKeyPlayer32;
		m_nCBCheckSum ^= m_nHashKeyPlayer64;
	}

	szFEN = JumpNoMatterChar(szFEN + 1);
//	if (*szFEN == '-')
//	{
//		szFEN = JumpNoMatterChar(szFEN + 1);
//		szFEN = JumpNoMatterChar(szFEN + 1);
//		
//		m_nBouts = atoi(szFEN);	//����˫��û�г��ӵ����岽��(��غ���)
//		szFEN = JumpDigit(szFEN);
//		szFEN = JumpNoMatterChar(szFEN);
//		
//		m_nCurBout = atoi(szFEN);//���뵱ǰ�غ�
//		szFEN = JumpDigit(szFEN);
//		szFEN = JumpNoMatterChar(szFEN);
//	}

	m_nNoEatMoveCnt_BS = 0;
	if ((szFEN = strstr(szFEN, "moves")) != NULL)
	{
		FSINT nToChessID;
		FSINT nToPieceID;
		
		szFEN += 5;
		while (*szFEN != '\0')
		{
			szFEN = JumpNoMatterChar(szFEN);
			if (*szFEN == '\0')
			{
				break;
			}
			
			strncpy(szMove, szFEN , 4);
			if (Str2ChessMove(szMove, cm) == FALSE)
			{
				return FALSE;
			}
			m_nHistoryCBCheckSum_NoEat[m_nNoEatMoveCnt_BS] = m_nCBCheckSum;
			MakeMove(m_Moves_NoEat[m_nNoEatMoveCnt_BS] = cm, nToChessID, nToPieceID);
			m_Moves_NoEat[m_nNoEatMoveCnt_BS].nKind = m_cmFight[m_nCurDepth - 1].nKind;
			m_nNoEatMoveCnt_BS++;
			
			FILLUNDOMOVE(undoMove, cm, nToChessID, nToPieceID);
			m_UndoMoveStack.push(undoMove);
			
			szFEN += 4;
		}
	}
	m_nNoEatMoveCnt_IS = m_nNoEatMoveCnt_BS;

	FSINT k;

	m_nBoardValue = 0;
	//�췽
	for (i = 0; i < 16; i++)
	{
		j = m_nPiecePos[i];
		if (j != INVALIDPIECEPOS)
		{
			k = m_nPieceID2ChessID[i] - 7;
			DEBUGASSERT(k >= 0 && k < 8);

			m_nBoardValue -= g_nBasicValues[k];
			m_nBoardValue -= g_nPosValues[k][j];
		}
	}
	//�ڷ�
	for (i = 16; i < 32; i++)
	{
		j = m_nPiecePos[i];
		if (j != INVALIDPIECEPOS)
		{
			k = m_nPieceID2ChessID[i];
			DEBUGASSERT(k >= 0 && k < 8);

			m_nBoardValue += g_nBasicValues[k];
			m_nBoardValue += g_nPosValues[k][89 - j];
		}
	}

	return TRUE;
}

/*******************************************************************************************************
������ : ChessMove2Str
��  �� : 
��  �� : 
        CHESSMOVE cm   : 
		FSCHAR *szMove : 
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��11��6�� 15:49:39
*******************************************************************************************************/
FSVOID CSearchEngine::ChessMove2Str(const CHESSMOVE& cm, FSCHAR *szMove)
{
	switch(m_EgnProtocol)
	{
	case EgnPUCCI:
		ChessMove2UCCIStr(cm, szMove);
		break;
	
	case EgnPQianHong:
		ChessMove2QHStr(cm, szMove);
		break;

	default:
		break;
	}
}

/*******************************************************************************************************
������ : Str2ChessMove
��  �� : 
��  �� : 
        FSCHAR *szMoveStr : 
		CHESSMOVE& cm     : 
����ֵ : ����Ϊ FSBOOL
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��11��6�� 15:49:33
*******************************************************************************************************/
FSBOOL CSearchEngine::Str2ChessMove(FSCHAR *szMoveStr, CHESSMOVE& cm)
{
	switch(m_EgnProtocol)
	{
	case EgnPUCCI:
		return UCCIStr2ChessMove(szMoveStr, cm);
	
	case EgnPQianHong:
		return QHStr2ChessMove(szMoveStr, cm);

	default:
		return TRUE;
	}
}

/*******************************************************************************************************
������ : OutChessMove
��  �� : 
��  �� : 
        CHESSMOVE cm : 
����ֵ : ��
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:43:20
*******************************************************************************************************/
FSVOID CSearchEngine::ChessMove2UCCIStr(const CHESSMOVE& cm, FSCHAR *szMove)
{
	_ASSERT(cm.nFrom >= 0 && cm.nFrom <= MAXCHESSGRIDNUM);
	_ASSERT(cm.nTo >= 0 && cm.nTo <= MAXCHESSGRIDNUM);
	_ASSERT(szMove != 0);

	szMove[0] = m_CGN2Pos[cm.nFrom].x + 'a';
	szMove[1] = m_CGN2Pos[cm.nFrom].y + '0';
	
	szMove[2] = m_CGN2Pos[cm.nTo].x + 'a';
	szMove[3] = m_CGN2Pos[cm.nTo].y + '0';

	szMove[4] = '\0';
}

/*******************************************************************************************************
������ : UCCIStr2ChessMove
��  �� : 
��  �� : 
        const FSCHAR *szMove : 
		CHESSMOVE& cm        : 
����ֵ : ����Ϊ FSBOOL
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��11��6�� 16:00:28
*******************************************************************************************************/
FSBOOL CSearchEngine::UCCIStr2ChessMove(FSCHAR *szMove, CHESSMOVE& cm)
{
	_ASSERT(szMove != 0);
	
	strlwr(szMove);

	if (szMove[0] < 'a' || szMove[0] > 'i' || szMove[1] < '0' || szMove[1] > '9' || 
		szMove[2] < 'a' || szMove[2] > 'i' || szMove[3] < '0' || szMove[3] > '9')
	{
		return FALSE;
	}

	cm.nFrom = m_nPos2CGN[szMove[0] - 'a'][szMove[1] - '0'];
	cm.nTo = m_nPos2CGN[szMove[2] - 'a'][szMove[3] - '0'];

	return TRUE;
}

/*******************************************************************************************************
������ : ChessMove2QHStr
��  �� : 
��  �� : 
        CHESSMOVE cm   : 
		FSCHAR *szICCS : 
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��11��6�� 15:49:47
*******************************************************************************************************/
FSVOID CSearchEngine::ChessMove2QHStr(const CHESSMOVE& cm, FSCHAR *szICCS)
{
	_ASSERT(cm.nFrom >= 0 && cm.nFrom <= MAXCHESSGRIDNUM);
	_ASSERT(cm.nTo >= 0 && cm.nTo <= MAXCHESSGRIDNUM);
	_ASSERT(szICCS != 0);

	szICCS[0] = m_CGN2Pos[cm.nFrom].x + 'A';
	szICCS[1] = m_CGN2Pos[cm.nFrom].y + '0';
	
	szICCS[2] = '-';

	szICCS[3] = m_CGN2Pos[cm.nTo].x + 'A';
	szICCS[4] = m_CGN2Pos[cm.nTo].y + '0';

	szICCS[5] = '\0';
}

/*******************************************************************************************************
������ : QHStr2ChessMove
��  �� : 
��  �� : 
        FSCHAR *szICCS : 
		CHESSMOVE& cm  : 
����ֵ : ����Ϊ FSBOOL
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��11��6�� 15:49:52
*******************************************************************************************************/
FSBOOL CSearchEngine::QHStr2ChessMove(FSCHAR *szICCS, CHESSMOVE& cm)
{
	_ASSERT(szICCS != 0);
	
	if (szICCS[0] < 'a' || szICCS[0] > 'i' || szICCS[1] < '0' || szICCS[1] > '9' || 
		szICCS[3] < 'a' || szICCS[3] > 'i' || szICCS[4] < '0' || szICCS[4] > '9')
	{
		return FALSE;
	}

	cm.nFrom = m_nPos2CGN[szICCS[0] - 'a'][szICCS[1] - '0'];
	cm.nTo = m_nPos2CGN[szICCS[3] - 'a'][szICCS[4] - '0'];

	return TRUE;
}

/*******************************************************************************************************
������ : AheiPProc
��  �� : AHEIЭ�鴦��
��  �� : ��
����ֵ : ��
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:43:27
*******************************************************************************************************/
FSVOID CSearchEngine::AheiPProc()
{

}

/*******************************************************************************************************
������ : QianHongPProc
��  �� : ǳ��Э�鴦��
��  �� : ��
����ֵ : ��
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:43:40
*******************************************************************************************************/
FSVOID CSearchEngine::QianHongPProc(FSCHAR *szParam)
{
	STATEMENT_TEST
	(
	if (strncmp(szParam, "", 1))
	{
		CString str;
		
		str.Format("ǳ��Э����������:%s", szParam);
		MessageBox(NULL, str, "����", MB_OK);
	}
	);

	if (!strncmp(szParam, "-info", 6))
	{
		PrintQHInfo();
		ExitEgn();	//�˳�����
		exit(EXIT_SUCCESS);
	}
	
	FSCHAR	*szQianHongCmd = new FSCHAR[MAXBUFFERLEN];
	FSCHAR	*szCmdCpy = szQianHongCmd; //szQianHongCmd�Ŀ���,�����ͷ�szUCCICmd���ڴ�
	FSINT	nQHLevel;
	FSINT	nToChessID;
	FSINT	nToPieceID;

	QianHongCmd	qianHongCmd;
	UNDOMOVE	undoMove;
	CHESSMOVE	cm;

#ifdef _WIN32
	FSVOID (CALLBACK CSearchEngine::*pInternalCallBack)
		(FSUINT uID, FSUINT uMsg, FSDWORD dwUser, FSDWORD dw1, FSDWORD dw2) = ThinkTimeOut;
//	LPTIMECALLBACK pTimeOutCallBack;

//	memcpy(&pTimeOutCallBack, &pInternalCallBack, sizeof(pInternalCallBack));
#else
#endif

	if (m_bAllocateMem == FALSE)
	{
		Initial();
	}

	LoadFEN((FSCHAR*)g_szOpenGamePos);

	m_nDrawMoves = 320;

GetQianHongCmd:
	szQianHongCmd = szCmdCpy;
	strncpy(szQianHongCmd, "", 1);
	gets(szQianHongCmd);

	if (strncmp(szQianHongCmd, "", 1))
	{
		STATEMENT_TEST
		(
		CString str;
		
		str.Format("ǳ��Э����������:%s", szQianHongCmd);
		MessageBox(NULL, str, "����", MB_OK);
		);
	}

	qianHongCmd = GetQianHongCmd(szQianHongCmd);
	szQianHongCmd = JumpNoMatterChar(szQianHongCmd);

	switch(qianHongCmd)
	{
	case QHCNone:
		break;

	case QHCInvalid:
		DISPMSG2_ENDL(cout,	"����ʶ���ǳ��Э��ָ��:", szCmdCpy);
		break;

	case QHCLevel:
		if (szQianHongCmd[0] == '\0')
		{
			DISPMSG1_ENDL(cout,	m_nQHLevel);
		}
		else
		{
			nQHLevel = atoi(szQianHongCmd);
			if (nQHLevel < MINQHLEVEL || nQHLevel > MAXQHLEVEL)
			{
				DISPMSG6_ENDL(cout,	"ERROR - Bad level ", nQHLevel, 
					" use ", MINQHLEVEL, " to ", MAXQHLEVEL);

				STATEMENT_TEST
				(
				CString str;
				
				str.Format("ERROR - Bad level %d use %d to %d", nQHLevel, MINQHLEVEL, MAXQHLEVEL);
				MessageBox(NULL, str, "����", MB_OK);
				);
			}
			else
			{
				m_nQHLevel = nQHLevel;
				m_nSearchDepth = g_nQHSchDptByLvl[m_nQHLevel - 1];
				DISPMSG2_ENDL(cout,	"OK - Set AI level to ", nQHLevel);

				STATEMENT_TEST
				(
				CString str;
				
				str.Format("OK - Set AI level to %d", nQHLevel);
				MessageBox(NULL, str, "����", MB_OK);
				);
			}
		}
		break;

	case QHCFEN:
		szQianHongCmd = JumpNoMatterChar(szQianHongCmd);
		if (strlen(szQianHongCmd) < 3)
		{
			DISPMSG3_ENDL(cout,	"ERROR - The FEN ", szQianHongCmd, "is syntax error");

			break;
		}

		//�������,���⴦��
		if (IsSpecialPos(szQianHongCmd) == TRUE)
		{
			break;
		}

		if (LoadFEN(szQianHongCmd) == TRUE)
		{
			DISPMSG1_ENDL(cout,	"OK");
		}
		else
		{
			DISPMSG3_ENDL(cout,	"ERROR - The FEN ", szQianHongCmd, "is syntax error");
		}
		break;

	case QHCPlay:
		if (QHStr2ChessMove(szQianHongCmd, cm) == FALSE)
		{
			DISPMSG2_ENDL(cout,	"ERROR - Bad move:", szQianHongCmd);
			break;
		}
		// ����Ҫ�ж�ѭ��,������Ҫ�ѷǳ����߷���������[2005/12/22 14:44:28]
		if (m_nCBPieceID[cm.nTo] == INVALIDPIECEID)
		{
			m_nHistoryCBCheckSum_NoEat[m_nNoEatMoveCnt_BS] = m_nCBCheckSum;
			MakeMove(m_Moves_NoEat[m_nNoEatMoveCnt_BS] = cm, nToChessID, nToPieceID);
			m_Moves_NoEat[m_nNoEatMoveCnt_BS].nKind = m_cmFight[m_nCurDepth - 1].nKind;
			m_nNoEatMoveCnt_BS++;
		}
		else
		{
			m_nNoEatMoveCnt_BS = 0;
			MakeMove(cm, nToChessID, nToPieceID);
		}

		if (m_RedoMoveStack.size() > 0)
		{
			if (m_RedoMoveStack.top().cm == cm)
			{
				m_RedoMoveStack.pop();
			}
			else
			{
				ClearStack(m_RedoMoveStack);
			}
		}

		FILLUNDOMOVE(undoMove, cm, nToChessID, nToPieceID);
		m_UndoMoveStack.push(undoMove);

		DISPMSG1_ENDL(cout,	"OK");
		break;

	case QHCLoad:
		break;

	case QHCAI:
		m_ThinkType = TT_DEPTH;

		STATEMENT_DEBUG(memcpy(m_nPiecePosBeforeAI, m_nPiecePos, 32 * sizeof(FSINT)););

		//���浱ǰ����FEN��
		CBChessID2FEN(m_szFEN);

		SearchGo();
		break;

	case QHCAbort:
		QHStopThink();
		break;

	case QHCQuit:
		//���������߳�
		KillThread(m_hThinkThread);

		cout << "bye" << endl;

		TRYDELETE(SAFEDELETE(szCmdCpy));

		ExitEgn();	//�˳�����
		exit(EXIT_SUCCESS);
		break;

	case QHCQuitQH:
		return;

	case QHCUndo:
		#ifdef _WIN32
		if (m_UndoMoveStack.size() == 0)
		#else
		#endif
		{
			DISPMSG1_ENDL(cout,	"ERROR");
		}
		else
		{
			undoMove = m_UndoMoveStack.top();
			UnMakeMove(undoMove.cm, undoMove.nToChessID, undoMove.nToPieceID);
			m_UndoMoveStack.pop();
			m_RedoMoveStack.push(undoMove);

			DISPMSG1_ENDL(cout,	"OK");
		}
		break;

	case QHCHints:
		break;

	case QHCBan:
		break;

	case QHCBGThink:
		break;

	case QHCTimeOut:
		QHStopThink();
		break;
	}

	Sleep(1);

	goto GetQianHongCmd;
}

/*******************************************************************************************************
������ : 
��  �� : ��������,szParam����ǳ��Э��
��  �� : ��
����ֵ : 
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:44:17
*******************************************************************************************************/
FSVOID CSearchEngine::RunEngine(FSCHAR *szParam)
{
	FSCHAR szProtocol[MAXBUFFERLEN];

	// ǳ��Э��[2005/11/3 13:38:27]
	if (!strncmp(szParam, "-plugin", 7) || !strncmp(szParam, "-info", 5))
	{
		m_EgnProtocol = EgnPQianHong;
		QianHongPProc(szParam);
	}
	
	// ���������Ϣ
	DISPMSG1_ENDL(cout, "������Ϣ");
	DISPMSG1_ENDL(cout, "�������� : "ENGINENAME);
	DISPMSG1_ENDL(cout, "����汾 : "ENGINEVERSION);
	DISPMSG1_ENDL(cout, "����ʱ�� : "__DATE__" "__TIME__);
	DISPMSG0_ENDL(cout);

	// �����֧�ֵĽ�������Э��
	DISPMSG1_ENDL(cout, "protocol UCCI");
	DISPMSG1_ENDL(cout, "protocol QianHong");
	DISPMSG1_ENDL(cout, "protocol ACCI");

GetEgnP:
	*szProtocol = '\0';
	gets(szProtocol);

	strlwr(szProtocol);
	
	switch(GetEgnP(szProtocol))
	{
	case EgnPUCCI:	//ucciЭ��
		m_EgnProtocol = EgnPUCCI;
		UCCIProc();
		break;

	case EgnPQianHong:	//ǳ��Э��
		m_EgnProtocol = EgnPQianHong;
		QianHongPProc("");
		break;

	case EgnPAhei:	//aheiЭ��
		m_EgnProtocol = EgnPAhei;
		AheiPProc();
		break;

	case EgnPNone:
		if (!strncmp(szProtocol, "quit", 5))
		{
			cout <<	"bye" << endl;

			exit(EXIT_SUCCESS);
		}

		if (strncmp(szProtocol, "", 1))
		{
			DISPMSG2_ENDL(cout,		"����ʶ���Э����:", szProtocol);
		}

		break;

	default:
		break;
	}

	Idle();

	goto GetEgnP;
}

/*******************************************************************************************************
������ : GetEgnP
��  �� : �õ�����Э��,�����ַ���ȫΪСд
��  �� : 
        const FSCHAR *szStr : 
����ֵ : EgnProtocol
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:44:48
*******************************************************************************************************/
EgnProtocol CSearchEngine::GetEgnP(FSCHAR *szStr)
{
	FSINT nFSCHARSize = sizeof(FSCHAR);

	szStr = JumpNoMatterChar(szStr);

	//UCCIЭ��
	if (!strncmp(szStr, "ucci", 5))
	{
		return EgnPUCCI;
	}

	//aheiЭ��
	if (!strncmp(szStr, "ahei", 5))
	{
		return EgnPAhei;
	}
	
	//ǳ��Э��
	if (!strncmp(szStr, "qianhong", 9))
	{
		return EgnPQianHong;
	}

	return EgnPNone;
}

/*******************************************************************************************************
������ : IsChecked
��  �� : 1 - bIsRed���Ƿ�bIsRed������.
��  �� : 
        BOOL bIsRed : 
����ֵ : FSBOOL
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:47:30
*******************************************************************************************************/
inline FSBOOL CSearchEngine::IsChecked(BOOL bIsRed)
{
	FSINT nFoeKingP;
	FSINT nFoeKingI;
	FSINT nFoeKingJ;
	FSINT nFoeKingPosCheckNum;

	FSINT nRookPos1;
	FSINT nRookPos2;

	FSINT nKnightI1;
	FSINT nKnightJ1;
	FSINT nKnightI2;
	FSINT nKnightJ2;
	FSINT nKnightPos1;
	FSINT nKnightPos2;
	FSINT nKnightLegCheckSum1;
	FSINT nKnightLegCheckSum2;
	
	FSINT nCannonPos1;
	FSINT nCannonPos2;

	FSINT nPawnPosCheckNum1;
	FSINT nPawnPosCheckNum2;
	FSINT nPawnPosCheckNum3;
	FSINT nPawnPosCheckNum4;
	FSINT nPawnPosCheckNum5;

	FSINT nFoeKingBitFile;
	FSINT nFoeKingBitRank;

	FSINT nRook;
	FSINT nKnight;
	FSINT nCannon;
	FSINT nPawn;

	nRook = m_nRook[bIsRed];
	nKnight = m_nKnight[bIsRed];
	nCannon = m_nCannon[bIsRed];
	nPawn = m_nPawn[bIsRed];

	//��
	nFoeKingP = m_nPiecePos[m_nKingID2PieceID[1 - bIsRed]];

	nFoeKingPosCheckNum = m_nBRKingPosCheckNum[1 - bIsRed][nFoeKingP];
	
	nFoeKingI = m_CGN2Pos[nFoeKingP].x;
	nFoeKingJ = m_CGN2Pos[nFoeKingP].y;

	//��
	nRookPos1 = m_nPiecePos[m_nChessID2PieceID[nRook][0]];
	nRookPos2 = m_nPiecePos[m_nChessID2PieceID[nRook][1]];

	//��
	nKnightPos1 = m_nPiecePos[m_nChessID2PieceID[nKnight][0]];
	nKnightPos2 = m_nPiecePos[m_nChessID2PieceID[nKnight][1]];

	nKnightI1 = m_CGN2Pos[nKnightPos1].x;
	nKnightJ1 = m_CGN2Pos[nKnightPos1].y;

	nKnightI2 = m_CGN2Pos[nKnightPos2].x;
	nKnightJ2 = m_CGN2Pos[nKnightPos2].y;

	nKnightLegCheckSum1 = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[nKnightI1][m_nBitFile[nKnightJ1]]][m_nFileRankAroundCheckSum[nKnightJ1][m_nBitRank[nKnightI1]]];
	nKnightLegCheckSum2 = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[nKnightI2][m_nBitFile[nKnightJ2]]][m_nFileRankAroundCheckSum[nKnightJ2][m_nBitRank[nKnightI2]]];

	//��
	nCannonPos1 = m_nPiecePos[m_nChessID2PieceID[nCannon][0]];
	nCannonPos2 = m_nPiecePos[m_nChessID2PieceID[nCannon][1]];

	//��
	nPawnPosCheckNum1 = 
		m_nPawnPosCheckNum[bIsRed][m_nPiecePos[m_nChessID2PieceID[nPawn][0]]];
	nPawnPosCheckNum2 = 
		m_nPawnPosCheckNum[bIsRed][m_nPiecePos[m_nChessID2PieceID[nPawn][1]]];
	nPawnPosCheckNum3 = 
		m_nPawnPosCheckNum[bIsRed][m_nPiecePos[m_nChessID2PieceID[nPawn][2]]];
	nPawnPosCheckNum4 = 
		m_nPawnPosCheckNum[bIsRed][m_nPiecePos[m_nChessID2PieceID[nPawn][3]]];
	nPawnPosCheckNum5 = 
		m_nPawnPosCheckNum[bIsRed][m_nPiecePos[m_nChessID2PieceID[nPawn][4]]];
	DEBUGASSERT(nPawnPosCheckNum1 >= MINPAWNPOSCHECKNUM && nPawnPosCheckNum1 <= MAXPAWNPOSCHECKNUM);
	DEBUGASSERT(nPawnPosCheckNum2 >= MINPAWNPOSCHECKNUM && nPawnPosCheckNum2 <= MAXPAWNPOSCHECKNUM);
	DEBUGASSERT(nPawnPosCheckNum3 >= MINPAWNPOSCHECKNUM && nPawnPosCheckNum3 <= MAXPAWNPOSCHECKNUM);
	DEBUGASSERT(nPawnPosCheckNum4 >= MINPAWNPOSCHECKNUM && nPawnPosCheckNum4 <= MAXPAWNPOSCHECKNUM);
	DEBUGASSERT(nPawnPosCheckNum5 >= MINPAWNPOSCHECKNUM && nPawnPosCheckNum5 <= MAXPAWNPOSCHECKNUM);

	nFoeKingBitFile = m_nBitFile[nFoeKingJ];
	nFoeKingBitRank = m_nBitRank[nFoeKingI];

	return m_bIsRookFilePreCheck[bIsRed][nRookPos1][nFoeKingPosCheckNum][nFoeKingBitFile] ||	//����1
		   m_bIsRookFilePreCheck[bIsRed][nRookPos2][nFoeKingPosCheckNum][nFoeKingBitFile] ||	//����2
		   m_bIsRookRankPreCheck[bIsRed][nRookPos1][nFoeKingPosCheckNum][nFoeKingBitRank] ||	//����1
		   m_bIsRookRankPreCheck[bIsRed][nRookPos2][nFoeKingPosCheckNum][nFoeKingBitRank] ||	//����2
		   
		   m_bIsKnightPreCheck[bIsRed][nKnightPos1][nFoeKingPosCheckNum][nKnightLegCheckSum1] ||	//��1
		   m_bIsKnightPreCheck[bIsRed][nKnightPos2][nFoeKingPosCheckNum][nKnightLegCheckSum2] ||	//��2

		   m_bIsCannonFilePreCheck[bIsRed][nCannonPos1][nFoeKingPosCheckNum][nFoeKingBitFile] ||	//����1
		   m_bIsCannonFilePreCheck[bIsRed][nCannonPos2][nFoeKingPosCheckNum][nFoeKingBitFile] ||	//����2
		   m_bIsCannonRankPreCheck[bIsRed][nCannonPos1][nFoeKingPosCheckNum][nFoeKingBitRank] ||	//����1
		   m_bIsCannonRankPreCheck[bIsRed][nCannonPos2][nFoeKingPosCheckNum][nFoeKingBitRank] ||	//����2

		   m_bIsPawnPreCheck[bIsRed][nPawnPosCheckNum1][nFoeKingPosCheckNum] ||	//��1
		   m_bIsPawnPreCheck[bIsRed][nPawnPosCheckNum2][nFoeKingPosCheckNum] ||	//��2
		   m_bIsPawnPreCheck[bIsRed][nPawnPosCheckNum3][nFoeKingPosCheckNum] ||	//��3
		   m_bIsPawnPreCheck[bIsRed][nPawnPosCheckNum4][nFoeKingPosCheckNum] ||	//��4
		   m_bIsPawnPreCheck[bIsRed][nPawnPosCheckNum5][nFoeKingPosCheckNum];	//��5
}

/*******************************************************************************************************
������ : IsRep
��  �� : �ú�����MakeMove���������.
��  �� : ��
����ֵ : FSBOOL ����1��ʾ�ظ�, 2��ʾ��������, 4��ʾ�Է�����
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:47:56
*******************************************************************************************************/
inline FSBOOL CSearchEngine::IsRep(FSINT nCanRep)
{
	if (m_nNoEatMoveCnt_BS + m_nCurDepth <= 4)
	{
		return FALSE;
	}

	//hash����δ��¼�˾���,�϶������ظ�
	if (m_nRepHashTab[m_nHashTabIndex & m_nRepHashTabMask] == -1)
	{
		return FALSE;
	}

	FSINT	nIndex = m_nCurDepth - 2;
	FSBOOL	bOwnPerpCheck = TRUE;	//��������
	FSBOOL	bOppPerpCheck = TRUE;	//�Է�����
	FSBOOL	bBreak = FALSE;
	FSINT	nRepCnt = 0;

	CHESSMOVE cmThis;

	DEBUGASSERT(m_nCurDepth == 0 || m_cmFight[m_nCurDepth] == m_cmNullMove || 
		m_nHistoryCBCheckSum[m_nCurDepth] == m_nCBCheckSum);
	
	// m_cmFight�Ǵ�indexΪ1��ʼ��,�ʴ˴��ж�����Ϊ>=1[2005/11/11 22:58:08]
	while (nIndex >= 1)
	{
		cmThis = m_cmFight[nIndex];

		//���Ż�����߷�,�϶������ظ�
		if (cmThis == m_cmNullMove || cmThis.nKind == EAT)
		{
			bBreak = TRUE;
			break;
		}

		if (cmThis.nKind != CHECK)
		{
			bOppPerpCheck = FALSE;
		}
		if (m_cmFight[nIndex + 1].nKind != CHECK)
		{
			bOwnPerpCheck = FALSE;
		}

		if (m_nHistoryCBCheckSum[nIndex] == m_nCBCheckSum)
		{
			nRepCnt++;
			
			if (nRepCnt >= nCanRep)
			{
				if (bOppPerpCheck == TRUE)
				{
					return 4;
				}
				
				if (bOwnPerpCheck == TRUE)
				{
					return 2;
				}
				
				return 1;
			}
		}
		
		nIndex -= 2;
	}

	if (bBreak == TRUE)
	{
		return FALSE;
	}

	//�߷�·���к���ż�����߷�,��ʱ�߷�·���е��߷����Ѽ���
	if ((m_nCurDepth & 1) == 0)
	{
		nIndex = m_nNoEatMoveCnt_BS - 1;
	}
	else
	{
		m_Moves_NoEat[m_nNoEatMoveCnt_BS] = m_cmFight[1];
		nIndex = m_nNoEatMoveCnt_BS;
	}

	nIndex--;
	while (nIndex >= 0)
	{
		cmThis = m_Moves_NoEat[nIndex];

		if (cmThis.nKind != CHECK)
		{
			bOppPerpCheck = FALSE;
		}
		if (m_Moves_NoEat[nIndex + 1].nKind != CHECK)
		{
			bOwnPerpCheck = FALSE;
		}

		if (m_nHistoryCBCheckSum_NoEat[nIndex] == m_nCBCheckSum)
		{
			nRepCnt++;

			if (nRepCnt >= nCanRep)
			{
				if (bOppPerpCheck == TRUE)
				{
					return 4;
				}
				
				if (bOwnPerpCheck == TRUE)
				{
					return 2;
				}
				
				return 1;
			}
		}

		nIndex -= 2;
	}

	return FALSE;
}

/*******************************************************************************************************
������ : GetVersion
��  �� : �õ�������汾��
��  �� : 
        FSCHAR *szVersion : 
����ֵ : ��
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��8��7�� 12:48:02
*******************************************************************************************************/
FSVOID CSearchEngine::GetVersion(FSCHAR *szVersion) const
{
	_ASSERT(szVersion != 0);

	szVersion = ENGINEVERSION;
}

/*******************************************************************************************************
������ : JumpNoMatterChar
��  �� : ����szStrǰ��Ŀո�,Tab
��  �� : 
        FSCHAR *szStr :  
����ֵ : ����Ϊ FSCHAR *
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��9��28�� 22:23:23
*******************************************************************************************************/
FSCHAR * CSearchEngine::JumpNoMatterChar(FSCHAR *szStr)
{
	_ASSERT(szStr != 0);

	FSINT nPos = 0;

	//����ǰ��Ŀո�,Tab��
	while (szStr[nPos] == ' ' || szStr[nPos] == '\t')
	{
		nPos++;
	}

	return szStr + nPos;
}

/*******************************************************************************************************
������ : JumpDigit
��  �� : ����szStrǰ�������
��  �� : 
        FSCHAR *szStr :  
����ֵ : ����Ϊ FSCHAR *
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��9��28�� 22:23:51
*******************************************************************************************************/
FSCHAR * CSearchEngine::JumpDigit(FSCHAR *szStr)
{
	_ASSERT(szStr != 0);

	FSINT nPos = 0;

	//����ǰ�������
	while (isdigit(szStr[nPos]))
	{
		nPos++;
	}

	return szStr + nPos;
}

/*******************************************************************************************************
������ : Rand32
��  �� : ����32λ�������
��  �� : ��
����ֵ : ����Ϊ FSULONG
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��9��28�� 22:24:02
*******************************************************************************************************/
inline FSULONG CSearchEngine::Rand32()
{
	return rand() ^ ((FSULONG)rand() << 15) ^ ((FSULONG)rand() << 30);
}

/*******************************************************************************************************
������ : Rand64
��  �� : ����32λ�������
��  �� : ��
����ֵ : ����Ϊ FSUINT64
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��9��28�� 22:24:19
*******************************************************************************************************/
inline FSUINT64 CSearchEngine::Rand64()
{
	return rand() ^ ((FSUINT64)rand() << 15) ^ ((FSUINT64)rand() << 30) ^
		((FSUINT64)rand() << 45) ^ ((FSUINT64)rand() << 60);
}

/*******************************************************************************************************
������ : UDgreater
��  �� : �����߷��ĵ÷ֱȽ������߷�cm1, cm2
��  �� : 
        CHESSMOVE cm1 :  
		CHESSMOVE cm2 :  
����ֵ : ����Ϊ FSbool
��  �� : 0.1
��  �� : ahei
ʱ  �� : 2005��9��28�� 22:24:31
*******************************************************************************************************/
inline FSbool UDgreater(CHESSMOVE cm1, CHESSMOVE cm2)
{
	return cm1.nScore > cm2.nScore;
}

/*******************************************************************************************************
������ : ExitEgn
��  �� : 
��  �� : ��
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��10��21�� 22:40:19
*******************************************************************************************************/
FSVOID CSearchEngine::ExitEgn()
{
//	SaveKnightPreMoves();
	if (m_bAllocateMem == TRUE)
	{
		FreeMem();
	}
}

/*******************************************************************************************************
������ : IsValidPosition
��  �� : 
��  �� : ��
����ֵ : ����Ϊ FSBOOL
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��10��24�� 16:04:17
*******************************************************************************************************/
FSBOOL CSearchEngine::IsValidPosition()
{
	//�������κ�һ���Ͻ��Ѳ�����
//	if (m_nPiecePos[m_nChessID2PieceID[m_nKing[m_bIsRed]][0]] == INVALIDPIECEPOS || 
//		m_nPiecePos[m_nChessID2PieceID[m_nKing[1 - m_bIsRed]][0]] == INVALIDPIECEPOS)
//	{
//		return FALSE;
//	}

	return TRUE;
}

/*******************************************************************************************************
������ : PrintPVLine
��  �� : ���PV�߷�
��  �� : ��
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��10��24�� 22:20:03
*******************************************************************************************************/
inline FSVOID CSearchEngine::PrintPVLine()
{
	if (m_PVLine.nCount <= 0)
	{
		return;
	}
	
	FSINT	i;
	FSINT	t = clock() - m_nTimeStartThink;
	FSCHAR	szMove[MAXMOVESTRLEN];

	SWITCHEGN
	(
	DISPMSG5(cout, "info depth ", m_nCurSearchDepth_ID, " score ", m_PVLine.pMoveList[0].nScore, " pv");
	for (i = 0; i < m_PVLine.nCount; i++)
	{
		ChessMove2Str(m_PVLine.pMoveList[i], szMove);
		DISPMSG2(cout, " ", szMove);
	}
	DISPMSG0_ENDL(cout);
	DISPMSG2_ENDL(cout, "info time ", MS2S(t));
	STATEMENT_STAT
	(
		if (t > 0)
		{
			DISPMSG2_ENDL(cout, "info nps ", m_nVistNodeCnt * 1000 / t);
		}
		m_nNonQuiesNodeCnt = m_nVistNodeCnt - m_nQuiesNodeCnt;
		
		DISPMSG2_ENDL(cout, "info nodes ", m_nVistNodeCnt);
		
		DISPMSG7_ENDL(cout, "info nonquies nodes ", m_nNonQuiesNodeCnt, " ", 
			setprecision(3), setw(3), RATE(m_nNonQuiesNodeCnt, m_nVistNodeCnt), "%");
		DISPMSG11_ENDL(cout, "info alpha nodes ", m_nAlphaNodeCnt, " ", 
			setprecision(3), setw(3), RATE(m_nAlphaNodeCnt, m_nVistNodeCnt), "% in all and ", 
			setprecision(3), setw(3), RATE(m_nAlphaNodeCnt, m_nNonQuiesNodeCnt), "% in nonquies nodes");
		DISPMSG11_ENDL(cout, "info pv nodes ", m_nAlphaNodeCnt, " ", 
			setprecision(3), setw(3), RATE(m_nPVNodeCnt, m_nVistNodeCnt), "% in all and ", 
			setprecision(3), setw(3), RATE(m_nPVNodeCnt, m_nNonQuiesNodeCnt), "% in nonquies nodes");
		DISPMSG11_ENDL(cout, "info beta nodes ", m_nAlphaNodeCnt, " ", 
			setprecision(3), setw(3), RATE(m_nBetaNodeCnt, m_nVistNodeCnt), "% in all and ", 
			setprecision(3), setw(3), RATE(m_nBetaNodeCnt, m_nNonQuiesNodeCnt), "% in nonquies nodes");
		DISPMSG11_ENDL(cout, "info null nodes ", m_nAlphaNodeCnt, " ", 
			setprecision(3), setw(3), RATE(m_nNullNodeCnt, m_nVistNodeCnt), "% in all and ", 
			setprecision(3), setw(3), RATE(m_nNullNodeCnt, m_nNonQuiesNodeCnt), "% in nonquies nodes");
		
		DISPMSG7_ENDL(cout, "info quies nodes ", m_nQuiesNodeCnt, " ", 
			setprecision(3), setw(3), RATE(m_nQuiesNodeCnt, m_nVistNodeCnt), "%");
		DISPMSG11_ENDL(cout, "info quies alpha nodes ", m_nQuiesAlphaNodeCnt, " ", 
			setprecision(3), setw(3), RATE(m_nQuiesAlphaNodeCnt, m_nVistNodeCnt), "% in all and ", 
			setprecision(3), setw(3), RATE(m_nQuiesAlphaNodeCnt, m_nQuiesNodeCnt), "% in quies nodes");
		DISPMSG11_ENDL(cout, "info quies pv nodes ", m_nQuiesAlphaNodeCnt, " ", 
			setprecision(3), setw(3), RATE(m_nQuiesPVNodeCnt, m_nVistNodeCnt), "% in all and ", 
			setprecision(3), setw(3), RATE(m_nQuiesPVNodeCnt, m_nQuiesNodeCnt), "% in quies nodes");
		DISPMSG11_ENDL(cout, "info quies beta nodes ", m_nQuiesAlphaNodeCnt, " ", 
			setprecision(3), setw(3), RATE(m_nQuiesBetaNodeCnt, m_nVistNodeCnt), "% in all and ", 
			setprecision(3), setw(3), RATE(m_nQuiesBetaNodeCnt, m_nQuiesNodeCnt), "% in quies nodes");
		DISPMSG11_ENDL(cout, "info quies null nodes ", m_nQuiesAlphaNodeCnt, " ", 
			setprecision(3), setw(3), RATE(m_nQuiesNullNodeCnt, m_nVistNodeCnt), "% in all and ", 
			setprecision(3), setw(3), RATE(m_nQuiesNullNodeCnt, m_nQuiesNodeCnt), "% in quies nodes");

		DISPMSG4_ENDL(cout, "Hash hit rate is ", setprecision(3), 
			RATE(m_nHitHashCnt, m_nRecordHashCnt), "%");
		DISPMSG4_ENDL(cout, "Hash probe hit rate is ", setprecision(3), 
			RATE(m_nHitHashCnt, m_nProbeHashCnt), "%");
		DISPMSG4_ENDL(cout, "QSHash hit rate is ", setprecision(3), 
			RATE(m_nHitQSHashCnt, m_nRecordQSHashCnt), "%");
		DISPMSG4_ENDL(cout, "QSHash probe hit rate is ", setprecision(3), 
			RATE(m_nHitQSHashCnt, m_nProbeQSHashCnt), "%");

		DISPMSG4_ENDL(cout, "Hash move cut rate is ", setprecision(3), 
			RATE(m_nHashBetaNodeCnt, m_nBetaNodeCnt), "%");
		DISPMSG4_ENDL(cout, "Eat move cut rate is ", setprecision(3), 
			RATE(m_nCapMoveBetaNodeCnt, m_nBetaNodeCnt), "%");
		DISPMSG4_ENDL(cout, "Killer move cut rate is ", setprecision(3), 
			RATE(m_nKillerMoveBetaNodeCnt, m_nBetaNodeCnt), "%");
		DISPMSG4_ENDL(cout, "NoCap move cut rate is ", setprecision(3), 
			RATE(m_nNoCapMoveBetaNodeCnt, m_nBetaNodeCnt), "%");
	);
	STATEMENT_GETMAXSCHDPT(DEBUGASSERT(m_nMaxSearchDepth == 0 || 
		m_nMaxSearchDepth >= m_nCurSearchDepth_ID););
	STATEMENT_GETMAXSCHDPT(DISPMSG2_ENDL(cout, "info maxdepth ", m_nMaxSearchDepth););
	STATEMENT_GETMAXSELSCHDPT(DISPMSG2_ENDL(cout, "info seldepth ", m_nMaxSelSchDpt););
	);
}

/*******************************************************************************************************
������ : SetBestMove
��  �� : 
��  �� : ��
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��10��26�� 15:59:26
*******************************************************************************************************/
//FSVOID CSearchEngine::SetBestMove()
//{
//
//}

/*******************************************************************************************************
������ : SearchTest
��  �� : 
��  �� : 
        FSINT nDepth                 : 
		FSINT alpha                  : 
		FSINT beta                   : 
		KILLERMOVETAB &killerMoveTab : 
����ֵ : ����Ϊ FSINT
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��10��26�� 15:59:21
*******************************************************************************************************/
/*FSINT CSearchEngine::SearchTest(FSINT nDepth, FSINT nSearchDepth, FSINT alpha, FSINT beta, KILLERMOVETAB& killerMoveTab)
{
	if (nDepth >= nSearchDepth)
	{
	//	return QuiesSearch(nDepth + 1, alpha, beta);
		return Evaluate();
	}
	
	DEBUGASSERT(nDepth >= 0 && nDepth <= MAXSEARCHDEPTH);
//	DEBUGASSERT(nDepth >= 0);
	DEBUGASSERT(beta <= -g_nPerCheckValue);
	DEBUGASSERT(alpha >= g_nPerCheckValue && alpha <= beta);

	//˫������
	DEBUGASSERT(m_nPiecePos[m_nKingID2PieceID[m_bIsRed]] != INVALIDPIECEPOS);
	DEBUGASSERT(m_nPiecePos[m_nKingID2PieceID[1 - m_bIsRed]] != INVALIDPIECEPOS);

	FSINT	i;
	FSINT	nBestEval = m_nCurDepth - g_nMateValue;
	FSINT	nNewDepth;
	FSINT	nCount;							//�߷���
	FSINT	nEval;							//��ֵ
	FSINT	nToChessID;						//���Ե��������ͱ��
	FSINT	nToPieceID;						//���Ե����ӱ��
	FSINT	nHashFlag = HASHALPHA;			//hash�ڵ��־
	FSBOOL	bCutoff = FALSE;				//�Ƿ񱻽ض�
	FSBOOL	bFindPV = FALSE;				//�Ƿ��ҵ�PV�߷�
	FSINT	nBestMoveNum = 0;				//�õ��߷���,���ڼ�¼��ʷ�÷�
	FSBOOL	bFirstMove = TRUE;				//���������߷��Ƿ��ǵ�һ���߷�,������ʷ�ü���
	FSBOOL	bMateThreat = FALSE;			//ɱ����в,����ɱ����в����
	FSBOOL	bSingleResponse = FALSE;		//��һӦ��,���ڵ�һӦ������
//	FSBOOL	bDangerPosition = FALSE;		//Σ�վ���,������в����������[2006/1/1 11:24:24]
	FSINT	nCanMoveCnt = 0;				//����Ӧ�ŵ��߷���,���ڵ�һӦ����

	CHESSMOVE		cm = m_cmNullMove;
	CHESSMOVE		cmBestMove = m_cmNullMove;
	KILLERMOVETAB	subKillerMoveTab; //ɱ���߷���

	//��ʱ��˼��ʱ,��ʱ����
	if (m_ThinkType == TT_TIME && (clock() - m_nTimeStartThink) > m_nTimeThinkLimit && 
		(m_bPonder == FALSE || m_bPonderHit == TRUE))
	{
		SearchStop();
#ifdef _WIN32
		ExitThread(0);
#else
#endif
	}

	//̽��hash��
	if ((nEval = ProbeHash(nDepth, nSearchDepth, alpha, beta, cm)) != g_nInvalidValue)
	{
		// ���ڵ����ظ�[2005/11/14 16:55:27]
		if (nDepth > 0)
		{
			return nEval;
		}

		MakeMove(cm, nToChessID, nToPieceID);
		if (IsRep(3) == FALSE)
		{
			UnMakeMove(cm, nToChessID, nToPieceID);

			m_PVLine.nCount = 0;
			GetPVLineFromHash(m_nCurSearchDepth_ID);
			PrintPVLine();
			
			return nEval;
		}
		// ˵���Ѿ�����ѭ��,������û��������Ͳ���������߷�[2005/12/21 22:02:08]
		cm = m_cmNullMove;
		UnMakeMove(cm, nToChessID, nToPieceID);
	}

	// �ڵ�����4:History Move[2005/10/23 18:45:05]
	//�������в������߷�
	CHESSMOVE cmPrev = m_cmFight[m_nCurDepth];

	GenMoves(nDepth);
	nCount = m_nMovesCnt;
	
	for (i = 0; i < nCount; i++)
	{
		cm = m_MoveList[nDepth][i];

//		if ((cm.nKind & CAP) == 0 || (cm.nKind & ESCAPE) == 0)
		if ((cm.nKind & CAP) == 0)
		{
			continue;
		}

		MakeMove(cm, nToChessID, nToPieceID);

		//�жϸ��߷��Ƿ�Ϸ�
		if (IsLegalMove(cm) == FALSE)
		{
			UnMakeMove(cm, nToChessID, nToPieceID);
			continue;
		}
		
		// ��������,ɱ����в����,��һӦ������,��������[2005/11/1 18:49:14]
		DEBUGASSERT(m_nCurDepth >= 1);
		nNewDepth = nSearchDepth;

		nEval = -SearchTest(nDepth + 1, nNewDepth, -beta, -alpha, subKillerMoveTab);
		
		UnMakeMove(cm, nToChessID, nToPieceID);

		m_MoveList[nDepth][i].nScore = nEval;

		if (nEval > nBestEval)
		{
			nBestEval = nEval;
			nBestMoveNum = i;
			cmBestMove = cm;
			cmBestMove.nScore = nEval;

			if (nEval > alpha)
			{
				alpha = nEval;
				
				if (nBestEval >= beta)
				{
					nHashFlag = HASHBETA;
					
					m_nMoveCutoffCnt[cm.nFrom][cm.nTo]++;

					STATEMENT_STAT(m_nBetaNodeCnt++;);
					STATEMENT_STAT(m_nNoCapMoveBetaNodeCnt++;);

					RecordHash(nDepth, nSearchDepth, alpha, nHashFlag, cmBestMove);

					return alpha;
				}
				
				nHashFlag = HASHEXACT;

				if (nDepth == 0)
				{
					PrintBetterPV(nDepth, nSearchDepth, nEval, nHashFlag, cmBestMove);
					if (nEval + MAXSEARCHDEPTH >= g_nMateValue)
					{
						return nEval;
					}
				}
			}
		}
	}

//	for (i = 0; i < nCount; i++)
//	{
//		cm = m_MoveList[nDepth][i];
//
//		if ((cm.nKind & ESCAPE) == 0)
//		{
//			continue;
//		}
//
//		MakeMove(cm, nToChessID, nToPieceID);
//
//		//�жϸ��߷��Ƿ�Ϸ�
//		if (IsLegalMove(cm) == FALSE)
//		{
//			UnMakeMove(cm, nToChessID, nToPieceID);
//			continue;
//		}
//		
//		// ��������,ɱ����в����,��һӦ������,��������[2005/11/1 18:49:14]
//		DEBUGASSERT(m_nCurDepth >= 1);
//		nNewDepth = nSearchDepth;
//
//		nEval = -SearchTest(nDepth + 1, nNewDepth, -beta, -alpha, subKillerMoveTab);
//		
//		UnMakeMove(cm, nToChessID, nToPieceID);
//
//		m_MoveList[nDepth][i].nScore = nEval;
//
//		if (nEval > nBestEval)
//		{
//			nBestEval = nEval;
//			nBestMoveNum = i;
//			cmBestMove = cm;
//			cmBestMove.nScore = nEval;
//
//			if (nEval > alpha)
//			{
//				alpha = nEval;
//				
//				if (nBestEval >= beta)
//				{
//					nHashFlag = HASHBETA;
//					
//					m_nMoveCutoffCnt[cm.nFrom][cm.nTo]++;
//
//					STATEMENT_STAT(m_nBetaNodeCnt++;);
//					STATEMENT_STAT(m_nNoCapMoveBetaNodeCnt++;);
//
//					RecordHash(nDepth, nSearchDepth, alpha, nHashFlag, cmBestMove);
//
//					return alpha;
//				}
//				
//				nHashFlag = HASHEXACT;
//
//				if (nDepth == 0)
//				{
//					PrintBetterPV(nDepth, nSearchDepth, nEval, nHashFlag, cmBestMove);
//					if (nEval + MAXSEARCHDEPTH >= g_nMateValue)
//					{
//						return nEval;
//					}
//				}
//			}
//		}
//	}

//	if (cmPrev != m_cmNullMove)
//	{
////		CHESSMOVE pMoveList[MAXALLMOVESCNT];
//		CHESSMOVE pMoveList[MAXMOVECNT];
//		FSINT nTempCnt;
//		FSINT nChessID;
//		
//		m_bIsRed = 1 - m_bIsRed;
//		nTempCnt = (this->*m_GenMovesFun2[g_nChessID2Chess[m_nCBChessID[cmPrev.nTo]]])(cmPrev.nTo, pMoveList);
//		m_bIsRed = 1 - m_bIsRed;
//		m_nMovesCnt = 0;
//		for (i = 0; i < nTempCnt; i++)
//		{
//			cm = pMoveList[i];
//			
//			if ((nChessID = m_nCBChessID[cm.nTo]) != NOCHESS && m_bIsBlack_ChessID[nChessID] != m_bIsBlack_ChessID[m_nCBChessID[cmPrev.nTo]])
//			{
//				DEBUGASSERT((m_bIsBlack_ChessID[nChessID] ^ m_bIsBlack_ChessID[m_nCBChessID[cmPrev.nTo]]) > 0);
//				(this->*m_GenMovesFun[g_nChessID2Chess[nChessID]])(cm.nTo, nDepth);
//			}
//		}
//	}
//	else
//	{
//		GenMoves(nDepth);
//	}
//
//	nCount = m_nMovesCnt;
//	DEBUGASSERT(nCount >= 0 && nCount <= MAXALLMOVESCNT);
//	//ɨ�������߷�
//	for (i = 0; i < nCount; i++)
//	{
//		cm = m_MoveList[nDepth][i];
//
//		MakeMove(cm, nToChessID, nToPieceID);
//
//		//�жϸ��߷��Ƿ�Ϸ�
//		if (IsLegalMove(cm) == FALSE)
//		{
//			UnMakeMove(cm, nToChessID, nToPieceID);
//			continue;
//		}
//		
//		// ��������,ɱ����в����,��һӦ������,��������[2005/11/1 18:49:14]
//		DEBUGASSERT(m_nCurDepth >= 1);
//		nNewDepth = nSearchDepth;
//
//		nEval = -SearchTest(nDepth + 1, nNewDepth, -beta, -alpha, subKillerMoveTab);
//		
//		UnMakeMove(cm, nToChessID, nToPieceID);
//
//		m_MoveList[nDepth][i].nScore = nEval;
//
//		if (nEval > nBestEval)
//		{
//			nBestEval = nEval;
//			nBestMoveNum = i;
//			cmBestMove = cm;
//			cmBestMove.nScore = nEval;
//
//			if (nEval > alpha)
//			{
//				alpha = nEval;
//				
//				if (nBestEval >= beta)
//				{
//					nHashFlag = HASHBETA;
//					
//					m_nMoveCutoffCnt[cm.nFrom][cm.nTo]++;
//
//					STATEMENT_STAT(m_nBetaNodeCnt++;);
//					STATEMENT_STAT(m_nNoCapMoveBetaNodeCnt++;);
//
//					RecordHash(nDepth, nSearchDepth, alpha, nHashFlag, cmBestMove);
//
//					return alpha;
//				//	break;
//				}
//				
//				nHashFlag = HASHEXACT;
//
//				if (nDepth == 0)
//				{
//					PrintBetterPV(nDepth, nSearchDepth, nEval, nHashFlag, cmBestMove);
//					if (nEval + MAXSEARCHDEPTH >= g_nMateValue)
//					{
//						return nEval;
//					}
//				}
//			}
//		}
//	}
//
//	GenMoves(nDepth);
//	nCount = m_nMovesCnt;
//	DEBUGASSERT(nCount >= 0 && nCount <= MAXALLMOVESCNT);
	//ɨ�������߷�
	for (i = 0; i < nCount; i++)
	{
		cm = m_MoveList[nDepth][i];

		MakeMove(cm, nToChessID, nToPieceID);

		//�жϸ��߷��Ƿ�Ϸ�
		if (IsLegalMove(cm) == FALSE)
		{
			UnMakeMove(cm, nToChessID, nToPieceID);
			continue;
		}
		
		// ��������,ɱ����в����,��һӦ������,��������[2005/11/1 18:49:14]
		DEBUGASSERT(m_nCurDepth >= 1);
		nNewDepth = nSearchDepth;

		nEval = -SearchTest(nDepth + 1, nNewDepth, -beta, -alpha, subKillerMoveTab);
		
		UnMakeMove(cm, nToChessID, nToPieceID);

		m_MoveList[nDepth][i].nScore = nEval;

		if (nEval > nBestEval)
		{
			nBestEval = nEval;
			nBestMoveNum = i;
			cmBestMove = cm;
			cmBestMove.nScore = nEval;

			if (nEval > alpha)
			{
				alpha = nEval;
				
				if (nBestEval >= beta)
				{
					nHashFlag = HASHBETA;
					
					m_nMoveCutoffCnt[cm.nFrom][cm.nTo]++;

					STATEMENT_STAT(m_nBetaNodeCnt++;);
					STATEMENT_STAT(m_nNoCapMoveBetaNodeCnt++;);

					RecordHash(nDepth, nSearchDepth, alpha, nHashFlag, cmBestMove);

					break;
				}
				
				nHashFlag = HASHEXACT;

				if (nDepth == 0)
				{
					PrintBetterPV(nDepth, nSearchDepth, nEval, nHashFlag, cmBestMove);
					if (nEval + MAXSEARCHDEPTH >= g_nMateValue)
					{
						return nEval;
					}
				}
			}
		}
	}

	RecordHash(nDepth, nSearchDepth, alpha, nHashFlag, cmBestMove);

	return nBestEval;
}*/

/*******************************************************************************************************
������ : SearchStop
��  �� : 
��  �� : ��
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��10��26�� 15:59:10
*******************************************************************************************************/
FSVOID CSearchEngine::SearchStop()
{
	FSINT	nTimeThinked;
	FSCHAR	szMove[MAXBUFFERLEN];
	FSINT	nToChessID;
	FSINT	nToPieceID;

	CHESSMOVE cm;

	STATEMENT_TEST(CString str;);

	UNDOMOVE undoMove;
	
	nTimeThinked = clock() - m_nTimeStartThink;
	m_nTotalSearchTime += nTimeThinked;
	if (nTimeThinked > m_nMaxSearchTime)
	{
		m_nMaxSearchTime = nTimeThinked;
	}
	
	if (m_bStopThink == TRUE)
	{
		m_nCurSearchDepth_ID--;
	}

	switch(m_EgnProtocol)
	{
	case EgnPUCCI:
	case EgnPAhei:
		break;
	
	case EgnPQianHong:
		cm = m_PVLine.pMoveList[0];
		if (m_nCBPieceID[cm.nTo] == INVALIDPIECEID)
		{
			m_nHistoryCBCheckSum_NoEat[m_nNoEatMoveCnt_BS] = m_nCBCheckSum;
			MakeMove(m_Moves_NoEat[m_nNoEatMoveCnt_BS] = cm, nToChessID, nToPieceID);
			m_Moves_NoEat[m_nNoEatMoveCnt_BS].nKind = m_cmFight[m_nCurDepth - 1].nKind;
			m_nNoEatMoveCnt_BS++;
		}
		else
		{
			m_nNoEatMoveCnt_BS = 0;
			MakeMove(cm, nToChessID, nToPieceID);
		}

		FILLUNDOMOVE(undoMove, m_PVLine.pMoveList[0], nToChessID, nToPieceID);
		m_UndoMoveStack.push(undoMove);
		break;

	default:
		break;
	}

	//��ʾ����˼�����
	ChessMove2Str(m_PVLine.pMoveList[0], szMove);
	switch(m_EgnProtocol)
	{
	case EgnPUCCI:
	case EgnPAhei:
		cout << "bestmove " << szMove;
		if (m_PVLine.nCount > 1)
		{
			ChessMove2Str(m_PVLine.pMoveList[1], szMove);
			cout << " ponder " << szMove;
		}
		cout << endl;
		DISPMSG2_ENDL(cout, "info time ", MS2S(nTimeThinked));
		cout << "info longest time " << setprecision(3) << MS2S(m_nMaxSearchTime) << endl;
		cout << "info average time " << setprecision(3) 
			 << m_nTotalSearchTime * 1.0 / (1000 * m_nSearchCnt)  << endl;
		STATEMENT_STAT(
		DISPMSG4_ENDL(cout, "Hash hit rate is ", setprecision(3), 
			RATE(m_nHitHashCnt, m_nRecordHashCnt), "%");
		DISPMSG4_ENDL(cout, "Hash probe hit rate is ", setprecision(3), 
			RATE(m_nHitHashCnt, m_nProbeHashCnt), "%");
		DISPMSG4_ENDL(cout, "QSHash hit rate is ", setprecision(3), 
			RATE(m_nHitQSHashCnt, m_nRecordQSHashCnt), "%");
		DISPMSG4_ENDL(cout, "QSHash probe hit rate is ", setprecision(3), 
			RATE(m_nHitQSHashCnt, m_nProbeQSHashCnt), "%");

		DISPMSG4_ENDL(cout, "Hash move cut rate is ", setprecision(3), 
			RATE(m_nHashBetaNodeCnt, m_nBetaNodeCnt), "%");
		DISPMSG4_ENDL(cout, "Eat move cut rate is ", setprecision(3), 
			RATE(m_nCapMoveBetaNodeCnt, m_nBetaNodeCnt), "%");
		DISPMSG4_ENDL(cout, "Killer move cut rate is ", setprecision(3), 
			RATE(m_nKillerMoveBetaNodeCnt, m_nBetaNodeCnt), "%");
		DISPMSG4_ENDL(cout, "NoCap move cut rate is ", setprecision(3), 
			RATE(m_nNoCapMoveBetaNodeCnt, m_nBetaNodeCnt), "%");
		);
		break;
		
	case EgnPQianHong:
		cout << szMove << endl;
		STATEMENT_TEST
		(
		str.Format("%s", szMove);
		MessageBox(NULL, str, "����", MB_OK);
		)
		STATEMENT_TESTQHP
		(
		cout << "bestmove " << szMove;
		if (m_PVLine.nCount > 1)
		{
			ChessMove2Str(m_PVLine.pMoveList[1], szMove);
			cout << " ponder " << szMove;
		}
		cout << endl;
		DISPMSG2_ENDL(cout, "info time ", MS2S(t));
		cout << "info longest time " << setprecision(3) << MS2S(m_nMaxSearchTime) << endl;
		cout << "info average time " << setprecision(3) 
			 << m_nTotalSearchTime * 1.0 / (1000 * m_nSearchCnt)  << endl;
		);
		break;

	default:
		break;
	}
	
	ChessMove2Str(m_PVLine.pMoveList[0], szMove);
	if (m_nCurSearchDepth_ID > 1)
	{
		ChessMove2Str(m_PVLine.pMoveList[1], szMove);
	}

	m_bPonder = FALSE;
	m_bPonderHit = FALSE;

	m_hThinkThread = NULL;
	m_esUCCIEgnStatus = EgnSIdle;	//��������б�־
}

/*******************************************************************************************************
������ : UCCIBusy
��  �� : 
��  �� : ��
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��10��26�� 15:59:04
*******************************************************************************************************/
inline FSVOID CSearchEngine::UCCIBusy()
{
	cout << "busy" << endl;
}

/*******************************************************************************************************
������ : ThinkTimeOut
��  �� : 
��  �� : 
        FSUINT uID     : 
		FSUINT uMsg    : 
		FSDWORD dwUser : 
		FSDWORD dw1    : 
		FSDWORD dw2    : 
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��10��26�� 15:58:56
*******************************************************************************************************/
#ifdef _WIN32
FSVOID CALLBACK CSearchEngine::ThinkTimeOut(FSUINT uTimerID, FSUINT uMsg, FSDWORD dwUser, FSDWORD dw1, FSDWORD dw2)
{
//	MMRESULT mmResult = timeKillEvent(uTimerID);

//	if (mmResult == MMSYSERR_INVALPARAM)
	{
		DISPMSG3_ENDL(cout,		"��ʱ��", uTimerID, "������!");
	}
//	UCCIStopThink();
	((CSearchEngine*)dwUser)->UCCIStopThink();
}
#else
#endif

/*******************************************************************************************************
������ : UCCIStopThink
��  �� : 
��  �� : ��
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��10��26�� 15:58:51
*******************************************************************************************************/
#ifdef _WIN32
FSVOID CSearchEngine::UCCIStopThink()
{
	if (m_hThinkThread == NULL)
	{
		return;
	}
	
	m_bStopThink = TRUE;
	m_esUCCIEgnStatus = EgnSIdle;

	//���������߳�
	KillThread(m_hThinkThread);

	SearchStop();
}
#else
#endif

/*******************************************************************************************************
������ : IDSearchInit
��  �� : 
��  �� : ��
����ֵ : ����Ϊ inline
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��11��6�� 15:52:08
*******************************************************************************************************/
inline FSVOID CSearchEngine::IDSearchInit()
{
	STATEMENT_STAT
	(
	m_nVistNodeCnt = 0;
	m_nAlphaNodeCnt = 0;
	m_nPVNodeCnt = 0;
	m_nBetaNodeCnt = 0;
	m_nNullNodeCnt = 0;

	m_nHashBetaNodeCnt = 0;
	m_nCapMoveBetaNodeCnt = 0;
	m_nKillerMoveBetaNodeCnt = 0;
	m_nNoCapMoveBetaNodeCnt = 0;

	m_nQuiesNodeCnt = 0;
	m_nQuiesAlphaNodeCnt = 0;	
	m_nQuiesPVNodeCnt = 0;
	m_nQuiesBetaNodeCnt = 0;
	m_nQuiesNullNodeCnt = 0;
	);

	STATEMENT_GETMAXSCHDPT(m_nMaxSearchDepth = 0;);
	STATEMENT_GETMAXSELSCHDPT(m_nMaxSelSchDpt = 0;);
}

/*******************************************************************************************************
������ : MakeNullMove
��  �� : 
��  �� : ��
����ֵ : ����Ϊ inline
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��11��6�� 15:52:02
*******************************************************************************************************/
inline FSVOID CSearchEngine::MakeNullMove()
{
	ChangeSide();
	m_nCurDepth++;
	m_cmFight[m_nCurDepth] = m_cmNullMove;
	DEBUGASSERT(m_nCurDepth >= 1);
#	ifdef GETMAXSCHDPT
	if (m_nCurDepth > m_nMaxSearchDepth)
	{
		m_nMaxSearchDepth = m_nCurDepth;
	}
#	endif	
}

/*******************************************************************************************************
������ : UnMakeNullMove
��  �� : 
��  �� : ��
����ֵ : ����Ϊ inline
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��11��6�� 15:51:52
*******************************************************************************************************/
inline FSVOID CSearchEngine::UnMakeNullMove()
{
	ChangeSide();
	m_nCurDepth--;
	DEBUGASSERT(m_nCurDepth >= 0);
}

/*******************************************************************************************************
������ : PrintQHInfo
��  �� : 
��  �� : ��
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��11��6�� 15:51:47
*******************************************************************************************************/
FSVOID CSearchEngine::PrintQHInfo()
{
	DISPMSG1_ENDL(cout, QIANHONGPVER);
	DISPMSG1_ENDL(cout, ENGINENAME" "ENGINEVERSION);
	
	DISPMSG2_ENDL(cout, "LEVELS ", QIANHONGLEVELCNT);
	DISPMSG1_ENDL(cout, "1 - 6 plies");
	DISPMSG1_ENDL(cout, "2 - 7 plies");
	DISPMSG1_ENDL(cout, "3 - 8 plies");
	DISPMSG1_ENDL(cout, "4 - 9 plies");
	DISPMSG1_ENDL(cout, "5 - 10 plies");
	DISPMSG1_ENDL(cout, "6 - 11 plies");
	DISPMSG1_ENDL(cout, "7 - 12 plies");
	DISPMSG1_ENDL(cout, "8 - 13 plies");
	DISPMSG1_ENDL(cout, "9 - max ply");
	
	DISPMSG1_ENDL(cout, "UNDO 1");
	DISPMSG1_ENDL(cout, "HINTS 0");
	DISPMSG1_ENDL(cout, "RULES 0");
	DISPMSG1_ENDL(cout, "BGTHINK 0");
	DISPMSG1_ENDL(cout, "TIMEOUT 1");
	DISPMSG1_ENDL(cout, "QianHong AI Engine for QianHong "QIANHONGPVER);
	DISPMSG1_ENDL(cout, "By ahei, 2005.3 - Now");
	DISPMSG1_ENDL(cout, "ENDINFO");
}

/*******************************************************************************************************
������ : GetQianHongCmd
��  �� : 
��  �� : 
        FSCHAR *& szQianHongCmd : 
����ֵ : ����Ϊ QianHongCmd
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��11��6�� 15:51:39
*******************************************************************************************************/
QianHongCmd CSearchEngine::GetQianHongCmd(FSCHAR *& szQianHongCmd)
{
	FSINT nFSCHARSize = sizeof(FSCHAR);

	if (strlen(szQianHongCmd) >= 3)
	{
		FSCHAR	szTemp[MAXBUFFERLEN];
		
		szQianHongCmd = JumpNoMatterChar(szQianHongCmd);	
		
		strncpy(szTemp, szQianHongCmd, 3);
		szTemp[3] = '\0';
		strlwr(szTemp);
		
		if (!strncmp(szTemp, "fen", 3))
		{
			szQianHongCmd += 3;
			return QHCFEN;
		}
	}

	strlwr(szQianHongCmd);

	if (!strncmp(szQianHongCmd, "level", 5))
	{
		szQianHongCmd += 5;
		return QHCLevel;
	}
	
	if (!strncmp(szQianHongCmd, "play", 4))
	{
		szQianHongCmd += 4;
		return QHCPlay;
	}

	if (!strncmp(szQianHongCmd, "load", 4))
	{
		szQianHongCmd += 4;
		return QHCLoad;
	}

	if (!strncmp(szQianHongCmd, "ai", 2))
	{
		szQianHongCmd += 2;
		return QHCAI;
	}

	if (!strncmp(szQianHongCmd, "abort", 5))
	{
		szQianHongCmd += 5;
		return QHCAbort;
	}

	//������Ҫ����quit����ǰ�洦��
	if (!strncmp(szQianHongCmd, "quitqh", 6))
	{
		szQianHongCmd += 6;
		return QHCQuitQH;
	}

	if (!strncmp(szQianHongCmd, "quit", 4))
	{
		szQianHongCmd += 4;
		return QHCQuit;
	}

	if (!strncmp(szQianHongCmd, "exit", 4))
	{
		szQianHongCmd += 4;
		return QHCQuit;
	}

	if (!strncmp(szQianHongCmd, "undo", 4))
	{
		szQianHongCmd += 4;
		return QHCUndo;
	}

	if (!strncmp(szQianHongCmd, "hints", 5))
	{
		szQianHongCmd += 5;
		return QHCHints;
	}

	if (!strncmp(szQianHongCmd, "ban", 3))
	{
		szQianHongCmd += 3;
		return QHCBan;
	}

	if (!strncmp(szQianHongCmd, "bgthink", 7))
	{
		szQianHongCmd += 7;
		return QHCBGThink;
	}

	if (!strncmp(szQianHongCmd, "timeout", 7))
	{
		szQianHongCmd += 7;
		return QHCTimeOut;
	}

	if (strncmp(szQianHongCmd, "", 1))
	{
		return QHCInvalid;
	}

	return QHCNone;
}

/*******************************************************************************************************
������ : Initial
��  �� : 
��  �� : ��
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��11��6�� 15:51:27
*******************************************************************************************************/
FSVOID CSearchEngine::Initial()
{
	FSINT t;
	FSINT tStart = clock();


//	cout << "���ڷ����ڴ�..." << endl;
	AllocateMem();	//�����ڴ�
	m_bAllocateMem = TRUE;

//	cout << "���ڳ�ʼ������..." << endl;
	InitVar();		//��ʼ������

	GenPreMoves();	//�����߷�Ԥ��������

//	cout << "���ڲ�������Ԥ�ж�����..." << endl;
	t = clock();
	GenPreCheck();	//��������Ԥ�ж�����
//	cout << "��������Ԥ�ж�����ʱ��:" << clock() - t << "ms" << endl;

	switch(m_EgnProtocol)
	{
	case EgnPUCCI:
	case EgnPAhei:
		DISPMSG1_ENDL(cout, "�����߷���Ч��Ԥ�ж�����...");
		break;

	case EgnPQianHong:
		STATEMENT_TESTQHP(DISPMSG1_ENDL(cout, "�����߷���Ч��Ԥ�ж�����..."););
		break;
					
	default:
		break;
	}
	t = clock();
	GenPreValidMove();	//�����߷���Ч��Ԥ�ж�����
//	cout << "�����߷���Ч��Ԥ�ж�����ʱ��:" << clock() - t << "ms" << endl;

	SWITCHEGN(
	DISPMSG3_ENDL(cout,	"����ʱ��:", clock() - tStart, "ms");
	DISPMSG3_ENDL(cout,	"����", ENGINENAME" "ENGINEVERSION, "��ʼ����"););
}

/*******************************************************************************************************
������ : InitPointer
��  �� : 
��  �� : ��
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��11��6�� 15:51:19
*******************************************************************************************************/
FSVOID CSearchEngine::InitPointer()
{
	//��ʼ��
	//�߷�Ԥ��������
	//��
	m_pcmRookFilePreMoves = 0;
	m_pcmRookRankPreMoves = 0;

	m_pcmRookFileNoCapPreMoves = 0;
	m_pcmRookRankNoCapPreMoves = 0;

	m_pcmRookFileCapPreMoves = 0;
	m_pcmRookRankCapPreMoves = 0;

	//��
	//�������߷�
	m_pcmKnightNoCapPreMoves = 0;
	//�����߷�
	m_pcmKnightCapPreMoves = 0;

	//��
	m_pcmCannonFilePreMoves = 0;
	m_pcmCannonRankPreMoves = 0;

	m_pcmCannonFileNoCapPreMoves = 0;
	m_pcmCannonRankNoCapPreMoves = 0;

	m_pcmCannonFileCapPreMoves = 0;
	m_pcmCannonRankCapPreMoves = 0;
	
	//��
	m_pcmKingPreMoves = 0;

	//ʿ
	m_pcmAdvisorPreMoves = 0;

	//��
	m_pcmBishopPreMoves = 0;
	m_pcmBishopCapPreMoves = 0;

	//��
	m_PawnPreMoves = 0;
	m_PawnNoCapPreMoves = 0;
	m_PawnCapPreMoves = 0;

	m_nRookFoeFileRankCheckNum = 0;

	m_nCannonFoeFileRankCheckNum = 0;

	m_bIsTwoKingMeet= 0;
	
	//����Ԥ�ж�����
	//��
	m_bIsRookFilePreCheck = 0;
	m_bIsRookRankPreCheck = 0;

	//��
	m_bIsKnightPreCheck = 0;

	//��
	m_bIsCannonFilePreCheck = 0;
	m_bIsCannonRankPreCheck = 0;

	//��
	m_bIsPawnPreCheck = 0;

	//�߷���Ч��Ԥ�ж�����
//	//��
//	m_bIsRookFilePreValidMove = 0;
//	m_bIsRookRankPreValidMove = 0;
//
//	//��
//	m_bIsCannonFilePreValidMove = 0;
//	m_bIsCannonRankPreValidMove = 0;

	//��
	m_bIsRookPreValidMove = 0;

	//��
	m_bIsCannonPreValidMove = 0;

	//hash��
	m_pHashTab[0] = 0;
	m_pHashTab[1] = 0;

	m_pHashTabByDepth[0] = 0;
	m_pHashTabByDepth[1] = 0;

	m_pQSHashTab[0] = 0;
	m_pQSHashTab[1] = 0;
}

/*******************************************************************************************************
������ : CanMemMeet
��  �� : 
��  �� : ��
����ֵ : ����Ϊ FSBOOL
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��11��6�� 15:51:09
*******************************************************************************************************/
FSBOOL CSearchEngine::CanMemMeet()
{
	return TRUE;
}

/*******************************************************************************************************
������ : InitPreMoveListPointer
��  �� : 
��  �� : ��
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��11��6�� 15:50:06
*******************************************************************************************************/
FSVOID CSearchEngine::InitPreMoveListPointer()
{
	//�߷�Ԥ���������е��߷�����
	FSINT w,p,i,j,k;
	
	//��
	for (p = 0; p < 91; p++)
	{
		for (i = 0; i < ROOKFILECHECKSUMMAXCNT; i++)
		{
			m_pcmRookFileNoCapPreMoves[p][i].pMoveList = &m_InvalidMovePointer;

			for (j = 0; j < 4; j++)
			{
				m_pcmRookFilePreMoves[p][i][j].pMoveList = &m_InvalidMovePointer;
				m_pcmRookFileCapPreMoves[p][i][j].pMoveList = &m_InvalidMovePointer;
			}
		}
		
		for (i = 0; i < ROOKRANKCHECKSUMMAXCNT; i++)
		{
			m_pcmRookRankNoCapPreMoves[p][i].pMoveList = &m_InvalidMovePointer;

			for (j = 0; j < 4; j++)
			{
				m_pcmRookRankPreMoves[p][i][j].pMoveList = &m_InvalidMovePointer;
				m_pcmRookRankCapPreMoves[p][i][j].pMoveList = &m_InvalidMovePointer;
			}
		}
	}
	
	//��
	//�������߷�
	for (p = 0; p < 91; p++)
	{
		for (i = 0; i < 256; i++)
		{
			for (j = 0; j < 16; j++)
			{
				m_pcmKnightNoCapPreMoves[p][i][j].pMoveList = &m_InvalidMovePointer;
			}
		}
	}
	//�����߷�
	for (p = 0; p < 91; p++)
	{
		for (i = 0; i < 256; i++)
		{
			for (j = 0; j < 16; j++)
			{
				m_pcmKnightCapPreMoves[p][i][j].pMoveList = &m_InvalidMovePointer;
			}
		}
	}

	//��
	for (p = 0; p < 91; p++)
	{
		for (i = 0; i < CANNONFILECHECKSUMMAXCNT; i++)
		{
			m_pcmCannonFileNoCapPreMoves[p][i].pMoveList = &m_InvalidMovePointer;

			for (j = 0; j < 4; j++)
			{
				m_pcmCannonFilePreMoves[p][i][j].pMoveList = &m_InvalidMovePointer;
				m_pcmCannonFileCapPreMoves[p][i][j].pMoveList = &m_InvalidMovePointer;
			}
		}
		
		for (i = 0; i < CANNONRANKCHECKSUMMAXCNT; i++)
		{
			m_pcmCannonRankNoCapPreMoves[p][i].pMoveList = &m_InvalidMovePointer;

			for (j = 0; j < 4; j++)
			{
				m_pcmCannonRankPreMoves[p][i][j].pMoveList = &m_InvalidMovePointer;
				m_pcmCannonRankCapPreMoves[p][i][j].pMoveList = &m_InvalidMovePointer;
			}
		}
	}

	//��
	for (i = 0; i < 18; i++)
	{
		for (j = 0; j < 16; j++)
		{
			for (k = 0; k < 16; k++)
			{
				m_pcmKingPreMoves[i][j][k].pMoveList = &m_InvalidMovePointer;
			}
		}
	}
	for (i = 0; i < 18; i++)
	{
		for (k = 0; k < 16; k++)
		{
			m_pcmKingNoCapPreMoves[i][k].pMoveList = &m_InvalidMovePointer;
			m_pcmKingCapPreMoves[i][k].pMoveList = &m_InvalidMovePointer;
		}
	}

	//ʿ
	for (i= 0;i<11;i++)
	{
		for (j = 0; j < 16; j++)
		{
			for (k = 0; k < 16; k++)
			{
				m_pcmAdvisorPreMoves[i][j][k].pMoveList = &m_InvalidMovePointer;
			}
		}
	}
	//�����߷�
	for (i= 0;i<11;i++)
	{
		for (k = 0; k < 16; k++)
		{
			m_pcmAdvisorNoCapPreMoves[i][k].pMoveList = &m_InvalidMovePointer;
			m_pcmAdvisorCapPreMoves[i][k].pMoveList = &m_InvalidMovePointer;
		}
	}

	//��
	for (p = 0; p < 15; p++)
	{
		for (i = 0; i < 16; i++)
		{
			for (j = 0; j < 16; j++)
			{ 
				for (k = 0; k < 16; k++)
				{
					m_pcmBishopPreMoves[p][i][j][k].pMoveList = &m_InvalidMovePointer;
				}
			}
		}
	}
	//�������߷�
	for (p = 0; p < 15; p++)
	{
		for (j = 0; j < 16; j++)
		{ 
			for (k = 0; k < 16; k++)
			{
				m_pcmBishopNoCapPreMoves[p][j][k].pMoveList = &m_InvalidMovePointer;
				m_pcmBishopCapPreMoves[p][j][k].pMoveList = &m_InvalidMovePointer;
			}
		}
	}

	//��
	for (w = 0; w < 2; w++)
	{
		for (p = 0; p < 56; p++)
		{
			for (i= 0;i<8;i++)
			{
				for (j = 0; j < 8; j++)
				{
					m_PawnPreMoves[w][p][i][j].pMoveList = &m_InvalidMovePointer;
				}
			}
		}
	}
	for (w = 0; w < 2; w++)
	{
		for (p = 0; p < 56; p++)
		{
			for (j = 0; j < 8; j++)
			{
				m_PawnNoCapPreMoves[w][p][j].pMoveList = &m_InvalidMovePointer;
				m_PawnCapPreMoves[w][p][j].pMoveList = &m_InvalidMovePointer;
			}
		}
	}
}

/*******************************************************************************************************
������ : QHStopThink
��  �� : 
��  �� : ��
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��11��6�� 15:49:23
*******************************************************************************************************/
#ifdef _WIN32
FSVOID CSearchEngine::QHStopThink()
{
	if (m_hThinkThread == NULL)
	{
		return;
	}
	
	m_bStopThink = TRUE;
	
	//���������߳�
	KillThread(m_hThinkThread);
	
	//�ָ�������ʱ�ľ���,�����UCCIЭ���ʱ����Ʋ�ͬ [2005/11/7 16:47:03]
	LoadFEN(m_szFEN);
	DEBUGASSERT(!memcmp(m_nPiecePosBeforeAI, m_nPiecePos, 32 * sizeof(FSINT)));

	SearchStop();
}
#else
#endif

/*******************************************************************************************************
������ : CBChessID2FEN
��  �� : 
��  �� : 
        FSCHAR *szFEN : 
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��11��8�� 22:24:00
*******************************************************************************************************/
FSVOID CSearchEngine::CBChessID2FEN(FSCHAR *szFEN)
{
	FSINT i, j;
	FSINT nSpaceCnt;
	FSINT nPos = 4;

	strncpy(szFEN, "fen ", 4);

	for (j = 9; j >= 0; j--)
	{
		i = 0;

		while (i < 9)
		{
			if (m_nCBChessID[m_nPos2CGN[i][j]] == NOCHESS)
			{
				nSpaceCnt = 0;
				while (i < 9 && m_nCBChessID[m_nPos2CGN[i][j]] == NOCHESS)
				{
					nSpaceCnt++;
					i++;
				}

				szFEN[nPos++] = '0' + nSpaceCnt;
			}
			else
			{
				szFEN[nPos++] = g_cChessID2Alpha[m_nCBChessID[m_nPos2CGN[i][j]]];
				i++;
			}
		}

		szFEN[nPos++] = '/';
	}

	szFEN[nPos - 1] = ' ';

	if (m_bIsRed == RED)
	{
		szFEN[nPos] = 'w';
	}
	else
	{
		szFEN[nPos] = 'b';
	}

	strncpy(szFEN + nPos + 1, " - - ", 5);
	nPos += 6;
	szFEN[nPos++] = '0' + m_nBouts;
	szFEN[nPos++] = ' ';
	szFEN[nPos++] = '0' + m_nCurBout;
	if (m_nNoEatMoveCnt_BS > 0)
	{
		FSCHAR szMove[MAXBUFFERLEN];
		FSINT i;

		for (i = 0; i < m_nNoEatMoveCnt_BS; i++)
		{
			szFEN[nPos++] = ' ';
			ChessMove2Str(m_Moves_NoEat[i], szMove);
			strncpy(szFEN + nPos, szMove, 4);
			nPos += 4;
		}
	}

	szFEN[nPos] = '\0';
}

/*******************************************************************************************************
������ : GetPVLineFromHash
��  �� : 
��  �� : ��
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��11��8�� 22:23:52
*******************************************************************************************************/
FSVOID CSearchEngine::GetPVLineFromHash(FSINT nSearchDepth)
{
	CHESSMOVE cm = m_cmNullMove;
	FSINT nToChessID;
	FSINT nToPieceID;

	ProbeHash(0, nSearchDepth, g_nPerCheckValue, -g_nPerCheckValue, cm);
	if (cm != m_cmNullMove)
	{
		m_PVLine.pMoveList[m_PVLine.nCount++] = cm;

		MakeMove(cm, nToChessID, nToPieceID);

		// �����ظ�,��û�ջ���[2005/11/12 14:54:05]
		if (IsRep(1) == FALSE)
		{
			GetPVLineFromHash(nSearchDepth - 1);
		}

		UnMakeMove(cm, nToChessID, nToPieceID);
	}
}

/*******************************************************************************************************
������ : ClearStack
��  �� : 
��  �� : 
        T stackRedoMove : 
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��11��8�� 22:23:43
*******************************************************************************************************/
template<class T> FSVOID CSearchEngine::ClearStack(T s)
{
	while (s.size() > 0)
	{
		s.pop();
	}
}

/*******************************************************************************************************
������ : ChangeSide
��  �� : 
��  �� : ��
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��11��12�� 14:58:42
*******************************************************************************************************/
inline FSVOID CSearchEngine::ChangeSide()
{
	m_bIsRed = 1 - m_bIsRed;

	m_nHashTabIndex ^= m_nHashKeyPlayer32;
	m_nCBCheckSum ^= m_nHashKeyPlayer64;
}

/*******************************************************************************************************
������ : KillThread
��  �� : 
��  �� : 
        HANDLE h : 
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��11��12�� 14:58:36
*******************************************************************************************************/
FSVOID CSearchEngine::KillThread(HANDLE h)
{
	if (h == 0)
	{
		return;
	}

	if (::TerminateThread(h, 0) == FALSE)
	{
		DISPMSG1_ENDL(cout,	"Terminate Thread Fail!");

		return;
	}
	::WaitForSingleObject(h, INFINITE);
	::CloseHandle(h);
}

/*******************************************************************************************************
������ : IsSpecialPos
��  �� : 
��  �� : 
        FSCHAR *& szStr : 
����ֵ : ����Ϊ BOOL
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��11��12�� 14:58:30
*******************************************************************************************************/
BOOL CSearchEngine::IsSpecialPos(FSCHAR *& szStr)
{
	szStr = JumpNoMatterChar(szStr);

	// ���־���
	if (!strncmp(szStr, "startpos", 8) || strncmp(szStr, "opengamepos", 11) == 0)
	{
		LoadFEN((FSCHAR*)g_szOpenGamePos);
		return TRUE;
	}

	// �о־���
	if (!strncmp(szStr, "midgamepos", 10))
	{
		LoadFEN((FSCHAR*)g_szMidGamePos);
		return TRUE;
	}

	// �о־���
	if (!strncmp(szStr, "endgamepos", 10))
	{
		LoadFEN((FSCHAR*)g_szEndGamePos);
		return TRUE;
	}

	// ��ɱ�ž�
	if (!strncmp(szStr, "checkmatepos", 12))
	{
		LoadFEN((FSCHAR*)g_szCheckMatePos);
		return TRUE;
	}

	// zugzwang����
	if (!strncmp(szStr, "zugzwangpos", 11))
	{
		LoadFEN((FSCHAR*)g_szZugzwangPos);
		return TRUE;
	}

	// ���Null Move�ľ���
	if (!strncmp(szStr, "detectnm", 8))
	{
		LoadFEN((FSCHAR*)g_szDetectNMPos);
		return TRUE;
	}

	// ��Ե�ʿ
	if (!strncmp(szStr, "knightvssgladvisor", 18))
	{
		LoadFEN((FSCHAR*)g_szKnightVSSglAdvisorPos);
		return TRUE;
	}

	// ��������
	if (!strncmp(szStr, "fishformooninsea", 15))
	{
		LoadFEN((FSCHAR*)g_szFishforMoonInSeaPos);
		return TRUE;
	}

	// ����ѭ��
	if (!strncmp(szStr, "detectrep", 15))
	{
		LoadFEN((FSCHAR*)g_szRepDetectPos);
		return TRUE;
	}

	return FALSE;
}

/*******************************************************************************************************
������ : RepValue
��  �� : 
��  �� : 
        FSINT nRep : 
����ֵ : ����Ϊ FSINT
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��11��12�� 14:58:22
*******************************************************************************************************/
inline FSINT CSearchEngine::RepValue(FSINT nRep)
{
	//��������
	if (nRep == 2)
	{
		return m_nCurDepth + g_nPerCheckValue;
	}

	//�Է�����
	if (nRep == 4)
	{
		return -g_nPerCheckValue - m_nCurDepth;
	}

	//�ظ�
	return DrawValue();
}

/*******************************************************************************************************
������ : DrawValue
��  �� : �����ֵ
��  �� : ��
����ֵ : ����Ϊ inline
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��11��12�� 14:58:16
*******************************************************************************************************/
inline FSINT CSearchEngine::DrawValue()
{
	return (m_nCurDepth & 1) == 0 ? -m_nContemptFactor : m_nContemptFactor;
}

/*******************************************************************************************************
������ : IsDraw
��  �� : �Ƿ�Ϊ����
��  �� : ��
����ֵ : ����Ϊ FSBOOL
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��11��12�� 14:58:05
*******************************************************************************************************/
FSBOOL CSearchEngine::IsDraw()
{
	DEBUGASSERT(m_nChessCnt[BROOK] <= 2);
	DEBUGASSERT(m_nChessCnt[BKNIGHT] <= 2);
	DEBUGASSERT(m_nChessCnt[BCANNON] <= 2);
	DEBUGASSERT(m_nChessCnt[RROOK] <= 2);
	DEBUGASSERT(m_nChessCnt[RKNIGHT] <= 2);
	DEBUGASSERT(m_nChessCnt[RCANNON] <= 2);

	return m_nNoEatMoveCnt_IS >= m_nDrawMoves || 
		   (m_nChessCnt[BROOK] == 0 && m_nChessCnt[BKNIGHT]  == 0 && m_nChessCnt[BCANNON] == 0 && m_nChessCnt[BPAWN] == 0 && 
		    m_nChessCnt[RROOK] == 0 && m_nChessCnt[RKNIGHT]  == 0 && m_nChessCnt[RCANNON] == 0 && m_nChessCnt[RPAWN] == 0);

//	return (m_nChessCnt[BROOK] == 0 && m_nChessCnt[BKNIGHT]  == 0 && m_nChessCnt[BCANNON] == 0 && m_nChessCnt[BPAWN] == 0 && 
//		    m_nChessCnt[RROOK] == 0 && m_nChessCnt[RKNIGHT]  == 0 && m_nChessCnt[RCANNON] == 0 && m_nChessCnt[RPAWN] == 0) || 
//			m_nNoEatMoveCnt_IS >= m_nDrawMoves;

//	return m_nNoEatMoveCnt_IS >= m_nDrawMoves || 
//		   (m_nChessCnt[BROOK] + m_nChessCnt[BKNIGHT] + m_nChessCnt[BCANNON] + m_nChessCnt[BPAWN] + 
//		    m_nChessCnt[RROOK] + m_nChessCnt[RKNIGHT] + m_nChessCnt[RCANNON] + m_nChessCnt[RPAWN] == 0);
}

/*******************************************************************************************************
������ : SearchGo
��  �� : 
��  �� : ��
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��11��18�� 19:29:33
*******************************************************************************************************/
FSVOID CSearchEngine::SearchGo()
{
#ifdef _WIN32
	FSDWORD	(WINAPI CSearchEngine::*p1)() = SearchMain;
	FSDWORD	(WINAPI *p2)(LPVOID);

	memcpy(&p2, &p1, sizeof(p1));

	m_hThinkThread = ::CreateThread(0, 0, p2, this, 0, 0);//�����߳�ִ������
	if(m_hThinkThread == NULL)
	{
		cout << "���������߳�ʱ���ִ���!" << endl;
	}
#else
#endif

	m_bPonderHit = FALSE;
}

/*******************************************************************************************************
������ : PrintEgnStatus
��  �� : 
��  �� : ��
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��11��18�� 19:29:26
*******************************************************************************************************/
FSVOID CSearchEngine::PrintEgnStatus()
{
	switch(m_esUCCIEgnStatus)
	{
	case EgnSIdle:
		DISPMSG1_ENDL(cout,	"idle");
		break;
	
	case EgnSBusy:
		DISPMSG1_ENDL(cout,	"busy");
		break;

	default:
		break;
	}
}

/*******************************************************************************************************
������ : OnlyLegalNum
��  �� : 
��  �� : ��
����ֵ : ����Ϊ FSINT
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��11��18�� 19:29:19
*******************************************************************************************************/
FSINT CSearchEngine::LegalMoves(FSINT& nLegalNum)
{
	FSINT i;
	FSINT nCnt;
	FSINT nToChessID;
	FSINT nToPieceID;
	FSINT nLegalCnt = 0;

	CHESSMOVE cm;

	// [2006/1/7 21:32:50]
//	if (IsChecked(1 - m_bIsRed) == TRUE)
//	{
//		m_nCheckExtensionPlies = 2;
//	}
	
	GenMoves(0);
	nCnt = m_nMovesCnt;

	for (i = 0; i < nCnt; i++)
	{
		cm = m_MoveList[0][i];

		MakeMove(cm, nToChessID, nToPieceID);

		if (m_nPiecePos[m_nKingID2PieceID[m_bIsRed]] == INVALIDPIECEPOS && cm.nKind == EAT)
		{
			nLegalNum = i;
			nLegalCnt = 1;
			return -1;
		}

		if (IsLegalMove(cm) == TRUE)
		{
			nLegalNum = i;
			nLegalCnt++;

			// [2006/1/7 21:42:44]
//			if (IsChecked(1 - m_bIsRed) == TRUE)
//			{
//				m_nCheckExtensionPlies = 2;
//			}
		}

		UnMakeMove(cm, nToChessID, nToPieceID);
	}

	return nLegalCnt;
}

/*******************************************************************************************************
������ : GetMoveFromBook
��  �� : 
��  �� : ��
����ֵ : ����Ϊ CHESSMOVE
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��12��10�� 14:16:53
*******************************************************************************************************/
CHESSMOVE CSearchEngine::GetMoveFromBook()
{
	FILE *fp = fopen(m_szBookFile, "r");
	if (fp == NULL)
	{
		SWITCHEGN(DISPMSG4_ENDL(cout, "Open book ", m_szDir, m_szBookFile, " fail"););

		return m_cmNullMove;
	}

	FSCHAR szTemp[1024];
	CHESSMOVE cmTemp;
	FSCHAR szMove[8];
	FSINT nToChessID;
	FSINT nToPieceID;
#ifdef _WIN32
	vector<CHESSMOVE> vectorBookMove;
#endif

	while (feof(fp) == 0)
	{
		fgets(szTemp, 1024, fp);

		if (szTemp[0] != '\0' && strlen(szTemp) > 4)
		{
			if ((cmTemp = LoadBookFEN(szTemp)) != m_cmNullMove)
			{
//				if (vectorBookMove.size() > 0)
//				{
//					if (cmTemp.nScore > vectorBookMove[0].nScore)
//					{
//						vectorBookMove.clear();
//						vectorBookMove.push_back(cmTemp);
//					}
//					else if (cmTemp.nScore == vectorBookMove[0].nScore)
//					{
//						vectorBookMove.push_back(cmTemp);
//					}
//				}
//				else
//				{
//					vectorBookMove.push_back(cmTemp);
//				}

				MakeMove(cmTemp, nToChessID, nToPieceID);
				if (IsRep(3) == FALSE)
				{
					vectorBookMove.push_back(cmTemp);
					
					ChessMove2Str(cmTemp, szMove);
					
					SWITCHEGN(DISPMSG4_ENDL(cout, "info depth 0 score ", cmTemp.nScore, " pv ", szMove););
				}
				UnMakeMove(cmTemp, nToChessID, nToPieceID);
			}
		}
	}
	fclose(fp);

	FSINT nSize = vectorBookMove.size();
	FSINT i;
	FSINT nAllScore = 0;
	vector<FSFLOAT> vectorProb;

	if (nSize == 0)
	{
		SWITCHEGN(DISPMSG4_ENDL(cout,	"Load move from book ", m_szDir, m_szBookFile, " fail"););

		m_nGetOpenBookMoveErrCnt++;

		return m_cmNullMove;
	}

	for (i = 0; i < nSize; i++)
	{
		nAllScore += vectorBookMove[i].nScore;
	}

	if (nAllScore == 0)
	{
		for (i = 0; i < nSize; i++)
		{
			vectorBookMove[i].nScore = 1;
		}

		nAllScore = nSize;
	}
	
	srand(time(0));
	nAllScore = Rand32() % nAllScore;
	
	//���̷�������ѡ���߷�
	for (i = 0; i < nSize; i++)
	{
		nAllScore -= vectorBookMove[i].nScore;
		if (nAllScore < 0)
		{
			return vectorBookMove[i];
		}
	}
	
	return vectorBookMove[nSize - 1];
}

/*******************************************************************************************************
������ : LoadBookFEN
��  �� : 
��  �� : 
        FSCHAR *szFEN : 
����ֵ : ����Ϊ inline
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��12��10�� 14:16:43
*******************************************************************************************************/
inline CHESSMOVE CSearchEngine::LoadBookFEN(FSCHAR *szFEN)
{
	if (szFEN == 0)
	{
		return m_cmNullMove;
	}

	FSINT i;
	FSINT nCuri = 0;
	FSINT nCurj = 9;
	FSINT nCount;
	FSINT nPos;
	FSINT nCBChessID[90];
	FSBOOL bSide;
	FSCHAR szMove[5];
	CHESSMOVE cmTemp;
	FSINT nScore;

	memset(nCBChessID, NOCHESS, MAXCHESSGRIDNUM * sizeof(FSINT));

	//����ǰ��Ŀո�,Tab��
	szFEN = JumpNoMatterChar(szFEN);
	strncpy(szMove, szFEN, 4);
	szFEN += 4;
	szFEN = JumpNoMatterChar(szFEN);
	nScore = atoi(szFEN);
	szFEN = JumpDigit(szFEN);
	szFEN = JumpNoMatterChar(szFEN);

	nPos = 0;
	while(szFEN[nPos] != ' ')
	{
		if (isdigit(szFEN[nPos]))
		{
			nCount = atoi(szFEN + nPos);
			for (i = nCuri; i < nCuri + nCount; i++)
			{
				nCBChessID[10 * i + nCurj] = NOCHESS;
			}
			nCuri += nCount;
		}
		else if (szFEN[nPos] == '/')
		{
			nCuri = 0;
			nCurj--;
		}
		else if (szFEN[nPos] >= 'a' && szFEN[nPos] <= 'z')//ΪСд��ĸ
		{
			nCBChessID[10 * nCuri + nCurj] = g_nAlpha2Chess[szFEN[nPos] -'a'];
			nCuri++;
		}
		else if (szFEN[nPos] >= 'A' && szFEN[nPos] <= 'Z')//Ϊ��д��ĸ
		{
			nCBChessID[10 * nCuri + nCurj] = g_nAlpha2Chess[szFEN[nPos] -'A'] + 7;
			nCuri++;
		}
		else//������
		{
			return m_cmNullMove;
		}

		nPos++;
	}

	szFEN = JumpNoMatterChar(szFEN + nPos);
	bSide = (*szFEN != 'b');

	UCCIStr2ChessMove(szMove, cmTemp);
//	Str2ChessMove(szMove, cmTemp);
	cmTemp.nScore = nScore;

	if (GetCBCheckSum(nCBChessID, bSide) == m_nCBCheckSum)
	{
		return cmTemp;
	}

	//ע�͵��Ĳ���Ϊ���ұ任
	TXTBOOKRECORD txtBookRecord;
#ifndef LRTEXTBOOK
	TXTBOOKRECORD txtBookRecord2;
#endif

	txtBookRecord.cm = cmTemp;
	memcpy(txtBookRecord.nCBChessID, nCBChessID, 90 * sizeof(FSINT));

#ifndef LRTEXTBOOK
	txtBookRecord2.cm = cmTemp;
	memcpy(txtBookRecord2.nCBChessID, nCBChessID, 90 * sizeof(FSINT));
	LRTransform(txtBookRecord2);

	if (GetCBCheckSum(txtBookRecord2.nCBChessID, bSide) == m_nCBCheckSum)
	{
		return txtBookRecord2.cm;
	}
#endif

#ifndef UDTEXTBOOK
	bSide = 1 - bSide;

	UDTransform(txtBookRecord);
	if (GetCBCheckSum(txtBookRecord.nCBChessID, bSide) == m_nCBCheckSum)
	{
		return txtBookRecord.cm;
	}
#endif

#ifndef LRTEXTBOOK
#ifndef UDTEXTBOOK
	UDTransform(txtBookRecord2);
	if (GetCBCheckSum(txtBookRecord2.nCBChessID, bSide) == m_nCBCheckSum)
	{
		return txtBookRecord2.cm;
	}
#endif
#endif

	return m_cmNullMove;
}

/*******************************************************************************************************
������ : GetCBCheckSum
��  �� : 
��  �� : 
        const FSINT nCBChessID[] : 
		FSBOOL bSide             : 
����ֵ : ����Ϊ inline
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��12��10�� 14:16:30
*******************************************************************************************************/
inline FSUINT64 CSearchEngine::GetCBCheckSum(const FSINT nCBChessID[], FSBOOL bSide)
{
	FSINT p;
	FSUINT64 nCBCheckSum = 0;

	for (p = 0; p < 90; p++)
	{
		nCBCheckSum ^= m_nHashKey64[nCBChessID[p]][m_CGN2Pos[p].x][m_CGN2Pos[p].y];
	}

	if (bSide == RED)
	{
		nCBCheckSum ^= m_nHashKeyPlayer64;
	}

	return nCBCheckSum;
}

/*******************************************************************************************************
������ : LRTransform
��  �� : 
��  �� : 
        TXTBOOKRECORD& txtBookRecord : 
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��12��10�� 14:16:23
*******************************************************************************************************/
FSVOID CSearchEngine::LRTransform(TXTBOOKRECORD& txtBookRecord)
{
	FSINT *nCBChessID = txtBookRecord.nCBChessID;
	CHESSMOVE *pCM = &txtBookRecord.cm;

	pCM->nFrom += (8 - 2 * m_CGN2Pos[pCM->nFrom].x) * 10;
	pCM->nTo += (8 - 2 * m_CGN2Pos[pCM->nTo].x) * 10;

	FSINT nChessID;
	FSINT i, j;

	for (j = 0; j < 10; j++)
	{
		for (i = 0; i < 4; i++)
		{
			nChessID = nCBChessID[m_nPos2CGN[i][j]];
			nCBChessID[m_nPos2CGN[i][j]] = nCBChessID[m_nPos2CGN[8 - i][j]];
			nCBChessID[m_nPos2CGN[8 - i][j]] = nChessID;
		}
	}
}

/*******************************************************************************************************
������ : UDTransform
��  �� : 
��  �� : 
        TXTBOOKRECORD& txtBookRecord : 
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��12��10�� 14:16:15
*******************************************************************************************************/
FSVOID CSearchEngine::UDTransform(TXTBOOKRECORD& txtBookRecord)
{
	FSINT *nCBChessID = txtBookRecord.nCBChessID;
	CHESSMOVE *pCM = &txtBookRecord.cm;

	pCM->nFrom += 9 - 2 * m_CGN2Pos[pCM->nFrom].y;
	pCM->nTo += 9 - 2 * m_CGN2Pos[pCM->nTo].y;

	FSINT nChessID;
	FSINT i, j;

	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 5; j++)
		{
			nChessID = nCBChessID[m_nPos2CGN[i][j]];
			nCBChessID[m_nPos2CGN[i][j]] = nCBChessID[m_nPos2CGN[i][9 - j]];
			nCBChessID[m_nPos2CGN[i][9 - j]] = nChessID;

			nCBChessID[m_nPos2CGN[i][j]] = m_nConvertChessID[nCBChessID[m_nPos2CGN[i][j]]];
			nCBChessID[m_nPos2CGN[i][9 - j]] = m_nConvertChessID[nCBChessID[m_nPos2CGN[i][9 - j]]];
		}
	}
}

//#ifdef _WIN32
///*******************************************************************************************************
//������ : DispTime
//��  �� : 
//��  �� : 
//        FSUINT uTimerID : 
//		FSUINT uMsg     : 
//		FSDWORD dwUser  : 
//		FSDWORD dw1     : 
//		FSDWORD dw2     : 
//����ֵ : ����Ϊ FSVOID
//��  �� : 0.2.0.0
//��  �� : ahei
//ʱ  �� : 2005��12��10�� 14:16:03
//*******************************************************************************************************/
//FSVOID CALLBACK CSearchEngine::DispTime(FSUINT uTimerID, FSUINT uMsg, FSDWORD dwUser, FSDWORD dw1, FSDWORD dw2)
//{
////	MMRESULT mmResult = timeKillEvent(uTimerID);
//
////	if (mmResult == MMSYSERR_INVALPARAM)
//	{
//		DISPMSG3_ENDL(cout,		"��ʱ��", uTimerID, "������!");
//	}
//}
//#else
//#endif

/*******************************************************************************************************
������ : SetChessStyle
��  �� : 
��  �� : 
        FSCHAR *szStyle : 
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��12��22�� 16:03:03
*******************************************************************************************************/
FSVOID CSearchEngine::SetChessStyle(FSCHAR *szStyle)
{
	if (szStyle == 0)
	{
		return;
	}

	szStyle = JumpNoMatterChar(szStyle);

	if (szStyle == 0)
	{
		return;
	}

	strlwr(szStyle);

	FSINT nFSCHARSize = sizeof(FSCHAR);

	if (strncmp(szStyle, "keep", 5) == 0)
	{
		m_ChessStyle = Keep;
		m_nContemptFactor = g_nContemptFactor_Keep;

		return;
	}

	if (strncmp(szStyle, "normal", 7) == 0)
	{
		m_ChessStyle = Normal;
		m_nContemptFactor = g_nContemptFactor_Normal;

		return;
	}

	if (strncmp(szStyle, "risky", 6) == 0)
	{
		m_ChessStyle = Risky;
		m_nContemptFactor = g_nContemptFactor_Risky;

		return;
	}
}

/*******************************************************************************************************
������ : ClearHash
��  �� : 
��  �� : ��
����ֵ : ����Ϊ inline
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��12��22�� 16:02:53
*******************************************************************************************************/
inline FSVOID CSearchEngine::ClearHash()
{
	FSINT i;
#ifdef TIMECALC
	FSINT tStart = clock();
	FSINT tEnd;
#endif

	for (i = 0; i < m_nHashTabMask + 1; i++)
	{
		m_pHashTab[BLACK][i].nCBCheckSum = 0;
		m_pHashTab[RED][i].nCBCheckSum = 0;

//		m_pHashTabByDepth[BLACK][i].nCBCheckSum = 0;
//		m_pHashTabByDepth[RED][i].nCBCheckSum = 0;

		// ��̬������hash
		m_pQSHashTab[BLACK][i].nCBCheckSum = 0;
		m_pQSHashTab[RED][i].nCBCheckSum = 0;
	}

#ifdef TIMECALC
	tEnd = clock();
	DISPMSG3_ENDL(cout,	"���hash��ʱ��:", tEnd - tStart, "ms");
#endif
}

/*******************************************************************************************************
������ : PrintBetterPV
��  �� : 
��  �� : 
        FSINT nDepth            : 
		FSINT nSearchDepth      : 
		FSINT nEval             : 
		FSINT nHashFlag         : 
		CHESSMOVE &cmBetterMove : 
����ֵ : ����Ϊ FSVOID
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��12��22�� 16:02:45
*******************************************************************************************************/
inline FSVOID CSearchEngine::PrintBetterPV(FSINT nDepth, FSINT nSearchDepth, FSINT nEval, FSINT nHashFlag, CHESSMOVE &cmBetterMove)
{
	m_PVLine.nCount = 0;
	RecordHash(nDepth, nSearchDepth, nEval, nHashFlag, cmBetterMove);
	GetPVLineFromHash(m_nCurSearchDepth_ID);
	m_PVLine.pMoveList[0] = cmBetterMove;
	PrintPVLine();
}

/*******************************************************************************************************
������ : GetHash
��  �� : 
��  �� : 
        HASHITEM **pHashTab   : 
		FSINT nDepth          : 
		FSINT nSearchDepth    : 
		FSINT alpha           : 
		FSINT beta            : 
		CHESSMOVE &cmBestMove : 
����ֵ : ����Ϊ inline
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��12��26�� 20:52:11
*******************************************************************************************************/
inline FSINT CSearchEngine::GetHash(HASHITEM **pHashTab, FSINT nDepth, FSINT nSearchDepth, FSINT alpha, FSINT beta, CHESSMOVE &cmBestMove)
{
	HASHITEM hashItem;

	DEBUGASSERT((m_nHashTabIndex & m_nHashTabMask) >= 0 && (m_nHashTabIndex & m_nHashTabMask) <= m_nHashTabMask);
	
	STATEMENT_STAT(m_nProbeHashCnt++;);

	hashItem = pHashTab[m_bIsRed][m_nHashTabIndex & m_nHashTabMask];
	if (hashItem.nCBCheckSum == m_nCBCheckSum)
	{
		FSINT nHashVal = hashItem.nEval;

		// ˵����hash���ǳ���ʱ�ľ���,����Ҫ[2005/12/22 18:23:51]
		if (nHashVal > g_nMateValue)
		{
			return g_nInvalidValue;
		}

		//��������߷�
		cmBestMove = hashItem.cmBestMove;

		if (hashItem.nDepth < nSearchDepth - nDepth)
		{
			return g_nInvalidValue;
		}

		if (hashItem.nHashFlag == HASHEXACT)
		{
			m_PVLineThink.pMoveList[nDepth] = hashItem.cmBestMove;
			STATEMENT_STAT(m_nHitHashCnt++;);
			return nHashVal;
		}

		//hashItem.nHashFlag != HASHEXACT
		if (hashItem.nHashFlag == HASHALPHA && nHashVal <= alpha)
		{
			STATEMENT_STAT(m_nHitHashCnt++;);
			return alpha;
		}

		if (hashItem.nHashFlag == HASHBETA && nHashVal >= beta)
		{
			STATEMENT_STAT(m_nHitHashCnt++;);
			return beta;
		}

		return g_nInvalidValue;
	}

	return g_nInvalidValue;
}

/*******************************************************************************************************
������ : GetFlexValue
��  �� : 
��  �� : ��
����ֵ : ����Ϊ inline
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��12��26�� 20:51:46
*******************************************************************************************************/
inline FSINT CSearchEngine::GetFlexValue()
{
	return GetFlexValue(m_bIsRed) - GetFlexValue(1 - m_bIsRed);
}

/*******************************************************************************************************
������ : GetFlexValue
��  �� : 
��  �� : 
        FSBOOL bIsRed : 
����ֵ : ����Ϊ inline
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��12��26�� 20:51:40
*******************************************************************************************************/
inline FSINT CSearchEngine::GetFlexValue(FSBOOL bIsRed)
{
	FSINT nPiecePos;

	FSINT nRook;
	FSINT nKnight;
	FSINT nCannon;
	FSINT nAdvisor;
	FSINT nBishop;
	FSINT nPawn;

	FSINT nRet = 0;

	nRook = m_nRook[bIsRed];
	nKnight = m_nKnight[bIsRed];
	nCannon = m_nCannon[bIsRed];
	nAdvisor = m_nAdvisor[bIsRed];
	nBishop = m_nBishop[bIsRed];
	nPawn = m_nPawn[bIsRed];

	//��
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nRook][0]]) != INVALIDPIECEPOS)
	{
		nRet += GetRookFlexValue(nPiecePos);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nRook][1]]) != INVALIDPIECEPOS)
	{
		nRet += GetRookFlexValue(nPiecePos);
	}
	
	//��
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nKnight][0]]) != INVALIDPIECEPOS)
	{
		nRet += GetKnightFlexValue(nPiecePos);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nKnight][1]]) != INVALIDPIECEPOS)
	{
		nRet += GetKnightFlexValue(nPiecePos);
	}
	
	//��
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nCannon][0]]) != INVALIDPIECEPOS)
	{
		nRet += GetCannonFlexValue(nPiecePos);
	}
	if ((nPiecePos = m_nPiecePos[m_nChessID2PieceID[nCannon][1]]) != INVALIDPIECEPOS)
	{
		nRet += GetCannonFlexValue(nPiecePos);
	}

	return nRet;
}

/*******************************************************************************************************
������ : GetRookFlexValue
��  �� : 
��  �� : 
        FSINT p : 
����ֵ : ����Ϊ inline
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��12��26�� 20:51:32
*******************************************************************************************************/
inline FSINT CSearchEngine::GetRookFlexValue(FSINT p)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);

	FSINT i, j;
	FSINT nCount;
	FSINT nRookCheckNum;//��У�����
	PRECHESSMOVE pcmFile;
	PRECHESSMOVE pcmRank;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;
	
	//��
	nRookCheckNum = m_nRookFileRankCheckNum[i][m_nBitFile[j]];
	pcmFile = m_pcmRookFileNoCapPreMoves[p][nRookCheckNum];
	nCount = pcmFile.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= MAXROOKFILENOCAPMOVECNT);

	//��
	nRookCheckNum = m_nRookFileRankCheckNum[j][m_nBitRank[i]];
	pcmRank = m_pcmRookRankNoCapPreMoves[p][nRookCheckNum];
	nCount += pcmRank.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= MAXROOKRANKNOCAPMOVECNT + MAXROOKFILENOCAPMOVECNT);

	return m_nRookFlexValue[nCount];
}

/*******************************************************************************************************
������ : GetKnightFlexValue
��  �� : 
��  �� : 
        FSINT p : 
����ֵ : ����Ϊ inline
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��12��26�� 20:51:24
*******************************************************************************************************/
inline FSINT CSearchEngine::GetKnightFlexValue(FSINT p)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);

	FSINT i, j;
	FSINT nKnightLegCheckSum;
	FSINT nKnightFile0AroundCheckSum;
	FSINT nKnightFile1AroundCheckSum;
	FSINT nKnightFile2AroundCheckSum;
	FSINT nKnightFile3AroundCheckSum;
	FSINT nKnightFileAroundCheckSum;

	PRECHESSMOVE pcm;
	
	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;
	
	nKnightFile0AroundCheckSum = m_nFileRankAroundCheckSum[i][m_nBitFile[m_nAbs[j][2]]];
	nKnightFile1AroundCheckSum = m_nFileRankAroundCheckSum2[i][m_nBitFile[m_nAbs[j][1]]];
	nKnightFile2AroundCheckSum = m_nFileRankAroundCheckSum2[i][m_nBitFile[j + 1]];
	nKnightFile3AroundCheckSum = m_nFileRankAroundCheckSum[i][m_nBitFile[j + 2]];

	nKnightFileAroundCheckSum = 
		m_nKnightAroundCheckSum[m_nAroundCheckSum[nKnightFile0AroundCheckSum][nKnightFile1AroundCheckSum]][m_nAroundCheckSum[nKnightFile2AroundCheckSum][nKnightFile3AroundCheckSum]];
	nKnightLegCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBitFile[j]]][m_nFileRankAroundCheckSum[j][m_nBitRank[i]]];
	
	//�������߷�
	pcm = m_pcmKnightNoCapPreMoves[p][nKnightFileAroundCheckSum][nKnightLegCheckSum];
	DEBUGASSERT(pcm.nCount >= 0 && pcm.nCount <= MAXKNIGHTNOCAPMOVECNT);
	return m_nKnightFlexValue[pcm.nCount];
}

/*******************************************************************************************************
������ : GetCannonFlexValue
��  �� : 
��  �� : 
        FSINT p : 
����ֵ : ����Ϊ inline
��  �� : 0.2.0.0
��  �� : ahei
ʱ  �� : 2005��12��26�� 20:51:13
*******************************************************************************************************/
inline FSINT CSearchEngine::GetCannonFlexValue(FSINT p)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);

	FSINT i, j;
	FSINT nCount;
	FSINT nCannonCheckNum;
	PRECHESSMOVE pcmFile;
	PRECHESSMOVE pcmRank;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	//��
	nCannonCheckNum = m_nCannonFileRankCheckNum[i][m_nBitFile[j]];
	pcmFile = m_pcmCannonFileNoCapPreMoves[p][nCannonCheckNum];
	nCount = pcmFile.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= MAXCANNONFILENOCAPMOVECNT);

	//��
	nCannonCheckNum = m_nCannonFileRankCheckNum[j][m_nBitRank[i]];
	pcmRank = m_pcmCannonRankNoCapPreMoves[p][nCannonCheckNum];
	nCount += pcmRank.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= MAXCANNONRANKNOCAPMOVECNT + MAXCANNONFILENOCAPMOVECNT);

	return m_nCannonFlexValue[nCount];
}

inline FSBOOL CSearchEngine::IsEmptyHeartCannon(FSBOOL bIsRed)
{
	FSINT nFoeKingP;
	FSINT nFoeKingI;
	FSINT nFoeKingJ;
	FSINT nFoeKingPosCheckNum;
	
	FSINT nRookPos1;
	FSINT nRookPos2;

	FSINT nKnightPos1;
	FSINT nKnightPos2;

	FSINT nCannonPos1;
	FSINT nCannonPos2;

	FSINT nFoeKingBitFile;
	FSINT nFoeKingBitRank;

	FSINT nFoeAdvisorPos1;
	FSINT nFoeAdvisorPos2;

	FSINT nRook;
	FSINT nKnight;
	FSINT nCannon;
	FSINT nFoeAdvisor;

	//�Է�ʿ
	nFoeAdvisor = m_nAdvisor[1 - bIsRed];

	nFoeAdvisorPos1 = m_nPiecePos[m_nChessID2PieceID[nFoeAdvisor][0]];
	nFoeAdvisorPos2 = m_nPiecePos[m_nChessID2PieceID[nFoeAdvisor][1]];

	//��˫ʿ
	if (nFoeAdvisorPos1 == INVALIDPIECEPOS && nFoeAdvisorPos1 == INVALIDPIECEPOS)
	{
		return FALSE;
	}

	nRook = m_nRook[bIsRed];
	nKnight = m_nKnight[bIsRed];

	//��
	nRookPos1 = m_nPiecePos[m_nChessID2PieceID[nRook][0]];
	nRookPos2 = m_nPiecePos[m_nChessID2PieceID[nRook][1]];

	//��
	nKnightPos1 = m_nPiecePos[m_nChessID2PieceID[nKnight][0]];
	nKnightPos2 = m_nPiecePos[m_nChessID2PieceID[nKnight][1]];

	//�޳���
	if (nRookPos1 == INVALIDPIECEPOS && nRookPos2 == INVALIDPIECEPOS && 
		nKnightPos1 == INVALIDPIECEPOS && nKnightPos2 == INVALIDPIECEPOS)
	{
		return FALSE;
	}

	nCannon = m_nCannon[bIsRed];

	//�Է���
	nFoeKingP = m_nPiecePos[m_nKingID2PieceID[1 - bIsRed]];

	nFoeKingPosCheckNum = m_nBRKingPosCheckNum[1 - bIsRed][nFoeKingP];
	
	nFoeKingI = m_CGN2Pos[nFoeKingP].x;
	nFoeKingJ = m_CGN2Pos[nFoeKingP].y;

	//��
	nCannonPos1 = m_nPiecePos[m_nChessID2PieceID[nCannon][0]];
	nCannonPos2 = m_nPiecePos[m_nChessID2PieceID[nCannon][1]];

	nFoeKingBitFile = m_nBitFile[nFoeKingJ];
	nFoeKingBitRank = m_nBitRank[nFoeKingI];

	return m_bIsRookFilePreCheck[bIsRed][nCannonPos1][nFoeKingPosCheckNum][nFoeKingBitFile] ||	//����1
		   m_bIsRookFilePreCheck[bIsRed][nCannonPos2][nFoeKingPosCheckNum][nFoeKingBitFile] ||	//����2
		   m_bIsRookRankPreCheck[bIsRed][nCannonPos1][nFoeKingPosCheckNum][nFoeKingBitRank] ||	//����1
		   m_bIsRookRankPreCheck[bIsRed][nCannonPos2][nFoeKingPosCheckNum][nFoeKingBitRank];		//����2
}
/*
inline FSINT CSearchEngine::GenRookMoves(FSINT p, CHESSMOVE* pMoveList)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);

	FSINT i, j;
	FSINT nCount;
	FSINT nRet;
	FSINT nRookCheckNum;//��У�����
	FSINT nRookFoeCheckNum;
	PRECHESSMOVE pcmFile;
	PRECHESSMOVE pcmRank;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;
	
	//��
	nRookCheckNum = m_nRookFileRankCheckNum[i][m_nBitFile[j]];
	nRookFoeCheckNum = m_nRookFoeFileRankCheckNum[i][nRookCheckNum][m_nBRBitFile[1 - m_bIsRed][j]];
	pcmFile = m_pcmRookFilePreMoves[p][nRookCheckNum][nRookFoeCheckNum];
	nCount = pcmFile.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 8);
	memcpy(pMoveList, pcmFile.pMoveList, m_nBytes[nCount]);
	nRet = nCount;

	//��
	nRookCheckNum = m_nRookFileRankCheckNum[j][m_nBitRank[i]];
	nRookFoeCheckNum = m_nRookFoeFileRankCheckNum[j][nRookCheckNum][m_nBRBitRank[1 - m_bIsRed][i]];
	pcmRank = m_pcmRookRankPreMoves[p][nRookCheckNum][nRookFoeCheckNum];
	nCount = pcmRank.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 9);
	memcpy(pMoveList + nRet, pcmRank.pMoveList, m_nBytes[nCount]);
	nRet += nCount;

	return nRet;
}

inline FSINT CSearchEngine::GenKnightMoves(FSINT p, CHESSMOVE* pMoveList)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);

	FSINT i, j;
	FSINT nCount;
	FSINT nRet;
	FSINT nKnightLegCheckSum;
	FSINT nKnightFile0AroundCheckSum;
	FSINT nKnightFile1AroundCheckSum;
	FSINT nKnightFile2AroundCheckSum;
	FSINT nKnightFile3AroundCheckSum;
	FSINT nKnightFileAroundCheckSum;

	FSINT nKnightFoeFile0AroundCheckSum;
	FSINT nKnightFoeFile1AroundCheckSum;
	FSINT nKnightFoeFile2AroundCheckSum;
	FSINT nKnightFoeFile3AroundCheckSum;
	FSINT nKnightFoeFileAroundCheckSum;

	PRECHESSMOVE pcm;
	
	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;
	
	nKnightFile0AroundCheckSum = m_nFileRankAroundCheckSum[i][m_nBitFile[m_nAbs[j][2]]];
	nKnightFile1AroundCheckSum = m_nFileRankAroundCheckSum2[i][m_nBitFile[m_nAbs[j][1]]];
	nKnightFile2AroundCheckSum = m_nFileRankAroundCheckSum2[i][m_nBitFile[j + 1]];
	nKnightFile3AroundCheckSum = m_nFileRankAroundCheckSum[i][m_nBitFile[j + 2]];

	nKnightFoeFile0AroundCheckSum = m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][m_nAbs[j][2]]];
	nKnightFoeFile1AroundCheckSum = m_nFileRankAroundCheckSum2[i][m_nBRBitFile[1 - m_bIsRed][m_nAbs[j][1]]];
	nKnightFoeFile2AroundCheckSum = m_nFileRankAroundCheckSum2[i][m_nBRBitFile[1 - m_bIsRed][j + 1]];
	nKnightFoeFile3AroundCheckSum = m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][j + 2]];

	nKnightFileAroundCheckSum = 
		m_nKnightAroundCheckSum[m_nAroundCheckSum[nKnightFile0AroundCheckSum][nKnightFile1AroundCheckSum]][m_nAroundCheckSum[nKnightFile2AroundCheckSum][nKnightFile3AroundCheckSum]];
	nKnightFoeFileAroundCheckSum = 
		m_nKnightAroundCheckSum[m_nAroundCheckSum[nKnightFoeFile0AroundCheckSum][nKnightFoeFile1AroundCheckSum]][m_nAroundCheckSum[nKnightFoeFile2AroundCheckSum][nKnightFoeFile3AroundCheckSum]];
	nKnightLegCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBitFile[j]]][m_nFileRankAroundCheckSum[j][m_nBitRank[i]]];
	
	//�������߷�
	pcm = m_pcmKnightNoCapPreMoves[p][nKnightFileAroundCheckSum][nKnightLegCheckSum];
	nCount = pcm.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 8);
	memcpy(pMoveList, pcm.pMoveList, m_nBytes[nCount]);
	nRet = nCount;

	//�����߷�
	pcm = m_pcmKnightCapPreMoves[p][nKnightFoeFileAroundCheckSum][nKnightLegCheckSum];
	nCount = pcm.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 8);
	memcpy(pMoveList + nRet, pcm.pMoveList, m_nBytes[nCount]);
	nRet += nCount;

	return nRet;
}

inline FSINT CSearchEngine::GenCannonMoves(FSINT p, CHESSMOVE* pMoveList)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);

	FSINT i, j;
	FSINT nCount;
	FSINT nRet;
	FSINT nCannonCheckNum;
	FSINT nCannonFoeCheckNum;
	PRECHESSMOVE pcmFile;
	PRECHESSMOVE pcmRank;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	//��
	nCannonCheckNum = m_nCannonFileRankCheckNum[i][m_nBitFile[j]];
	nCannonFoeCheckNum =
		m_nCannonFoeFileRankCheckNum[i][nCannonCheckNum][m_nBRBitFile[1 - m_bIsRed][j]];
	pcmFile = m_pcmCannonFilePreMoves[p][nCannonCheckNum][nCannonFoeCheckNum];
	nCount = pcmFile.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 8);
	memcpy(pMoveList, pcmFile.pMoveList, m_nBytes[nCount]);
	nRet = nCount;

	//��
	nCannonCheckNum = m_nCannonFileRankCheckNum[j][m_nBitRank[i]];
	nCannonFoeCheckNum = 
		m_nCannonFoeFileRankCheckNum[j][nCannonCheckNum][m_nBRBitRank[1 - m_bIsRed][i]];
	pcmRank = m_pcmCannonRankPreMoves[p][nCannonCheckNum][nCannonFoeCheckNum];
	nCount = pcmRank.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 9);
	memcpy(pMoveList + nRet, pcmRank.pMoveList, m_nBytes[nCount]);
	nRet += nCount;

	return nRet;
}

inline FSINT CSearchEngine::GenKingMoves(FSINT p, CHESSMOVE* pMoveList)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);

	FSINT i, j;
	FSINT nCount;
	FSINT nPosCheckNum;
	FSINT nAroundCheckSum;
	FSINT nFoeAroundCheckSum;
	PRECHESSMOVE pcm;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	nPosCheckNum = m_nKingPosCheckNum[p];
	DEBUGASSERT(nPosCheckNum >= MINKINGPOSCHECKNUM && nPosCheckNum <= MAXKINGPOSCHECKNUM);
	nAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBitFile[j]]][m_nFileRankAroundCheckSum[j][m_nBitRank[i]]];
	nFoeAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][j]]][m_nFileRankAroundCheckSum[j][m_nBRBitRank[1 - m_bIsRed][i]]];
	
	pcm = m_pcmKingPreMoves[nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum];
	nCount = pcm.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 4);
	memcpy(pMoveList + nCount, pcm.pMoveList, m_nBytes[nCount]);

	return nCount;
}

inline FSINT CSearchEngine::GenAdvisorMoves(FSINT p, CHESSMOVE* pMoveList)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);

	FSINT i, j;
	FSINT nCount;
	FSINT nPosCheckNum;
	FSINT nAroundCheckSum;
	FSINT nFoeAroundCheckSum;
	PRECHESSMOVE pcm;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	nPosCheckNum = m_nAdvisorPosCheckNum[p];
	nAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBitFile[m_nAbs[j][1]]]][m_nFileRankAroundCheckSum[i][m_nBitFile[j + 1]]];
	nFoeAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][m_nAbs[j][1]]]][m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][j + 1]]];
	
	pcm = m_pcmAdvisorPreMoves[nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum];
	nCount = pcm.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 4);
	memcpy(pMoveList + nCount, pcm.pMoveList, m_nBytes[nCount]);

	return nCount;
}

inline FSINT CSearchEngine::GenBishopMoves(FSINT p, CHESSMOVE* pMoveList)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);

	FSINT i, j;
	FSINT nCount;
	FSINT nPosCheckNum;
	FSINT nElephantEyeCheckSum;
	FSINT nAroundCheckSum;
	FSINT nFoeAroundCheckSum;
	PRECHESSMOVE pcm;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	nPosCheckNum = m_nBishopPosCheckNum[p];
	nAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum2[i][m_nBitFile[m_nAbs[j][2]]]][m_nFileRankAroundCheckSum2[i][m_nBitFile[j + 2]]];
	nFoeAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum2[i][m_nBRBitFile[1 - m_bIsRed][m_nAbs[j][2]]]][m_nFileRankAroundCheckSum2[i][m_nBRBitFile[1 - m_bIsRed][j + 2]]];
	nElephantEyeCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBitFile[m_nAbs[j][1]]]][m_nFileRankAroundCheckSum[i][m_nBitFile[j + 1]]];

	pcm = 
		m_pcmBishopPreMoves[nPosCheckNum][nElephantEyeCheckSum][nAroundCheckSum][nFoeAroundCheckSum];
	nCount = pcm.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 4);
	memcpy(pMoveList + nCount, pcm.pMoveList, m_nBytes[nCount]);

	return nCount;
}

inline FSINT CSearchEngine::GenPawnMoves(FSINT p, CHESSMOVE* pMoveList)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);

	FSINT i, j;
	FSINT nCount;
	FSINT nPosCheckNum;
	FSINT nAroundCheckSum;
	FSINT nFoeAroundCheckSum;
	PRECHESSMOVE pcm;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	nPosCheckNum = m_nPawnPosCheckNum[m_bIsRed][p];
	nAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBitFile[j]]][m_nPawnRankAroundCheckSum[m_bIsRed][j][m_nBitRank[i]]];
	nFoeAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][j]]][m_nPawnRankAroundCheckSum[m_bIsRed][j][m_nBRBitRank[1 - m_bIsRed][i]]];

	pcm = m_PawnPreMoves[m_bIsRed][nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum];
	nCount = pcm.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= MAXPAWNCAPMOVECNT);
	memcpy(pMoveList, pcm.pMoveList, m_nBytes[nCount]);

	return nCount;
}

inline FSINT CSearchEngine::GenRookMoves(FSINT p)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);

	FSINT i, j;
	FSINT nCount;
	FSINT nRet;
	FSINT nRookCheckNum;//��У�����
	FSINT nRookFoeCheckNum;
	PRECHESSMOVE pcmFile;
	PRECHESSMOVE pcmRank;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;
	
	//��
	nRookCheckNum = m_nRookFileRankCheckNum[i][m_nBitFile[j]];
	nRookFoeCheckNum = m_nRookFoeFileRankCheckNum[i][nRookCheckNum][m_nBRBitFile[1 - m_bIsRed][j]];
	pcmFile = m_pcmRookFilePreMoves[p][nRookCheckNum][nRookFoeCheckNum];
	nCount = pcmFile.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 8);
	nRet = nCount;

	//��
	nRookCheckNum = m_nRookFileRankCheckNum[j][m_nBitRank[i]];
	nRookFoeCheckNum = m_nRookFoeFileRankCheckNum[j][nRookCheckNum][m_nBRBitRank[1 - m_bIsRed][i]];
	pcmRank = m_pcmRookRankPreMoves[p][nRookCheckNum][nRookFoeCheckNum];
	nCount = pcmRank.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 9);
	nRet += nCount;

	return nRet;
}

inline FSINT CSearchEngine::GenKnightMoves(FSINT p)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);

	FSINT i, j;
	FSINT nCount;
	FSINT nRet;
	FSINT nKnightLegCheckSum;
	FSINT nKnightFile0AroundCheckSum;
	FSINT nKnightFile1AroundCheckSum;
	FSINT nKnightFile2AroundCheckSum;
	FSINT nKnightFile3AroundCheckSum;
	FSINT nKnightFileAroundCheckSum;

	FSINT nKnightFoeFile0AroundCheckSum;
	FSINT nKnightFoeFile1AroundCheckSum;
	FSINT nKnightFoeFile2AroundCheckSum;
	FSINT nKnightFoeFile3AroundCheckSum;
	FSINT nKnightFoeFileAroundCheckSum;

	PRECHESSMOVE pcm;
	
	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;
	
	nKnightFile0AroundCheckSum = m_nFileRankAroundCheckSum[i][m_nBitFile[m_nAbs[j][2]]];
	nKnightFile1AroundCheckSum = m_nFileRankAroundCheckSum2[i][m_nBitFile[m_nAbs[j][1]]];
	nKnightFile2AroundCheckSum = m_nFileRankAroundCheckSum2[i][m_nBitFile[j + 1]];
	nKnightFile3AroundCheckSum = m_nFileRankAroundCheckSum[i][m_nBitFile[j + 2]];

	nKnightFoeFile0AroundCheckSum = m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][m_nAbs[j][2]]];
	nKnightFoeFile1AroundCheckSum = m_nFileRankAroundCheckSum2[i][m_nBRBitFile[1 - m_bIsRed][m_nAbs[j][1]]];
	nKnightFoeFile2AroundCheckSum = m_nFileRankAroundCheckSum2[i][m_nBRBitFile[1 - m_bIsRed][j + 1]];
	nKnightFoeFile3AroundCheckSum = m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][j + 2]];

	nKnightFileAroundCheckSum = 
		m_nKnightAroundCheckSum[m_nAroundCheckSum[nKnightFile0AroundCheckSum][nKnightFile1AroundCheckSum]][m_nAroundCheckSum[nKnightFile2AroundCheckSum][nKnightFile3AroundCheckSum]];
	nKnightFoeFileAroundCheckSum = 
		m_nKnightAroundCheckSum[m_nAroundCheckSum[nKnightFoeFile0AroundCheckSum][nKnightFoeFile1AroundCheckSum]][m_nAroundCheckSum[nKnightFoeFile2AroundCheckSum][nKnightFoeFile3AroundCheckSum]];
	nKnightLegCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBitFile[j]]][m_nFileRankAroundCheckSum[j][m_nBitRank[i]]];
	
	//�������߷�
	pcm = m_pcmKnightNoCapPreMoves[p][nKnightFileAroundCheckSum][nKnightLegCheckSum];
	nCount = pcm.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 8);
	nRet = nCount;

	//�����߷�
	pcm = m_pcmKnightCapPreMoves[p][nKnightFoeFileAroundCheckSum][nKnightLegCheckSum];
	nCount = pcm.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 8);
	nRet += nCount;

	return nRet;
}

inline FSINT CSearchEngine::GenCannonMoves(FSINT p)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);

	FSINT i, j;
	FSINT nCount;
	FSINT nRet;
	FSINT nCannonCheckNum;
	FSINT nCannonFoeCheckNum;
	PRECHESSMOVE pcmFile;
	PRECHESSMOVE pcmRank;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	//��
	nCannonCheckNum = m_nCannonFileRankCheckNum[i][m_nBitFile[j]];
	nCannonFoeCheckNum =
		m_nCannonFoeFileRankCheckNum[i][nCannonCheckNum][m_nBRBitFile[1 - m_bIsRed][j]];
	pcmFile = m_pcmCannonFilePreMoves[p][nCannonCheckNum][nCannonFoeCheckNum];
	nCount = pcmFile.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 8);
	nRet = nCount;

	//��
	nCannonCheckNum = m_nCannonFileRankCheckNum[j][m_nBitRank[i]];
	nCannonFoeCheckNum = 
		m_nCannonFoeFileRankCheckNum[j][nCannonCheckNum][m_nBRBitRank[1 - m_bIsRed][i]];
	pcmRank = m_pcmCannonRankPreMoves[p][nCannonCheckNum][nCannonFoeCheckNum];
	nCount = pcmRank.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 9);
	nRet += nCount;

	return nRet;
}

inline FSINT CSearchEngine::GenKingMoves(FSINT p)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);

	FSINT i, j;
	FSINT nPosCheckNum;
	FSINT nAroundCheckSum;
	FSINT nFoeAroundCheckSum;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	nPosCheckNum = m_nKingPosCheckNum[p];
	DEBUGASSERT(nPosCheckNum >= MINKINGPOSCHECKNUM && nPosCheckNum <= MAXKINGPOSCHECKNUM);
	nAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBitFile[j]]][m_nFileRankAroundCheckSum[j][m_nBitRank[i]]];
	nFoeAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][j]]][m_nFileRankAroundCheckSum[j][m_nBRBitRank[1 - m_bIsRed][i]]];
	
	return m_pcmKingPreMoves[nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].nCount;
}

inline FSINT CSearchEngine::GenAdvisorMoves(FSINT p)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);

	FSINT i, j;
	FSINT nPosCheckNum;
	FSINT nAroundCheckSum;
	FSINT nFoeAroundCheckSum;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	nPosCheckNum = m_nAdvisorPosCheckNum[p];
	nAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBitFile[m_nAbs[j][1]]]][m_nFileRankAroundCheckSum[i][m_nBitFile[j + 1]]];
	nFoeAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][m_nAbs[j][1]]]][m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][j + 1]]];
	
	return m_pcmAdvisorPreMoves[nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].nCount;
}

inline FSINT CSearchEngine::GenBishopMoves(FSINT p)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);

	FSINT i, j;
	FSINT nPosCheckNum;
	FSINT nElephantEyeCheckSum;
	FSINT nAroundCheckSum;
	FSINT nFoeAroundCheckSum;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	nPosCheckNum = m_nBishopPosCheckNum[p];
	nAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum2[i][m_nBitFile[m_nAbs[j][2]]]][m_nFileRankAroundCheckSum2[i][m_nBitFile[j + 2]]];
	nFoeAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum2[i][m_nBRBitFile[1 - m_bIsRed][m_nAbs[j][2]]]][m_nFileRankAroundCheckSum2[i][m_nBRBitFile[1 - m_bIsRed][j + 2]]];
	nElephantEyeCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBitFile[m_nAbs[j][1]]]][m_nFileRankAroundCheckSum[i][m_nBitFile[j + 1]]];

	return m_pcmBishopPreMoves[nPosCheckNum][nElephantEyeCheckSum][nAroundCheckSum][nFoeAroundCheckSum].nCount;
}

inline FSINT CSearchEngine::GenPawnMoves(FSINT p)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);

	FSINT i, j;
	FSINT nPosCheckNum;
	FSINT nAroundCheckSum;
	FSINT nFoeAroundCheckSum;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	nPosCheckNum = m_nPawnPosCheckNum[m_bIsRed][p];
	nAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBitFile[j]]][m_nPawnRankAroundCheckSum[m_bIsRed][j][m_nBitRank[i]]];
	nFoeAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][j]]][m_nPawnRankAroundCheckSum[m_bIsRed][j][m_nBRBitRank[1 - m_bIsRed][i]]];

	return m_PawnPreMoves[m_bIsRed][nPosCheckNum][nAroundCheckSum][nFoeAroundCheckSum].nCount;
}

inline FSINT CSearchEngine::GenRookCapMoves(FSINT p)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);

	FSINT i, j;
	FSINT nCount;
	FSINT nRet;
	FSINT nRookCheckNum;//��У�����
	FSINT nRookFoeCheckNum;
	PRECHESSMOVE pcmFile;
	PRECHESSMOVE pcmRank;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;
	
	//��
	nRookCheckNum = m_nRookFileRankCheckNum[i][m_nBitFile[j]];
	nRookFoeCheckNum = m_nRookFoeFileRankCheckNum[i][nRookCheckNum][m_nBRBitFile[1 - m_bIsRed][j]];
	pcmFile = m_pcmRookFileCapPreMoves[p][nRookCheckNum][nRookFoeCheckNum];
	nCount = pcmFile.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= MAXROOKFILECAPMOVECNT);
	nRet = nCount;

	//��
	nRookCheckNum = m_nRookFileRankCheckNum[j][m_nBitRank[i]];
	nRookFoeCheckNum = m_nRookFoeFileRankCheckNum[j][nRookCheckNum][m_nBRBitRank[1 - m_bIsRed][i]];
	pcmRank = m_pcmRookRankCapPreMoves[p][nRookCheckNum][nRookFoeCheckNum];
	nCount = pcmRank.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= MAXROOKRANKCAPMOVECNT);
	nRet += nCount;

	return nRet;
}
/*
inline FSINT CSearchEngine::GenKnightCapMoves(FSINT p)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);

	FSINT i, j;
	FSINT nKnightLegCheckSum;

	FSINT nKnightFoeFile0AroundCheckSum;
	FSINT nKnightFoeFile1AroundCheckSum;
	FSINT nKnightFoeFile2AroundCheckSum;
	FSINT nKnightFoeFile3AroundCheckSum;
	FSINT nKnightFoeFileAroundCheckSum;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	nKnightFoeFile0AroundCheckSum = m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][m_nAbs[j][2]]];
	nKnightFoeFile1AroundCheckSum = m_nFileRankAroundCheckSum2[i][m_nBRBitFile[1 - m_bIsRed][m_nAbs[j][1]]];
	nKnightFoeFile2AroundCheckSum = m_nFileRankAroundCheckSum2[i][m_nBRBitFile[1 - m_bIsRed][j + 1]];
	nKnightFoeFile3AroundCheckSum = m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][j + 2]];

	nKnightFoeFileAroundCheckSum = 
		m_nKnightAroundCheckSum[m_nAroundCheckSum[nKnightFoeFile0AroundCheckSum][nKnightFoeFile1AroundCheckSum]][m_nAroundCheckSum[nKnightFoeFile2AroundCheckSum][nKnightFoeFile3AroundCheckSum]];
	nKnightLegCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBitFile[j]]][m_nFileRankAroundCheckSum[j][m_nBitRank[i]]];

	//�����߷�
	return m_pcmKnightCapPreMoves[p][nKnightFoeFileAroundCheckSum][nKnightLegCheckSum].nCount;
}*/
/*
inline FSINT CSearchEngine::GenCannonCapMoves(FSINT p)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);

	FSINT i, j;
	FSINT nCount;
	FSINT nRet;
	FSINT nCannonCheckNum;
	FSINT nCannonFoeCheckNum;
	PRECHESSMOVE pcmFile;
	PRECHESSMOVE pcmRank;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	//��
	nCannonCheckNum = m_nCannonFileRankCheckNum[i][m_nBitFile[j]];
	nCannonFoeCheckNum =
		m_nCannonFoeFileRankCheckNum[i][nCannonCheckNum][m_nBRBitFile[1 - m_bIsRed][j]];
	pcmFile = m_pcmCannonFileCapPreMoves[p][nCannonCheckNum][nCannonFoeCheckNum];
	nCount = pcmFile.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 2);
	nRet= nCount;

	//��
	nCannonCheckNum = m_nCannonFileRankCheckNum[j][m_nBitRank[i]];
	nCannonFoeCheckNum = 
		m_nCannonFoeFileRankCheckNum[j][nCannonCheckNum][m_nBRBitRank[1 - m_bIsRed][i]];
	pcmRank = m_pcmCannonRankCapPreMoves[p][nCannonCheckNum][nCannonFoeCheckNum];
	nCount = pcmRank.nCount;
	DEBUGASSERT(nCount >= 0 && nCount <= 2);
	nRet += nCount;

	return nRet;
}

inline FSINT CSearchEngine::GenKingCapMoves(FSINT p)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);

	FSINT i, j;
	FSINT nPosCheckNum;
	FSINT nFoeAroundCheckSum;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	nPosCheckNum = m_nKingPosCheckNum[p];
	nFoeAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][j]]][m_nFileRankAroundCheckSum[j][m_nBRBitRank[1 - m_bIsRed][i]]];
	
	return m_pcmKingCapPreMoves[nPosCheckNum][nFoeAroundCheckSum].nCount;
}

inline FSINT CSearchEngine::GenAdvisorCapMoves(FSINT p)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);

	FSINT i, j;
	FSINT nPosCheckNum;
	FSINT nFoeAroundCheckSum;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	nPosCheckNum = m_nAdvisorPosCheckNum[p];
	DEBUGASSERT(nPosCheckNum >= 0);
	nFoeAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][m_nAbs[j][1]]]][m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][j + 1]]];
	
	return m_pcmAdvisorCapPreMoves[nPosCheckNum][nFoeAroundCheckSum].nCount;
}

inline FSINT CSearchEngine::GenBishopCapMoves(FSINT p)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);

	FSINT i, j;
	FSINT nPosCheckNum;
	FSINT nElephantEyeCheckSum;
	FSINT nFoeAroundCheckSum;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	nPosCheckNum = m_nBishopPosCheckNum[p];
	nFoeAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum2[i][m_nBRBitFile[1 - m_bIsRed][m_nAbs[j][2]]]][m_nFileRankAroundCheckSum2[i][m_nBRBitFile[1 - m_bIsRed][j + 2]]];
	nElephantEyeCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBitFile[m_nAbs[j][1]]]][m_nFileRankAroundCheckSum[i][m_nBitFile[j + 1]]];

	return m_pcmBishopCapPreMoves[nPosCheckNum][nElephantEyeCheckSum][nFoeAroundCheckSum].nCount;
}

inline FSINT CSearchEngine::GenPawnCapMoves(FSINT p)
{
	DEBUGASSERT(p >= 0 && p < MAXCHESSGRIDNUM);

	FSINT i, j;
	FSINT nPosCheckNum;
	FSINT nFoeAroundCheckSum;

	i = m_CGN2Pos[p].x;
	j = m_CGN2Pos[p].y;

	nPosCheckNum = m_nPawnPosCheckNum[m_bIsRed][p];
	nFoeAroundCheckSum = 
		m_nAroundCheckSum[m_nFileRankAroundCheckSum[i][m_nBRBitFile[1 - m_bIsRed][j]]][m_nPawnRankAroundCheckSum[m_bIsRed][j][m_nBRBitRank[1 - m_bIsRed][i]]];

	return m_PawnCapPreMoves[m_bIsRed][nPosCheckNum][nFoeAroundCheckSum].nCount;
}
*/
inline FSBOOL CSearchEngine::IsCheckMove(CHESSMOVE &cm)
{
	FSINT nToChessID;
	FSINT nToPieceID;
	FSBOOL bRet;

	MakeMove(cm, nToChessID, nToPieceID);
	bRet = IsChecked(1 - m_bIsRed);
	UnMakeMove(cm, nToChessID, nToPieceID);

	return bRet;
}

const char* CSearchEngine::GetSpecialFEN(char *str)
{
	if (strncmp(str, "opengamepos", 11) == 0)
	{
		return g_szOpenGamePos;
	}

	if (strncmp(str, "midgamepos", 10) == 0)
	{
		return g_szMidGamePos;
	}

	if (strncmp(str, "endgamepos", 10) == 0)
	{
		return g_szEndGamePos;
	}

	if (strncmp(str, "checkmatepos", 12) == 0)
	{
		return g_szCheckMatePos;
	}

	if (strncmp(str, "zugzwangpos", 12) == 0)
	{
		return g_szZugzwangPos;
	}

	if (strncmp(str, "detectnmpos", 11) == 0)
	{
		return g_szDetectNMPos;
	}

	if (strncmp(str, "knightvssingleadvisor", 21) == 0)
	{
		return g_szKnightVSSglAdvisorPos;
	}

	if (strncmp(str, "fishformooninsea", 16) == 0)
	{
		return g_szFishforMoonInSeaPos;
	}

	if (strncmp(str, "repdetectpos", 12) == 0)
	{
		return g_szRepDetectPos;
	}

	return "nothatpos";
}