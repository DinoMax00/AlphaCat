// ChessDlg.h : header file
//

#if !defined(AFX_CHESSDLG_H__C9065501_927E_437F_8700_63743E930B62__INCLUDED_)
#define AFX_CHESSDLG_H__C9065501_927E_437F_8700_63743E930B62__INCLUDED_

#pragma warning(disable:4786)//��ֹwaring��Ϊ4786��warning��Ϣ

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Define.h"
#include "SetDlg.h"
#include "CoolButton.h"
#include "GradientProgressCtrl.h"
#include "HelpDlg.h"
#include "NegaScout_TT_HH.h"
#include "NegaScout_TT_HH2.h"
#include "NegaScout_TT_HH3.h"
#include "NegaScout_TT_HH4.h"
#include "NegaScout_TT_HH5.h"
#include "NegaScout_TT_HH6.h"
#include "SplashDlg.h"
#include "TipDlg.h"
#include <fstream>
#include <stack>
#include <vector>

/////////////////////////////////////////////////////////////////////////////
// CChessDlg dialog

using namespace std;

struct MOVECHESS
{
	BYTE nChessID;
	POINT ptMovePoint;
};

typedef enum{Chessing,SetChessBoarding,Previewing}Status;
typedef vector<OPENBOOKTREENODE*> MOVEVECTOR;

class CChessDlg : public CDialog
{
// Construction
public:
	CChessDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CChessDlg)
	enum { IDD = IDD_CHESS_DIALOG };
	CStatic	m_staticTip;
	CCoolButton	m_btnStop;
	CCoolButton	m_btnUndo;
	CCoolButton	m_btnRedo;
	CCoolButton	m_btnComputer;
	CListBox	m_lstChessRecord;
	CGradientProgressCtrl m_progressThink;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChessDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

private:
	void CoolBeep();
	void OnCancel();

	void ConvertChessMove(CHESSMOVE *pCm);//�ԳƵ�ת���߷�
	void LoadOpenBook(CString strFile,OPENBOOKTREENODE *pOpenBookTree,MOVEVECTOR& vectorChessMoveNode);//���ؿ��ֿ�
	void UpdateOpenBook(CString strFile,MOVEVECTOR vectorChessMoveNode,MOVEVECTOR& vectorPassNode);//���¿��ֿ�
	bool FindMoveInOB(OPENBOOKTREENODE *pOpenBookTree,CHESSMOVE& cmRival,MOVEVECTOR& vectorPassNode);//�ڿ��ֿ����Ҷ�Ӧ���߷�
	int SelBestMove(vector<float> vectorScore);//���ݸ��ڵ�ĵ÷�ѡ��ڵ�

	void DrawTipBox(CDC *pDc, CPoint point,int iBitmap);//���߷�����ʾ��
	void TransparentBlt2(HDC hdcDest,int nXOriginDest,int nYOriginDest,int nWidthDest,int nHeightDest,HDC hdcSrc,int nXOriginSrc,int nYOriginSrc,int nWidthSrc,int nHeightSrc,UINT crTransparent);
	
	CString GetMoveStr(int nFromX,int nFromY,int nToX,int nToY,int nSourceID);
										   //�����߷��õ����߷���Ӧ����������
	CString ConvertDigit2Chinese(int nNum);//ת������Ϊ����
	bool IsChessOver(int nChessSort);	   //�ж�ĳ��������������ʣ��
	void InvertChessBoard(BYTE cb[][9]);   //�Ի����˫������
	void AddChessRecord(int nFromX,int nFromY,int nToX,int nToY,int nUserChessColor,int nSourceID);
										   //��¼���岽��
	int IsGameOver(BYTE position[][9]);    //�ж���Ϸ�Ƿ����

public:
	void Think();

// Implementation
protected:
	HICON m_hIcon;
	HICON m_hUndoIcon;
	HICON m_hRedoIcon;
	HICON m_hComputerIcon;
	HICON m_hStopIcon;
	CSetDlg m_SetDlg;
	CToolTipCtrl m_tooltip;
	CSplashDlg m_splashDlg;

	//�������߳�ʱ��
	HANDLE m_hHandle;
	DWORD m_dwThreadID;

	CString m_strWelcome;
	bool m_bIsNeedUpdateOB;          //�Ƿ���Ҫ���¿��ֿ�
	bool m_bIsGameOver;              //�������Ƿ����
	int m_iWhoChess;				 //�²���˭����
	int m_nUserChessColor;		     //�û�������ɫ
	BYTE m_byChessBoard[10][9];      //�������飬������ʾ����
	BYTE m_byBackupChessBoard[10][9];//�����������飬���ڳ���ָ�
	BYTE m_byShowChessBoard[10][9];  //������ʾ������
	MOVECHESS m_MoveChess;		     //���ڱ��浱ǰ����ק�����ӵĽṹ
	POINT m_ptMoveChess;		     //���ڱ��浱ǰ����ק�����ӵ�λ��
	CBitmap m_BoardBmp;			     //bitmapͼ������ʾ����
	CImageList m_Chessman;		     //���ڻ������ӵ�ImageList����
	int m_nBoardWidth;			     //���̿��
	int m_nBoardHeight;			     //���̸߶�

	CMoveGenerator* m_pMG;   //�߷�������
	CBaseEveluation* m_pEval;//��ֵ����
	CBaseSearchEngine* m_pSE;//��������ָ��

	CPoint m_MousePoint;   //��ǰ������ڵ�
	Status m_Status;       //��ǰ״̬
	bool m_bIsThinking;    //�����Ƿ�������
	bool m_bIsBegin;       //�Ƿ��ѿ�ʼ����
	bool m_bIsNeedOpenBook;//���ֿ��Ƿ��Ѿ�ʹ�����
	bool m_bIsConvert;     //�ٿ��ֿ���ƥ���߷�ʱ�Ƿ���Ҫ�ԳƵ�ת��һ��
	int m_nWillChessColor; //��һ�����ķ���
	CHESSMOVE m_cmBestMove;//����߷�
	CHESSMOVE m_cmShow;    //Ҫ��ʾ���߷�
	UNDOMOVE m_umUndoMove; //�����߷�
	int m_iBout;		   //���µĻغ���,���л������
	int m_iResult;         //������Ľ��
	bool bAddFlag[2];      //��������Ƿ����ӵı�־
	int m_iInitDepth;	   //��ʼ�������
	
	//-------����-------
	int m_iChessSort;  //���巽ʽ
	int m_iDepthSort;  //��ȷ�ʽ
	bool m_bMoveSound; //����ʱ�Ƿ�����
	float m_fTimeLimit;//ʱ������,������ÿ������õ�ʱ��
	int m_iDepth;      //�������
	//-----------------

	stack<UNDOMOVE> m_stackUndoMove;//��¼�߷���ջ�����ڻ���
	stack<UNDOMOVE> m_stackRedoMove;//��¼�ѻ�����߷���ջ�����ڻ�ԭ

	OPENBOOKTREENODE *m_pOpenBookTree;				//���ֿ���
	OPENBOOKTREENODE *m_pOpenBookTreeBackup;		//���ֿ�������
	vector<OPENBOOKTREENODE*> m_vectorPassNode;		//����ʱʹ�õĽڵ�,�������˳�ʱ�Կ��ֿ��еĽڵ�����
	vector<OPENBOOKTREENODE*> m_vectorChessMoveNode;//���ֿ�����,�������ڵ���Ҹ��ڵ�ʱ��
	
	// Generated message map functions
	//{{AFX_MSG(CChessDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSetchessboard();
	afx_msg void OnSet();
	afx_msg void OnAbout();
	afx_msg void OnOpenfile();
	afx_msg void OnSavefile();
	afx_msg void OnScbover();
	afx_msg void OnRpawn();
	afx_msg void OnRcanon();
	afx_msg void OnRcar();
	afx_msg void OnRhorse();
	afx_msg void OnRelephant();
	afx_msg void OnRbishop();
	afx_msg void OnRking();
	afx_msg void OnBpawn();
	afx_msg void OnBcanon();
	afx_msg void OnBcar();
	afx_msg void OnBhorse();
	afx_msg void OnBelephant();
	afx_msg void OnBbishop();
	afx_msg void OnBking();
	afx_msg void OnDelete();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnClearcb();
	afx_msg void OnNewgame();
	afx_msg void OnBtncomputer();
	afx_msg void OnBtnundo();
	afx_msg void OnBtnredo();
	afx_msg void OnDblclkListchessrecord();
	afx_msg void OnBtnStop();
	afx_msg void OnSelchangeListchessrecord();
	afx_msg void OnHelp();
	afx_msg void OnInversecb();
	afx_msg void OnSave();
	afx_msg void OnExit();
	afx_msg void OnOpen();
	afx_msg void OnUpdatePreview(CCmdUI* pCmdUI);
	afx_msg void OnMakemanual();
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHESSDLG_H__C9065501_927E_437F_8700_63743E930B62__INCLUDED_)
