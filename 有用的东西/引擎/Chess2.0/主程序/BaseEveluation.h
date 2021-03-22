// EveluationBase.h: interface for the CBaseEveluation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVELUATIONBASE_H__02E08D9B_918A_4860_A745_4076F632250D__INCLUDED_)
#define AFX_EVELUATIONBASE_H__02E08D9B_918A_4860_A745_4076F632250D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Define.h"
#include <vector>

//���ÿ�����ӵĻ�����ֵ����Щ��ֵ����ս�ֵĲ��Ͻ��ж������仯
const int iChessBaseValue[15][10][9]=
{
	//����
	{
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
	},

	//�ڽ�
	{
		{10000,10000,10000,10000,10000,10000,10000,10000,10000},
		{10000,10000,10000,10000,10000,10000,10000,10000,10000},
		{10000,10000,10000,10000,10000,10000,10000,10000,10000},
		{10000,10000,10000,10000,10000,10000,10000,10000,10000},
		{10000,10000,10000,10000,10000,10000,10000,10000,10000},
		{10000,10000,10000,10000,10000,10000,10000,10000,10000},
		{10000,10000,10000,10000,10000,10000,10000,10000,10000},
		{10000,10000,10000,10000,10000,10000,10000,10000,10000},
		{10000,10000,10000,10000,10000,10000,10000,10000,10000},
		{10000,10000,10000,10000,10000,10000,10000,10000,10000},
	},

	//�ڳ�
	{
		{1300,1300,1300,1300,1300,1300,1300,1300,1300},
		{1300,1300,1300,1350,1300,1350,1300,1300,1300},
		{1300,1300,1300,1300,1300,1300,1300,1300,1300},
		{1300,1300,1300,1300,1300,1300,1300,1300,1300},
		{1325,1350,1350,1350,1350,1350,1350,1350,1325},//�ӽ�
		{1325,1350,1350,1350,1350,1350,1350,1350,1325},//�ӽ�
		{1300,1350,1400,1300,1300,1300,1400,1350,1300},
		{1300,1300,1300,1300,1300,1300,1300,1300,1300},
		{1400,1300,1300,1300,1300,1300,1300,1400,1300},//ѹ��
		{1300,1300,1300,1300,1300,1300,1300,1300,1300},
	},

	//����
	{
		{520,520,540,570,540,570,540,520,520},
		{520,540,560,570,500,570,560,540,520},
		{520,560,580,590,560,590,580,560,520},
		{560,580,620,590,620,590,620,580,560},
		{540,620,650,660,670,660,650,620,540},
		{540,640,630,670,680,670,630,640,540},
		{570,720,630,710,640,710,630,720,570},
		{560,620,630,670,630,670,630,620,560},
		{540,600,670,610,580,610,670,600,540},
		{540,540,540,600,540,600,540,540,540},
	},

	//����
	{
		{600,600,600,600,600,600,600,600,600},
		{600,600,600,600,600,600,600,600,600},
		{600,600,625,600,640,600,625,600,600},
		{600,600,600,600,640,600,600,600,600},
		{550,600,550,600,550,600,550,600,550},
		{600,600,600,600,600,600,600,600,600},
		{600,600,600,600,640,600,600,600,600},
		{600,600,600,600,600,600,600,600,600},
		{600,600,600,600,600,600,600,600,600},
		{600,600,600,600,600,600,600,600,600},
	},

	//��ʿ
	{
		{0,0,0,250,0,250,0,0,0},
		{0,0,0,0,260,0,0,0,0},
		{0,0,0,240,0,240,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
	},

	//����
	{
		{0,0,250,0,0,0,250,0,0},
		{0,0,0,0,0,0,0,0,0},
		{240,0,0,0,260,0,0,0,240},
		{0,0,0,0,0,0,0,0,0},
		{0,0,240,0,0,0,240,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
	},

	//����
	{
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{100,0,100,0,200,0,100,0,100},
		{100,0,125,0,200,0,125,0,100},
		{200,200,200,200,220,200,200,200,200},
		{200,220,240,240,240,240,240,220,200},
		{220,220,240,250,250,250,240,220,220},
		{200,200,220,230,230,230,220,200,200},
		{50 ,75 ,100,100,100,100,100,75 ,50 },
	},

	//��˧
	{
		{10000,10000,10000,10000,10000,10000,10000,10000,10000},
		{10000,10000,10000,10000,10000,10000,10000,10000,10000},
		{10000,10000,10000,10000,10000,10000,10000,10000,10000},
		{10000,10000,10000,10000,10000,10000,10000,10000,10000},
		{10000,10000,10000,10000,10000,10000,10000,10000,10000},
		{10000,10000,10000,10000,10000,10000,10000,10000,10000},
		{10000,10000,10000,10000,10000,10000,10000,10000,10000},
		{10000,10000,10000,10000,10000,10000,10000,10000,10000},
		{10000,10000,10000,10000,10000,10000,10000,10000,10000},
		{10000,10000,10000,10000,10000,10000,10000,10000,10000},
	},

	//�쳵
	{
		{1300,1300,1300,1300,1300,1300,1300,1300,1300},
		{1300,1400,1300,1300,1300,1300,1300,1300,1400},//ѹ��
		{1300,1300,1300,1300,1300,1300,1300,1300,1300},
		{1300,1350,1400,1300,1300,1300,1400,1350,1300},
		{1325,1350,1350,1350,1350,1350,1350,1350,1325},//�ӽ�
		{1325,1350,1350,1350,1350,1350,1350,1350,1325},//�ӽ�
		{1300,1300,1300,1300,1300,1300,1300,1300,1300},
		{1300,1300,1300,1300,1300,1300,1300,1300,1300},
		{1300,1300,1300,1350,1300,1350,1300,1300,1300},
		{1300,1300,1300,1300,1300,1300,1300,1300,1300},
	},

	//����
	{
		{540,540,540,600,540,600,540,540,540},
		{540,600,670,610,580,610,670,600,540},
		{560,620,630,670,630,670,630,620,560},
		{570,720,630,710,640,710,630,720,570},
		{540,640,630,670,680,670,630,640,540},
		{540,620,650,660,670,660,650,620,540},
		{560,580,620,590,620,590,620,580,560},
		{520,560,580,590,560,590,580,560,520},
		{520,540,560,570,500,570,560,540,520},
		{520,520,540,570,540,570,540,520,520},
	},

	//����
	{
		{600,600,600,600,600,600,600,600,600},
		{600,600,600,600,600,600,600,600,600},
		{600,600,600,600,600,600,600,600,600},
		{600,600,600,600,640,600,600,600,600},
		{600,600,600,600,600,600,600,600,600},
		{550,600,550,600,550,600,550,600,550},
		{600,600,600,600,640,600,600,600,600},
		{600,600,625,600,640,600,625,600,600},
		{600,600,600,600,600,600,600,600,600},
		{600,600,600,600,600,600,600,600,600},
	},

	//��ʿ
	{
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,240,0,240,0,0,0},
		{0,0,0,0,260,0,0,0,0},
		{0,0,0,250,0,250,0,0,0},
	},

	//����
	{
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,240,0,0,0,240,0,0},
		{0,0,0,0,0,0,0,0,0},
		{240,0,0,0,260,0,0,0,240},
		{0,0,0,0,0,0,0,0,0},
		{0,0,250,0,0,0,250,0,0},
	},

	//���
	{
		{50 ,75 ,100,100,100,100,100,75 ,50 },
		{200,200,220,230,230,230,220,200,200},
		{220,220,240,250,250,250,240,220,220},
		{200,220,240,240,240,240,240,220,200},
		{200,200,200,200,220,200,200,200,200},
		{100,0,125,0,200,0,125,0,100},
		{100,0,100,0,200,0,100,0,100},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
	},
};

//ÿ�����ӵķ�������
const UINT uiRecoveryValue[15]=
{
	0, //����
	0, //��
	50,//�ڳ�
	25,//����
	25,//����
	25,//��ʿ
	30,//����
	0, //��
	0, //˧
	50,//�쳵
	25,//����
	25,//����
	25,//��ʿ
	30,//����
	0, //��
};

//��ǰ����������Ŀ
const int iChessCount[15]=
{
	0,//����
	1,//�ڽ�
	2,//�ڳ�
	2,//����
	2,//����
	2,//��ʿ
	2,//����
	5,//����
	1,//��˧
	2,//�쳵
	2,//����
	2,//����
	2,//��ʿ
	2,//����
	5,//���
};

//���������
const int iFlexValue[15]=
{
	0, //����
	0, //�ڽ�
	6, //�ڳ�
	12,//����
	6, //����
	1, //��ʿ
	1, //����
	15,//����
	0, //��˧
	6, //�쳵
	12,//����
	6, //����
	1, //��ʿ
	1, //����
	15,//���
};

using namespace std;

class CBaseEveluation
{
public:
	CBaseEveluation();
	virtual ~CBaseEveluation();

public:
	virtual int Eveluate(BYTE byChessBoard[10][9],BOOL bIsRedTurn);
	int GetAccessCount(){return m_nAccessCount;};  //�õ����ʽڵ���
	UINT GetAllChessCnt(){return m_uiAllChessCnt;};//�õ����е�������
	void ClearAccessCount(){m_nAccessCount=0;};    //���÷��ʽڵ���Ϊ0
	void UpdateCurChess(BYTE byChess,bool bIsRed); //��������������
	void UnUpdateCurChess(BYTE byChess,bool bIsRed);
	void InitVar(BYTE byChessBoar[][9]);//��ʼ����������ֵı���

protected:
	void AddPoint(int x,int y);//��һ��λ�ü������λ�ö���
	//�ж�λ��From�������Ƿ����ߵ�λ��To
	bool CanTouch(BYTE byChessBoard[10][9],int nFromX,int nFromY,int nToX,int nToY);
	//�о���ָ��λ�õ�������ص�����
	int GetRelatePiece(BYTE byChessBoard[10][9],int j,int i);
	void UpdateValue(BYTE byChessBoard[][9]);//����ս�ֵı仯��ʱ�������ӵļ�ֵ�����εļ�ֵ
	int Multiply(int a,int b);//�������

public:
	//------------��ʼ��ֵ�����------------
	vector<CHESSMANPOS> m_vectorCurChessPos[15];//��ǰ��������λ��ջ

	UINT m_uiCurBigChessCnt[15]; //ͳ�ƹ��Ӹ��ӵĸ���
	UINT m_uiCurLBigChessCnt[15];//��������ߵ����Ӹ���
	UINT m_uiCurRBigChessCnt[15];//��������ߵ����Ӹ���
	//--------------------------------------

protected:
	//-------���εļ�ֵ-------
	//��
	int m_iLineupB5ConnectPawn;//5����
	int m_iLineupB4ConnectPawn;//4����
	int m_iLineupB3ConnectPawn;//3����
	int m_iLineupB2ConnectPawn;//2����

	int m_iLineupBConnectElephant; //������
	int m_iLineupBConnectBishop;   //����ʿ
	int m_iLineupBConnectHorse;    //������
	int m_iLineupBScndConnectHorse;//��������
	int m_iLineupBSznConnectHorse; //��������
	int m_iLineupBConnectCanon;    //������

	//���ŵ�
	int m_iLineupBVCanonKing;			//������:����ͬ����һ���ڣ�������֮������
	int m_iLineupBVHorseCanon;			//����ڽ���������ͽ��ľ���Ϊ2
	int m_iLineupBVScndHorseCanon;		//����ڽ���������ͽ��ľ��벻Ϊ2
	int m_iLineupBVCanonCar;		    //�ڳ�����
	int m_iLineupBVCarCanonKing;		//���ڽ�
	int m_iLineupBVDblCanon;			//˫�ڽ���
	int m_iLineupBVCanon2Horese;        //�ںͽ�֮�������ӣ�������һ���ǶԷ�����
	int m_iLineupBCannonKingLieMidHorse;//�ںͽ�֮����һ�������ҽ����߶���ʿ
	int m_iLineupBVCanonCar2;			//�ںͽ�֮��������,�����м����ĳ�
	int m_iLineupBVCanon2Else;			//�ںͽ�֮�������ӣ�����û�жԷ�����

	//���ŵ�
	int m_iLineupBHCanonKing;	  //����ͬ�л�ͬ����һ���ڣ�������֮������
	int m_iLineupBHHorseCanon;	  //����ڽ���������ͽ��ľ���Ϊ2
	int m_iLineupBHScndHorseCanon;//����ڽ���������ͽ��ľ��벻Ϊ2
	int m_iLineupBHCanonCar;	  //�ڳ�����
	int m_iLineupBHDblCanon;	  //˫�ڽ���
	int m_iLineupBHCanon2Horese;  //�ںͽ�֮�������ӣ�������һ���ǶԷ�����
	int m_iLineupBHCanonCar2;	  //�ںͽ�֮��������,�����м����ĳ�
	int m_iLineupBHCanon2Else;	  //�ںͽ�֮�������ӣ�����û�жԷ�����

	int m_iLineupBCarKing;    //���ͽ���һ�л�һ��
	int m_iLineupBCarHorse;   //��������һ�л�һ��
	int m_iLineupBCarCanon;   //��������һ�л�һ��
	int m_iLineupBCarBishop;  //����ʿ��һ�л�һ��
	int m_iLineupBCarElephant;//��������һ�л�һ��
	int m_iLineupBCarPawn;    //���ͱ���һ�л�һ��

	int m_iLineupBCanonKingCar;    //��ǣ�ƽ��������г�
	int m_iLineupBCanonCarCar;     //��ǣ�Ƴ��������г�
	int m_iLineupBCanonHorseCar;   //��ǣ���������г�
	int m_iLineupBCanonElephantCar;//��ǣ���󣬽����г�
	int m_iLineupBCanonBishopCar;  //��ǣ��ʿ�������г�
	int m_iLineupBCanonPawnCar;    //��ǣ��ʿ�������г�

	int m_iLineupBLieSlotHorseCar;//�Բ���ͳ�,������Ĳ�λ:���Գ鳵����

	int m_iLineupBDblCarLoneKing;			   //˫���Թ½�
	int m_iLineupBDblCarLoneKingElephant;	   //˫���Թ½�һ��
	int m_iLineupBDblCarLoneKing2Elephant;	   //˫���Թ½�����
	int m_iLineupBDblCarLoneKingBishop;		   //˫���Թ½�һʿ
	int m_iLineupBDblCarLoneKingBishopElephant;//˫���Թ½�һʿһ��

	int m_iLineupBLieMidCar;			//���ĳ�
	int m_iLineupBLieMidCar_LoneKing;   //���ĳ��Թ½�
	int m_iLineupBLieMidCar_BigCar;		//���ĳ�,�й��ӳ�
	int m_iLineupBLieMidCar_Car;		//���ĳ�,�г�,�޹��ӳ�
	int m_iLineupBLieMidCar_CanonBishop;//���ĳ�,���ں�ʿ,�����
	int m_iLineupBLieMidCar_Horse[3];   //���ĳ�,�й�����,�Է�������ĸ���:0~2

	int m_iLineupBLieMidPawn;		     //���ı�
	int m_iLineupBLieMidPawn_LoneKing;   //���ı��Թ½�
	int m_iLineupBLieMidPawn_BigCar;	 //���ı�,�й��ӳ�
	int m_iLineupBLieMidPawn_Car;		 //���ı�,�г�,�޹��ӳ�
	int m_iLineupBLieMidPawn_CanonBishop;//���ı�,���ں�ʿ,�����
	int m_iLineupBLieMidPawn_Horse[3];   //���ı�,�й��ͱ���,�Է�������ĸ���:0~2

	int m_iLineupBLieSlotHorse;     //�Բ���
	int m_iLineupBLieMidHorse;      //������
	int m_iLineupBImpertinenceHorse;//³ç��:��δ�ж���������

	int m_iLineupBBigAngleHorse;			//�����
	int m_iLineupBBigAngleHorse_LoneKing;   //�����Թ½�
	int m_iLineupBBigAngleHorse_BigCar;		//�����,�й��ӳ�
	int m_iLineupBBigAngleHorse_Car;		//�����,�г�,�޹��ӳ�
	int m_iLineupBBigAngleHorse_CanonBishop;//�����,���ں�ʿ,�����
	int m_iLineupBBigAngleHorse_Horse[3];   //�����,�й�����,�Է�������ĸ���:0~2

	int m_iLineupBNoProtectKing;			 //����ԭλ,��һά:��,��:��,��:��
	int m_iLineupBNoProtectKing_CHC[3][3][3];//����ԭλ,��һά:�Է����ӳ��ĸ���,��:�Է�������ĸ���,��:�Է������ڵĸ���,�Է����ӳ��ĸ���:0~2,�Է�������ĸ���:0~2,�Է������ڵĸ���:0~2

	int m_iLineupBLoneKingVsCarHorse;        //�½��Գ���
	int m_iLineupBKingVsCarHorse[3][3][3][6];//�½��Գ���:����ʿ����,�Ը��Է�����һ��һ��,������޼����������Ļ�,�ǳ�Σ��
											 //��һά:����ʿ,��:������,��:�Է���,��:�Է���

	int m_iLineupBKingAttack;    //����������˨
	int m_iLineupBKingCarAttack; //����������˨,�ҳ��ͽ���һ��,�������ڵ�����Է������ڵ��еĽ����������
	int m_iLineupBScndKingAttack;//����������˨,�������ڵ�����Է������ڵ��еĽ����������

	int m_iLineupBAttackTogetherFactor;//Эͬ��ս����

	int m_iLineupBCarHorseCannonPawnAttackTogether;//�����ڱ�������ս
	int m_iLineupBCarHorseCannonAttackTogether;    //������������ս
	int m_iLineupBCarHorsePawnAttackTogether;	   //�����������ս
	int m_iLineupBCarCannonPawnAttackTogether;	   //���ڱ�������ս
	int m_iLineupBHorseCannonPawnAttackTogether;   //���ڱ�������ս

	int m_iLineupBCarHorseKing;//����
	int m_iLineupBCarPawnKing; //������

	int m_iLineupBHCarCanonKing;			 //���ڽ�:���뵤ɳ
	int m_iLineupBHCarCanonKing_CHC[2][3][3];//���ڽ�:���뵤ɳ:��һά:�������ӳ��ĸ���-1

	//��
	int m_iLineupR5ConnectPawn;//5����
	int m_iLineupR4ConnectPawn;//4����
	int m_iLineupR3ConnectPawn;//3����
	int m_iLineupR2ConnectPawn;//2����

	int m_iLineupRConnectElephant; //������
	int m_iLineupRConnectBishop;   //����ʿ
	int m_iLineupRConnectHorse;    //������
	int m_iLineupRScndConnectHorse;//��������
	int m_iLineupRSznConnectHorse; //��������
	int m_iLineupRConnectCanon;    //������

	//���ŵ�
	int m_iLineupRVCanonKing;			//����ͬ�л�ͬ����һ���ڣ�������֮������
	int m_iLineupRVHorseCanon;			//����ڽ���������ͽ��ľ���Ϊ2
	int m_iLineupRVScndHorseCanon;		//����ڽ���������ͽ��ľ��벻Ϊ2
	int m_iLineupRVCanonCar;		    //�ڳ�����
	int m_iLineupRVCarCanonKing;		//���ڽ�
	int m_iLineupRVDblCanon;			//˫�ڽ���
	int m_iLineupRVCanon2Horese;        //�ںͽ�֮�������ӣ�������һ���ǶԷ�����
	int m_iLineupRCannonKingLieMidHorse;//�ںͽ�֮����һ�������ҽ����߶���ʿ
	int m_iLineupRVCanonCar2;			//�ںͽ�֮��������,�����м����ĳ�
	int m_iLineupRVCanon2Else;			//�ںͽ�֮�������ӣ�����û�жԷ�����

	//���ŵ�
	int m_iLineupRHCanonKing;	  //����ͬ�л�ͬ����һ���ڣ�������֮������
	int m_iLineupRHHorseCanon;	  //����ڽ���������ͽ��ľ���Ϊ2
	int m_iLineupRHScndHorseCanon;//����ڽ���������ͽ��ľ��벻Ϊ2
	int m_iLineupRHCanonCar;	  //�ڳ�����
	int m_iLineupRHDblCanon;	  //˫�ڽ���
	int m_iLineupRHCanon2Horese;  //�ںͽ�֮�������ӣ�������һ���ǶԷ�����
	int m_iLineupRHCanonCar2;	  //�ںͽ�֮��������,�����м����ĳ�
	int m_iLineupRHCanon2Else;	  //�ںͽ�֮�������ӣ�����û�жԷ�����
	
	int m_iLineupRCarKing;    //���ͽ���һ�л�һ��
	int m_iLineupRCarHorse;   //��������һ�л�һ��
	int m_iLineupRCarCanon;   //��������һ�л�һ��
	int m_iLineupRCarBishop;  //����ʿ��һ�л�һ��
	int m_iLineupRCarElephant;//��������һ�л�һ��
	int m_iLineupRCarPawn;    //���ͱ���һ�л�һ��

	int m_iLineupRCanonKingCar;    //��ǣ�ƽ��������г�
	int m_iLineupRCanonCarCar;     //��ǣ�Ƴ��������г�
	int m_iLineupRCanonHorseCar;   //��ǣ���������г�
	int m_iLineupRCanonElephantCar;//��ǣ���󣬽����г�
	int m_iLineupRCanonBishopCar;  //��ǣ��ʿ�������г�
	int m_iLineupRCanonPawnCar;    //��ǣ��ʿ�������г�

	int m_iLineupRLieSlotHorseCar;//�Բ���ͳ�,������Ĳ�λ:���Գ鳵����

	int m_iLineupRDblCarLoneKing;			   //˫���Թ½�
	int m_iLineupRDblCarLoneKingElephant;	   //˫���Թ½�һ��
	int m_iLineupRDblCarLoneKing2Elephant;	   //˫���Թ½�����
	int m_iLineupRDblCarLoneKingBishop;		   //˫���Թ½�һʿ
	int m_iLineupRDblCarLoneKingBishopElephant;//˫���Թ½�һʿһ��

	int m_iLineupRLieMidCar;			//���ĳ�
	int m_iLineupRLieMidCar_LoneKing;   //���ĳ��Թ½�
	int m_iLineupRLieMidCar_BigCar;		//���ĳ�,�й��ӳ�
	int m_iLineupRLieMidCar_Car;	    //���ĳ�,�г�,�޹��ӳ�
	int m_iLineupRLieMidCar_CanonBishop;//���ĳ�,���ں�ʿ,�����
	int m_iLineupRLieMidCar_Horse[3];   //���ĳ�,�й�����

	int m_iLineupRLieMidPawn;		     //���ı�
	int m_iLineupRLieMidPawn_LoneKing;   //���ı��Թ½�
	int m_iLineupRLieMidPawn_BigCar;	 //���ı�,�й��ӳ�
	int m_iLineupRLieMidPawn_Car;		 //���ı�,�г�,�޹��ӳ�
	int m_iLineupRLieMidPawn_CanonBishop;//���ı�,���ں�ʿ,�����
	int m_iLineupRLieMidPawn_Horse[3];   //���ı�,�й��ͱ���

	int m_iLineupRLieSlotHorse;     //�Բ���
	int m_iLineupRLieMidHorse;      //������
	int m_iLineupRImpertinenceHorse;//³ç��:��δ�ж���������

	int m_iLineupRBigAngleHorse;			//�����
	int m_iLineupRBigAngleHorse_LoneKing;   //�����Թ½�
	int m_iLineupRBigAngleHorse_BigCar;		//�����,�й��ӳ�
	int m_iLineupRBigAngleHorse_Car;		//�����,�г�,�޹��ӳ�
	int m_iLineupRBigAngleHorse_CanonBishop;//�����,���ں�ʿ,�����
	int m_iLineupRBigAngleHorse_Horse[3];   //�����,�й�����

	int m_iLineupRNoProtectKing;			 //�ޱ�����
	int m_iLineupRNoProtectKing_CHC[3][3][3];//�ޱ�����:����ԭλ,��һά:��,��:��,��:��,�Է����ӳ��ĸ���:0~2,�Է�������ĸ���:0~2,�Է������ڵĸ���:0~2

	int m_iLineupRLoneKingVsCarHorse;        //�½��Գ���
	int m_iLineupRKingVsCarHorse[3][3][3][6];//�½��Գ���:����ʿ����,�Ը��Է�����һ��һ��,������޼����������Ļ�,�ǳ�Σ��
											 //��һά:ʿ,��:��,��:��,��:��

	int m_iLineupRKingAttack;    //����������˨
	int m_iLineupRKingCarAttack; //����������˨,�ҳ��ͽ���һ��,�������ڵ�����Է������ڵ��еĽ����������
	int m_iLineupRScndKingAttack;//����������˨

	int m_iLineupRAttackTogetherFactor;		   //Эͬ��ս����

	int m_iLineupRCarHorseCannonPawnAttackTogether;//�����ڱ�������ս
	int m_iLineupRCarHorseCannonAttackTogether;    //������������ս
	int m_iLineupRCarHorsePawnAttackTogether;	   //�����������ս
	int m_iLineupRCarCannonPawnAttackTogether;	   //���ڱ�������ս
	int m_iLineupRHorseCannonPawnAttackTogether;   //���ڱ�������ս

	int m_iLineupRCarHorseKing;//����
	int m_iLineupRCarPawnKing; //������

	int m_iLineupRHCarCanonKing;			 //���ڽ�
	int m_iLineupRHCarCanonKing_CHC[2][3][3];//���ڽ�:���뵤ɳ:��һά:�������ӳ��ĸ���-1
	//------------------------

	int m_iChessBaseValue[15][10][9];//��Ÿ������ӵļ�ֵ
	int m_nAttackValue[15];			 //���Ƽ�ֵ
	int m_nAttackValue_EB[15][3][3]; //���ÿ�����ӵĹ��Ƽ�ֵ,�ڶ�ά:��,����ά:ʿ
	UINT m_uiRecoveryValue[15];      //���ÿ�����ӵķ�������
	int m_iFlexValue[15];		     //�����������Է�ֵ
	short m_AttackPos[10][9];	     //���ÿһλ�ñ���в����Ϣ
	BYTE m_GuardPos[10][9];          //���ÿһλ�ñ���������Ϣ
	BYTE m_FlexibilityPos[10][9];    //���ÿһλ�������ӵ�����Է�ֵ
	int m_chessValue[10][9];	     //���ÿһλ�������ӵ��ܼ�ֵ
	int nPosCount;				     //��¼һ���ӵ����λ�ø���
	POINT RelatePos[20];		     //��¼һ���ӵ����λ��
	long m_nAccessCount;             //��¼��ֵ���������ʵĴ���

	//------------��ʼ��ֵ�����------------
	int m_iChessCnt[15];  //��ǰ����������Ŀ

	UINT m_uiAllChessCnt; //��ǰ��ֹ��е�������
	UINT m_uiAllBChessCnt;//��ǰ��ֺڷ����е�������
	UINT m_uiAllRChessCnt;//��ǰ��ֺ췽���е�������

	UINT m_uiCurChessAge; //��ǰ��������Ľ׶�

	vector<BYTE> m_vectorBCurChess;//�ڷ���ǰ��ӵ�е�����
	vector<BYTE> m_vectorRCurChess;//�췽��ǰ��ӵ�е�����

	int m_nMainChessCnt; //��ǰ�����ӵ�е���������:������
	int m_nBMainChessCnt;//��ǰ��ֺڷ���ӵ�е���������:������
	int m_nRMainChessCnt;//��ǰ��ֺ췽��ӵ�е���������:������
	//--------------------------------------
};

#endif // !defined(AFX_EVELUATIONBASE_H__02E08D9B_918A_4860_A745_4076F632250D__INCLUDED_)
