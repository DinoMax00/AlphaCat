#if !defined(AFX_HYPERLINK_H__91C79E53_3E00_4E28_8CC7_61874C808806__INCLUDED_)
#define AFX_HYPERLINK_H__91C79E53_3E00_4E28_8CC7_61874C808806__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HyperLink.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHyperLink window

//���������
#define HLS_URL  1//��ҳ
#define HLS_MAIL 2//�ʼ�

class CHyperLink : public CStatic
{
// Construction
public:
	CHyperLink();
	virtual ~CHyperLink();

// Attributes
public:

// Operations
public:
    void SetURL(CString strURL);
    CString GetURL() const{return m_strURL;};
	
	void SetMail(CString strMail);
	CString GetMail() const{return m_strMail;};

    void SetColours(COLORREF crLinkColour,COLORREF crVisitedColour,COLORREF crHoverColour=-1);
    COLORREF GetLinkColour() const{return m_crLinkColour;};
    COLORREF GetVisitedColour() const{return m_crVisitedColour;};
    COLORREF GetHoverColour() const{return m_crHoverColour;};
	
    void SetVisited(BOOL bVisited=TRUE);
    BOOL GetVisited() const{return m_bVisited;};
	
    void SetLinkCursor(HCURSOR hCursor);
    HCURSOR GetLinkCursor() const{return m_hLinkCursor;};
	
    void SetUnderline(BOOL bUnderline=TRUE);
    BOOL GetUnderline() const{return m_bUnderline;};
	
    void SetAutoSize(BOOL bAutoSize=TRUE);
    BOOL GetAutoSize() const{return m_bAdjustToFit;};
	
	HINSTANCE GotoURL(LPCTSTR url,int showcmd);
	void GotoMail(CString strMail,int iShowCmd);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHyperLink)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation	

	// Generated message map functions
protected:   
    void ReportError(int nError);
    LONG GetRegKey(HKEY key,LPCTSTR subkey,LPTSTR retdata);
    void PositionWindow();
    void SetDefaultCursor();
	
	// Protected attributes
protected:
	int m_iHyperLinkSort;      //���������
    COLORREF m_crLinkColour;   //��������ɫ
	COLORREF m_crVisitedColour;//�����ӱ����ʹ������ɫ
    COLORREF m_crHoverColour;  //Hover colour
    BOOL m_bOverControl;       //cursor over control?
    BOOL m_bVisited;           //�˵�ַ�Ƿ��ѱ�����
    BOOL m_bUnderline;         //�Ƿ���»���
    BOOL m_bAdjustToFit;       //Adjust window size to fit text?
    CString m_strURL;          //�����ӵ�ַ
	CString m_strMail;	       //�ʼ���ַ
    CFont m_Font;              //Underline font if necessary
    HCURSOR m_hLinkCursor;     //�������ϵĹ��
    CToolTipCtrl m_ToolTip;    //��ʾ�ؼ�

protected:
	//{{AFX_MSG(CHyperLink)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
    afx_msg void OnClicked();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HYPERLINK_H__91C79E53_3E00_4E28_8CC7_61874C808806__INCLUDED_)
