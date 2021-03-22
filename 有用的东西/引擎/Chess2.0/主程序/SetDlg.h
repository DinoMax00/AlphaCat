#if !defined(AFX_NEWGAMEDLG_H__CEFCDE83_32F2_41CA_A087_070499127128__INCLUDED_)
#define AFX_NEWGAMEDLG_H__CEFCDE83_32F2_41CA_A087_070499127128__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewGameDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetDlg dialog

class CSetDlg : public CDialog
{
// Construction
public:
	CSetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetDlg)
	enum { IDD = IDD_SET };
	CButton	m_btnOk;
	CButton	m_btnCancel;
	CSpinButtonCtrl	m_SetPly;
	BOOL	m_bMoveSound;
	float	m_fTimeLimit;
	int		m_iDepth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	int GetUserChessColor(){return m_nUserChessColor;};//ȡ�û�ѡ�е�������ɫ
	int GetChessSort(){return m_nChessSort;};          //ȡ���巽ʽ
	int GetDepth(){return m_iDepth;};				   //�õ����
	void SetDepth(int iDepth){m_iDepth=iDepth;};       //�����������
	float GetTimeLimit(){return m_fTimeLimit;};        //�õ�����ʱ��
	bool GetMoveSount(){return m_bMoveSound;};         //�õ�����ʱ�Ƿ�����

protected:
	CString m_strTip;     //��ʾ��Ϣ
	int m_nUserChessColor;//�û�������ɫ
	int m_nChessSort;     //���巽ʽ

// Implementation
protected:
	HICON m_hSetIcon;
	CToolTipCtrl m_tooltip;
	CBitmap m_bitmapOk,m_bitmapCancel;

	// Generated message map functions
	//{{AFX_MSG(CSetDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioblackchess();
	afx_msg void OnRadioredchess();
	afx_msg void OnDblclkListengine();
	afx_msg void OnRadiopc();
	afx_msg void OnRadiocc();
	afx_msg void OnRadiopp();
	afx_msg void OnRadiohash();
	afx_msg void OnDoubleclickedRadiopc();
	afx_msg void OnDoubleclickedRadiocc();
	afx_msg void OnDoubleclickedRadiopp();
	afx_msg void OnDoubleclickedRadiohash();
	afx_msg void OnDoubleclickedRadioredchess();
	afx_msg void OnDoubleclickedRadioblackchess();
	afx_msg void OnDoubleclickedRadioDefaultset();
	afx_msg void OnDoubleclickedRadioUserdefine();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWGAMEDLG_H__CEFCDE83_32F2_41CA_A087_070499127128__INCLUDED_)
