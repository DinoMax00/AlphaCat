// MoveGenerator.h: interface for the CMoveGenerator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOVEGENERATOR_H__946D3A1A_4027_4830_B1C7_7DA4B20391B6__INCLUDED_)
#define AFX_MOVEGENERATOR_H__946D3A1A_4027_4830_B1C7_7DA4B20391B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Define.h"

//�߷�������
class CMoveGenerator  
{
public:
	CMoveGenerator();
	virtual ~CMoveGenerator();

public:
//	static BOOL IsValidMove(BYTE byChessBoard[10][9],int nFromX,int nFromY,int nToX,int nToY,int nUserChessColor);
																	                    //�߷��Ƿ���Ч
	BOOL IsValidMove(BYTE byChessBoard[][9],int nFromX,int nFromY,int nToX,int nToY,int nUserChessColor);
	int CreatePossibleMove(BYTE byChessBoard[10][9],int nPly,int nSide,int nUserChessColor);//�������п��ܵ��߷�

protected:
	void Gen_BPawnMove(BYTE byChessBoard[][9],int i,int j,int nPly);   //����������߷�,nPly�������뵽List�ڼ���
	void Gen_RPawnMove(BYTE byChessBoard[][9], int i,int j,int nPly);  //����������߷�,nPly�������뵽List�ڼ���

	void Gen_BBishopMove(BYTE byChessBoard[][9],int i,int j,int nPly); //������ʿ���߷�,nPly�������뵽List�ڼ���
	void Gen_RBishopMove(BYTE byChessBoard[][9],int i,int j,int nPly); //������ʿ���߷�,nPly�������뵽List�ڼ���

	void Gen_CarMove(BYTE byChessBoard[][9], int i,int j,int nPly);    //���������߷�,nPly�������뵽List�ڼ���
	void Gen_HorseMove(BYTE byChessBoard[][9], int i,int j,int nPly);  //��������߷�,nPly�������뵽List�ڼ���
	void Gen_ElephantMove(BYTE byChessBoard[][9],int i,int j,int nPly);//������/����߷�,nPly�������뵽List�ڼ���

	void Gen_KingMove(BYTE byChessBoard[][9],int i,int j,int nPly);	 //������/˧���߷�,nPly�������뵽List�ڼ���
	void Gen_CanonMove(BYTE byChessBoard[][9],int i,int j,int nPly);   //�����ڵ��߷�,nPly�������뵽List�ڼ���

	int AddMove(int nFromX,int nToX,int nFromY,int nToY,int nPly,int nChessID);
																	 //��m_MoveList�в���һ���߷�,nPly�������뵽List�ڼ���

public:
	CHESSMOVE m_MoveList[50][80];//���CreatePossibleMove�����������߷��Ķ���

protected:
	int m_nMoveCount;//��¼m_MoveList���߷�������
	int m_nUserChessColor;
};

#endif // !defined(AFX_MOVEGENERATOR_H__946D3A1A_4027_4830_B1C7_7DA4B20391B6__INCLUDED_)
