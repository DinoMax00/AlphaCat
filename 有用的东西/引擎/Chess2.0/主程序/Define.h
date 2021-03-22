//define.h
#ifndef DEFINE_H_
#define DEFINE_H_

#pragma warning(disable:4786)//��ֹwaring��Ϊ4786��warning��Ϣ

#include <stack>
#include <vector>

//����������Ͷ���
#define CR_LONGGEN 1//������׽
#define CR_SENDGEN 2//�ͽ�
#define CR_FAIL    3//�Ѱ�
#define CR_OBEY    4//����

#define BLACKCHESS 1//�ڷ�
#define REDCHESS   2//�췽

//��ֲ�ͬ�׶ζ���
#define CA_START 1//����
#define CA_MID   2//�о�
#define CA_END   3//�о�

//��ȷ�ʽDepthSort
#define DS_DEFAULTSET 1
#define DS_USERDEFINE 2

#define CS_PCCHESS   1//�˻�����
#define CS_PPCHESS	 2//���˶���
#define CS_CCCHESS   3//��������
#define CS_HASHCHESS 4//���Ӷ���

#define MAX_SEARCHDEPTH 50//����������

//--------����--------
#define NOCHESS    0 //û������

#define B_KING	   1 //��˧
#define B_CAR	   2 //�ڳ�
#define B_HORSE	   3 //����
#define B_CANON	   4 //����
#define B_BISHOP   5 //��ʿ
#define B_ELEPHANT 6 //����
#define B_PAWN     7 //����
#define B_BEGIN    B_KING
#define B_END      B_PAWN

#define R_KING	   8 //�콫
#define R_CAR      9 //�쳵
#define R_HORSE    10//����
#define R_CANON    11//����
#define R_BISHOP   12//��ʿ
#define R_ELEPHANT 13//����
#define R_PAWN     14//���
#define R_BEGIN    R_KING
#define R_END      R_PAWN
//--------------------

#define IsBlack(x) (x>=B_BEGIN && x<=B_END)//�ж�ĳ�������ǲ��Ǻ�ɫ
#define IsRed(x)   (x>=R_BEGIN && x<=R_END)//�ж�ĳ�������ǲ��Ǻ�ɫ

//�ж����������ǲ���ͬɫ
#define IsSameSide(x,y) ((IsBlack(x) && IsBlack(y)) || (IsRed(x) && IsRed(y)))

#pragma pack(push,1)

//����λ��
struct CHESSMANPOS
{
	BYTE x;
	BYTE y;
};

//�����߷�
struct CHESSMOVE
{
	short nChessID;  //������ʲô����
	CHESSMANPOS From;//��ʼλ��
	CHESSMANPOS To;  //�ߵ�ʲôλ��
	int iScore;      //�߷��ķ���
};

//����ʱ��Ҫ�����ݽṹ
struct UNDOMOVE
{
	CHESSMOVE cmChessMove;
	short nChessID;//���Ե�������
};

//���ֿ����ڵ㶨��
struct OPENBOOKTREENODE
{
	int iDepth;			  //�ýڵ��������е����
	int iPassCnt;         //�ýڵ��ʹ�ô���
	float fScore;		  //�ýڵ�ĵ÷�
	CHESSMOVE cmChessMove;//�ýڵ���������,���߷�
	std::vector<OPENBOOKTREENODE*> vectorChildNode;//���ӽڵ�
	OPENBOOKTREENODE *pObnParent;//���ڵ�
};

//���̺��߷�,�жϳ�����׽ʱ�õ�
struct CBANDMOVE
{
	BYTE byChessBoard[10][9];
	CHESSMOVE cm;
};

#pragma pack(push,1)

#endif
