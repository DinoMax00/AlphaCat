// EveluationBase.cpp: implementation of the CBaseEveluation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseEveluation.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseEveluation::CBaseEveluation()
{
	int i,j,k,m,n;

	memcpy(m_iChessBaseValue,iChessBaseValue,1350*sizeof(int));
	memcpy(m_iChessCnt,iChessCount,15*sizeof(int));
	memcpy(m_iFlexValue,iFlexValue,15*sizeof(int));
	memcpy(m_uiRecoveryValue,uiRecoveryValue,15*sizeof(UINT));

	//-------------���ӹ��Ƽ�ֵ-------------
	//�ڳ�
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
			m_nAttackValue_EB[B_CAR][i][j]=32+(2-i)*5+(2-j)*3;
	}

	//����
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
			m_nAttackValue_EB[B_HORSE][i][j]=80+(2-i)*5+(2-j)*3;
	}

	//����
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
			m_nAttackValue_EB[B_CANON][i][j]=25-((2-i)*3+(2-j)*2);
	}

	//�ڱ�
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
			m_nAttackValue_EB[B_PAWN][i][j]=35+(2-i)*5+(2-j)*3;
	}


	//�쳵
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
			m_nAttackValue_EB[R_CAR][i][j]=32+(2-i)*5+(2-j)*3;
	}

	//����
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
			m_nAttackValue_EB[R_HORSE][i][j]=80+(2-i)*5+(2-j)*3;
	}

	//����
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
			m_nAttackValue_EB[R_CANON][i][j]=25-((2-i)*3+(2-j)*2);
	}

	//���
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
			m_nAttackValue_EB[R_PAWN][i][j]=35+(2-i)*5+(2-j)*3;
	}
	//--------------------------------------

	//--------��ʼ�����μ�ֵ--------
	//��
	//����
	m_iLineupB5ConnectPawn=500;//5����
	m_iLineupB4ConnectPawn=400;//4����
	m_iLineupB3ConnectPawn=300;//3����
	m_iLineupB2ConnectPawn=200;//2����

	m_iLineupBConnectElephant=50; //������
	m_iLineupBConnectBishop=25;   //����ʿ
	m_iLineupBConnectHorse=75;    //������
	m_iLineupBScndConnectHorse=30;//��������
	m_iLineupBSznConnectHorse=20; //��������
	m_iLineupBConnectCanon=30;    //������

	//���ŵ�
	m_iLineupBVCanonKing=200;			//������:����ͬ����һ���ڣ�������֮������
	m_iLineupBVHorseCanon=1000;			//����ڽ���������ͽ��ľ���Ϊ2
	m_iLineupBVScndHorseCanon=500;		//����ڽ���������ͽ��ľ��벻Ϊ2
	m_iLineupBVCanonCar=500;			//�ڳ�����
	m_iLineupBVCarCanonKing=150;		//���ڽ�
	m_iLineupBVDblCanon=500;			//˫�ڽ���
	m_iLineupBVCanon2Horese=350;		//�ںͽ�֮�������ӣ�������һ���ǶԷ�����
	m_iLineupBCannonKingLieMidHorse=500;//�ںͽ�֮����һ�������ҽ����߶���ʿ
	m_iLineupBVCanonCar2=250;			//�ںͽ�֮�������ӣ������м����ĳ�
	m_iLineupBVCanon2Else=175;			//�ںͽ�֮�������ӣ�����û�жԷ�����

	//���ŵ�
	m_iLineupBHCanonKing=150;	  //������:����ͬ����һ���ڣ�������֮������
	m_iLineupBHHorseCanon=9950;	  //����ڽ���������ͽ��ľ���Ϊ2
	m_iLineupBHScndHorseCanon=400;//����ڽ���������ͽ��ľ��벻Ϊ2
	m_iLineupBHCanonCar=450;	  //�ڳ�����
	m_iLineupBHDblCanon=450;	  //˫�ڽ���
	m_iLineupBHCanon2Horese=300;  //�ںͽ�֮�������ӣ�������һ���ǶԷ�����
	m_iLineupBHCanonCar2=240;	  //�ںͽ�֮�������ӣ������м����ĳ�
	m_iLineupBHCanon2Else=150;	  //�ںͽ�֮�������ӣ�����û�жԷ�����

	m_iLineupBCarKing=-500;   //���ͽ���һ�л�һ��
	m_iLineupBCarHorse=-200;  //��������һ�л�һ��
	m_iLineupBCarCanon=-100;  //��������һ�л�һ��
	m_iLineupBCarBishop=-50;  //����ʿ��һ�л�һ��
	m_iLineupBCarElephant=-50;//��������һ�л�һ��
	m_iLineupBCarPawn=-25;    //���ͱ���һ�л�һ��

	m_iLineupBCanonKingCar=1300;  //��ǣ�ƽ��������г�
	m_iLineupBCanonCarCar=1300;   //��ǣ�Ƴ��������г�
	m_iLineupBCanonHorseCar=200;  //��ǣ��������г�
	m_iLineupBCanonElephantCar=50;//��ǣ��������г�
	m_iLineupBCanonBishopCar=50;  //��ǣ��ʿ��ʿ���г�
	m_iLineupBCanonPawnCar=75;    //��ǣ�Ʊ��������г�

	m_iLineupBLieMidCar_LoneKing=2000;  //���ĳ��Թ½�
	m_iLineupBLieMidCar_BigCar=1000;	//���ĳ�,�й��ӳ�
	m_iLineupBLieMidCar_Car=500;		//���ĳ�,�г�,�޹��ӳ�
	m_iLineupBLieMidCar_CanonBishop=200;//���ĳ�,���ں�ʿ,�����
	//���ĳ�,�й����� m_iLineupBLieMidCar_Horse[3]
	for(i=0;i<3;i++)
		m_iLineupBLieMidCar_Horse[i]=30*i;

	m_iLineupBLieMidPawn_LoneKing=2000;  //���ı��Թ½�
	m_iLineupBLieMidPawn_BigCar=800;	 //���ı�,�й��ӳ�
	m_iLineupBLieMidPawn_Car=400;		 //���ı�,�г�,�޹��ӳ�
	m_iLineupBLieMidPawn_CanonBishop=150;//���ı�,���ں�ʿ,�����
	//���ı�,�й��ӱ��� m_iLineupBLieMidPawn_Horse[3]
	for(i=0;i<3;i++)
		m_iLineupBLieMidPawn_Horse[i]=25*i;

	m_iLineupBDblCarLoneKing=1000;		       //˫���Թ½�
	m_iLineupBDblCarLoneKingElephant=950;      //˫���Թ½�һ��
	m_iLineupBDblCarLoneKing2Elephant=900;	   //˫���Թ½�����
	m_iLineupBDblCarLoneKingBishop=950;		   //˫���Թ½�һʿ
	m_iLineupBDblCarLoneKingBishopElephant=900;//˫���Թ½�һʿһ��

	m_iLineupBLieSlotHorse=500;     //�Բ���
	m_iLineupBLieMidHorse=-100;     //������
	m_iLineupBImpertinenceHorse=-80;//³ç��:��δ�ж���������

	m_iLineupBBigAngleHorse_LoneKing=2000;  //�����Թ½�
	m_iLineupBBigAngleHorse_BigCar=500;		//�����,�й��ӳ�
	m_iLineupBBigAngleHorse_Car=400;		//�����,�г�,�޹��ӳ�
	m_iLineupBBigAngleHorse_CanonBishop=200;//�����,���ں�ʿ,�����
    //�����,�й����� m_iLineupBBigAngleHorse_Horse[3]
	for(i=0;i<3;i++)
		m_iLineupBBigAngleHorse_Horse[i]=30*i;

	//����ԭλ m_iLineupBNoProtectKing_CHC[3][3][3]
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
			for(k=0;k<3;k++)
				m_iLineupBNoProtectKing_CHC[i][j][k]=i*100+j*80+k*40;
	}

	m_iLineupBLoneKingVsCarHorse=-500;//�½��Գ���:����ʿ����,�Ը��Է�����һ��һ��,������޼����������Ļ�,�ǳ�Σ��
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
			for(m=0;m<3;m++)
				for(n=0;n<6;n++)
					m_iLineupBKingVsCarHorse[i][j][m][n]=-((2-i)*60+(2-j)*30+m*50+n*10);
	}

	m_iLineupBKingAttack=50;    //����������˨
	m_iLineupBKingCarAttack=200;//����������˨,�ҳ��ͽ���һ��,�������ڵ�����Է������ڵ��еĽ����������
	m_iLineupBScndKingAttack=30;//����������˨

	m_iLineupBAttackTogetherFactor=25;//Эͬ��ս����

	m_iLineupBCarHorseCannonPawnAttackTogether=300;//�����ڱ�������ս
	m_iLineupBCarHorseCannonAttackTogether=250;    //������������ս
	m_iLineupBCarHorsePawnAttackTogether=200;	   //�����������ս
	m_iLineupBCarCannonPawnAttackTogether=150;	   //���ڱ�������ս
	m_iLineupBHorseCannonPawnAttackTogether=100;   //���ڱ�������ս

	//���ڽ�:���뵤ɳ m_iLineupBHCarCanonKing_CHC[2][3][3]
	for(i=0;i<2;i++)
	{
		for(j=0;j<3;j++)
			for(k=0;k<3;k++)
				m_iLineupBHCarCanonKing_CHC[i][j][k]=300+i*100+j*50+k*40;
	}

	m_iLineupBCarHorseKing=100;//����
	m_iLineupBCarPawnKing=10;  //������

	//��
	m_iLineupR5ConnectPawn=500;//5����
	m_iLineupR4ConnectPawn=400;//4����
	m_iLineupR3ConnectPawn=300;//3����
	m_iLineupR2ConnectPawn=200;//2����

	m_iLineupRConnectElephant=50; //������
	m_iLineupRConnectBishop=25;   //����ʿ
	m_iLineupRConnectHorse=75;    //������
	m_iLineupRScndConnectHorse=30;//��������
	m_iLineupRSznConnectHorse=20; //��������
	m_iLineupRConnectCanon=100;   //������

	//���ŵ�
	m_iLineupRVCanonKing=200;			 //����ͬ����һ���ڣ�������֮������
	m_iLineupRVHorseCanon=1000;			 //����ڽ���������ͽ��ľ���Ϊ2
	m_iLineupRVScndHorseCanon=500;		 //����ڽ���������ͽ��ľ��벻Ϊ2
	m_iLineupRVCanonCar=500;			 //�ڳ�����
	m_iLineupRVCarCanonKing=150;		 //���ڽ�
	m_iLineupRVDblCanon=500;			 //˫�ڽ���
	m_iLineupRVCanon2Horese=350;		 //�ںͽ�֮�������ӣ�������һ���ǶԷ�����
	m_iLineupRCannonKingLieMidHorse=500;//�ںͽ�֮����һ�������ҽ����߶���ʿ
	m_iLineupRVCanonCar2=250;			 //�ںͽ�֮�������ӣ������м����ĳ�
	m_iLineupRVCanon2Else=175;			 //�ںͽ�֮�������ӣ�����û�жԷ�����

	//���ŵ�
	m_iLineupRHCanonKing=150;	  //����ͬ����һ���ڣ�������֮������
	m_iLineupRHHorseCanon=9950;	  //����ڽ���������ͽ��ľ���Ϊ2
	m_iLineupRHScndHorseCanon=400;//����ڽ���������ͽ��ľ��벻Ϊ2
	m_iLineupRHCanonCar=450;	  //�ڳ�����
	m_iLineupRHCarCanonKing=300;  //���ڽ�
	m_iLineupRHDblCanon=450;	  //˫�ڽ���
	m_iLineupRHCanon2Horese=300;  //�ںͽ�֮�������ӣ�������һ���ǶԷ�����
	m_iLineupRHCanonCar2=240;	  //�ںͽ�֮�������ӣ������м����ĳ�
	m_iLineupRHCanon2Else=150;	  //�ںͽ�֮�������ӣ�����û�жԷ�����

	m_iLineupRCarKing=-500;      //���ͽ���һ�л�һ��
	m_iLineupRCarHorse=-200;     //��������һ�л�һ��
	m_iLineupRCarCanon=-100;     //��������һ�л�һ��
	m_iLineupRCarBishop=-50;     //����ʿ��һ�л�һ��
	m_iLineupRCarElephant=-50;   //��������һ�л�һ��
	m_iLineupRCarPawn=-25;       //���ͱ���һ�л�һ��

	m_iLineupRCanonKingCar=1300;  //��ǣ�ƽ��������г�
	m_iLineupRCanonCarCar=1300;   //��ǣ�Ƴ��������г�
	m_iLineupRCanonHorseCar=200;  //��ǣ���������г�
	m_iLineupRCanonElephantCar=50;//��ǣ���󣬽����г�
	m_iLineupRCanonBishopCar=50;  //��ǣ��ʿ�������г�
	m_iLineupRCanonPawnCar=75;    //��ǣ�Ʊ��������г�

	m_iLineupRDblCarLoneKing=1000;		       //˫���Թ½�
	m_iLineupRDblCarLoneKingElephant=950;      //˫���Թ½�һ��
	m_iLineupRDblCarLoneKing2Elephant=900;	   //˫���Թ½�����
	m_iLineupRDblCarLoneKingBishop=950;		   //˫���Թ½�һʿ
	m_iLineupRDblCarLoneKingBishopElephant=900;//˫���Թ½�һʿһ��

	m_iLineupRLieMidCar_LoneKing=2000;  //���ĳ��Թ½�
	m_iLineupRLieMidCar_BigCar=1000;	//���ĳ�,�й��ӳ�
	m_iLineupRLieMidCar_Car=500;		//���ĳ�,�г�,�޹��ӳ�
	m_iLineupRLieMidCar_CanonBishop=200;//���ĳ�,���ں�ʿ,�����
	//���ĳ�,�й�����
	for(i=0;i<3;i++)
		m_iLineupRLieMidCar_Horse[i]=30*i;

	m_iLineupRLieMidPawn_LoneKing=2000;  //���ı��Թ½�
	m_iLineupRLieMidPawn_BigCar=800;	 //���ı�,�й��ӳ�
	m_iLineupRLieMidPawn_Car=400;		 //���ı�,�г�,�޹��ӳ�
	m_iLineupRLieMidPawn_CanonBishop=150;//���ı�,���ں�ʿ,�����
	//���ı�,�й��ӱ���
	for(i=0;i<3;i++)
		m_iLineupRLieMidPawn_Horse[i]=25*i+20*j;

	m_iLineupRLieSlotHorse=500;     //�Բ���
	m_iLineupRLieMidHorse=-100;     //������
	m_iLineupRImpertinenceHorse=-80;//³ç��:��δ�ж���������
	
	m_iLineupRBigAngleHorse_LoneKing=2000;  //�����Թ½�
	m_iLineupRBigAngleHorse_BigCar=500;		//�����,�й��ӳ�
	m_iLineupRBigAngleHorse_Car=400;		//�����,�г�,�޹��ӳ�
	m_iLineupRBigAngleHorse_CanonBishop=200;//�����,���ں�ʿ,�����
    //�����,�й�����
	for(i=0;i<3;i++)
		m_iLineupRBigAngleHorse_Horse[i]=30*i;

	//����ԭλ m_iLineupRNoProtectKing_CHC[3][3][3]
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
			for(k=0;k<3;k++)
				m_iLineupRNoProtectKing_CHC[i][j][k]=i*100+j*80+k*40;
	}

	m_iLineupRLoneKingVsCarHorse=-500;//�½��Գ���:����ʿ����,�Ը��Է�����һ��һ��,������޼����������Ļ�,�ǳ�Σ��
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
			for(m=0;m<3;m++)
				for(n=0;n<6;n++)
					m_iLineupRKingVsCarHorse[i][j][m][n]=-((2-i)*60+(2-j)*30+m*50+n*10);
	}

	m_iLineupRKingAttack=50;    //����������˨
	m_iLineupRKingCarAttack=200;//����������˨,�ҳ��ͽ���һ��,�������ڵ�����Է������ڵ��еĽ����������
	m_iLineupRScndKingAttack=30;//����������˨

	m_iLineupRAttackTogetherFactor=25;//Эͬ��ս����

	m_iLineupRCarHorseCannonPawnAttackTogether=300;//�����ڱ�������ս
	m_iLineupRCarHorseCannonAttackTogether=250;    //������������ս
	m_iLineupRCarHorsePawnAttackTogether=200;	   //�����������ս
	m_iLineupRCarCannonPawnAttackTogether=150;	   //���ڱ�������ս
	m_iLineupRHorseCannonPawnAttackTogether=100;   //���ڱ�������ս

	//���ڽ�:���뵤ɳ m_iLineupRHCarCanonKing_CHC[2][3][3]
	for(i=0;i<2;i++)
	{
		for(j=0;j<3;j++)
			for(k=0;k<3;k++)
				m_iLineupRHCarCanonKing_CHC[i][j][k]=300+i*100+j*50+k*40;
	}

	m_iLineupRCarHorseKing=100;//����
	m_iLineupRCarPawnKing=10;  //������
	//------------------------------

	m_nAccessCount=0;
}

CBaseEveluation::~CBaseEveluation()
{

}

int CBaseEveluation::Eveluate(BYTE byChessBoard[10][9],BOOL bIsRedTurn)
{
	return 0;
}

int CBaseEveluation::GetRelatePiece(BYTE byChessBoard[][9], int j, int i)
{
	nPosCount=0;
	BYTE nChessID;
	BYTE flag;
	int x,y;
	
	nChessID=byChessBoard[i][j];

	switch(nChessID)
	{
	case R_KING://��˧
	case B_KING://�ڽ�
		//ѭ�����Ź�֮����Щλ�ÿɵ���/����
		//ɨ�����߾͹���������������
		for(y=0;y<3;y++)
			for(x=3;x<6;x++)
				if(CanTouch(byChessBoard,j,i,x,y))//�ܷ񵽴�
					AddPoint(x,y);//�ɴﵽ/������λ�ü�������

		//ѭ�����Ź�֮����Щλ�ÿɵ���/����
		//ɨ�����߾͹���������������
		for(y=7;y<10;y++)
			for(x=3;x<6;x++)
				if(CanTouch(byChessBoard,j,i,x,y))//�ܷ񵽴�
					AddPoint(x,y);//�ɴﵽ/������λ�ü�������

		break;

	case R_BISHOP://��ʿ
		//ѭ�����Ź�֮����Щλ�ÿɵ���/����
		for(y=7;y<10;y++)
			for(x=3;x<6;x++)
				if(CanTouch(byChessBoard,j,i,x,y))
					AddPoint(x,y);//�ɴﵽ/������λ�ü�������
		
		break;

	case B_BISHOP://��ʿ
		//ѭ�����Ź�֮����Щλ�ÿɵ���/����
		for(y=0;y<3;y++)
			for(x=3;x<6;x++)
				if(CanTouch(byChessBoard,j,i,x,y))
					AddPoint(x,y);//�ɴﵽ/������λ�ü�������
		
		break;

	case R_ELEPHANT://����
	case B_ELEPHANT://����
		//����
		x=j+2;
		y=i+2;
		if(x<9 && y<10 && CanTouch(byChessBoard,j,i,x,y))
			AddPoint(x,y);

		//����
		x=j+2;
		y=i-2;
		if(x<9 && y>=0 && CanTouch(byChessBoard,j,i,x,y))
			AddPoint(x,y);

		//����
		x=j-2;
		y=i+2;
		if(x>=0 && y<10 && CanTouch(byChessBoard,j,i,x,y))
			AddPoint(x,y);

		//����
		x=j-2;
		y=i-2;
		if(x>=0 && y>=0 && CanTouch(byChessBoard,j,i,x,y))
			AddPoint(x,y);

		break;

		case R_HORSE://����
		case B_HORSE://����
			//������·��ܷ񵽴�/����
			x=j+2;
			y=i+1;
			if((x<9 && y<10)&&CanTouch(byChessBoard,j,i,x,y))
				AddPoint(x,y);

			//������Ϸ��ܷ񵽴�/����
			x=j+2;
			y=i-1;
			if((x<9 && y>=0)&&CanTouch(byChessBoard,j,i,x,y))
				AddPoint(x,y);

			//������·��ܷ񵽴�/����
			x=j-2;
			y=i+1;
			if((x>=0 && y<10)&&CanTouch(byChessBoard,j,i,x,y))
				AddPoint(x,y);

			//������Ϸ��ܷ񵽴�/����
			x=j-2;
			y=i-1;
			if((x>=0 && y>=0)&&CanTouch(byChessBoard,j,i,x,y))
				AddPoint(x,y);

			//������·��ܷ񵽴�/����
			x=j+1;
			y=i+2;
			if((x<9 && y<10)&&CanTouch(byChessBoard,j,i,x,y))
				AddPoint(x,y);

			//������Ϸ��ܷ񵽴�/����
			x=j+1;
			y=i-2;
			if((x<9 && y>=0)&&CanTouch(byChessBoard,j,i,x,y))
				AddPoint(x,y);

			//������·��ܷ񵽴�/����
			x=j-1;
			y=i+2;
			if((x>=0 && y<10)&&CanTouch(byChessBoard,j,i,x,y))
				AddPoint(x,y);

			//������Ϸ��ܷ񵽴�/����
			x=j-1;
			y=i-2;
			if((x>=0 && y>=0)&&CanTouch(byChessBoard,j,i,x,y))
				AddPoint(x,y);

			break;

		case R_CAR://�쳵
		case B_CAR://�ڳ�
			//��������ܷ񵽴�/����
			x=j+1;
			y=i;
			while(x<9)
			{
				if(NOCHESS==byChessBoard[y][x])//�հ�
					AddPoint(x,y);
				else{
					//������һ������
					AddPoint(x,y);
					break;//�����λ�ò�������
				}
				x++;
			}

			//��������ܷ񵽴�/����
			x=j-1;
			y=i;
			while(x>=0)
			{
				if(NOCHESS==byChessBoard[y][x])//�հ�
					AddPoint(x,y);
				else{
					//������һ������
					AddPoint(x,y);
					break;//�����λ�ò�������
				}
				x--;
			}

			//��������ܷ񵽴�/����
			x=j;
			y=i+1;
			while(y<10)
			{
				if(NOCHESS==byChessBoard[y][x])//�հ�
					AddPoint(x,y);
				else{
					//������һ������
					AddPoint(x,y);
					break;//�����λ�ò�������
				}
				y++;
			}

			//��������ܷ񵽴�/����
			x=j;
			y=i-1;
			while(y>=0)
			{
				if(NOCHESS==byChessBoard[y][x])//�հ�
					AddPoint(x,y);
				else{
					//������һ������
					AddPoint(x,y);
					break;//�����λ�ò�������
				}
				y--;
			}

			break;

		case R_PAWN://���
			//�ۿ���ǰ�Ƿ񵽵�
			y=i-1;
			x=j;
			if(y>=0)
				AddPoint(x,y);//û���ף�����
			if(i<5)
			{
				//���ѹ���
				y=i;
				x=j+1;//����
				if(x<9)
					AddPoint(x,y);//δ���ұߣ�����
				x=j-1;//����
				if(x>=0)
					AddPoint(x,y);//δ����ߣ�����
			}

			break;

		case B_PAWN://����
			//�ۿ���ǰ�Ƿ񵽵�
			y=i+1;
			x=j;
			if(y<10)
				AddPoint(x,y);//û���ף�����
			if(i>4)
			{
				//���ѹ���
				y=i;
				x=j+1;//����
				if(x<9)
					AddPoint(x,y);//δ���ұߣ�����
				x=j-1;//����
				if(x>=0)
					AddPoint(x,y);//δ����ߣ�����
			}

			break;

		case B_CANON://����
		case R_CANON://����
			//��������ܷ񵽴�/������λ��
			x=j+1;
			y=i;
			flag=FALSE;
			while(x<9)
			{
				if(NOCHESS==byChessBoard[y][x])
				{
					//�հ�λ��
					if(!flag)
						AddPoint(x,y);
				}
				else
				{
					if(!flag)
						flag=TRUE;//�ǵ�һ������
					else
					{
						//�ǵڶ�������
						AddPoint(x,y);
						break;
					}
				}
				x++;//��������
			}

			//��������ܷ񵽴�/������λ��
			x=j-1;
			y=i;
			flag=FALSE;
			while(x>=0)
			{
				if(NOCHESS==byChessBoard[y][x])
				{
					//�հ�λ��
					if(!flag)
						AddPoint(x,y);
				}
				else
				{
					if(!flag)
						flag=TRUE;//�ǵ�һ������
					else
					{
						//�ǵڶ�������
						AddPoint(x,y);
						break;
					}
				}
				x--;//��������
			}

			//��������ܷ񵽴�/������λ��
			x=j;
			y=i+1;
			flag=FALSE;
			while(y<10)
			{
				if(NOCHESS==byChessBoard[y][x])
				{
					//�հ�λ��
					if(!flag)
						AddPoint(x,y);
				}
				else
				{
					if(!flag)
						flag=TRUE;//�ǵ�һ������
					else
					{
						//�ǵڶ�������
						AddPoint(x,y);
						break;
					}
				}
				y++;//��������
			}

			//��������ܷ񵽴�/������λ��
			x=j;
			y=i-1;
			flag=FALSE;
			while(y>=0)
			{
				if(NOCHESS==byChessBoard[y][x])
				{
					//�հ�λ��
					if(!flag)
						AddPoint(x,y);
				}
				else
				{
					if(!flag)
						flag=TRUE;//�ǵ�һ������
					else
					{
						//�ǵڶ�������
						AddPoint(x,y);
						break;
					}
				}
				y--;//��������
			}

			break;

		default:
			break;
		}

	return nPosCount;
}

bool CBaseEveluation::CanTouch(BYTE byChessBoard[][9], int nFromX, int nFromY, int nToX, int nToY)
{
	int i,j;
	int nMoveChessID,nTargetID;

	if(nFromX==nToX && nFromY==nToY)
		return false;//Ŀ����Դ��ͬ���Ƿ�

	nMoveChessID=byChessBoard[nFromX][nFromY];
	nTargetID=byChessBoard[nToX][nToY];

	if(IsSameSide(nMoveChessID,nTargetID))
		return false;//���Լ����壬�Ƿ�

	switch(nMoveChessID)
	{
	case B_KING://�ڽ�
		if(nTargetID==R_KING)//�ж��Ƿ�˧����
		{
			if(nFromX!=nToX)//�����겻���
				return false;//��˧����ͬһ��

			for(i=nFromY+1;i<nToY;i++)
				if(byChessBoard[i][nFromX]!=NOCHESS)
					return false;//�м��������
		}
		else
		{
			if(nToY>2 || nToX>5 || nToX<3)
				return false;//Ŀ����ھŹ�֮��

			if(abs(nFromY-nToY)+abs(nFromX-nToX)>1)
				return false;//��˧ֻ��һ��ֱ��
		}

		break;

	case R_KING://��˧
		if(nTargetID==B_KING)//�ж��Ƿ�˧����
		{
			if(nFromX!=nToX)//�����겻���
				return false;//��˧����ͬһ��

			for(i=nFromY-1;i>nToY;i--)
				if(byChessBoard[i][nFromX]!=NOCHESS)
					return false;//�м��������
		}
		else
		{
			if(nToY<7 || nToX>5 || nToX<3)
				return false;//Ŀ����ھŹ�֮��

			if(abs(nFromY-nToY)+abs(nFromX-nToX)>1)
				return false;//��˧ֻ��һ��ֱ��
		}

		break;

	case R_BISHOP://��ʿ
		if(nToY<7 || nToX>5 || nToX<3)
			return false;//ʿ���Ź�

		if(abs(nFromX-nToX)!=1 || abs(nFromY-nToY)!=1)
			return false;//ʿ��б��

		break;

	case B_BISHOP://��ʿ
		if(nToY>2 || nToX>5 || nToX<3)
			return false;//ʿ���Ź�

		if(abs(nFromX-nToX)!=1 || abs(nFromY-nToY)!=1)
			return false;//ʿ��б��

		break;

	case R_ELEPHANT://����
		if(nToY<5)
			return false;//�಻�ܹ���

		if(abs(nFromX-nToX)!=2 || abs(nFromY-nToY)!=2)
			return false;//��������

		if(byChessBoard[(nFromY +nToY)/2][(nFromX +nToX)/2]!=NOCHESS)
			return FALSE;//���۱���

		break;

	case B_ELEPHANT://����
		if(nToY>4)
			return false;//���ܹ���

		if(abs(nFromX-nToX)!=2 || abs(nFromY-nToY)!=2)
			return false;//��������

		if(byChessBoard[(nFromY +nToY)/2][(nFromX +nToX)/2]!=NOCHESS)
			return FALSE;//���۱���

		break;

	case B_PAWN://����
		if(nToY<nFromY)
			return false;//�䲻�ܻ�ͷ

		if(nFromY<5 && nFromY==nToY)
			return FALSE;//�����ǰֻ��ֱ��

		if(nToY-nFromY+abs(nToX -nFromX)>1)
			return FALSE;//��ֻ��һ��ֱ��

		break;

	case R_PAWN://���
		if(nToY<nFromY)
			return false;//�����ܻ�ͷ

		if(nFromY>4 && nFromY==nToY)
			return FALSE;//������ǰֻ��ֱ��

		if(nToY-nFromY+abs(nToX -nFromX)>1)
			return FALSE;//��ֻ��һ��ֱ��

		break;

	case B_CAR://�ڳ�
	case R_CAR://�쳵
		if(nFromY!=nToY && nFromX!=nToX)
			return FALSE;//����ֱ��

		if(nFromY==nToY)
		{
			if(nFromX<nToX)
			{
				for(i=nFromX+1;i<nToX;i++)
					if(byChessBoard[nFromY][i]!=NOCHESS)
						return FALSE;
			}
			else
			{
				for(i=nToX+1;i<nFromX;i++)
					if(byChessBoard[nFromY][i]!=NOCHESS)
						return FALSE;
			}
		}
		else
		{
			if(nFromY<nToY)
			{
				for(j=nFromY+1;j<nToY;j++)
					if(byChessBoard[j][nFromX]!=NOCHESS)
						return FALSE;
			}
			else
			{
				for(j=nToY+1;j<nFromY;j++)
					if(byChessBoard[j][nFromX]!=NOCHESS)
						return FALSE;
			}
		}
		
		break;

	case B_HORSE://����
	case R_HORSE://����
		if(!((abs(nToX-nFromX)==1 && abs(nToY -nFromY)==2) || (abs(nToX-nFromX)==2&&abs(nToY -nFromY)==1)))
			return FALSE;//��������

		if(nToX-nFromX==2)
		{
			i=nFromX+1;
			j=nFromY;
		}
		else
			if(nFromX-nToX==2)
			{
				i=nFromX-1;
				j=nFromY;
			}
			else
				if(nToY-nFromY==2)
				{
					i=nFromX;
					j=nFromY+1;
				}
				else
					if(nFromY-nToY==2)
					{
						i=nFromX;
						j=nFromY-1;
					}

		if(byChessBoard[j][i]!=NOCHESS)
			return FALSE;//������
		
		break;

	case B_CANON://����
	case R_CANON://����
		if(nFromY!=nToY && nFromX!=nToX)
			return FALSE;//����ֱ��

		//�ڳ���ʱ������·���в���������
		if(byChessBoard[nToY][nToX]==NOCHESS)
		{
			if(nFromY==nToY)
			{
				if(nFromX<nToX)
				{
					for(i=nFromX+1;i<nToX;i++)
						if(byChessBoard[nFromY][i]!=NOCHESS)
							return FALSE;
				}
				else
				{
					for(i=nToX+1;i<nFromX;i++)
						if(byChessBoard[nFromY][i]!=NOCHESS)
							return FALSE;
				}
			}
			else
			{
				if(nFromY<nToY)
				{
					for(j=nFromY+1;j<nToY;j++)
						if(byChessBoard[j][nFromX]!=NOCHESS)
							return FALSE;
				}
				else
				{
					for(j=nToY+1;j<nFromY;j++)
						if(byChessBoard[j][nFromX]!=NOCHESS)
							return FALSE;
				}
			}
		}		
		else//�ڳ���ʱ
		{
			int j=0;
			if(nFromY==nToY)
			{
				if(nFromX<nToX)
				{
					for(i=nFromX+1;i<nToX;i++)
						if(byChessBoard[nFromY][i]!=NOCHESS)
							j++;
						if(j!=1)
							return FALSE;
				}
				else
				{
					for(i=nToX+1;i<nFromX;i++)
						if(byChessBoard[nFromY][i]!=NOCHESS)
							j++;
						if(j!=1)
							return FALSE;
				}
			}
			else
			{
				if(nFromY<nToY)
				{
					for(j=nFromY+1;j<nToY;j++)
						if(byChessBoard[j][nFromX]!=NOCHESS)
							j++;
					if(j!=1)
						return FALSE;
				}
				else
				{
					for(j=nToY+1;j<nFromY;j++)
						if(byChessBoard[j][nFromX]!=NOCHESS)
							j++;
					if(j!=1)
						return FALSE;
				}
			}
		}
		
		break;

	default:
		return false;
	}
		
	return true;
}

void CBaseEveluation::AddPoint(int x, int y)
{
	RelatePos[nPosCount].x=x;
	RelatePos[nPosCount].y=y;

	nPosCount++;
}

void CBaseEveluation::UpdateCurChess(BYTE byChess, bool bIsRed)
{
	if(byChess==NOCHESS)
		return;

	BYTE byTempChess;
	int iSize;
	int i;

	if(bIsRed)
	{
		//-------------���º췽��ǰ��ӵ�е�����-------------
		iSize=m_vectorRCurChess.size();
		for(i=0;i<iSize;i++)
		{
			if(byChess==m_vectorRCurChess[i])
			{
				//�ѵ�ǰ�����붥���ӽ���
				byTempChess=m_vectorRCurChess[i];
				m_vectorRCurChess[i]=m_vectorRCurChess[iSize-1];
				m_vectorRCurChess[iSize-1]=byTempChess;

				//����������
				m_vectorRCurChess.pop_back();
			}
		}
		//------------------------------------------

		//���º췽����������
		if(byChess==R_CAR || byChess==R_HORSE || byChess==R_CANON)
		{
			m_nMainChessCnt--; //��ǰ�����ӵ�е���������:������
			m_nRMainChessCnt--;//��ǰ��ֺ췽��ӵ�е���������:������
		}

		m_uiAllRChessCnt--;//��ǰ��ֺ췽���е�������
	}
	else
	{
		//-------------���º췽��ǰ��ӵ�е�����-------------
		iSize=m_vectorBCurChess.size();
		for(i=0;i<iSize;i++)
		{
			if(byChess==m_vectorBCurChess[i])
			{
				byTempChess=m_vectorBCurChess[i];
				m_vectorBCurChess[i]=m_vectorBCurChess[iSize-1];
				m_vectorBCurChess[iSize-1]=byTempChess;
				m_vectorBCurChess.pop_back();
			}
		}
		//------------------------------------------

		//���ºڷ�����������
		if(byChess==B_CAR || byChess==B_HORSE || byChess==B_CANON)
		{
			m_nMainChessCnt--; //��ǰ�����ӵ�е���������:������
			m_nBMainChessCnt--;//��ǰ��ֺڷ���ӵ�е���������:������
		}

		m_uiAllBChessCnt--;//��ǰ��ֺڷ����е�������
	}

	m_iChessCnt[byChess]--;//��ǰ����������Ŀ
	m_uiAllChessCnt--;     //��ǰ��ֹ��е�������

	//�жϵ�ǰ��������׶�
	if(m_uiAllChessCnt>=28)
		m_uiCurChessAge=CA_START;
	else
	{
		if(m_nMainChessCnt>4)
			m_uiCurChessAge=CA_MID;
		else
			m_uiCurChessAge=CA_END;
	}

	//��ǰ��������λ��ջ,���Ӹ��Ӹ�����CBaseSearchEngine��MakeMove�и���
}

void CBaseEveluation::UpdateValue(BYTE byChessBoard[][9])
{
	int i,j;
	int iSize;
	int iDist;
	bool bFlag;

	//--------------��ѹ��--------------
	//��
	if(byChessBoard[0][0]==B_CAR && byChessBoard[0][1]==B_HORSE)
		m_iChessBaseValue[R_CAR][1][1]=1400;
	else
		m_iChessBaseValue[R_CAR][1][1]=1300;
	if(byChessBoard[0][8]==B_CAR && byChessBoard[0][7]==B_HORSE)
		m_iChessBaseValue[R_CAR][1][7]=1400;
	else
		m_iChessBaseValue[R_CAR][1][7]=1300;

	//��
	if(byChessBoard[9][0]==R_CAR && byChessBoard[9][1]==R_HORSE)
		m_iChessBaseValue[B_CAR][8][1]=1400;
	else
		m_iChessBaseValue[B_CAR][8][1]=1300;
	if(byChessBoard[9][8]==R_CAR && byChessBoard[9][7]==R_HORSE)
		m_iChessBaseValue[B_CAR][8][7]=1400;
	else
		m_iChessBaseValue[B_CAR][8][7]=1300;
	//----------------------------------

	//----------����/��ʱ��/���ֵ����----------
	//��
	if(m_iChessCnt[B_ELEPHANT]==1)
	{
		m_iChessBaseValue[B_ELEPHANT][0][2]=200;
		m_iChessBaseValue[B_ELEPHANT][0][6]=200;
		m_iChessBaseValue[B_ELEPHANT][2][0]=200;
		m_iChessBaseValue[B_ELEPHANT][2][4]=200;
		m_iChessBaseValue[B_ELEPHANT][2][8]=200;
		m_iChessBaseValue[B_ELEPHANT][4][2]=200;
		m_iChessBaseValue[B_ELEPHANT][4][6]=200;
	}
	else
	{
		m_iChessBaseValue[B_ELEPHANT][0][2]=250;
		m_iChessBaseValue[B_ELEPHANT][0][6]=250;
		m_iChessBaseValue[B_ELEPHANT][2][0]=240;
		m_iChessBaseValue[B_ELEPHANT][2][4]=260;
		m_iChessBaseValue[B_ELEPHANT][2][8]=240;
		m_iChessBaseValue[B_ELEPHANT][4][2]=240;
		m_iChessBaseValue[B_ELEPHANT][4][6]=240;
	}

	//��
	if(m_iChessCnt[R_ELEPHANT]==1)
	{
		m_iChessBaseValue[R_ELEPHANT][5][2]=200;
		m_iChessBaseValue[R_ELEPHANT][5][6]=200;
		m_iChessBaseValue[R_ELEPHANT][7][0]=200;
		m_iChessBaseValue[R_ELEPHANT][7][4]=200;
		m_iChessBaseValue[R_ELEPHANT][7][8]=200;
		m_iChessBaseValue[R_ELEPHANT][9][2]=200;
		m_iChessBaseValue[R_ELEPHANT][9][6]=200;
	}
	else
	{
		m_iChessBaseValue[R_ELEPHANT][5][2]=250;
		m_iChessBaseValue[R_ELEPHANT][5][6]=250;
		m_iChessBaseValue[R_ELEPHANT][7][0]=240;
		m_iChessBaseValue[R_ELEPHANT][7][4]=260;
		m_iChessBaseValue[R_ELEPHANT][7][8]=240;
		m_iChessBaseValue[R_ELEPHANT][9][2]=240;
		m_iChessBaseValue[R_ELEPHANT][9][6]=240;
	}
	//------------------------------------------

	//----------------������ʱ,�ܱ���,��ֹ�������----------------
	//��
	if(byChessBoard[2][4]!=NOCHESS)
	{
		if(byChessBoard[6][2]==R_PAWN || byChessBoard[7][2]==R_PAWN)
			m_iChessBaseValue[B_ELEPHANT][2][0]=260;
		if(byChessBoard[6][6]==R_PAWN || byChessBoard[7][6]==R_PAWN)
			m_iChessBaseValue[B_ELEPHANT][2][8]=260;
	}
	else
	{
		m_iChessBaseValue[B_ELEPHANT][2][0]=240;
		m_iChessBaseValue[B_ELEPHANT][2][8]=240;
		m_iChessBaseValue[B_ELEPHANT][2][4]=260;
	}

	//��
	if(byChessBoard[7][4]!=NOCHESS)
	{
		if(byChessBoard[3][2]==B_PAWN || byChessBoard[4][2]==B_PAWN)
			m_iChessBaseValue[R_ELEPHANT][7][0]=260;
		if(byChessBoard[3][6]==B_PAWN || byChessBoard[4][6]==B_PAWN)
			m_iChessBaseValue[R_ELEPHANT][7][8]=260;
	}
	else
	{
		m_iChessBaseValue[R_ELEPHANT][7][0]=240;
		m_iChessBaseValue[R_ELEPHANT][7][8]=240;
		m_iChessBaseValue[R_ELEPHANT][7][4]=260;
	}
	//------------------------------------------------------------

	//---------���ƺӽ��ڴ�֮������---------
	//��
	if(byChessBoard[7][8]==R_CAR || byChessBoard[8][8]==R_CAR || byChessBoard[9][8]==R_CAR)
		m_iChessBaseValue[B_CANON][4][8]=400;
	else
		m_iChessBaseValue[B_CANON][4][8]=550;
	if(byChessBoard[7][0]==R_CAR || byChessBoard[8][0]==R_CAR || byChessBoard[9][0]==R_CAR)
		m_iChessBaseValue[B_CANON][4][0]=400;
	else
		m_iChessBaseValue[B_CANON][4][0]=550;

	//��
	if(byChessBoard[0][8]==B_CAR || byChessBoard[1][8]==B_CAR || byChessBoard[2][8]==B_CAR)
		m_iChessBaseValue[R_CANON][5][8]=400;
	else
		m_iChessBaseValue[R_CANON][5][8]=550;
	if(byChessBoard[0][0]==B_CAR || byChessBoard[1][0]==B_CAR || byChessBoard[2][0]==B_CAR)
		m_iChessBaseValue[R_CANON][5][0]=400;
	else
		m_iChessBaseValue[R_CANON][5][0]=550;
	//----------------------------------------

	//--------------������ȱ��ĳЩ����ʱ,���ڹ��ɵ�һϵ�н�����ֵ���罵��--------------
	//��
	//����û��˫��,˫��,˫��,ֻ��һ��
	if(!m_iChessCnt[B_CAR] && !m_iChessCnt[B_HORSE])
	{
		m_iLineupBVCanon2Else=100;
		m_iLineupBHCanon2Else=70;
	}
	else
	{
		m_iLineupBVCanon2Else=175;
		m_iLineupBHCanon2Else=150;
	}

	//��
	//����û��˫��,˫��,˫��,ֻ��һ��
	if(!m_iChessCnt[R_CAR] && !m_iChessCnt[R_HORSE])
	{
		m_iLineupRVCanon2Else=100;
		m_iLineupRHCanon2Else=70;
	}
	else
	{
		m_iLineupRVCanon2Else=175;
		m_iLineupRHCanon2Else=150;
	}
	//---------------------------------------------------------------------------------

	//---------------������---------------
	//��
	switch(m_iChessCnt[R_BISHOP])
	{
	case 0:
		m_iLineupBVCanonKing=5;
		m_iLineupBHCanonKing=0;

		for(i=0;i<m_uiCurBigChessCnt[R_CAR];i++)
		{
			m_iLineupBVCanonKing+=10;
			m_iLineupBHCanonKing+=10;
		}

		for(i=0;i<m_uiCurBigChessCnt[R_HORSE];i++)
		{
			m_iLineupBVCanonKing+=5;
			m_iLineupBHCanonKing+=5;
		}

		for(i=0;i<m_uiCurBigChessCnt[R_CANON];i++)
		{
			m_iLineupBVCanonKing+=5;
			m_iLineupBHCanonKing+=5;
		}

		for(i=0;i<m_uiCurBigChessCnt[R_PAWN];i++)
		{
			m_iLineupBVCanonKing+=1;
			m_iLineupBHCanonKing+=1;
		}


		for(i=0;i<m_uiCurBigChessCnt[B_CAR];i++)
		{
			m_iLineupBVCanonKing+=10;
			m_iLineupBHCanonKing+=10;
		}

		for(i=0;i<m_uiCurBigChessCnt[B_HORSE];i++)
		{
			m_iLineupBVCanonKing+=5;
			m_iLineupBHCanonKing+=5;
		}

		for(i=0;i<m_uiCurBigChessCnt[B_CANON];i++)
		{
			m_iLineupBVCanonKing+=5;
			m_iLineupBHCanonKing+=5;
		}

		for(i=0;i<m_uiCurBigChessCnt[B_PAWN];i++)
		{
			m_iLineupBVCanonKing+=1;
			m_iLineupBHCanonKing+=1;
		}
		break;

	case 1:
		m_iLineupBVCanonKing=40;
		m_iLineupBHCanonKing=30;

		for(i=0;i<m_uiCurBigChessCnt[R_CAR];i++)
		{
			m_iLineupBVCanonKing+=20;
			m_iLineupBHCanonKing+=20;
		}

		for(i=0;i<m_uiCurBigChessCnt[R_HORSE];i++)
		{
			m_iLineupBVCanonKing+=10;
			m_iLineupBHCanonKing+=10;
		}
		
		for(i=0;i<m_uiCurBigChessCnt[R_CANON];i++)
		{
			m_iLineupBVCanonKing+=10;
			m_iLineupBHCanonKing+=10;
		}

		for(i=0;i<m_uiCurBigChessCnt[R_PAWN];i++)
		{
			m_iLineupBVCanonKing+=5;
			m_iLineupBHCanonKing+=5;
		}


		for(i=0;i<m_uiCurBigChessCnt[B_CAR];i++)
		{
			m_iLineupBVCanonKing+=15;
			m_iLineupBHCanonKing+=15;
		}

		for(i=0;i<m_uiCurBigChessCnt[B_HORSE];i++)
		{
			m_iLineupBVCanonKing+=10;
			m_iLineupBHCanonKing+=10;
		}

		for(i=0;i<m_uiCurBigChessCnt[B_CANON];i++)
		{
			m_iLineupBVCanonKing+=10;
			m_iLineupBHCanonKing+=10;
		}

		for(i=0;i<m_uiCurBigChessCnt[B_PAWN];i++)
		{
			m_iLineupBVCanonKing+=5;
			m_iLineupBHCanonKing+=5;
		}
		break;

	default:
		m_iLineupBVCanonKing=80;
		m_iLineupBHCanonKing=60;

		for(i=0;i<m_uiCurBigChessCnt[R_CAR];i++)
		{
			m_iLineupBVCanonKing+=25;
			m_iLineupBHCanonKing+=25;
		}

		for(i=0;i<m_uiCurBigChessCnt[R_HORSE];i++)
		{
			m_iLineupBVCanonKing+=15;
			m_iLineupBHCanonKing+=15;
		}

		for(i=0;i<m_uiCurBigChessCnt[R_CANON];i++)
		{
			m_iLineupBVCanonKing+=15;
			m_iLineupBHCanonKing+=15;
		}

		for(i=0;i<m_uiCurBigChessCnt[R_PAWN];i++)
		{
			m_iLineupBVCanonKing+=5;
			m_iLineupBHCanonKing+=5;
		}


		for(i=0;i<m_uiCurBigChessCnt[B_CAR];i++)
		{
			m_iLineupBVCanonKing+=15;
			m_iLineupBHCanonKing+=15;
		}

		for(i=0;i<m_uiCurBigChessCnt[B_HORSE];i++)
		{
			m_iLineupBVCanonKing+=10;
			m_iLineupBHCanonKing+=10;
		}

		for(i=0;i<m_uiCurBigChessCnt[B_CANON];i++)
		{
			m_iLineupBVCanonKing+=10;
			m_iLineupBHCanonKing+=10;
		}

		for(i=0;i<m_uiCurBigChessCnt[B_PAWN];i++)
		{
			m_iLineupBVCanonKing+=5;
			m_iLineupBHCanonKing+=5;
		}
		break;
	}

	//��
	switch(m_iChessCnt[B_BISHOP])
	{
	case 0:
		m_iLineupRVCanonKing=5;
		m_iLineupRHCanonKing=0;

		for(i=0;i<m_uiCurBigChessCnt[B_CAR];i++)
		{
			m_iLineupRVCanonKing+=10;
			m_iLineupRHCanonKing+=10;
		}

		for(i=0;i<m_uiCurBigChessCnt[B_HORSE];i++)
		{
			m_iLineupRVCanonKing+=5;
			m_iLineupRHCanonKing+=5;
		}

		for(i=0;i<m_uiCurBigChessCnt[B_CANON];i++)
		{
			m_iLineupRVCanonKing+=5;
			m_iLineupRHCanonKing+=5;
		}

		for(i=0;i<m_uiCurBigChessCnt[B_PAWN];i++)
		{
			m_iLineupRVCanonKing+=1;
			m_iLineupRHCanonKing+=1;
		}


		for(i=0;i<m_uiCurBigChessCnt[R_CAR];i++)
		{
			m_iLineupRVCanonKing+=10;
			m_iLineupRHCanonKing+=10;
		}

		for(i=0;i<m_uiCurBigChessCnt[R_HORSE];i++)
		{
			m_iLineupRVCanonKing+=5;
			m_iLineupRHCanonKing+=5;
		}

		for(i=0;i<m_uiCurBigChessCnt[R_CANON];i++)
		{
			m_iLineupRVCanonKing+=5;
			m_iLineupRHCanonKing+=5;
		}

		for(i=0;i<m_uiCurBigChessCnt[R_PAWN];i++)
		{
			m_iLineupRVCanonKing+=1;
			m_iLineupRHCanonKing+=1;
		}
		break;

	case 1:
		m_iLineupRVCanonKing=40;
		m_iLineupRHCanonKing=30;

		for(i=0;i<m_uiCurBigChessCnt[B_CAR];i++)
		{
			m_iLineupRVCanonKing+=20;
			m_iLineupRHCanonKing+=20;
		}

		for(i=0;i<m_uiCurBigChessCnt[B_HORSE];i++)
		{
			m_iLineupRVCanonKing+=10;
			m_iLineupRHCanonKing+=10;
		}

		for(i=0;i<m_uiCurBigChessCnt[B_CANON];i++)
		{
			m_iLineupRVCanonKing+=10;
			m_iLineupRHCanonKing+=10;
		}

		for(i=0;i<m_uiCurBigChessCnt[B_PAWN];i++)
		{
			m_iLineupRVCanonKing+=5;
			m_iLineupRHCanonKing+=5;
		}


		for(i=0;i<m_uiCurBigChessCnt[R_CAR];i++)
		{
			m_iLineupRVCanonKing+=15;
			m_iLineupRHCanonKing+=15;
		}

		for(i=0;i<m_uiCurBigChessCnt[R_HORSE];i++)
		{
			m_iLineupRVCanonKing+=10;
			m_iLineupRHCanonKing+=10;
		}

		for(i=0;i<m_uiCurBigChessCnt[R_CANON];i++)
		{
			m_iLineupRVCanonKing+=10;
			m_iLineupRHCanonKing+=10;
		}

		for(i=0;i<m_uiCurBigChessCnt[R_PAWN];i++)
		{
			m_iLineupRVCanonKing+=5;
			m_iLineupRHCanonKing+=5;
		}
		break;

	default:
		m_iLineupRVCanonKing=80;
		m_iLineupRHCanonKing=60;

		for(i=0;i<m_uiCurBigChessCnt[B_CAR];i++)
		{
			m_iLineupRVCanonKing+=25;
			m_iLineupRHCanonKing+=25;
		}

		for(i=0;i<m_uiCurBigChessCnt[B_HORSE];i++)
		{
			m_iLineupRVCanonKing+=15;
			m_iLineupRHCanonKing+=15;
		}

		for(i=0;i<m_uiCurBigChessCnt[B_CANON];i++)
		{
			m_iLineupRVCanonKing+=15;
			m_iLineupRHCanonKing+=15;
		}

		for(i=0;i<m_uiCurBigChessCnt[B_PAWN];i++)
		{
			m_iLineupRVCanonKing+=5;
			m_iLineupRHCanonKing+=5;
		}


		for(i=0;i<m_uiCurBigChessCnt[R_CAR];i++)
		{
			m_iLineupRVCanonKing+=15;
			m_iLineupRHCanonKing+=15;
		}

		for(i=0;i<m_uiCurBigChessCnt[R_HORSE];i++)
		{
			m_iLineupRVCanonKing+=10;
			m_iLineupRHCanonKing+=10;
		}

		for(i=0;i<m_uiCurBigChessCnt[R_CANON];i++)
		{
			m_iLineupRVCanonKing+=10;
			m_iLineupRHCanonKing+=10;
		}

		for(i=0;i<m_uiCurBigChessCnt[R_PAWN];i++)
		{
			m_iLineupRVCanonKing+=5;
			m_iLineupRHCanonKing+=5;
		}
		break;
	}
	//------------------------------------

	//---------------�Է���˫��������£��˵ļ�ֵ���Ա���ļ�ֵ��---------------
	//��
	if(m_iChessCnt[B_CAR]==2 && m_iChessCnt[B_CANON]==0)
	{
		//ʿ
		m_iChessBaseValue[R_BISHOP][9][3]=275;
		m_iChessBaseValue[R_BISHOP][9][5]=275;
		m_iChessBaseValue[R_BISHOP][8][4]=285;
		m_iChessBaseValue[R_BISHOP][7][3]=265;
		m_iChessBaseValue[R_BISHOP][7][5]=265;

		//��
		m_iChessBaseValue[R_ELEPHANT][5][2]=225;
		m_iChessBaseValue[R_ELEPHANT][5][6]=225;
		m_iChessBaseValue[R_ELEPHANT][7][0]=215;
		m_iChessBaseValue[R_ELEPHANT][7][4]=235;
		m_iChessBaseValue[R_ELEPHANT][7][8]=215;
		m_iChessBaseValue[R_ELEPHANT][9][2]=215;
		m_iChessBaseValue[R_ELEPHANT][9][6]=215;
	}
	else
	{
		//ʿ
		m_iChessBaseValue[R_BISHOP][9][3]=250;
		m_iChessBaseValue[R_BISHOP][9][5]=250;
		m_iChessBaseValue[R_BISHOP][8][4]=260;
		m_iChessBaseValue[R_BISHOP][7][3]=240;
		m_iChessBaseValue[R_BISHOP][7][5]=240;

		//��
		m_iChessBaseValue[R_ELEPHANT][5][2]=250;
		m_iChessBaseValue[R_ELEPHANT][5][6]=250;
		m_iChessBaseValue[R_ELEPHANT][7][0]=240;
		m_iChessBaseValue[R_ELEPHANT][7][4]=260;
		m_iChessBaseValue[R_ELEPHANT][7][8]=240;
		m_iChessBaseValue[R_ELEPHANT][9][2]=240;
		m_iChessBaseValue[R_ELEPHANT][9][6]=240;
	}

	//��
	if(m_iChessCnt[R_CAR]==2 && m_iChessCnt[R_CANON]==0)
	{
		//ʿ
		m_iChessBaseValue[B_BISHOP][0][3]=275;
		m_iChessBaseValue[B_BISHOP][0][5]=275;
		m_iChessBaseValue[B_BISHOP][1][4]=285;
		m_iChessBaseValue[B_BISHOP][2][3]=265;
		m_iChessBaseValue[B_BISHOP][2][5]=265;

		//��
		m_iChessBaseValue[B_ELEPHANT][0][2]=225;
		m_iChessBaseValue[B_ELEPHANT][0][6]=225;
		m_iChessBaseValue[B_ELEPHANT][2][0]=215;
		m_iChessBaseValue[B_ELEPHANT][2][4]=235;
		m_iChessBaseValue[B_ELEPHANT][2][8]=215;
		m_iChessBaseValue[B_ELEPHANT][4][2]=215;
		m_iChessBaseValue[B_ELEPHANT][4][6]=215;
	}
	else
	{
		//ʿ
		m_iChessBaseValue[B_BISHOP][0][3]=250;
		m_iChessBaseValue[B_BISHOP][0][5]=250;
		m_iChessBaseValue[B_BISHOP][1][4]=260;
		m_iChessBaseValue[B_BISHOP][2][3]=240;
		m_iChessBaseValue[B_BISHOP][2][5]=240;

		//��
		m_iChessBaseValue[B_ELEPHANT][0][2]=250;
		m_iChessBaseValue[B_ELEPHANT][0][6]=250;
		m_iChessBaseValue[B_ELEPHANT][2][0]=240;
		m_iChessBaseValue[B_ELEPHANT][2][4]=260;
		m_iChessBaseValue[B_ELEPHANT][2][8]=240;
		m_iChessBaseValue[B_ELEPHANT][4][2]=240;
		m_iChessBaseValue[B_ELEPHANT][4][6]=240;
	}
	//--------------------------------------------------------------------------

	//---------------�Է���˫�ڵ�����£�˫��ļ�ֵ���Ա�˫�˸�---------------
	//��
	if(m_iChessCnt[B_CAR]==0 && m_iChessCnt[B_CANON]==2)
	{
		//ʿ
		m_iChessBaseValue[R_BISHOP][9][3]=225;
		m_iChessBaseValue[R_BISHOP][9][5]=225;
		m_iChessBaseValue[R_BISHOP][8][4]=235;
		m_iChessBaseValue[R_BISHOP][7][3]=215;
		m_iChessBaseValue[R_BISHOP][7][5]=215;

		//��
		m_iChessBaseValue[R_ELEPHANT][5][2]=275;
		m_iChessBaseValue[R_ELEPHANT][5][6]=275;
		m_iChessBaseValue[R_ELEPHANT][7][0]=265;
		m_iChessBaseValue[R_ELEPHANT][7][4]=285;
		m_iChessBaseValue[R_ELEPHANT][7][8]=265;
		m_iChessBaseValue[R_ELEPHANT][9][2]=265;
		m_iChessBaseValue[R_ELEPHANT][9][6]=265;
	}
	else
	{
		//ʿ
		m_iChessBaseValue[R_BISHOP][9][3]=250;
		m_iChessBaseValue[R_BISHOP][9][5]=250;
		m_iChessBaseValue[R_BISHOP][8][4]=260;
		m_iChessBaseValue[R_BISHOP][7][3]=240;
		m_iChessBaseValue[R_BISHOP][7][5]=240;

		//��
		m_iChessBaseValue[R_ELEPHANT][5][2]=250;
		m_iChessBaseValue[R_ELEPHANT][5][6]=250;
		m_iChessBaseValue[R_ELEPHANT][7][0]=240;
		m_iChessBaseValue[R_ELEPHANT][7][4]=260;
		m_iChessBaseValue[R_ELEPHANT][7][8]=240;
		m_iChessBaseValue[R_ELEPHANT][9][2]=240;
		m_iChessBaseValue[R_ELEPHANT][9][6]=240;
	}

	//��
	if(m_iChessCnt[R_CAR]==2 && m_iChessCnt[R_CANON]==0)
	{
		//ʿ
		m_iChessBaseValue[B_BISHOP][0][3]=225;
		m_iChessBaseValue[B_BISHOP][0][5]=225;
		m_iChessBaseValue[B_BISHOP][1][4]=235;
		m_iChessBaseValue[B_BISHOP][2][3]=215;
		m_iChessBaseValue[B_BISHOP][2][5]=215;

		//��
		m_iChessBaseValue[B_ELEPHANT][0][2]=275;
		m_iChessBaseValue[B_ELEPHANT][0][6]=275;
		m_iChessBaseValue[B_ELEPHANT][2][0]=265;
		m_iChessBaseValue[B_ELEPHANT][2][4]=285;
		m_iChessBaseValue[B_ELEPHANT][2][8]=265;
		m_iChessBaseValue[B_ELEPHANT][4][2]=265;
		m_iChessBaseValue[B_ELEPHANT][4][6]=265;
	}
	else
	{
		//ʿ
		m_iChessBaseValue[B_BISHOP][0][3]=250;
		m_iChessBaseValue[B_BISHOP][0][5]=250;
		m_iChessBaseValue[B_BISHOP][1][4]=260;
		m_iChessBaseValue[B_BISHOP][2][3]=240;
		m_iChessBaseValue[B_BISHOP][2][5]=240;

		//��
		m_iChessBaseValue[B_ELEPHANT][0][2]=250;
		m_iChessBaseValue[B_ELEPHANT][0][6]=250;
		m_iChessBaseValue[B_ELEPHANT][2][0]=240;
		m_iChessBaseValue[B_ELEPHANT][2][4]=260;
		m_iChessBaseValue[B_ELEPHANT][2][8]=240;
		m_iChessBaseValue[B_ELEPHANT][4][2]=240;
		m_iChessBaseValue[B_ELEPHANT][4][6]=240;
	}
	//------------------------------------------------------------------------

	//-------------�����оֺ���Ĺ�����ǿ���ڵĹ��Ƽ���-------------
//	switch(m_uiCurChessAge)
//	{
//	case CA_START:
//		m_iAttackValue[B_HORSE]=30;
//		m_iAttackValue[R_HORSE]=30;
//
//		m_iAttackValue[B_CANON]=10;
//		m_iAttackValue[R_CANON]=10;
//		break;
//
//	case CA_MID:
//		m_iAttackValue[B_HORSE]=50;
//		m_iAttackValue[R_HORSE]=50;
//
//		m_iAttackValue[B_CANON]=20;
//		m_iAttackValue[R_CANON]=20;
//		break;
//
//	case CA_END:
//		m_iAttackValue[B_HORSE]=60;
//		m_iAttackValue[R_HORSE]=60;
//
//		m_iAttackValue[B_CANON]=15;
//		m_iAttackValue[R_CANON]=15;
//
//		//�оֱ���������
//		for(i=1;i<7;i++)
//		{
//			for(j=0;j<9;j++)
//				m_iChessBaseValue[R_PAWN][i][j]+=50;
//		}
//		for(i=3;i<9;i++)
//		{
//			for(j=0;j<9;j++)
//				m_iChessBaseValue[B_PAWN][i][j]+=50;
//		}
//		break;
//	}
	//------------------------------------------------------------------

	//------------��������ʱ�������������һ��λ�õĳ���ֵ���Ը�------------
	//��
	if(byChessBoard[7][2]==R_HORSE)
	{
		m_iChessBaseValue[B_CAR][6][1]=1350;
		m_iChessBaseValue[B_CAR][6][2]=1400;
	}
	else
	{
		m_iChessBaseValue[B_CAR][6][1]=1300;
		m_iChessBaseValue[B_CAR][6][2]=1300;
	}
	if(byChessBoard[7][6]==R_HORSE)
	{
		m_iChessBaseValue[B_CAR][6][7]=1350;
		m_iChessBaseValue[B_CAR][6][6]=1400;
	}
	else
	{
		m_iChessBaseValue[B_CAR][6][7]=1300;
		m_iChessBaseValue[B_CAR][6][6]=1300;
	}

	//��
	if(byChessBoard[2][2]==B_HORSE)
	{
		m_iChessBaseValue[R_CAR][3][1]=1350;
		m_iChessBaseValue[R_CAR][3][2]=1400;
	}
	else
	{
		m_iChessBaseValue[R_CAR][3][1]=1300;
		m_iChessBaseValue[R_CAR][3][2]=1300;
	}
	if(byChessBoard[2][6]==B_HORSE)
	{
		m_iChessBaseValue[R_CAR][3][7]=1350;
		m_iChessBaseValue[R_CAR][3][6]=1400;
	}
	else
	{
		m_iChessBaseValue[R_CAR][3][7]=1300;
		m_iChessBaseValue[R_CAR][3][6]=1300;
	}
	//----------------------------------------------------------------------

	//-------------------�Բ���-------------------
	//��
	switch(m_iChessCnt[B_CAR])
	{
	case 0:
		m_iLineupBLieSlotHorse=100;
		break;

	case 1:
		m_iLineupBLieSlotHorse=300;
		break;

	case 2:
		m_iLineupBLieSlotHorse=500;
	}

	//��
	switch(m_iChessCnt[R_CAR])
	{
	case 0:
		m_iLineupRLieSlotHorse=100;
		break;

	case 1:
		m_iLineupRLieSlotHorse=300;
		break;

	case 2:
		m_iLineupRLieSlotHorse=500;
	}
	//--------------------------------------------

	//---------------��˫��������¹½���ֵ����---------------
	//��
	if(m_iChessCnt[B_CAR]<2)
		m_iLineupBNoProtectKing=-200;
	else
		m_iLineupBNoProtectKing=-600;

	//��
	if(m_iChessCnt[R_CAR]<2)
		m_iLineupRNoProtectKing=-200;
	else
		m_iLineupRNoProtectKing=-600;
	//--------------------------------------------------------

	//--------------˫���Թ½�ϵ��--------------
	//��
	switch(m_iChessCnt[R_CAR])
	{
	case 2:
		m_iLineupBDblCarLoneKing=400;		       //˫���Թ½�
		m_iLineupBDblCarLoneKingElephant=350;      //˫���Թ½�һ��
		m_iLineupBDblCarLoneKing2Elephant=300;	   //˫���Թ½�����
		m_iLineupBDblCarLoneKingBishop=350;		   //˫���Թ½�һʿ
		m_iLineupBDblCarLoneKingBishopElephant=300;//˫���Թ½�һʿһ��
		break;

	case 1:
		m_iLineupBDblCarLoneKing=1000;		       //˫���Թ½�
		m_iLineupBDblCarLoneKingElephant=950;      //˫���Թ½�һ��
		m_iLineupBDblCarLoneKing2Elephant=900;	   //˫���Թ½�����
		m_iLineupBDblCarLoneKingBishop=950;		   //˫���Թ½�һʿ
		m_iLineupBDblCarLoneKingBishopElephant=900;//˫���Թ½�һʿһ��
		break;

	case 0:
		m_iLineupBDblCarLoneKing=1500;		        //˫���Թ½�
		m_iLineupBDblCarLoneKingElephant=1450;      //˫���Թ½�һ��
		m_iLineupBDblCarLoneKing2Elephant=1400;	    //˫���Թ½�����
		m_iLineupBDblCarLoneKingBishop=1450;		//˫���Թ½�һʿ
		m_iLineupBDblCarLoneKingBishopElephant=1400;//˫���Թ½�һʿһ��
		break;
	}

	//��
	switch(m_iChessCnt[B_CAR])
	{
	case 2:
		m_iLineupRDblCarLoneKing=400;		       //˫���Թ½�
		m_iLineupRDblCarLoneKingElephant=350;      //˫���Թ½�һ��
		m_iLineupRDblCarLoneKing2Elephant=300;	   //˫���Թ½�����
		m_iLineupRDblCarLoneKingBishop=350;		   //˫���Թ½�һʿ
		m_iLineupRDblCarLoneKingBishopElephant=300;//˫���Թ½�һʿһ��
		break;

	case 1:
		m_iLineupRDblCarLoneKing=1000;		       //˫���Թ½�
		m_iLineupRDblCarLoneKingElephant=950;      //˫���Թ½�һ��
		m_iLineupRDblCarLoneKing2Elephant=900;	   //˫���Թ½�����
		m_iLineupRDblCarLoneKingBishop=950;		   //˫���Թ½�һʿ
		m_iLineupRDblCarLoneKingBishopElephant=900;//˫���Թ½�һʿһ��
		break;

	case 0:
		m_iLineupRDblCarLoneKing=1500;		        //˫���Թ½�
		m_iLineupRDblCarLoneKingElephant=1450;      //˫���Թ½�һ��
		m_iLineupRDblCarLoneKing2Elephant=1400;	    //˫���Թ½�����
		m_iLineupRDblCarLoneKingBishop=1450;		//˫���Թ½�һʿ
		m_iLineupRDblCarLoneKingBishopElephant=1400;//˫���Թ½�һʿһ��
		break;
	}
	//------------------------------------------

	//-------------------������-------------------
	//��
	m_iLineupBLieMidHorse=0;
	for(i=0;i<m_vectorCurChessPos[R_CAR].size();i++)
	{
		if(m_vectorCurChessPos[R_CAR][i].x<=4)
			m_iLineupBLieMidHorse+=-50;
		else
			m_iLineupBLieMidHorse+=-30;
	}
	for(i=0;i<m_vectorCurChessPos[R_PAWN].size();i++)
	{
		if(m_vectorCurChessPos[R_PAWN][i].x<=1)
			m_iLineupBLieMidHorse+=-25;
		else
		{
			if(m_vectorCurChessPos[R_PAWN][i].x<=3)
				m_iLineupBLieMidHorse+=-15;
			else
			{
				if(m_vectorCurChessPos[R_PAWN][i].x<=4)
					m_iLineupBLieMidHorse+=-10;
			}
		}
	}

	//��
	m_iLineupRLieMidHorse=0;
	for(i=0;i<m_vectorCurChessPos[B_CAR].size();i++)
	{
		if(m_vectorCurChessPos[B_CAR][i].x>=5)
			m_iLineupRLieMidHorse+=-50;
		else
			m_iLineupRLieMidHorse+=-30;
	}
	for(i=0;i<m_vectorCurChessPos[B_PAWN].size();i++)
	{
		if(m_vectorCurChessPos[B_PAWN][i].x>=8)
			m_iLineupRLieMidHorse+=-25;
		else
		{
			if(m_vectorCurChessPos[B_PAWN][i].x>=6)
				m_iLineupRLieMidHorse+=-15;
			else
			{
				if(m_vectorCurChessPos[B_PAWN][i].x>=5)
					m_iLineupRLieMidHorse+=-10;
			}
		}
	}
	//--------------------------------------------

	//----------------���ݸ��������������ĳ��ļ�ֵ----------------
	//��
	if(!m_iChessCnt[R_BISHOP])
	{
		if(m_uiAllRChessCnt==1)
			m_iLineupBLieMidCar=m_iLineupBLieMidCar_LoneKing;
		else
		{
			if(m_uiCurBigChessCnt[B_CAR]==2)
				m_iLineupBLieMidCar=m_iLineupBLieMidCar_BigCar;
			else
			{
				if(m_iChessCnt[B_CAR]==2)
					m_iLineupBLieMidCar=m_iLineupBLieMidCar_Car;
				else
				{
					m_iLineupBLieMidCar=m_iLineupBLieMidCar_Horse[m_uiCurBigChessCnt[B_HORSE]];
					iSize=m_vectorCurChessPos[B_PAWN].size();
					for(i=0;i<iSize;i++)
					{
						if(m_vectorCurChessPos[B_PAWN][i].x>=5 && m_vectorCurChessPos[B_PAWN][i].x<=8)
						{
							iDist=abs(7-m_vectorCurChessPos[B_PAWN][i].x)+abs(m_vectorCurChessPos[B_PAWN][i].y-4);
							if(iDist==0)
								m_iLineupBLieMidCar+=20;
							else
							{
								if(iDist<=2)
									m_iLineupBLieMidCar+=10;
								else
								{
									if(iDist<=4)
										m_iLineupBLieMidCar+=5;
								}
							}
						}
					}
					if(!m_iLineupBLieMidCar)
					{
						if(m_iChessCnt[B_BISHOP]>=1)
							m_iLineupBLieMidCar=m_iLineupBLieMidCar_CanonBishop;
					}
				}
			}
		}
	}

	//��
	if(!m_iChessCnt[B_BISHOP])
	{
		if(m_uiAllBChessCnt==1)
			m_iLineupRLieMidCar=m_iLineupRLieMidCar_LoneKing;
		else
		{
			if(m_uiCurBigChessCnt[R_CAR]==2)
				m_iLineupRLieMidCar=m_iLineupRLieMidCar_BigCar;
			else
			{
				if(m_iChessCnt[R_CAR]==2)
					m_iLineupRLieMidCar=m_iLineupRLieMidCar_Car;
				else
				{
					m_iLineupRLieMidCar=m_iLineupRLieMidCar_Horse[m_uiCurBigChessCnt[R_HORSE]];
					iSize=m_vectorCurChessPos[R_PAWN].size();
					for(i=0;i<iSize;i++)
					{
						if(m_vectorCurChessPos[R_PAWN][i].x>=1 && m_vectorCurChessPos[R_PAWN][i].x<=4)
						{
							iDist=abs(2-m_vectorCurChessPos[R_PAWN][i].x)+abs(m_vectorCurChessPos[R_PAWN][i].y-4);
							if(iDist==0)
								m_iLineupRLieMidCar+=20;
							else
							{
								if(iDist<=2)
									m_iLineupRLieMidCar+=10;
								else
								{
									if(iDist<=4)
										m_iLineupRLieMidCar+=5;
								}
							}
						}
					}
					if(!m_iLineupRLieMidCar)
					{
						if(m_iChessCnt[R_BISHOP]>=1)
							m_iLineupRLieMidCar=m_iLineupRLieMidCar_CanonBishop;
					}
				}
			}
		}
	}
	//---------------------------------------------------------------------

	//----------------���ݸ��������������ı��ļ�ֵ----------------
	//��
	if(!m_iChessCnt[R_BISHOP])
	{
		if(m_uiAllRChessCnt==1)
			m_iLineupBLieMidPawn=m_iLineupBLieMidPawn_LoneKing;
		else
		{
			if(m_uiCurBigChessCnt[B_CAR]==2)
				m_iLineupBLieMidPawn=m_iLineupBLieMidPawn_BigCar;
			else
			{
				if(m_iChessCnt[B_CAR]==2)
					m_iLineupBLieMidPawn=m_iLineupBLieMidPawn_Car;
				else
				{
					m_iLineupBLieMidPawn=m_iLineupBLieMidPawn_Horse[m_uiCurBigChessCnt[B_HORSE]];
					iSize=m_vectorCurChessPos[B_PAWN].size();
					for(i=0;i<iSize;i++)
					{
						if(m_vectorCurChessPos[B_PAWN][i].x>=5 && m_vectorCurChessPos[B_PAWN][i].x<=8)
						{
							iDist=abs(7-m_vectorCurChessPos[B_PAWN][i].x)+abs(m_vectorCurChessPos[B_PAWN][i].y-4);
							if(iDist==0)
								m_iLineupBLieMidPawn+=20;
							else
							{
								if(iDist<=2)
									m_iLineupBLieMidPawn+=10;
								else
								{
									if(iDist<=4)
										m_iLineupBLieMidPawn+=5;
								}
							}
						}
					}
					if(!m_iLineupBLieMidPawn)
					{
						if(m_iChessCnt[B_BISHOP]>=1)
							m_iLineupBLieMidPawn=m_iLineupBLieMidPawn_CanonBishop;
					}
				}
			}
		}
	}

	//��
	if(!m_iChessCnt[B_BISHOP])
	{
		if(m_uiAllBChessCnt==1)
			m_iLineupRLieMidPawn=m_iLineupRLieMidPawn_LoneKing;
		else
		{
			if(m_uiCurBigChessCnt[R_CAR]==2)
				m_iLineupRLieMidPawn=m_iLineupRLieMidPawn_BigCar;
			else
			{
				if(m_iChessCnt[R_CAR]==2)
					m_iLineupRLieMidPawn=m_iLineupRLieMidPawn_Car;
				else
				{
					m_iLineupRLieMidPawn=m_iLineupRLieMidPawn_Horse[m_uiCurBigChessCnt[R_HORSE]];
					iSize=m_vectorCurChessPos[R_PAWN].size();
					for(i=0;i<iSize;i++)
					{
						if(m_vectorCurChessPos[R_PAWN][i].x>=1 && m_vectorCurChessPos[R_PAWN][i].x<=4)
						{
							iDist=abs(2-m_vectorCurChessPos[R_PAWN][i].x)+abs(m_vectorCurChessPos[R_PAWN][i].y-4);
							if(iDist==0)
								m_iLineupRLieMidPawn+=20;
							else
							{
								if(iDist<=2)
									m_iLineupRLieMidPawn+=10;
								else
								{
									if(iDist<=4)
										m_iLineupRLieMidPawn+=5;
								}
							}
						}
					}
					if(!m_iLineupRLieMidPawn)
					{
						if(m_iChessCnt[R_BISHOP]>=1)
							m_iLineupRLieMidPawn=m_iLineupRLieMidPawn_CanonBishop;
					}
				}
			}
		}
	}
	//---------------------------------------------------------------------

	//---------------³ç��---------------
	//��
	if(m_iChessCnt[B_CAR]<2 || m_iChessCnt[B_CANON]<2 || m_iChessCnt[B_HORSE]<2 || m_iChessCnt[B_PAWN]<3)
		m_iLineupBImpertinenceHorse=0;
	else
		m_iLineupBImpertinenceHorse=-80;

	//��
	if(m_iChessCnt[R_CAR]<2 || m_iChessCnt[R_CANON]<2 || m_iChessCnt[R_HORSE]<2 || m_iChessCnt[R_PAWN]<3)
		m_iLineupRImpertinenceHorse=0;
	else
		m_iLineupRImpertinenceHorse=-80;
	//------------------------------------

	//----------------�Բ���ͳ�,������Ĳ�λ:���Գ鳵����----------------
	//��
	m_iLineupBLieSlotHorseCar=0;
	for(i=0;i<m_iChessCnt[R_CAR];i++)
		m_iLineupBLieSlotHorseCar+=60;
	for(i=0;i<m_iChessCnt[R_HORSE];i++)
		m_iLineupBLieSlotHorseCar+=30;
	for(i=0;i<m_iChessCnt[R_CANON];i++)
		m_iLineupBLieSlotHorseCar+=30;
	for(i=0;i<m_iChessCnt[R_ELEPHANT];i++)
		m_iLineupBLieSlotHorseCar+=15;
	for(i=0;i<m_iChessCnt[R_BISHOP];i++)
		m_iLineupBLieSlotHorseCar+=15;
	for(i=0;i<m_uiCurBigChessCnt[R_PAWN];i++)
		m_iLineupBLieSlotHorseCar+=15;

	//��
	m_iLineupRLieSlotHorseCar=0;
	for(i=0;i<m_iChessCnt[B_CAR];i++)
		m_iLineupRLieSlotHorseCar+=60;
	for(i=0;i<m_iChessCnt[B_HORSE];i++)
		m_iLineupRLieSlotHorseCar+=30;
	for(i=0;i<m_iChessCnt[B_CANON];i++)
		m_iLineupRLieSlotHorseCar+=30;
	for(i=0;i<m_iChessCnt[B_ELEPHANT];i++)
		m_iLineupRLieSlotHorseCar+=15;
	for(i=0;i<m_iChessCnt[B_BISHOP];i++)
		m_iLineupRLieSlotHorseCar+=15;
	for(i=0;i<m_uiCurBigChessCnt[B_PAWN];i++)
		m_iLineupRLieSlotHorseCar+=15;
	//--------------------------------------------------------------------

	//---------------�ںͽ�֮���������---------------
	//��
	m_iLineupBCannonKingLieMidHorse=0;
	iSize=m_vectorCurChessPos[B_CAR].size();
	for(i=0;i<iSize;i++)
	{
		if((m_vectorCurChessPos[B_CAR][i].y==3 && byChessBoard[0][3]==B_KING) ||
		   (m_vectorCurChessPos[B_CAR][i].y==5 && byChessBoard[0][5]==B_KING))
		   m_iLineupBCannonKingLieMidHorse+=200;
		else
		{
			if(m_vectorCurChessPos[B_CAR][i].x>=5)
				m_iLineupBCannonKingLieMidHorse+=100;
			else
				m_iLineupBCannonKingLieMidHorse+=80;
		}
	}
	iSize=m_vectorCurChessPos[B_PAWN].size();
	for(i=0;i<iSize;i++)
	{
		if(m_vectorCurChessPos[B_PAWN][i].x>=5 && m_vectorCurChessPos[B_PAWN][i].x<=7)
		{
			iDist=7-m_vectorCurChessPos[B_PAWN][i].x+abs(m_vectorCurChessPos[B_PAWN][i].y-4);
			if(iDist==0)
				m_iLineupBCannonKingLieMidHorse+=20;
			else
			{
				if(iDist<=2)
					m_iLineupBCannonKingLieMidHorse+=10;
				else
				{
					if(iDist<=4)
						m_iLineupBCannonKingLieMidHorse+=5;
				}
			}
		}
		if((m_vectorCurChessPos[B_PAWN][i].y==3 && byChessBoard[0][3]==B_KING) ||
		   (m_vectorCurChessPos[B_PAWN][i].y==5 && byChessBoard[0][5]==B_KING))
		   m_iLineupBCannonKingLieMidHorse+=10;
	}

	//��
	m_iLineupRCannonKingLieMidHorse=0;
	iSize=m_vectorCurChessPos[R_CAR].size();
	for(i=0;i<iSize;i++)
	{
		if((m_vectorCurChessPos[R_CAR][i].y==3 && byChessBoard[9][3]==R_KING) ||
		   (m_vectorCurChessPos[R_CAR][i].y==5 && byChessBoard[9][5]==R_KING))
		   m_iLineupRCannonKingLieMidHorse+=200;
		else
		{
			if(m_vectorCurChessPos[R_CAR][i].x<=4)
				m_iLineupRCannonKingLieMidHorse+=100;
			else
				m_iLineupRCannonKingLieMidHorse+=80;
		}
	}
	iSize=m_vectorCurChessPos[R_PAWN].size();
	for(i=0;i<iSize;i++)
	{
		if(m_vectorCurChessPos[R_PAWN][i].x<=4 && m_vectorCurChessPos[R_PAWN][i].x>=2)
		{
			iDist=m_vectorCurChessPos[R_PAWN][i].x-2+abs(m_vectorCurChessPos[R_PAWN][i].y-4);
			if(iDist==0)
				m_iLineupRCannonKingLieMidHorse+=20;
			else
			{
				if(iDist<=2)
					m_iLineupRCannonKingLieMidHorse+=10;
				else
				{
					if(iDist<=4)
						m_iLineupRCannonKingLieMidHorse+=5;
				}
			}
		}
		if((m_vectorCurChessPos[R_PAWN][i].y==3 && byChessBoard[9][3]==R_KING) ||
		   (m_vectorCurChessPos[R_PAWN][i].y==5 && byChessBoard[9][5]==R_KING))
		   m_iLineupRCannonKingLieMidHorse+=10;
	}
	//------------------------------------------------

	//---------------------�����---------------------
	//��
	if(!m_iChessCnt[R_BISHOP])
	{
		if(m_uiAllRChessCnt==1)
			m_iLineupBBigAngleHorse=m_iLineupBBigAngleHorse_LoneKing;
		else
		{
			if(m_uiCurBigChessCnt[B_CAR]==2)
				m_iLineupBBigAngleHorse=m_iLineupBBigAngleHorse_BigCar;
			else
			{
				if(m_iChessCnt[B_CAR]==2)
					m_iLineupBBigAngleHorse=m_iLineupBBigAngleHorse_Car;
				else
				{
					m_iLineupBBigAngleHorse=m_iLineupBBigAngleHorse_Horse[m_uiCurBigChessCnt[B_HORSE]];
					iSize=m_vectorCurChessPos[B_PAWN].size();
					for(i=0;i<iSize;i++)
					{
						if(m_vectorCurChessPos[B_PAWN][i].x>=5 && m_vectorCurChessPos[B_PAWN][i].x<=8)
						{
							iDist=abs(7-m_vectorCurChessPos[B_PAWN][i].x)+abs(m_vectorCurChessPos[B_PAWN][i].y-4);
							if(iDist==0)
								m_iLineupBBigAngleHorse+=20;
							else
							{
								if(iDist<=2)
									m_iLineupBBigAngleHorse+=10;
								else
								{
									if(iDist<=4)
										m_iLineupBBigAngleHorse+=5;
								}
							}
						}
					}
					if(!m_iLineupBBigAngleHorse)
					{
						if(m_iChessCnt[B_BISHOP]>=1)
							m_iLineupBBigAngleHorse=m_iLineupBBigAngleHorse_CanonBishop;
					}
				}
			}
		}
	}

	//��
	if(!m_iChessCnt[B_BISHOP])
	{
		if(m_uiAllBChessCnt==1)
			m_iLineupRBigAngleHorse=m_iLineupRBigAngleHorse_LoneKing;
		else
		{
			if(m_uiCurBigChessCnt[R_CAR]==2)
				m_iLineupRBigAngleHorse=m_iLineupRBigAngleHorse_BigCar;
			else
			{
				if(m_iChessCnt[R_CAR]==2)
					m_iLineupRBigAngleHorse=m_iLineupRBigAngleHorse_Car;
				else
				{
					m_iLineupRBigAngleHorse=m_iLineupRBigAngleHorse_Horse[m_uiCurBigChessCnt[R_HORSE]];
					iSize=m_vectorCurChessPos[R_PAWN].size();
					for(i=0;i<iSize;i++)
					{
						if(m_vectorCurChessPos[R_PAWN][i].x>=5 && m_vectorCurChessPos[R_PAWN][i].x<=8)
						{
							iDist=7-m_vectorCurChessPos[R_PAWN][i].x+abs(m_vectorCurChessPos[R_PAWN][i].y-4);
							if(iDist==0)
								m_iLineupRBigAngleHorse+=20;
							else
							{
								if(iDist<=2)
									m_iLineupRBigAngleHorse+=10;
								else
								{
									if(iDist<=4)
										m_iLineupRBigAngleHorse+=5;
								}
							}
						}
					}
					if(!m_iLineupRBigAngleHorse)
					{
						if(m_iChessCnt[R_BISHOP]>=1)
							m_iLineupRBigAngleHorse=m_iLineupRBigAngleHorse_CanonBishop;
					}
				}
			}
		}
	}
	//------------------------------------------------

	//----------------�½�:����ԭλ----------------
	//��
	m_iLineupBNoProtectKing=m_iLineupBNoProtectKing_CHC[m_uiCurBigChessCnt[R_CAR]][m_uiCurBigChessCnt[R_HORSE]][m_uiCurBigChessCnt[R_CANON]];
	iSize=m_vectorCurChessPos[R_PAWN].size();
	for(i=0;i<iSize;i++)
	{
		if(m_vectorCurChessPos[R_PAWN][i].x>=2 && m_vectorCurChessPos[R_PAWN][i].x<=4)
		{
			iDist=m_vectorCurChessPos[R_PAWN][i].x-2+abs(m_vectorCurChessPos[R_PAWN][i].y-4);
			if(iDist==0)
				m_iLineupBNoProtectKing+=20;
			else
			{
				if(iDist<=2)
					m_iLineupBNoProtectKing+=10;
				else
				{
					if(iDist<=4)
						m_iLineupBNoProtectKing+=5;
				}
			}
		}
	}
	m_iLineupBNoProtectKing=-m_iLineupBNoProtectKing;

	//��
	m_iLineupRNoProtectKing=m_iLineupRNoProtectKing_CHC[m_uiCurBigChessCnt[B_CAR]][m_uiCurBigChessCnt[B_HORSE]][m_uiCurBigChessCnt[B_CANON]];
	iSize=m_vectorCurChessPos[B_PAWN].size();
	for(i=0;i<iSize;i++)
	{
		if(m_vectorCurChessPos[B_PAWN][i].x>=5 && m_vectorCurChessPos[B_PAWN][i].x<=7)
		{
			iDist=7-m_vectorCurChessPos[B_PAWN][i].x+abs(m_vectorCurChessPos[B_PAWN][i].y-4);
			if(iDist==0)
				m_iLineupRNoProtectKing+=20;
			else
			{
				if(iDist<=2)
					m_iLineupRNoProtectKing+=10;
				else
				{
					if(iDist<=4)
						m_iLineupRNoProtectKing+=5;
				}
			}
		}
	}
	m_iLineupRNoProtectKing=-m_iLineupRNoProtectKing;
	//------------------------------------

	//----------------���뵤ɳ----------------
	//��
	if(m_uiCurBigChessCnt[B_CAR])
	{
		m_iLineupBHCarCanonKing=m_iLineupBHCarCanonKing_CHC[m_uiCurBigChessCnt[B_CAR]-1][m_uiCurBigChessCnt[B_HORSE]][m_uiCurBigChessCnt[B_CANON]];
		iSize=m_vectorCurChessPos[B_PAWN].size();
		for(i=0;i<iSize;i++)
		{
			if(m_vectorCurChessPos[B_PAWN][i].x>=5 && m_vectorCurChessPos[B_PAWN][i].x<=7)
			{
				iDist=7-m_vectorCurChessPos[B_PAWN][i].x+abs(m_vectorCurChessPos[B_PAWN][i].y-4);
				if(iDist==0)
					m_iLineupBHCarCanonKing+=20;
				else
				{
					if(iDist<=2)
						m_iLineupBHCarCanonKing+=10;
					else
					{
						if(iDist<=4)
							m_iLineupBHCarCanonKing+=5;
					}
				}
			}
		}
		iSize=m_vectorCurChessPos[R_CAR].size();
		for(i=0;i<iSize;i++)
		{
			if(m_vectorCurChessPos[R_CAR][i].x<=2)
				m_iLineupBHCarCanonKing+=100;
		}
	}

	if(m_uiCurBigChessCnt[R_CAR])
	{
		m_iLineupRHCarCanonKing=m_iLineupRHCarCanonKing_CHC[m_uiCurBigChessCnt[R_CAR]-1][m_uiCurBigChessCnt[R_HORSE]][m_uiCurBigChessCnt[R_CANON]];
		iSize=m_vectorCurChessPos[R_PAWN].size();
		for(i=0;i<iSize;i++)
		{
			if(m_vectorCurChessPos[R_PAWN][i].x>=2 && m_vectorCurChessPos[R_PAWN][i].x<=4)
			{
				iDist=m_vectorCurChessPos[R_PAWN][i].x-2+abs(m_vectorCurChessPos[R_PAWN][i].y-4);
				if(iDist==0)
					m_iLineupRHCarCanonKing+=20;
				else
				{
					if(iDist<=2)
						m_iLineupRHCarCanonKing+=10;
					else
					{
						if(iDist<=4)
							m_iLineupRHCarCanonKing+=5;
					}
				}
			}
		}
		iSize=m_vectorCurChessPos[B_CAR].size();
		for(i=0;i<iSize;i++)
		{
			if(m_vectorCurChessPos[B_CAR][i].x<=2)
				m_iLineupRHCarCanonKing+=100;
		}
	}
	//----------------------------------------

	//--------------���ݽ����Ӿ������Ƽ�ֵ--------------
	//��
	//�ڳ�
	m_nAttackValue[B_CAR]=m_nAttackValue_EB[B_CAR][m_iChessCnt[R_ELEPHANT]][m_iChessCnt[R_BISHOP]];

	//����
	m_nAttackValue[B_HORSE]=m_nAttackValue_EB[B_HORSE][m_iChessCnt[R_ELEPHANT]][m_iChessCnt[R_BISHOP]];
	for(i=0;i<m_uiCurBigChessCnt[B_CAR];i++)
		m_nAttackValue[B_HORSE]+=6;

	//����
	m_nAttackValue[B_CANON]=m_nAttackValue_EB[B_CANON][m_iChessCnt[R_ELEPHANT]][m_iChessCnt[R_BISHOP]];

	//�ڱ�
	m_nAttackValue[B_PAWN]=m_nAttackValue_EB[B_PAWN][m_iChessCnt[R_ELEPHANT]][m_iChessCnt[R_BISHOP]];
	for(i=0;i<m_uiCurBigChessCnt[B_CAR];i++)
		m_nAttackValue[B_PAWN]+=5;
	
	//��
	//�쳵
	m_nAttackValue[R_CAR]=m_nAttackValue_EB[R_CAR][m_iChessCnt[B_ELEPHANT]][m_iChessCnt[B_BISHOP]];

	//����
	m_nAttackValue[R_HORSE]=m_nAttackValue_EB[R_HORSE][m_iChessCnt[B_ELEPHANT]][m_iChessCnt[B_BISHOP]];
	for(i=0;i<m_uiCurBigChessCnt[R_CAR];i++)
		m_nAttackValue[R_HORSE]+=6;

	//����
	m_nAttackValue[R_CANON]=m_nAttackValue_EB[R_CANON][m_iChessCnt[B_ELEPHANT]][m_iChessCnt[B_BISHOP]];

	//����
	m_nAttackValue[R_PAWN]=m_nAttackValue_EB[R_PAWN][m_iChessCnt[B_ELEPHANT]][m_iChessCnt[B_BISHOP]];
	for(i=0;i<m_uiCurBigChessCnt[R_CAR];i++)
		m_nAttackValue[R_PAWN]+=5;
	//--------------------------------------------------

	//---------------------�о����---------------------
	//--------------------------------------------------
}

void CBaseEveluation::UnUpdateCurChess(BYTE byChess, bool bIsRed)
{
	if(byChess==NOCHESS)
		return;

	m_iChessCnt[byChess]++;//��ǰ����������Ŀ
	m_uiAllChessCnt++;     //��ǰ��ֹ��е�������
	if(bIsRed)
	{
		m_uiAllRChessCnt++;//��ǰ��ֺ췽���е�������

		m_vectorRCurChess.push_back(byChess);//�ڷ���ǰ��ӵ�е�����

		//���º췽����������
		if(byChess==R_CAR || byChess==R_HORSE || byChess==R_CANON)
		{
			m_nMainChessCnt++; //��ǰ�����ӵ�е���������:������
			m_nRMainChessCnt++;//��ǰ��ֺ췽��ӵ�е���������:������
		}
	}
	else
	{
		m_uiAllBChessCnt++;//��ǰ��ֺڷ����е�������

		m_vectorRCurChess.push_back(byChess);//�췽��ǰ��ӵ�е�����

		//���ºڷ�����������
		if(byChess==B_CAR || byChess==B_HORSE || byChess==B_CANON)
		{
			m_nMainChessCnt++; //��ǰ�����ӵ�е���������:������
			m_nBMainChessCnt++;//��ǰ��ֺڷ���ӵ�е���������:������
		}
	}

	//�жϵ�ǰ��������׶�
	if(m_uiAllChessCnt>=28)
		m_uiCurChessAge=CA_START;
	else
	{
		if(m_nMainChessCnt>4)
			m_uiCurChessAge=CA_MID;
		else
			m_uiCurChessAge=CA_END;
	}
}

int CBaseEveluation::Multiply(int a, int b)
{
//	if(!a || !b)
//		return 0;
//
//	int n=0;
//	int iRet=0;
//
//	while((1<<n)<=a)
//		n++;
//
//	while(n>0)
//	{
//		n--;
//		if((1<<n)<=a)
//		{
//			iRet+=b<<n;
//			a-=1<<n;
//		}
//	}
//
//	return iRet;

	switch(a)
	{
	case 0:
		return 0;

	case 1:
		return b;

	case 2:
		return b<<1;

	case 6:
		return (b<<2)+(b<<1);

	case 12:
		return (b<<3)+(b<<2);

	case 15:
		return (b<<3)+(b<<2)+(b<<1)+b;
	}
	switch(b)
	{
	case 0:
		return 0;

	case 1:
		return a;

	case 2:
		return a<<1;

	case 6:
		return (a<<2)+(a<<1);

	case 12:
		return (a<<3)+(a<<2);

	case 15:
		return (a<<3)+(a<<2)+(a<<1)+a;
	}
}

void CBaseEveluation::InitVar(BYTE byChessBoard[][9])
{
	int i,j;
	CHESSMANPOS cmpTemp;

	memset(m_iChessCnt,0,15*sizeof(int));
	memset(m_uiCurBigChessCnt,0,15*sizeof(UINT));
	memset(m_uiCurLBigChessCnt,0,15*sizeof(UINT));
	memset(m_uiCurRBigChessCnt,0,15*sizeof(UINT));

	m_uiAllChessCnt=0;
	m_uiAllBChessCnt=0;
	m_uiAllRChessCnt=0;
	m_nMainChessCnt=0;
	m_nBMainChessCnt=0;
	m_nRMainChessCnt=0;

	m_vectorBCurChess.clear();
	m_vectorRCurChess.clear();
	for(i=0;i<15;i++)
		m_vectorCurChessPos[i].clear();

	for(i=0;i<10;i++)
	{
		for(j=0;j<9;j++)
		{
			if(byChessBoard[i][j]!=NOCHESS)
			{
				m_iChessCnt[byChessBoard[i][j]]++;
				m_uiAllChessCnt++;

				cmpTemp.x=i;
				cmpTemp.y=j;
				m_vectorCurChessPos[byChessBoard[i][j]].push_back(cmpTemp);

				if(IsBlack(byChessBoard[i][j]))
				{
					m_uiAllBChessCnt++;
					m_vectorBCurChess.push_back(byChessBoard[i][j]);
					if(i>=5)
					{
						m_uiCurBigChessCnt[byChessBoard[i][j]]++;
						if(j<=4)
							m_uiCurLBigChessCnt[byChessBoard[i][j]]++;
						if(j>=4)
							m_uiCurRBigChessCnt[byChessBoard[i][j]]++;
					}

					if(byChessBoard[i][j]==B_CAR || byChessBoard[i][j]==B_HORSE || byChessBoard[i][j]==B_CANON)
					{
						m_nMainChessCnt++;
						m_nBMainChessCnt++;
					}
				}
				else
				{
					m_uiAllRChessCnt++;
					m_vectorRCurChess.push_back(byChessBoard[i][j]);
					if(i<=4)
					{
						m_uiCurBigChessCnt[byChessBoard[i][j]]++;
						if(j<=4)
							m_uiCurLBigChessCnt[byChessBoard[i][j]]++;
						if(j>=4)
							m_uiCurRBigChessCnt[byChessBoard[i][j]]++;
					}

					if(byChessBoard[i][j]==R_CAR || byChessBoard[i][j]==R_HORSE || byChessBoard[i][j]==R_CANON)
					{
						m_nMainChessCnt++;
						m_nRMainChessCnt++;
					}
				}
			}
		}
	}

	//�жϵ�ǰ��������׶�
	if(m_uiAllChessCnt>=28)
		m_uiCurChessAge=CA_START;
	else
	{
		if(m_nMainChessCnt>4)
			m_uiCurChessAge=CA_MID;
		else
			m_uiCurChessAge=CA_END;
	}
}
