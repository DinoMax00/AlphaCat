
/* ============================================================================================ *\
 *	Author		:	ahei																		*
 *	Email		:	ahei0802@126.com															*
 *	Msn			:	ahei080210114@hotmail.com													*
 *	QQ			:	8261525																		*
 *	Version		:	0.2 																		*
 *	Created		:	2005/03/01 9:10:13															*
 *	Modified	:	2006/01/01 17:15:46															*
 *	FileName	: 	SearchEngine.h																*
 *	Description	:	This file is using for declaring the class CSearchEngine,and this class is	*
 *					using for generating moves,evaluating and searching.						*
 * 	Warning     : 	Copyright (c) ahei 2004 All rights reserved.  								*
\* ============================================================================================ */

#ifndef _SEARCHENGINE_H_
#define _SEARCHENGINE_H_

#include <fstream>
#include <stack>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

//********�������ͱ��********
#define NOCHESS		0	//������

#define BKING		1	//�ڽ�
#define BADVISOR	2	//��ʿ
#define BBISHOP		3	//����
#define BROOK		4	//�ڳ�
#define BKNIGHT		5	//����
#define BCANNON		6	//����
#define BPAWN		7	//����
#define BBEGIN		1
#define BEND		7

#define RKING		8	//��˧
#define RADVISOR	9	//��ʿ
#define RBISHOP		10	//����
#define RROOK		11	//�쳵
#define RKNIGHT		12	//����
#define RCANNON		13	//����
#define RPAWN		14	//���
#define RBEGIN		8
#define REND		14
//****************************

// [2005/10/20 22:55:11]
//******�ܵ��������ͱ��******
#define KING		1	//��
#define ADVISOR		2	//ʿ
#define BISHOP		3	//��
#define ROOK		4	//��
#define KNIGHT		5	//��
#define CANNON		6	//��
#define PAWN		7	//��
//****************************

#define BLACK	0	//�ڷ�
#define RED		1	//�췽

//�߷�����
#define NORMAL	1	//һ��
#define ESCAPE	2	//��
#define CAP		4	//׽
#define EAT		8	//����
#define CHECK	16	//����

#define ENGINEVERSION	"0.2"
#define ENGINENAME		"ZWM"
#define ENGINEAUTHOR	"������"

#define BOOKFILE	"Book.dat"

//ǳ��Э��汾
#define QIANHONGPVER	"QHPLUGIN V1.3"

#define QIANHONGLEVELCNT	9	//ǳ�켶����
#define MINQHLEVEL	1
#define MAXQHLEVEL	QIANHONGLEVELCNT

#define INVALIDCHECKSUM		-1	//��ЧУ���

#define	MAXSEARCHDEPTH				128		//������������ [2005/11/2 13:15:19]
#define MAXMOVECNTCAPAICITY			128		//�������ɵ�����߷���
#define MAXNOCAPMOVECNTCAPAICITY	128		//�������ɵ��������߷���
#define MAXCAPMOVECNTCAPAICITY		128		//�������ɵ��������߷���

#define MINVALIDCHESSID	0	//��С���������ͱ��
#define MAXVALIDCHESSID	14	//������Ч�������ͱ��

#define MINVALIDPIECEID		0						//��С�����ӱ��
#define MAXVALIDPIECEID		31						//������Ч���ӱ��
#define MAXPIECEID			(MAXVALIDPIECEID + 1)	//�������ӱ��

#define MAXXSCALENUM		9	//X�������
#define MAXYSCALENUM		10	//Y�������
#define MAXCHESSGRIDNUM		90	//���������

#define ERRCHESSID		-1					//������������ͱ��
#define ERRPIECEID		-1					//��������ӱ��
#define INVALIDPIECEID	MAXPIECEID			//��Ч���ӱ��
#define INVALIDPIECEPOS	MAXCHESSGRIDNUM		//��Ч������λ��

//У������ֵ
//��
#define ROOKFILECHECKSUMMAXCNT	25	//����У����������ֵ,RookFileCheckSumMaxCnt
#define ROOKRANKCHECKSUMMAXCNT	30	//����У����������ֵ,RookRankCheckSumMaxCnt
#define ROOKFRCHECKSUMMAXCNT	ROOKRANKCHECKSUMMAXCNT

//��
#define CANNONFILECHECKSUMMAXCNT	121	//����У����������ֵ,CannonFileCheckSumMaxCnt
#define CANNONRANKCHECKSUMMAXCNT	176	//����У����������ֵ,CannonRankCheckSumMaxCnt
#define CANNONFRCHECKSUMMAXCNT		CANNONRANKCHECKSUMMAXCNT

#define MAXBUFFERLEN	1024	//��������󳤶�
#define MAXMOVESTRLEN	8		//�߷��ַ�����󳤶�

#define MAXMOVESCNT_NOEAT	120 //˫��û�г��ӵ����岽��(��غ���)
								//ͨ����ֵ�ﵽ120��Ҫ�к�(��ʮ�غ���Ȼ����)

#define INVALIDPLY -1	//��Ч��

#define MINHASHTABSIZE		(1 << 19)	//hash������С
#define MAXHASHTABSIZE		(1 << 21)	//hash����С��С

// ��Ҫ�ı�,�����ı�,ҲҪ�ı�ɱ���߷����е�ɱ���߷����ĸ��·���[2005/10/24 14:54:58]
#define MAXKILLERMOVECNT	2	//ɱ���߷����ĸ���

//�߷������� [2005/10/20 12:46:33]
#define MAXALLMOVESCNT				119	//�����߷�

#define MAXMOVECNT					17	//�����߷������

#define MAXROOKMOVECNT				17	//��
#define MAXROOKFILEMOVECNT			8	//����
#define MAXROOKRANKMOVECNT			9	//����

#define MAXCANNONMOVECNT			17	//��
#define MAXCANNONFILEMOVECNT		8	//����
#define MAXCANNONRANKMOVECNT		9	//����

#define MAXKNIGHTMOVECNT			8	//��
#define MAXKINGMOVECNT				4	//��
#define MAXADVISORMOVECNT			4	//ʿ
#define MAXBISHOPMOVECNT			4	//��
#define MAXDOWNRIVERPAWNMOVECNT		1	//δ���ӱ�
#define MAXUPRIVERPAWNMOVECNT		3	//���ӱ�

// �������������߷���[2005/10/21 22:24:44]
#define MAXONECHESSMOVECNT			MAXROOKMOVECNT

//�������߷������� [2005/10/20 12:46:40]
#define MAXNOCAPMOVECNT				119	//���в������߷�

#define MAXROOKNOCAPMOVECNT			17	//��
#define MAXROOKFILENOCAPMOVECNT		8	//����
#define MAXROOKRANKNOCAPMOVECNT		9	//����

#define MAXCANNONNOCAPMOVECNT		17	//��
#define MAXCANNONFILENOCAPMOVECNT	8	//����
#define MAXCANNONRANKNOCAPMOVECNT	9	//����

#define MAXKNIGHTNOCAPMOVECNT			8	//��
#define MAXKINGNOCAPMOVECNT				4	//��
#define MAXADVISORNOCAPMOVECNT			4	//ʿ
#define MAXBISHOPNOCAPMOVECNT			4	//��
#define MAXDOWNRIVERPAWNNOCAPMOVECNT	1	//δ���ӱ�
#define MAXUPRIVERPAWNNOCAPMOVECNT		3	//���ӱ�
#define MAXPAWNNOCAPMOVECNT			MAXUPRIVERPAWNNOCAPMOVECNT

//�����߷������� [2005/10/20 12:46:40]
#define MAXCAPMOVECNT				67	//���г����߷�

#define MAXROOKCAPMOVECNT			4	//��
#define MAXROOKFILECAPMOVECNT		2	//����
#define MAXROOKRANKCAPMOVECNT		2	//����

#define MAXCANNONCAPMOVECNT			4	//��
#define MAXCANNONFILECAPMOVECNT		2	//����
#define MAXCANNONRANKCAPMOVECNT		2	//����

#define MAXKNIGHTCAPMOVECNT			8	//��
#define MAXKINGCAPMOVECNT			4	//��
#define MAXADVISORCAPMOVECNT		4	//ʿ
#define MAXBISHOPCAPMOVECNT			4	//��
#define MAXDOWNRIVERPAWNCAPMOVECNT	1	//δ���ӱ�
#define MAXUPRIVERPAWNCAPMOVECNT	3	//���ӱ�
#define MAXPAWNCAPMOVECNT			MAXUPRIVERPAWNCAPMOVECNT

//����,����������
#define MAXKNIGHTLEGCNT				4	//����
#define MAXBISHOPEYECNT				4	//����

//���ӻ�����ֵBV:BasicValue [2005/10/20 11:52:19]
#define BV_ROOK					200	//��

#define BV_KNIGHT_OPEN_GAME		88	//������
#define BV_KNIGHT_MID_GAME		92	//�о���
#define BV_KNIGHT_END_GAME		96	//�о���

#define BV_CANNON_OPEN_GAME		96	//������
#define BV_CANNON_MID_GAME		92	//�о���
#define BV_CANNON_END_GAME		88	//�о���

#define BV_KING					0	//��
#define BV_ADVISOR				40	//ʿ
#define BV_BISHOP				40	//��

#define BV_PAWN_OPEN_GAME		9	//���ֱ�
#define BV_PAWN_MID_GAME		10	//�оֱ�
#define BV_PAWN_END_GAME		20	//�оֱ�

#define BV_NOCHESS				0	//����

// ��������Լ�ֵ:FlexValue[2005/12/26 16:15:52]
#define FV_ROOK		2
#define FV_KNIGHT	3
#define FV_CANNON	1

//�����ڼ�ֵ[2006/1/1 14:29:02]
#define EMPTYHEARTVALUE		BV_BISHOP

// ��ֽ׶�[2005/10/25 11:26:46]
#define OPEN_GAME	0	//����
#define MID_GAME	1	//�о�
#define END_GAME	2	//�о�

//Ԥ�߷��ļ��� [2005/10/21 21:32:14]
#define KNIGHTPREMOVESFILE	"knightPreMoves.dat"	//��Ԥ�߷�

//��λ��У����ż�ֵ
#define MINKINGPOSCHECKNUM		0
#define MAXKINGPOSCHECKNUM		17

//��λ��У����ż�ֵ
#define MINPAWNPOSCHECKNUM		0
#define MAXPAWNPOSCHECKNUM		55

#define BEGAINGAMEMOVESCNT	10	//�����߷���

//#define TESTHH	//������ʷ����

//����ǳ��Э��
//#define TESTQHP

//��ȫ�ͷ��ڴ�
#define SAFEDEL

//�õ�����������
#define GETMAXSCHDPT

//�õ����ѡ�����������
#define GETMAXSELSCHDPT

//��־�ļ�
#define LOGFILE

//�����ļ�
#define ERRFILE

//ͳ��
//#define STAT

//�����ڴ�
//#define MEMCALC

//��������ʱ��
//#define TIMECALC

//����
//#define DEBUG

//����
//#define TEST

//�������ı����ֿ�
#define LRTEXTBOOK

//�������ı����ֿ�
//#define UDTEXTBOOK

#ifdef _DEBUG
#define DEBUG
#endif

//Debug״̬�µĵ������
#ifdef _DEBUG
#define DEBUGCOUT(str) (cout << (str))
#else 
#define DEBUGCOUT(str) 
#endif

// Release״̬�µĵ������
#ifndef _DEBUG
#define RELEASECOUT(str) (cout << (str))
#else 
#define RELEASECOUT(str) 
#endif

// _ASSERT���Ĳ������
#ifdef DEBUG 
#define DEBUGASSERT(exp) _ASSERT(exp)
#else 
#define DEBUGASSERT(exp) 
#endif

// ��try��new
#ifdef DEBUG

#define TRYNEW(NewStatement)		\
	try								\
	{								\
		NewStatement				\
	}								\
	catch(std::bad_alloc const &)	\
	{								\
		AllocateMemErrProc();		\
	}								
#else
#define TRYNEW(NewStatement) NewStatement
#endif

//��try��delete
#ifdef DEBUG

#define TRYDELETE(DeleteStatement)		\
	try									\
	{									\
		DeleteStatement					\
	}									\
	catch(...)							\
	{									\
		FreeMemErrProc();				\
	}									
#else
#define TRYDELETE(DeleteStatement) DeleteStatement
#endif

// �ͷ���Чָ��[2005/10/23 14:53:21]
#define VALIDDELETE(PointerToDel, InvalidPointer)	\
	if (PointerToDel != InvalidPointer)				\
	{												\
		TRYDELETE(SAFEDELETE(PointerToDel));		\
	}

//��ȫ�ͷ��ڴ�
//TwoDPointer:��άָ��
#ifdef SAFEDEL
#define SAFEDELETE(TwoDPointer)		{delete[] TwoDPointer; TwoDPointer = NULL;}
#else
#define SAFEDELETE(TwoDPointer)		{delete[] TwoDPointer;}
#endif

// msת����s[2005/10/22 15:48:38]
#define MS2S(t)		(t) * 1.0 / 1000

// �ֽ�ת��ΪM[2005/10/23 14:33:32]
#define B2M(s)		(s) * 1.0 / (1 << 20)

//���Ӷ�ά����ת��Ϊһά���� [2005/10/20 19:56:11]
#define POS2CGN(i, j)	MAXYSCALENUM * (i) + (j)

//������i�Ƿ���X���� [2005/10/20 20:24:17]
#define ISONXSCALE(i)	((i) >=0 && (i) < MAXXSCALENUM)

//������j�Ƿ���Y���� [2005/10/20 20:24:17]
#define ISONYSCALE(j)	((j) >= 0 && (j) < MAXYSCALENUM)

// �Ƿ���������[2005/10/24 15:32:36]
#define ISINBOARD(pos)	((pos) >= 0 && (pos) < MAXCHESSGRIDNUM)

//���CHESSMOVE [2005/10/20 16:10:25]
#define FILLCHESSMOVE(cm, from, to, score, eatChess, kind)		\
	{															\
		DEBUGASSERT(from >= 0 && from < MAXCHESSGRIDNUM);		\
		DEBUGASSERT(to >= 0 && to < MAXCHESSGRIDNUM);			\
		DEBUGASSERT(eatChess == NOCHESS ||						\
					m_bIsBlack_ChessID[eatChess] ||				\
					m_bIsRed_ChessID[eatChess]);				\
																\
		cm.nFrom = from;										\
		cm.nTo = to;											\
																\
		cm.nScore = score;										\
		cm.nEatChess = eatChess;								\
		cm.nKind = kind;										\
	}

#define FILLUNDOMOVE(undoMove, chessMove, toChessID, toPieceID)		\
	{																\
		undoMove.cm = chessMove;									\
		undoMove.nToChessID = toChessID;							\
		undoMove.nToPieceID = toPieceID;							\
	}

//�Լ�
#ifdef STAT
#define INCREMENT_STAT(a)	(a)++
#else
#define INCREMENT_STAT(a)	
#endif

// ����[2005/11/2 23:19:48]
#define RATE(a, b)	(a) * 100.0 / (b)

#ifdef STAT
#define STATEMENT_STAT(statement)	statement
#else
#define STATEMENT_STAT(statement)	
#endif

#ifdef TEST
#define STATEMENT_TEST(statement)	statement
#else
#define STATEMENT_TEST(statement)	
#endif

#ifdef DEBUG
#define STATEMENT_DEBUG(statement)	statement
#else
#define STATEMENT_DEBUG(statement)	
#endif

#ifdef TESTQHP
#define STATEMENT_TESTQHP(statement)	statement
#else
#define STATEMENT_TESTQHP(statement)	
#endif

#ifdef TESTQHP
#define STATEMENT_NOTESTQHP(statement)	
#else
#define STATEMENT_NOTESTQHP(statement)	statement
#endif

#ifdef GETMAXSCHDPT
#define STATEMENT_GETMAXSCHDPT(statement)	statement
#else
#define STATEMENT_GETMAXSCHDPT(statement)	
#endif

#ifdef GETMAXSELSCHDPT
#define STATEMENT_GETMAXSELSCHDPT(statement)	statement
#else
#define STATEMENT_GETMAXSELSCHDPT(statement)	
#endif

#ifdef LOGFILE
#define STATEMENT_LOGFILE(statement)	statement
#else
#define STATEMENT_LOGFILE(statement)	
#endif

#ifdef ERRFILE
#define STATEMENT_ERRFILE(statement)	statement
#else
#define STATEMENT_ERRFILE(statement)	
#endif

//���в���
#ifdef _WIN32
#define PARALLELDO(statement)						\
	{												\
		EnterCriticalSection(&g_CriticalSection);	\
		statement;									\
		LeaveCriticalSection(&g_CriticalSection);	\
	}
#else
#define PARALLELDO(statement) statement
#endif

#ifdef _WIN32
#define STRIDEPLATFORM(statement1, statement2)	statement1
#else
#define STRIDEPLATFORM(statement1, statement2)	statement2
#endif

#define SWITCHEGN(statement)			\
	switch(m_EgnProtocol)				\
	{									\
	case EgnPUCCI:						\
	case EgnPAhei:						\
		statement						\
		break;							\
										\
	case EgnPQianHong:					\
		STATEMENT_TESTQHP(statement);	\
		break;							\
										\
	default:							\
		break;							\
	}

//�������Ͷ���,���ڿ�ƽ̨��ֲ [2005/10/19 11:50:59]
#ifndef _WIN32
#define __int64 long long
#endif

typedef void		FSVOID;		//void
typedef char		FSCHAR;		//char
typedef bool		FSbool;		//bool
typedef long		FSBOOL;		//BOOL
typedef short		FSSHORT;	//short
typedef long		FSLONG;		//long
typedef long		FSINT;		//long
typedef DWORD		FSDWORD;	//DWORD
typedef float		FSFLOAT;	//float
typedef short		FRINT16;	//16λ����
typedef long		FRINT32;	//32λ����
typedef __int64 	FRINT64;	//64λ����

typedef unsigned char		FRUCHAR;	//�޷���char
typedef unsigned long		FRUBOOL;	//�޷���bool
typedef unsigned short		FRUSHORT;	//�޷���short
typedef unsigned long		FSULONG;	//�޷���long
typedef unsigned long		FSUINT;		//�޷���long
typedef unsigned short		FRUINT16;	//16λ�޷�������
typedef unsigned long		FRUINT32;	//32λ�޷�������
typedef unsigned __int64	FSUINT64;	//64λ�޷�������

typedef FSCHAR	 FEATCHAR;	//���ʵ�char����
typedef FSSHORT	 FEATSHORT; //���ʵ�BYTE����
typedef FSCHAR	 FEATBOOL;  //���ʵ�BOOL����

typedef FRUCHAR		FEATUCHAR;	//���ʵ��޷���char����
typedef FRUSHORT	FEATUSHORT;	//���ʵ��޷���short����
typedef FRUCHAR		FEATUBOOL;  //���ʵ��޷���BOOL����

typedef FSLONG 		SCORETYPE;	//�߷�����ʷ�÷ֵ�����

//�߷�
struct CHESSMOVE
{
	SCORETYPE	nScore;		//�߷��÷�

	FSCHAR		nFrom;		//Դλ��
	FSCHAR		nTo;		//Ŀ��λ��

	FSCHAR		nEatChess;	//�Ե�������
	FSCHAR		nKind;		//�߷�����

	CHESSMOVE()
	{
		nScore = 0;

		nFrom = 0;
		nTo = 0;

		nEatChess = NOCHESS;
		nKind = NORMAL;
	}

	FSBOOL operator == (CHESSMOVE cm)
	{
		return cm.nScore == nScore && cm.nEatChess == nEatChess && 
			cm.nFrom == nFrom && cm.nTo == nTo && cm.nKind == nKind;
	}

	FSBOOL operator != (CHESSMOVE cm)
	{
		return cm.nScore != nScore || cm.nEatChess != nEatChess || 
			cm.nFrom != nFrom || cm.nTo != nTo || cm.nKind != nKind;
	}
};

//ǳ�����ʱ�õ����ݽṹ
struct UNDOMOVE
{
	CHESSMOVE cm;

	FSINT nToChessID;
	FSINT nToPieceID;
};

typedef UNDOMOVE REDOMOVE;

#ifdef _WIN32
typedef stack<UNDOMOVE> UNDOMOVESTACK;
typedef stack<REDOMOVE> REDOMOVESTACK;
#endif

//Ԥ�����߷�(ע��:pMoveList��nCount������˳��Ҫ�ı�)
struct PRECHESSMOVE
{
	CHESSMOVE	*pMoveList;	//�߷���
	FSINT		nCount;		//�߷�����
};

// �߷�·��[2005/11/12 16:33:38]
struct CMLINE
{
	CHESSMOVE	pMoveList[MAXSEARCHDEPTH + MAXMOVESCNT_NOEAT];
	FSINT		nCount;
};

// ��Ҫ����·��[2005/11/12 16:34:06]
struct PVLINE
{
	CHESSMOVE	pMoveList[MAXSEARCHDEPTH];
	FSINT		nCount;
};

// ɱ���߷�[2005/10/24 14:21:22]
struct KILLERMOVETAB
{
	CHESSMOVE	cmKiller[MAXKILLERMOVECNT];
	FSINT		nKillerCount;
	FSINT		nPointer;

	KILLERMOVETAB()
	{
		nKillerCount = 0;
		nPointer = 0;
	}
};

// hash���нڵ�����
#define HASHALPHA	0
#define HASHEXACT	1
#define HASHBETA	2

// ���̾����hash����
struct HASHITEM
{
	FSUINT64	nCBCheckSum;	//�˾����У���
	CHESSMOVE	cmBestMove;		//�˾����Ӧ����õ��߷�
	FSSHORT		nEval;			//�˾���Ĺ�ֵ
	FSCHAR		nHashFlag;		//�ڵ�����
	FSCHAR		nDepth;			//�˾������ڲ������еĲ���
//	FSINT		nEval;			//�˾���Ĺ�ֵ
//	FSINT		nHashFlag;		//�ڵ�����
//	FSINT		nDepth;			//�˾������ڲ������еĲ���

	HASHITEM()
	{
		nDepth = INVALIDPLY;
	}
};

//���̾����hash����,���ھ�̬������
struct QSHASHITEM
{
	FSUINT64	nCBCheckSum;	//�˾����У���
	CHESSMOVE	cmBestMove;		//�˾����Ӧ����õ��߷�
	FSSHORT		nEval;			//�˾���Ĺ�ֵ
	FSSHORT		nHashFlag;		//�ڵ�����
};

//�ı����ֿ�ṹ
struct TXTBOOKRECORD
{
	FSINT nCBChessID[90];	//����
	CHESSMOVE cm;			//�߷�
};

// �߷�����[2005/12/24 19:14:39]
struct MOVESORT
{
	FSINT nPhase;
	FSINT nMoveIndex;
	CHESSMOVE pMoveList[MAXSEARCHDEPTH];

	MOVESORT(CHESSMOVE cmIterative, KILLERMOVETAB killerMoveTab)
	{
		nPhase = 0;
		nMoveIndex = 0;

		pMoveList[0] = cmIterative;

		FSINT i;
		FSINT nKillerCnt = killerMoveTab.nKillerCount;

		for (i = 1; i < nKillerCnt + 1; i++)
		{
			pMoveList[i] = killerMoveTab.cmKiller[i - 1];
		}
	}

	CHESSMOVE Next()
	{
		CHESSMOVE cm;
		return cm;
	}
};

// ����Э��
enum EgnProtocol
{
	EgnPNone,		//��
	EgnPUCCI,		//UCCIЭ��
	EgnPAhei,		//aheiЭ��
	EgnPQianHong,	//ǳ��Э��
};

// ����״̬
enum EgnStatus
{
	EgnSIdle,		// ����
	EgnSBusy,		// æ
	EgnSSuspend,	// ����
};

// UCCIЭ������
enum UCCICmd
{
	UCCICNone,			// ������
	UCCICInvalid,		// ��Ч����

	UCCICPonderHit,		// ��̨˼������, "ponderhit"

	UCCILoadBook,		// �������濪ʼʹ�ÿ��ֿ�, "loadbook"

	UCCINullMove,		// �Ƿ�����Null Move, "null move on/off"
	UCCIOpenBook,		// �Ƿ�ʹ�ÿ��ֿ�, "openbook on/off"
	UCCIEndBook,		// �Ƿ�ʹ�òоֿ�, "endbook on/off"
	
	UCCIClearHash,		// ���hash, "clearhash"

	UCCIHistroyShit,	// ������ʷ�ü�����ֵ, "historyshit param"

	UCCIDrawValue,		// �趨�����ֵ

	UCCIDrawMoves,		// �趨��Ȼ����

	UCCICEPlies,		// �趨�����������

	UCCIOnlyMove,		// Ψһ��ʱֱ�ӳ���,����˼��, "onlymove on/off"

	UCCITime,			// �鿴˼����˼��ʱ��

	UCCICStop,			// ֹͣ����˼��, "stop"
	UCCIPause,			// ��ͣ����˼��, "pause"
	UCCIResume,			// ��������˼��, "resume"

	UCCICIsReady,		// �����Ƿ�׼����, "isready"
	UCCICSetOption,		// ����ѡ��, "setoption ..."
	UCCICPosition,		// �������潫Ҫ˼���ľ���, "position ..."
	UCCICGo,			// ������˼��, "go ..."
	UCCICAddPosition, 
	UCCICDebug,			// ��������, "debug..."

	UCCIStatus,			// �õ�����״̬, "status"

	UCCICQuitUCCI,		// �˳�UCCIЭ��, "quitucci"
	UCCICQuitEgn,		// �˳�����, "quitegn"

	UCCIGetSpecialFEN,	// �õ���������FEN
};

//ǳ��Э������
enum QianHongCmd
{
	QHCNone,		//������
	QHCInvalid,		//��Ч����

	QHCLevel,		//���ü���
	QHCFEN,			//������
	QHCPlay,		
	QHCLoad,		
	QHCAI,			//����
	QHCAbort,		//�ж�

	QHCQuit,		//�˳�
	QHCQuitQH,		//�˳�ǳ��

	QHCUndo,		//����
	QHCHints,		//��ʾ
	QHCBan,			//����
	QHCBGThink,		//����˼��
	QHCTimeOut,		//��ʱ
};

//˼����ʽ
enum THINKTYPE
{
	TT_DEPTH,	//�����˼��
	TT_TIME,	//��ʱ��˼��
};

//������
enum CHESSSTYLE
{
	Keep,		//����
	Normal,		//����
	Risky,		//ð��
};

const FSCHAR g_szLogFile[MAXBUFFERLEN] = ENGINENAME"Log.txt";	//��־�ļ���
const FSCHAR g_szErrFile[MAXBUFFERLEN] = ENGINENAME"Err.txt";	//�����ļ���:��ſ��ܴ����ָ�����ʶ���ָ��

const FSCHAR g_szEngineName[MAXBUFFERLEN] = "ZWM";	//������

//FEN�ַ����е�Сд��ĸ��ڷ����ӵĶ�Ӧ��,Сд��ĸa~r
const FSINT g_nAlpha2Chess[18] = 
{
	ADVISOR, BISHOP, CANNON, -1,   -1,   -1,
	-1,		 -1,	 -1,     -1,   KING, -1,
	-1,      KNIGHT, -1,     PAWN, -1,	 ROOK,
};

const FSCHAR g_cChessID2Alpha[15] = 
{
	'0', //��

	'k', //�ڽ�
	'a', //��ʿ
	'b', //����
	'r', //�ڳ�
	'n', //����
	'c', //����
	'p', //�ڱ�

	'K', //�콫
	'A', //��ʿ
	'B', //����
	'R', //�쳵
	'N', //����
	'C', //����
	'P', //���
};

//��ʼ����
const FSINT g_nChessBoard[90] = 
{
	RROOK,	 NOCHESS,NOCHESS,RPAWN,  NOCHESS,NOCHESS,BPAWN,  NOCHESS,NOCHESS,BROOK,
	RKNIGHT, NOCHESS,RCANNON,NOCHESS,NOCHESS,NOCHESS,NOCHESS,BCANNON,NOCHESS,BKNIGHT,
	RBISHOP, NOCHESS,NOCHESS,RPAWN,  NOCHESS,NOCHESS,BPAWN,  NOCHESS,NOCHESS,BBISHOP,
	RADVISOR,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,BADVISOR,
	RKING,   NOCHESS,NOCHESS,RPAWN,  NOCHESS,NOCHESS,BPAWN,  NOCHESS,NOCHESS,BKING,
	RADVISOR,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,BADVISOR,
	RBISHOP, NOCHESS,NOCHESS,RPAWN,  NOCHESS,NOCHESS,BPAWN,  NOCHESS,NOCHESS,BBISHOP,
	RKNIGHT, NOCHESS,RCANNON,NOCHESS,NOCHESS,NOCHESS,NOCHESS,BCANNON,NOCHESS,BKNIGHT,
	RROOK,	 NOCHESS,NOCHESS,RPAWN,  NOCHESS,NOCHESS,BPAWN,  NOCHESS,NOCHESS,BROOK,
};

//��ʼ����λ��
const FSINT g_nPiecePos[32]=
{
	 0,10,20,30,40,50,60,70,
	80,12,72, 3,23,43,63,83,
	 6,26,46,66,86,17,77, 9,
	19,29,39,49,59,69,79,89,
};

//��ʼ��λ���ϵ�����
const FSINT g_nCBPieceID[90] =
{
	0,-1,-1,11,-1,-1,16,-1,-1,23,
	1,-1,9, -1,-1,-1,-1,21,-1,24,
	2,-1,-1,12,-1,-1,17,-1,-1,25,
	3,-1,-1,-1,-1,-1,-1,-1,-1,26,
	4,-1,-1,13,-1,-1,18,-1,-1,27,
	5,-1,-1,-1,-1,-1,-1,-1,-1,28,
	6,-1,-1,14,-1,-1,19,-1,-1,29,
	7,-1,10,-1,-1,-1,-1,22,-1,30,
	8,-1,-1,15,-1,-1,20,-1,-1,31,
};

//����߷�������Ӧ������λ��
const FSINT g_nKnightLegPos[43] = 
{
	//����λ��	//���߷�����	Index
	-10,		//-21			21 - 21 = 0
	0,			//��			21 - 20 = 1
	-10,		//-19			21 - 19 = 2
	0,			//��			21 - 18 = 3
	0,			//��			21 - 17 = 4
	0,			//��			21 - 16 = 5
	0,			//��			21 - 15 = 6
	0,			//��			21 - 14 = 7
	0,			//��			21 - 13 = 8
	-1,			//-12			21 - 12 = 9
	0,			//��			21 - 11 = 10
	0,			//��			21 - 10 = 11
	0,			//��			21 - 09 = 12
	1,			//-8			21 - 08 = 13
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	//14 ~ 28
	-1,			//8				21 + 08 = 29
	0, 0, 0,										//30 ~ 32
	1,			//12			21 + 12 = 33
	0, 0, 0, 0, 0, 0,								//34 ~ 39
	10,			//19			21 + 19 = 40
	0,			//��			21 + 20 = 8
	10,			//21			21 + 21 = 42
};

const FSINT g_nMateValue = 5000;										// ɱ���ֵ
const FSINT g_nPerCheckValue = -(g_nMateValue + MAXSEARCHDEPTH + 1);	// ������ֵ
const FSINT g_nGoodPositionValue = 300;									// ���ž����ֵ

const FSINT g_nContemptFactor_Keep = -20;			//�����ʱ����������,�������ֵ
const FSINT g_nContemptFactor_Normal = 0;			//������ʱ����������,�������ֵ
const FSINT g_nContemptFactor_Risky = 20;			//���ð��ʱ����������,�������ֵ

const FSINT g_nInvalidValue = g_nMateValue + 5000;	//������Чֵ,�����ֵ�������g_nMateValue
const FSINT g_nTimeOutScore = g_nInvalidValue;		//��ʱʱ���ص�ֵ,�����ֵ�������g_nMateValue

//�߷�����
//��
const FSINT g_nKnightMoveDelta[MAXKNIGHTMOVECNT] = 
{
	-12, 8, -21, 19, 
	-19, 21, -8, 12, 
};
//����
const FSINT g_nKnightLegDelta[MAXKNIGHTLEGCNT] = 
{
	-10, 10, -1, 1, 
};

//��
const FSINT g_nKingMoveDelta[MAXKINGMOVECNT] = 
{
	-10, 10, -1, 1, 
};

//ʿ
const FSINT g_nAdvisorMoveDelta[MAXADVISORMOVECNT] = 
{
	-11, 9, -9, 11, 
};

//��
const FSINT g_nBishopMoveDelta[MAXBISHOPMOVECNT] = 
{
	-22, 18, -18, 22, 
};
//����
const FSINT g_nBishopEyeDelta[MAXBISHOPEYECNT] = 
{
	-11, 9, -9, 11, 
};

//��
//δ���ӱ�
const FSINT g_nDownRiverPawnMoveDelta[2][MAXDOWNRIVERPAWNMOVECNT] = 
{
	//����
	{
		-1, 
	}, 
	//���
	{
		1, 
	}, 
};
//���ӱ�
const FSINT g_nUpRiverPawnMoveDelta[2][MAXUPRIVERPAWNMOVECNT] = 
{
	//����
	{
		-10, 10, -1, 
	},
	//���
	{
		-10, 10, 1, 
	}, 
};

// �������ͱ��ת��Ϊ�ܵ��������ͱ��[2005/10/20 22:53:40]
const FSINT g_nChessID2Chess[15] = 
{
	NOCHESS,	//NOCHESS

	KING,		//BKING
	ADVISOR,	//BADVISOR
	BISHOP,		//BBISHOP
	ROOK,		//BROOK
	KNIGHT,		//BKNIGHT
	CANNON,		//BCANNON
	PAWN,		//BPAWN

	KING,		//RKING
	ADVISOR,	//RADVISOR
	BISHOP,		//RBISHOP
	ROOK,		//RROOK
	KNIGHT,		//RKNIGHT
	CANNON,		//RCANNON
	PAWN,		//RPAWN
};

// �췽�ڶ�ƽ��Ŀ��־��� [2005/10/22 20:45:57](���Կ����ٶ�)
const FSCHAR g_szOpenGamePos[128] = 
	"fen rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w - - 0 1";
// �о־���(�����о��ٶ�)
const FSCHAR g_szMidGamePos[128] = 
	"fen 1rbak1bnr/4a4/n8/p1p1p1p1p/3R4c/P7P/2P1c1P2/NC2B2CN/9/2BAKA2R w - - 0 9 moves f0e1";
// �о־���(����ѭ��)(�ѽ�)
const FSCHAR g_szEndGamePos[64] = "fen 4k4/4a4/4P4/9/9/9/9/4B4/9/4K4 w - - 0 1";
// ��ɱ�ž�(����ɱ��)(�ѽ�)
const FSCHAR g_szCheckMatePos[64] = "fen 4kar2/4a2rn/4bc3/RN1c5/2bC5/9/4p4/9/4p4/3p1K3 w - - 0 1";
// zugzwang����(����Null Move)
const FSCHAR g_szZugzwangPos[64] = "fen 3k5/4PP3/4r4/3P5/9/9/9/9/9/5K3 w - - 0 1";
// ���Null Move�ľ���(����Null Move)(�ѽ�)
const FSCHAR g_szDetectNMPos[64] = "fen 3aka3/9/9/9/1C7/2n6/9/4C4/4K4/9 w - - 0 1";
// ��Ե�ʿ(����Null Move)(�ѽ�)
const FSCHAR g_szKnightVSSglAdvisorPos[64] = "fen 3k5/9/3a5/9/9/9/9/9/9/4K3N w - - 0 1";
// ��������(����ѭ��)(�ѽ�)
const FSCHAR g_szFishforMoonInSeaPos[64] = "fen 3k5/9/9/4C4/3r5/4R4/9/9/9/4K4 w - - 0 1";
// ����ѭ��
const FSCHAR g_szRepDetectPos[128] = "fen 4k1b2/4a4/4b3n/R5p1p/2p6/6P2/P1PC4P/9/3K5/6rc1 b - - 0 22 moves g0g1 d1d0 g1g0 d0d1 g0g1 d1d0 g1g0 d0d1 g0g1 d1d0 g1g0 d0d1";

// ǳ�찴�ȼ��ó��������
const FSINT g_nQHSchDptByLvl[QIANHONGLEVELCNT] = {6, 7, 8, 9, 10, 11, 12, 13, MAXSEARCHDEPTH};

// ����λ�ü�ֵ [2005/10/20 16:58:26]
const FSINT g_nPosValues[8][90] = 
{
	// NOCHESS
	{
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0
	},

	// King
	{
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		1, -8, -9,  0,  0,  0,  0,  0,  0,  0,
		5, -8, -9,  0,  0,  0,  0,  0,  0,  0,
		1, -8, -9,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0
	},
	// Advisor
	{
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  3,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0
	},
	// Bishop
	{
		0,  0, -2,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  3,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0, -2,  0,  0,  0,  0,  0,  0,  0
	},
	// Rook
	{
		-6,  5, -2,  4,  8,  8,  6,  6,  6,  6,
		6,   8,  8,  9, 12, 11, 13,  8, 12,  8,
		4,   6,  4,  4, 12, 11, 13,  7,  9,  7,
		12, 12, 12, 12, 14, 14, 16, 14, 16, 13,
		0,   0, 12, 14, 15, 15, 16, 16, 33, 14,
		12, 12, 12, 12, 14, 14, 16, 14, 16, 13,
		4,   6,  4,  4, 12, 11, 13,  7,  9,  7,
		6,   8,  8,  9, 12, 11, 13,  8, 12,  8,
		-6,  5, -2,  4,  8,  8,  6,  6,  6,  6
	},
	// Knight
	{
		0,  -3,  5,  4,  2,  2,  5,  4,  2,  2,
		-3,  2,  4,  6, 10, 12, 20, 10,  8,  2,
		2,   4,  6, 10, 13, 11, 12, 11, 15,  2,
		0,   5,  7,  7, 14, 15, 19, 15,  9,  8,
		2, -10,  4, 10, 15, 16, 12, 11,  6,  2,
		0,  5,   7,  7, 14, 15, 19, 15,  9,  8,
		2,  4,   6, 10, 13, 11, 12, 11, 15,  2,
		-3, 2,   4,  6, 10, 12, 20, 10,  8,  2,
		0, -3,   5,  4,  2,  2,  5,  4,  2,  2
	},
	// Cannon
	{
		0,  0,  1,  0, -1,  0,  0,  1,  2,  4,
		0,  1,  0,  0,  0,  0,  3,  1,  2,  4,
		1,  2,  4,  0,  3,  0,  3,  0,  0,  0,
		3,  2,  3,  0,  0,  0,  2, -5, -4, -5,
		3,  2,  5,  0,  4,  4,  4, -4, -7, -6,
		3,  2,  3,  0,  0,  0,  2, -5, -4, -5,
		1,  2,  4,  0,  3,  0,  3,  0,  0,  0,
		0,  1,  0,  0,  0,  0,  3,  1,  2,  4,
		0,  0,  1,  0, -1,  0,  0,  1,  2,  4
	},
	// Pawn
	{
		0,  0,  0, -2,  3, 10, 20, 20, 20,  0,
		0,  0,  0,  0,  0, 18, 27, 30, 30,  0,
		0,  0,  0, -2,  4, 22, 30, 45, 50,  0,
		0,  0,  0,  0,  0, 35, 40, 55, 65,  2,
		0,  0,  0,  6,  6, 40, 42, 55, 70,  4,
		0,  0,  0,  0,  0, 35, 40, 55, 65,  2,
		0,  0,  0, -2,  4, 22, 30, 45, 50,  0,
		0,  0,  0,  0,  0, 18, 27, 30, 30,  0,
		0,  0,  0, -2,  3, 10, 20, 20, 20,  0,
	}
};

// ����������
class CSearchEngine
{
public:
	CSearchEngine();
	virtual ~CSearchEngine();

	// ���ýӿ�
public:
	FSVOID RunEngine(FSCHAR *szParam);		// ��������

protected:
	//=============================�߷�����=============================
	//�������е��߷�
	FSVOID GenMoves(FSINT nDepth);
	
	FSVOID GenRookMoves   (FSINT p, FSINT nDepth);	//��
	FSVOID GenKnightMoves (FSINT p, FSINT nDepth);	//��
	FSVOID GenCannonMoves (FSINT p, FSINT nDepth);	//��
	FSVOID GenKingMoves   (FSINT p, FSINT nDepth);	//��
	FSVOID GenAdvisorMoves(FSINT p, FSINT nDepth);	//ʿ
	FSVOID GenBishopMoves (FSINT p, FSINT nDepth);	//��
	FSVOID GenPawnMoves   (FSINT p, FSINT nDepth);	//��

	// [2006/1/3 20:07:08]
//	FSINT GenRookMoves   (FSINT p, CHESSMOVE* pMoveList);	//��
//	FSINT GenKnightMoves (FSINT p, CHESSMOVE* pMoveList);	//��
//	FSINT GenCannonMoves (FSINT p, CHESSMOVE* pMoveList);	//��
//	FSINT GenKingMoves   (FSINT p, CHESSMOVE* pMoveList);	//��
//	FSINT GenAdvisorMoves(FSINT p, CHESSMOVE* pMoveList);	//ʿ
//	FSINT GenBishopMoves (FSINT p, CHESSMOVE* pMoveList);	//��
//	FSINT GenPawnMoves   (FSINT p, CHESSMOVE* pMoveList);	//��
//
//	// [2006/1/4 21:10:52]
//	FSINT GenRookMoves   (FSINT p);	//��
//	FSINT GenKnightMoves (FSINT p);	//��
//	FSINT GenCannonMoves (FSINT p);	//��
//	FSINT GenKingMoves   (FSINT p);	//��
//	FSINT GenAdvisorMoves(FSINT p);	//ʿ
//	FSINT GenBishopMoves (FSINT p);	//��
//	FSINT GenPawnMoves   (FSINT p);	//��
	
	//�������еĳ����߷�
	FSVOID GenCapMoves(FSINT nDepth);

	FSVOID GenRookCapMoves   (FSINT p, FSINT nDepth);	//��
	FSVOID GenKnightCapMoves (FSINT p, FSINT nDepth);	//��
	FSVOID GenCannonCapMoves (FSINT p, FSINT nDepth);	//��
	FSVOID GenKingCapMoves	 (FSINT p, FSINT nDepth);	//��
	FSVOID GenAdvisorCapMoves(FSINT p, FSINT nDepth);	//ʿ
	FSVOID GenBishopCapMoves (FSINT p, FSINT nDepth);	//��
	FSVOID GenPawnCapMoves	 (FSINT p, FSINT nDepth);	//��
	
	// [2006/1/5 12:05:41]
//	FSINT GenRookCapMoves   (FSINT p);	//��
//	FSINT GenKnightCapMoves (FSINT p);	//��
//	FSINT GenCannonCapMoves (FSINT p);	//��
//	FSINT GenKingCapMoves	(FSINT p);	//��
//	FSINT GenAdvisorCapMoves(FSINT p);	//ʿ
//	FSINT GenBishopCapMoves (FSINT p);	//��
//	FSINT GenPawnCapMoves	(FSINT p);	//��

	//�������еĲ������߷�
	FSVOID GenNoCapMoves(FSINT nDepth);

	FSVOID GenRookNoCapMoves   (FSINT p, FSINT nDepth);	//��
	FSVOID GenKnightNoCapMoves (FSINT p, FSINT nDepth);	//��
	FSVOID GenCannonNoCapMoves (FSINT p, FSINT nDepth);	//��
	FSVOID GenKingNoCapMoves   (FSINT p, FSINT nDepth);	//��
	FSVOID GenAdvisorNoCapMoves(FSINT p, FSINT nDepth);	//ʿ
	FSVOID GenBishopNoCapMoves (FSINT p, FSINT nDepth);	//��
	FSVOID GenPawnNoCapMoves   (FSINT p, FSINT nDepth);	//��
	
	//�߷�Ԥ����
	FSVOID GenPreMoves();

	FSVOID GenRookPreMoves   ();	//��
	FSVOID GenKnightPreMoves ();	//��
    FSVOID GenCannonPreMoves ();	//��
	FSVOID GenKingPreMoves   ();	//��
	FSVOID GenAdvisorPreMoves();	//ʿ
	FSVOID GenBishopPreMoves ();	//��
	FSVOID GenPawnPreMoves   ();	//��

	//����Ԥ�߷�
//	FSBOOL LoadKnightPreMove();		//��

	//����Ԥ�߷�
//	FSVOID SaveKnightPreMoves();	//��

	FSVOID InitPreMoveListPointer();	//��ʼ���߷�Ԥ�������������е�ָ��

	FSBOOL CanMemMeet();	//�ڴ��Ƿ�����Ҫ��
	FSVOID InitPointer();	//��ʼ��ָ��
	FSVOID Initial();		//��ʼ��

	//=============================��������=============================
	FSINT Evaluate();	//��ֵ

	//����Լ�ֵ
	FSINT GetFlexValue();
	FSINT GetFlexValue(FSBOOL bIsRed);

	FSINT GetRookFlexValue  (FSINT p);		//��
	FSINT GetKnightFlexValue(FSINT p);		//��
	FSINT GetCannonFlexValue(FSINT p);		//��

	FSBOOL IsEmptyHeartCannon(FSBOOL bIsRed);	// �Ƿ��п�����[2006/1/1 11:38:37]

	//=============================��������=============================
	//�����ж�
	FSBOOL IsChecked(BOOL bIsRed);

	//����Ԥ�ж�
	FSVOID GenPreCheck();

	FSVOID GenRookPreCheck();	//��
	FSVOID GenKnightPreCheck();	//��
	FSVOID GenCannonPreCheck();	//��
	FSVOID GenPawnPreCheck();	//��

	FSBOOL IsRep(FSINT nCanRep);		//ѭ�����:�Ƿ�������,��׽
	FSBOOL IsLegalMove(CHESSMOVE cm);	//�߷�cm�Ƿ�Ϸ�:����cm���Ƿ������ͽ�,����,��׽
	FSBOOL IsValidMove(CHESSMOVE cm);	//�߷�cm�Ƿ�����Ч���߷�

	FSVOID IDSearchInit();
	FSVOID SearchInit();
	FSVOID SearchStop();
#ifdef _WIN32
	DWORD WINAPI SearchMain();
#endif
	FSVOID SearchGo(); //��������
	FSINT QuiesSearch(FSINT nDepth, FSINT alpha, FSINT beta);	//ƽ������
	FSINT Search(FSINT nDepth, FSINT nSearchDepth, FSINT alpha, FSINT beta, KILLERMOVETAB& killerMoveTab);	
																//nDepthΪ��ǰ��������
//	FSINT SearchTest(FSINT nDepth, FSINT nSearchDepth, FSINT alpha, FSINT beta, KILLERMOVETAB& killerMoveTab);

	FSVOID MakeMove(CHESSMOVE& cm, FSINT& nToChessID, FSINT& nToPieceID);
	FSVOID UnMakeMove(const CHESSMOVE& cm, FSINT nToChessID, FSINT nToPieceID);

	FSVOID UnMakeNullMove();
	FSVOID MakeNullMove();

	FSULONG Rand32();	//����32λ�������
	FSUINT64 Rand64();	//����64λ�������

	//�����߷���Ч��Ԥ�ж�����
	FSVOID GenPreValidMove();

	FSVOID GenRookPreValidMove();		//��
//	FSVOID GenKnightPreValidMove();		//��
	FSVOID GenCannonPreValidMove();		//��
//	FSVOID GenKingPreValidMove();		//��
//	FSVOID GenAdvisorPreValidMove();	//ʿ
//	FSVOID GenBishopPreValidMove();		//��
//	FSVOID GenPawnPreValidMove();		//��

	FSINT ProbeHash(FSINT nDepth, FSINT nSearchDepth, FSINT alpha, FSINT beta, CHESSMOVE& cmBestMove);	
																//̽��hash��
	FSVOID RecordHash(FSINT nDepth, FSINT nSearchDepth, FSINT nEval, FSINT nHashFlag, CHESSMOVE cmBestMove);
																//¼��hash��

	FSINT ProbeQSHash(FSINT alpha, FSINT beta);
	FSVOID RecordQSHash(FSINT nEal, FSINT nHashFlag, CHESSMOVE cmBestMove);

//	FSVOID SetBestMove();
	FSBOOL IsGoodCapMove(const CHESSMOVE& cm);		//�߷�cm�Ƿ�Ϊ�õĳ����߷�

	FSVOID ChangeSide();	//�ı����ӷ�

	FSVOID GetPVLineFromHash(FSINT nSearchDepth);	//��hash����ȡPV

	BOOL IsSpecialPos(FSCHAR *& szStr);	//�Ƿ�Ϊ����ľ���,�����,����֮

	FSBOOL	IsDraw();				//�Ƿ�Ϊ����
	FSINT	DrawValue();			//�����ֵ
	FSINT	RepValue(FSINT nRep);	//�ظ���ֵ
	
	FSINT LegalMoves(FSINT& nLegalNum); //Ψһ�ĺϷ��߷�

	FSUINT64 GetCBCheckSum(const FSINT nCBChessID[], FSBOOL bSide);		//�õ����̵�У���

	CHESSMOVE LoadBookFEN(FSCHAR *szFEN);	//���ؿ��ֿ��е�FEN��
	CHESSMOVE GetMoveFromBook();			//�ӿ��ֿ��еõ��߷�

	FSVOID PrintBetterPV(FSINT nDepth, FSINT nSearchDepth, FSINT nEval, FSINT nHashFlag, CHESSMOVE& cmBetterMove);
	FSVOID ClearHash();
	FSVOID SetChessStyle(FSCHAR *szStyle);

	FSINT GetHash(HASHITEM **pHashTab, FSINT nDepth, FSINT nSearchDepth, FSINT alpha, FSINT beta, CHESSMOVE& cmBestMove);

	FSBOOL IsCheckMove(CHESSMOVE& cm);	//�Ƿ�Ϊ�����߷�

	//=============================UCCIЭ��=============================
	FSVOID UCCIProc();	//UCCIЭ�鴦��

	UCCICmd GetUCCICmd(FSCHAR * &szCmd);	//��������szCmd

	FSBOOL LoadFEN(FSCHAR *szFEN);		//���ַ���szFEN�з������ǰ����

	FSVOID ChessMove2UCCIStr(const CHESSMOVE& cm, FSCHAR *szMove);		//���߷�cmת��Ϊ�ַ���szMove
	FSBOOL UCCIStr2ChessMove(FSCHAR *szMove, CHESSMOVE& cm);	//���ַ���szMoveת��Ϊ�߷�cm

	FSVOID UCCIStopThink();
#ifdef _WIN32
	FSVOID CALLBACK ThinkTimeOut(FSUINT uTimerID, FSUINT uMsg, FSDWORD dwUser, FSDWORD dw1, FSDWORD dw2);
#endif
	FSVOID UCCIBusy();

	FSVOID KillThread(HANDLE h);	//�����߳�

	FSVOID PrintEgnStatus();	// ��ӡ����״̬

	const char* GetSpecialFEN(char *str); // �õ���������FEN��

	//=============================aheiЭ��=============================
	FSVOID AheiPProc();	//aheiЭ�鴦��

	//=============================ǳ��Э��=============================
	FSVOID QianHongPProc(FSCHAR *szParam);	//ǳ��Э�鴦��
	QianHongCmd GetQianHongCmd(FSCHAR *& szQianHongCmd);
	FSVOID PrintQHInfo();	//���ǳ����Ϣ

	FSBOOL QHStr2ChessMove(FSCHAR *szICCS, CHESSMOVE& cm);			//ǳ���߷��ַ���ת��Ϊ�߷�
	FSVOID ChessMove2QHStr(const CHESSMOVE& cm, FSCHAR *szICCS);	//�߷�ת��Ϊǳ���߷��ַ���

	FSVOID QHStopThink();

	FSVOID CBChessID2FEN(FSCHAR *szFEN);	//ǳ��Э��������õ�

	template<class T> FSVOID ClearStack(T s);	//���ջ,ǳ��Э��������õ�

	//=============================��������=============================
	FSVOID InitVar();	//��ʼ������

	FSBOOL IsInBoard(FSINT i,FSINT j);	//(i,j)�Ƿ���������
	FSBOOL IsInBoard(FSINT p);			//p�Ƿ���������

	FSBOOL IsInRBoard(FSINT p);	//p�Ƿ��ں췽������
	FSBOOL IsInBBoard(FSINT p);	//p�Ƿ��ںڷ�������

	FSBOOL IsInRBoard(FSINT i,FSINT j);	//(i,j)�Ƿ��ں췽������
	FSBOOL IsInBBoard(FSINT i,FSINT j);	//(i,j)�Ƿ��ںڷ�������

	FSBOOL IsInCity(FSINT i,FSINT j);	//(i,j)�Ƿ��ھŹ���
	FSBOOL IsInCity(FSINT p);			//p�Ƿ��ھŹ���

	EgnProtocol GetEgnP(FSCHAR *szStr);	//�õ�����Э��

	FSVOID AllocateMem();			//�����ڴ�
	FSVOID FreeMem();				//�ͷ��ڴ�
	FSVOID AllocateMemErrProc();	//�����ڴ������
	FSVOID FreeMemErrProc();		//�ͷ��ڴ������

	FSVOID GetVersion(FSCHAR *szVersion) const;	//�õ�������汾�汾��

	FSCHAR * JumpNoMatterChar(FSCHAR *szStr);	//����szStrǰ��Ŀո�,Tab
	FSCHAR * JumpDigit(FSCHAR *szStr);			//����szStrǰ�������

	FSVOID ExitEgn();	//�˳�����

	FSBOOL IsValidPosition();

	FSVOID PrintPVLine();	//���PVLine

	FSVOID ChessMove2Str(const CHESSMOVE& cm, FSCHAR *szMove);
	FSBOOL Str2ChessMove(FSCHAR *szMoveStr, CHESSMOVE& cm);

	FSVOID UDTransform(TXTBOOKRECORD& txtBookRecord);	// �����̽������¶Գ�ת��,��:������ڷ�
	FSVOID LRTransform(TXTBOOKRECORD& txtBookRecord);	// �����̽������ҶԳ�ת��

//#ifdef _WIN32
//	FSVOID CALLBACK DispTime(FSUINT uTimerID, FSUINT uMsg, FSDWORD dwUser, FSDWORD dw1, FSDWORD dw2);
//#endif

	int SelBestMove(vector<float> vectorScore); //ѡ��õ��߷�

protected:
	//=============================�߷�����=============================
	//�߷�����
	FSINT m_nMovesCnt;	//��ĳ������߷�ʱ�����߷������в�����߷���

	CHESSMOVE m_MoveList[MAXSEARCHDEPTH][MAXMOVECNTCAPAICITY];	//�߷�����,[����][ÿһ����߷����]

	//�߷�Ԥ����
	//�߷�Ԥ��������
	FEATCHAR m_nFileRankAroundCheckSum[10][1024];	//m_nFileRankAroundCheckSum[����λ��][λ�л�λ��],
													//m_nFileRankAroundCheckSum[i][k]��ʾ���iλ��
													//��Ϊ1��λ��Ӧ��У���,����ȡֵֻ��4�����0,1,2,3
													//��:m_nFileRankAroundCheckSum[1][10b]=0,
													//m_nFileRankAroundCheckSum[1][11b]=1,
													//m_nFileRankAroundCheckSum[1][110b]=2,
													//m_nFileRankAroundCheckSum[1][111b]=3
	FEATCHAR m_nFileRankAroundCheckSum2[10][1024];	//m_nFileRankAroundCheckSum2[����λ��][λ�л�λ��],
													//m_nFileRankAroundCheckSum2[i][k]��ʾ���iλ
													//���Ϊ2��λ��Ӧ��У���,����ȡֵֻ��4�����0,1,2,3
													//��:m_nFileRankAroundCheckSum2[1][100b]=0,
													//m_nFileRankAroundCheckSum2[1][101b]=1,
													//m_nFileRankAroundCheckSum2[1][10100b]=2,
													//m_nFileRankAroundCheckSum2[1][10101b]=3

	FSINT m_nAroundCheckSum[4][4];	//��������λ�Ķ��������ϳ�λһ��4λ�Ķ�������,
									//��һ��������Ϊ�Ͷ�λ,��:[01][00]=0001

    //��
	//TODO:bug�׷���:����ά����ȷ��
	PRECHESSMOVE (* m_pcmRookFilePreMoves)[ROOKFILECHECKSUMMAXCNT][4];
															//[91][ROOKFILECHECKSUMMAXCNT][4]
															//�������߷�Ԥ��������:
															//[����λ��][λ��У�����][�Է���λ��У�����]
	PRECHESSMOVE (* m_pcmRookRankPreMoves)[ROOKRANKCHECKSUMMAXCNT][4];	
															//[91][ROOKRANKCHECKSUMMAXCNT][4]
															//�������߷�Ԥ��������:
															//[����λ��][λ��У�����][�Է���λ��У�����]
	PRECHESSMOVE (* m_pcmRookFileCapPreMoves)[ROOKFILECHECKSUMMAXCNT][4];
															//[91][ROOKFILECHECKSUMMAXCNT][4]
															//�����г����߷�Ԥ��������:
															//[����λ��][λ��У�����][�Է���λ��У�����]
	PRECHESSMOVE (* m_pcmRookRankCapPreMoves)[ROOKRANKCHECKSUMMAXCNT][4];	
															//[91][ROOKRANKCHECKSUMMAXCNT][4]
															//�����г����߷�Ԥ��������:
															//[����λ��][λ��У�����][�Է���λ��У�����]
	PRECHESSMOVE (* m_pcmRookFileNoCapPreMoves)[ROOKFILECHECKSUMMAXCNT];
															//[91][ROOKFILECHECKSUMMAXCNT]
															//�����в������߷�Ԥ��������:
															//[����λ��][λ��У�����]
	PRECHESSMOVE (* m_pcmRookRankNoCapPreMoves)[ROOKRANKCHECKSUMMAXCNT];	
															//[91][ROOKRANKCHECKSUMMAXCNT]
															//�����в������߷�Ԥ��������:
															//[����λ��][λ��У�����]

	FEATCHAR m_nRookFileRankCheckNum[10][1024];	//����λ�к�λ�е�У�����:[�����л����е�λ��][λ�л�λ��]
												//ȡֵ��ΧΪ:0~29

	FSINT m_nRookFileRankCheckSum[10][ROOKFRCHECKSUMMAXCNT];	//����λ�к�λ�е�У���:
																//[�����л����е�λ��][У�����]
																//ȡֵ��ΧΪ:0~1023

	FEATCHAR (*m_nRookFoeFileRankCheckNum)[ROOKFRCHECKSUMMAXCNT][1024];
														//[10][ROOKFRCHECKSUMMAXCNT][1024]
														//�Է���λ�к�λ�ж�Ӧ��У�����:
														//[����λ�л�λ���е�λ��][����У�����][�Է��ӵ�λ�л�λ��]
														//ȡֵ��ΧΪ:0,1,2,3
	FEATSHORT m_nRookFoeFileRankCheckSum[10][ROOKFRCHECKSUMMAXCNT][4];
														//���ĶԷ���λ�л�λ�е�У���:
														//[����λ�л�λ���е�λ��][����У�����][�Է��ӵ�λ�л�λ�е�У�����]
														//ȡֵ��ΧΪ0~1023

	//��
	PRECHESSMOVE (*m_pcmKnightNoCapPreMoves)[256][16];		//[91][256][16]��Ĳ������߷�Ԥ��������:
															//[���λ��][��Ŀ��ߵ��У���][���ȵĵ�У���]
	PRECHESSMOVE (*m_pcmKnightCapPreMoves)[256][16];		//[91][256][16]��ĳ����߷�Ԥ��������:
															//[���λ��][��Ŀ��ߵ�Ϊ�Է��ӵ�У���][���ȵ�У���]

    FEATUCHAR m_nKnightAroundCheckSum[16][16];	//�����Χ�����У���:
												//[����������е�У�����][����������е�У�����]
												//ȡֵ��ΧΪ:0~255

	FEATBOOL m_bIsKnightInvalidArroundCheckSum[90][256];	//�Ƿ�Ϊ�����ЧУ���
															//[���λ��][�����Χ���У���]
															//ȡֵ��ΧΪ:0,1
	FEATBOOL m_bIsKnightInvalidLegCheckSum[90][256][16];	//�Ƿ�Ϊ���ȵ���ЧУ���
															//[���λ��][�����Χ���У���][����У���]
															//ȡֵ��ΧΪ:0,1

	//��
	PRECHESSMOVE (*m_pcmCannonFilePreMoves)[CANNONFILECHECKSUMMAXCNT][4];
															//[91][CANNONFILECHECKSUMMAXCNT][4]
															//�ڵ����߷�Ԥ��������:
															//[�ڵ�λ��][λ��У�����][�Է���λ��У�����]
    PRECHESSMOVE (*m_pcmCannonRankPreMoves)[CANNONRANKCHECKSUMMAXCNT][4];
															//[91][CANNONRANKCHECKSUMMAXCNT][4]
															//�ڵ����߷�Ԥ��������:
															//[�ڵ�λ��][λ��У�����][�Է���λ��У�����]
	PRECHESSMOVE (*m_pcmCannonFileCapPreMoves)[CANNONFILECHECKSUMMAXCNT][4];
															//[91][CANNONFILECHECKSUMMAXCNT][4]
															//�ڵ��г����߷�Ԥ��������:
															//[�ڵ�λ��][λ��У�����][�Է���λ��У�����]
    PRECHESSMOVE (*m_pcmCannonRankCapPreMoves)[CANNONRANKCHECKSUMMAXCNT][4];
															//[91][CANNONRANKCHECKSUMMAXCNT][4]
															//�ڵ��г����߷�Ԥ��������:
															//[�ڵ�λ��][λ��У�����][�Է���λ��У�����]
	PRECHESSMOVE (*m_pcmCannonFileNoCapPreMoves)[CANNONFILECHECKSUMMAXCNT];
															//[91][CANNONFILECHECKSUMMAXCNT]
															//�ڵ��г����߷�Ԥ��������:
															//[�ڵ�λ��][λ��У�����]
    PRECHESSMOVE (*m_pcmCannonRankNoCapPreMoves)[CANNONRANKCHECKSUMMAXCNT];
															//[91][CANNONRANKCHECKSUMMAXCNT]
															//�ڵ��г����߷�Ԥ��������:
															//[�ڵ�λ��][λ��У�����]

	FEATUCHAR m_nCannonFileRankCheckNum[10][1024];			//�ڵ�λ�к�λ�е�У�����:
															//[�����л����е�λ��][λ�л�λ��]
															//ȡֵ��ΧΪ:0~175
	FEATSHORT m_nCannonFileRankCheckSum[10][CANNONFRCHECKSUMMAXCNT];
															//�ڵ�λ�к�λ�е�У����Ŷ�Ӧ��λ�к�λ��:
															//[�����л����е�λ��][У�����]
															//ȡֵ��ΧΪ:0~1023
	FEATCHAR (*m_nCannonFoeFileRankCheckNum)[CANNONFRCHECKSUMMAXCNT][1024];
															//[10][CANNONFRCHECKSUMMAXCNT][1024]
															//�Է���λ�к�λ�ж�Ӧ��У�����,
															//[����λ�л�λ���е�λ��][�ڵ�У�����][�Է��ӵ�λ�л�λ��]
															//ȡֵ��ΧΪ:0,1,2,3
	FEATSHORT m_nCannonFoeFileRankCheckSum[10][CANNONFRCHECKSUMMAXCNT][4];
															////�ڵĶԷ���λ�л�λ�е�У���������Ӧ��λ�л�λ��
															//[����λ�л�λ���е�λ��][�ڵ�У�����][�Է��ӵ�λ�л�λ�е�У�����]
															//ȡֵ��ΧΪ:0~1023
	//��
	PRECHESSMOVE (*m_pcmKingPreMoves)[16][16];	//[18][16][16]�����߷�Ԥ��������:
												//[����λ��][����Χ�����У�����][����Χ�Է��������У�����]
	PRECHESSMOVE m_pcmKingNoCapPreMoves[18][16];//[18][16]���Ĳ������߷�Ԥ��������:
												//[����λ��][����Χ�����У�����]
	PRECHESSMOVE m_pcmKingCapPreMoves[18][16];	//[18][16]���ĳ����߷�Ԥ��������:
												//[����λ��][����Χ�Է��������У�����]
	
	FSINT m_nKingPosCheckNum[90];	//����λ��У�����:[����λ��](ע��:���ش���,���Բ���Ҫ����Ϊ91)
									//ȡֵ��ΧΪ:0~17
    FSINT m_nKingPosCheckSum[18];	//����λ��У���:[����λ�õ�У�����],ȡֵ��ΧΪ:0~89

    //ʿ
    PRECHESSMOVE (*m_pcmAdvisorPreMoves)[16][16];	//[11][16][16]ʿ���߷�Ԥ��������:
													//[ʿ��λ�õ�У���][ʿ����Χ�����λ���̵�У���][ʿ����Χ�Է��ӵ�λ���̵�У���]
	PRECHESSMOVE m_pcmAdvisorNoCapPreMoves[11][16];	//[11][16]ʿ�Ĳ������߷�Ԥ��������:
													//[ʿ��λ�õ�У���][ʿ����Χ�����У���]
	PRECHESSMOVE m_pcmAdvisorCapPreMoves[11][16];	//[11][16]ʿ�ĳ����߷�Ԥ��������:
													//[ʿ��λ�õ�У���][ʿ����ΧΪ�Է��������У���]

	FSINT m_nAdvisorPosCheckNum[91];	//ʿ��λ��У�����:[ʿ��λ��],ȡֵ��ΧΪ:0~10
    FSINT m_nAdvisorPosCheckSum[11];	//ʿ��λ��У���:[ʿ��λ�õ�У�����],ȡֵ��ΧΪ:0~89

    //��
    PRECHESSMOVE (*m_pcmBishopPreMoves)[16][16][16];	//[15][16][16][16]����߷�Ԥ��������:
														//[���λ�õ�У�����][���۵�У�����][��Ŀ��ߵ��У�����][��Ŀ��ߵ�Ϊ�Է��ӵ�У�����]
	PRECHESSMOVE (*m_pcmBishopNoCapPreMoves)[16][16];	//[15][16][16]��ĳ����߷�Ԥ��������:
														//[���λ�õ�У�����][���۵�У�����][��Ŀ��ߵ��У�����]
	PRECHESSMOVE (*m_pcmBishopCapPreMoves)[16][16];		//[15][16][16]��ĳ����߷�Ԥ��������:
														//[���λ�õ�У�����][���۵�У�����][��Ŀ��ߵ�Ϊ�Է��ӵ�У�����]

    FSINT m_nBishopPosCheckNum[91];	//���λ��У�����:[���λ��],ȡֵ��ΧΪ:0~14
    FSINT m_nBishopPosCheckSum[15];	//���λ��У���:[���λ�õ�У�����],ȡֵ��ΧΪ:0~89

	//��
	PRECHESSMOVE (*m_PawnPreMoves)[56][8][8];	//[2][56][8][8]�����߷�Ԥ��������:
												//[������][����λ�õ�У�����][���Ŀ��ߵ��У���][���Ŀ��ߵ�Ϊ�Է��ӵ�У���]
	PRECHESSMOVE (*m_PawnNoCapPreMoves)[56][8];	//[2][56][8]���ĳ����߷�Ԥ��������:
												//[������][����λ�õ�У�����][���Ŀ��ߵ��У���]
	PRECHESSMOVE (*m_PawnCapPreMoves)[56][8];	//[2][56][8]���ĳ����߷�Ԥ��������:
												//[������][����λ�õ�У�����][���Ŀ��ߵ�Ϊ�Է��ӵ�У���]

	FEATSHORT m_nPawnRankAroundCheckSum[2][10][1024];	//�������Χ�����У���:
														//[������][�����ڵ���][�����ڵ�λ��]
														///ȡֵ��ΧΪ:0~1

	FSINT m_nPawnPosCheckNum[2][91];	//���λ��У�����:[������][���λ��],ȡֵ��ΧΪ:0~55
    FSINT m_nPawnPosCheckSum[2][56];	//���λ��У���:[������][���λ�õ�У�����],ȡֵ��ΧΪ:0~89

	//˫�������ж�����
	FEATBOOL (*m_bIsTwoKingMeet)[10][1024];	//[10][10][1024]
											//[�ڽ�λ��У�����][��˧λ��У�����][�ڽ������е�λ��],
											//=0û������,��֮����
											//[x][y][z]:x��y����9��ʱ��,������TRUE
											//ȡֵ��ΧΪ:0,1

	FSINT m_nBRKingPosCheckNum[2][MAXCHESSGRIDNUM + 1];	//����λ��У�����:[����λ��],[0]:��,[1]:��
														//(ע��:���ش���,���Բ���Ҫ����Ϊ91)
														//����ֵ��ΧΪ:0~8

    FSINT m_nBRKingPosCheckSum[2][MAXXSCALENUM];	//����λ��У���:[�η�][����λ�õ�У�����],
													//[0][]:��,[1][]:��
													//����ֵ��ΧΪ:0~89

	//����Ԥ�ж�����
	//��
	FEATBOOL (*m_bIsRookFilePreCheck)[91][9][512];	//[2][91][9][512]���н����ж�
													//[�η���][����λ��][�Է�����λ�õ�У�����][������λ�õ�λ��]
													//ȡֵ��ΧΪ:0,1
	FEATBOOL (*m_bIsRookRankPreCheck)[91][9][1024];	//[2][91][9][1024]���н����ж�
													//[�η���][����λ��][�Է�����λ�õ�У�����][������λ�õ�λ��]
													//ȡֵ��ΧΪ:0,1

	//��
	FEATBOOL (*m_bIsKnightPreCheck)[91][9][16];	//[2][91][9][16]�����ж�
													//[�η���][���λ��][�Է�����λ�õ�У�����][����У���]
													//ȡֵ��ΧΪ:0,1

	//��
	FEATBOOL (*m_bIsCannonFilePreCheck)[91][9][512];	//[2][91][9][512]���н����ж�
														//[�η���][�ڵ�λ��][�Է�����λ�õ�У�����][������λ�õ�λ��]
														//ȡֵ��ΧΪ:0,1
	FEATBOOL (*m_bIsCannonRankPreCheck)[91][9][1024];	//[2][91][9][1024]���н����ж�
														//[�η���][�ڵ�λ��][�Է�����λ�õ�У�����][������λ�õ�λ��]
														//ȡֵ��ΧΪ:0,1
	
	//��
	FEATBOOL (*m_bIsPawnPreCheck)[56][9];	//[2][55][9]�������ж�
											//[�η���][����λ��][�Է�����λ�õ�У�����]
											//ȡֵ��ΧΪ:0,1

	CHESSMOVE m_InvalidMovePointer;	//��Ч���߷�ָ��,�����߷��п����߷�ʱ�õ�

	//=============================��������=============================
	FSINT m_nHave[2];		//m_nHave[0] = 0xFFFF, m_nHave[1] = 0
	FSINT m_nSign[2];		//m_nSign[0] = 1, m_nSign[1] = -1	

	FSINT m_nRookFlexValue[17];		//������Լ�ֵ:m_nRookFlexValue[���Ĳ������߷�����]
	FSINT m_nKnightFlexValue[8];	//������Լ�ֵ:m_nKnightFlexValue[��Ĳ������߷�����]
	FSINT m_nCannonFlexValue[17];	//������Լ�ֵ:m_nCannonFlexValue[�ڵĲ������߷�����]

	FSINT m_nEmptyHeartCannonValue[2]; //�����ڵļ�ֵ

	FSINT m_nKnightBasicValue[3];	//��Ļ�����ֵ
	FSINT m_nCannonBasicValue[3];	//�ڵĻ�����ֵ
	FSINT m_nPawnBasicValue[3];		//���Ļ�����ֵ

	//=============================��������=============================
	CHESSMOVE m_Moves_NoEat[MAXMOVESCNT_NOEAT];		//˫��û�г��ӵ��߷�,�жϳ�����׽ʱ��
	FSINT m_nNoEatMoveCnt_BS;						//����ǰ˫��û�г��ӵ��߷���
	FSINT m_nNoEatMoveCnt_IS;						//������û�г����߷����߷���
	FSINT m_nNoEatMoveCnt_IS_Backup;				//������û�г����߷����߷����ı���
	FSUINT64 m_nHistoryCBCheckSum_NoEat[MAXSEARCHDEPTH + MAXMOVESCNT_NOEAT];

	FSULONG m_nHashKey32[15][9][10];	//32λ�������,��������32λ��ϣֵ
	FSUINT64 m_nHashKey64[15][9][10];	//64λ�������,��������64λ��ϣֵ

	FSULONG m_nHashKeyPlayer32;		//����ChangeSide
	FSUINT64 m_nHashKeyPlayer64;	//����ChangeSide

	FSULONG m_nHashTabIndex;	//hash������:���ڽ��������hash��
	FSUINT64 m_nCBCheckSum;		//���̾����64λУ���

	HASHITEM *m_pHashTab[2];		//hash��ͷָ��
	HASHITEM *m_pHashTabByDepth[2];	//����滻���Ե�hash��ͷָ��
	QSHASHITEM *m_pQSHashTab[2];	//qshash��ͷָ��
	FSINT m_nHashTabMask;			//hash������λ,hash���СΪ���1

	FSINT m_nRepHashTabMask;	//�ظ����������õ�,����λ
	FSINT *m_nRepHashTab;		//�ظ��������õ�����hash��

	SCORETYPE m_nHistoryScore[2][MAXCHESSGRIDNUM + 1][MAXCHESSGRIDNUM + 1];	//�߷�����ʷ�÷ֱ�

	FSUINT m_nMoveSearchedCnt[MAXCHESSGRIDNUM][MAXCHESSGRIDNUM];	//�߷��������Ĵ���
	FSUINT m_nMoveCutoffCnt[MAXCHESSGRIDNUM][MAXCHESSGRIDNUM];		//�߷����ضϵĴ���

	FSINT m_nDRF_NM;	//Null Move���õ���ȼ�������(DepthReductionFactor)

	FSINT m_nHistoryShift; // ��ʷ��ü��ı�����ֵ��1 = 50%, 2 = 25%, 3 = 12.5%, �ȵ�

	CHESSMOVE m_cmKiller[MAXSEARCHDEPTH][MAXKILLERMOVECNT];	//ɱ���߷�
	FSINT m_nKillerMoveCnt[MAXSEARCHDEPTH];					//ÿ������ɱ���߷�����
	KILLERMOVETAB m_KillerMoveTab;							//ɱ���߷���

	PVLINE m_PVLineThink;	//�����е�PV·��
	PVLINE m_PVLine;		//�����õ�PV·��

	FSINT m_nTimeThinkLimit;	//���Ƶ�˼��ʱ��
	FSINT m_nTimeStartThink;	//�տ�ʼʱ˼����ʱ��

	THINKTYPE m_ThinkType;	//˼����ʽ

	FSBOOL m_bNullMoveOn;	//�Ƿ�����Null Move

	FSBOOL m_bOpenBookOn;	//�Ƿ�ʹ�ÿ��ֿ�
	FSBOOL m_bEndBookOn;	//�Ƿ�ʹ�òоֿ�

	FSBOOL m_bOnlyMoveOn;	//��Ψһ��ʱ,ֱ������,����˼��

	FSINT m_nGetOpenBookMoveErrCnt;		//�ӿ��ֿ��м����߷��������

	FSINT m_nContemptFactor;	//��������
	CHESSSTYLE m_ChessStyle;	//������

	FSINT m_nDrawMoves;		//˫��û�г��ӵ����岽��(��غ���)
							//ͨ����ֵ�ﵽ120��Ҫ�к�(��ʮ�غ���Ȼ����)

	FSINT m_nCheck[2][CHECK + 1]; // m_nCheck[FALSE][] = [], m_nCheck[TRUE][] = CHECK [2006/1/2 19:28:38]

//	FSVOID (CSearchEngine::*m_GenMovesFun[8])(FSINT, FSINT);		//�����߷��ĺ���
//	FSINT (CSearchEngine::*m_GenMovesFun2[8])(FSINT, CHESSMOVE*);	//�����߷��ĺ���
//	FSINT (CSearchEngine::*m_GenCapMovesFun[8])(FSINT);				//���������߷��ĺ���
	
	FSINT m_nCheckExtensionPlies;	// ��������Ĳ���[2006/1/7 21:02:29]

	FSBOOL m_bIsHaveCheckMateMove;	// �Ƿ��Ѿ�������ɱ��
	FSBOOL m_bIsHaveGoodPosition;	// �����Ƿ��Ѿ�ռ���� [2006/2/24 9:22:02]

	//�߷���Ч��Ԥ�ж�����
	//��
//	FEATBOOL (*m_bIsRookFilePreValidMove)[90][512];		//[90][90][512]:[��λ��][��Ҫ�����Ŀ��λ��][�����ڵ�λ��]
//	FEATBOOL (*m_bIsRookRankPreValidMove)[90][1024];	//[90][90][1024]:[��λ��][��Ҫ�����Ŀ��λ��][�����ڵ�λ��]
//	
//	//��
//	FEATBOOL m_bIsKnightPreValidMove[90][90][16];	//[��λ��][��Ҫ�����Ŀ��λ��][�������У���]
//
//	//��
//	FEATBOOL (*m_bIsCannonFilePreValidMove)[90][512][2];	//[90][90][512][2]:[��λ��][��Ҫ�����Ŀ��λ��][�����ڵ�λ��][Ŀ��λ���Ƿ�����]
//	FEATBOOL (*m_bIsCannonRankPreValidMove)[90][1024][2];	//[90][90][1024][2]:[��λ��][��Ҫ�����Ŀ��λ��][�����ڵ�λ��][Ŀ��λ���Ƿ�����]
//
//	//��
//	FEATBOOL m_bIsKingPreValidMove[90][90];		//[90][90]:[��λ��][��Ҫ�����Ŀ��λ��][�����ڵ�λ��/λ��]

	//��
	FEATBOOL (*m_bIsRookPreValidMove)[10][1024];	//[10][10][1024]:[��λ��][��Ҫ�����Ŀ��λ��][�����ڵ�λ��/λ��]
	
	//��
	FEATBOOL (*m_bIsCannonPreValidMove)[10][1024][2];	//[10][10][1024][2]:[��λ��][��Ҫ�����Ŀ��λ��][�����ڵ�λ��/λ��][Ŀ��λ���Ƿ�����]

	//=============================UCCIЭ��=============================
	FSBOOL m_bStopThink;

#ifdef _WIN32

	HANDLE m_hThinkThread;	//˼���߳�

	CRITICAL_SECTION g_CriticalSection;	//�ٽ���

#else
#endif

	FSBOOL m_bPonder;			//�Ƿ���к�̨˼��
	FSBOOL m_bPonderHit;		//��̨˼���Ƿ�����

	FSINT m_nBouts;
	FSINT m_nCurBout;

	//=============================ǳ��Э��=============================
	FSINT m_nQHLevel;	//ǳ��ȼ�

	//ǳ��Э���л���ʱ�õ�
#ifdef _WIN32
	UNDOMOVESTACK m_UndoMoveStack;
	REDOMOVESTACK m_RedoMoveStack;
#else
#endif

#ifdef DEBUG
	FSINT m_nPiecePosBeforeAI[33];	//����AI����֮ǰ��PiecePos
#endif

	//=============================��������=============================
	UINT m_nMask[10];			//m_nMask[i]��ʾ������ʱֻ�е�iλΪ1,����Ϊ0,��:m_nMask[3]=1000b
	UINT m_nReverseMask[10];	//[i]��ʾ������ʱֻ�е�iλΪ0,����Ϊ1,��:m_nMask[3]=11111...11110111b

	FSCHAR m_szWhoN2WhoStr[2][4]; //m_szWhoN2WhoStr[0] = "��", m_szWhoN2WhoStr[1] = "��"

	FSBOOL m_bAllocateMem;	//�Ƿ��ѷ����ڴ�

	EgnProtocol m_EgnProtocol;	//��ǰʹ�õ�Э��

	//���ڼ�������ʱ��
	FSINT m_nTotalSearchTime;	//������ʱ��
	FSINT m_nSearchCnt;			//��������
	FSINT m_nMaxSearchTime;		//�������ʱ��

	CHESSMOVE m_cmNullMove;

	//�ֽڼ���
	FSINT m_nBytes[MAXSEARCHDEPTH];	//[i]:����i *sizeof(CHESSMOVE)

#ifdef DEBUG
	FSINT m_nTestTimer;		//�������õļ�����	
#endif
	
	STATEMENT_GETMAXSCHDPT(FSINT m_nMaxSearchDepth;);	//����������

	STATEMENT_GETMAXSCHDPT(FSINT m_nMaxSelSchDpt;);		//ѡ���������ﵽ��������

	//�ظ����������õ�
	CHESSMOVE m_cmFight[MAXSEARCHDEPTH + MAXMOVESCNT_NOEAT];			//˫���ߵ��߷�
	FSUINT64 m_nHistoryCBCheckSum[MAXSEARCHDEPTH + MAXMOVESCNT_NOEAT];	//�����߷�·�߼�¼�����ľ���У���

	FSINT m_nCurDepth;	//��ǰ�������

	FSCHAR m_szBookFile[MAXBUFFERLEN];	//���ֿ�

	FSCHAR m_szDir[MAXBUFFERLEN];	//��ǰĿ¼

	FSINT m_nPieceID2Bit[33];	//���ӱ��ת��Ϊbit,
								//[nPieceID]:nPieceID == INVALIDPIECEID ret 0 else ret 1

	POINT m_CGN2Pos[91];		//�����ת��Ϊλ��m_CGN2Pos[i]����(i / 10, i % 10)
	FSINT m_nPos2CGN[9][10];	//λ��ת��Ϊ�����m_nPos2CGN[i][j] = i * 10 + j

	FSINT m_nPieceID2ChessID[32];		//[���ӱ��]:�����������ͱ��
	FSINT m_nChessID2PieceID[15][5];	//[�������ͱ��][���������еı��]�������ͱ��ת��Ϊ���ӱ��

	FSINT m_nInitChessCnt[15];	//��ʼ����������Ŀ

	FSINT m_nSearchDepth;			//�������
	FSINT m_nCurSearchDepth_ID;		//��������ʱ,��ǰ���������
	
	EgnStatus m_esUCCIEgnStatus;	//����״̬

	FSINT m_nAbs[10][3];	//[i][j]:if(i - j < 0) ret 0 else ret i - j;

	FSINT m_nRook[2];		//[0] = BROOK,		[1] = RROOK
	FSINT m_nKnight[2];		//[0] = BKnight,	[1] = RKnight
	FSINT m_nCannon[2];		//[0] = BROOK,		[1] = RROOK
	FSINT m_nKing[2];		//[0] = BKing,		[1] = RKing
	FSINT m_nAdvisor[2];	//[0] = BAdvisor,	[1] = RAdvisor
	FSINT m_nBishop[2];		//[0] = BBishop,	[1] = RBishop
	FSINT m_nPawn[2];		//[0] = BPawn,		[1] = RPawn

	FSINT m_nKingID2PieceID[2];	//m_nKingID2PieceID[0] = m_nChessID2PieceID[BKING][0]
								//m_nKingID2PieceID[1] = m_nChessID2PieceID[RKING][0]

	//���ӷ��ж�����
    FSBOOL m_bIsRed_PieceID[33];	//[���ӱ��]:����1��ʾ��λ���ϵ�����Ϊ����
	FSBOOL m_bIsBlack_PieceID[33];	//[���ӱ��]:����1��ʾ��λ���ϵ�����Ϊ����
	FSBOOL m_bIsRed_ChessID[15];	//[�������ͱ��]:����1��ʾ��λ���ϵ�����Ϊ����
	FSBOOL m_bIsBlack_ChessID[15];	//[�������ͱ��]:����1��ʾ��λ���ϵ�����Ϊ����

	FSINT m_nConvertChessID[15];

#ifdef STAT

	FSUINT m_nVistNodeCnt;	//���ʵĽڵ���:���ڼ���NPS

	FSUINT m_nNonQuiesNodeCnt;	//�Ǿ�̬�����Ľڵ���
	FSUINT m_nAlphaNodeCnt;		//alpha�ڵ�
	FSUINT m_nPVNodeCnt;		//PV�ڵ�
	FSUINT m_nBetaNodeCnt;		//beta�ڵ�
	FSUINT m_nNullNodeCnt;		//���ųɹ��Ľڵ���

	FSINT m_nHashBetaNodeCnt;		//hash move�ضϴ���
	FSINT m_nCapMoveBetaNodeCnt;	//Cap Move�ضϴ���
	FSINT m_nKillerMoveBetaNodeCnt;	//Killer Move�ضϴ���
	FSINT m_nNoCapMoveBetaNodeCnt;	//NoCap Move�ضϴ���

	FSUINT m_nQuiesNodeCnt;			//��̬�����Ľڵ���
	FSUINT m_nQuiesAlphaNodeCnt;	//��̬�����е�alpha�ڵ���
	FSUINT m_nQuiesPVNodeCnt;		//��̬�����е�PV�ڵ�
	FSUINT m_nQuiesBetaNodeCnt;		//��̬�����е�beta�ڵ�
	FSUINT m_nQuiesNullNodeCnt;		//��̬�����еĿ��ųɹ��Ľڵ���

	FSUINT m_nProbeHashCnt;		//̽��hash�����
	FSUINT m_nRecordHashCnt;	//��¼hash����
	FSUINT m_nHitHashCnt;		//hash���д���
	FSUINT m_nProbeQSHashCnt;	//̽��qshash�����
	FSUINT m_nRecordQSHashCnt;	//��¼qshash����
	FSUINT m_nHitQSHashCnt;		//qshash���д���

#endif
	
	//��ǰ�������
	FSCHAR	m_szFEN[MAXBUFFERLEN];	//FEN��

	FSINT	m_nBoardValue;	//��ǰ�����
	FSINT	m_nChessAge;	//��������׶�:���во�
	FSBOOL	m_bIsRed;		//���ӷ�:0:Black,1:Red

	FSINT m_nAllChessCnt;	//��������
	FSINT m_nBigChessCnt;	//������,����������
	FSINT m_nChessCnt[15];	//��������

	FSINT m_nCBChessID[90];	//����(�����߱��:��С�Ҵ�,��С�ϴ�):[����λ��],���ظ�λ�õ��������ͱ��
	FSINT m_nCBPieceID[90];	//[����λ��]�������ӱ��,����-1��ʾ���λ��û������

	FSINT m_nPiecePos[33];	//����λ������:[���ӱ��],����90��ʾ�����ŵ������Ѿ����Ե�

	//ע��:λ�к�λ�����䵽12
	FSINT m_nBitFile[12];	//����λ��
	FSINT m_nBitRank[12];	//����λ��

	FSINT m_nBRBitFile[3][12];	//�ں���λ��:m_nBRBitFile[0]����λ��,m_nBRBitFile[x][10 or 11] = 0
								//m_nBRBitFile[2]:��Ч,MakeMove�õ�
	FSINT m_nBRBitRank[3][12];	//�ں���λ��:m_nBRBitRank[0]����λ��,m_nBRBitRank[x][9 or 10] = 0
								//m_nBRBitRank[2]:��Ч,MakeMove�õ�
};

#endif