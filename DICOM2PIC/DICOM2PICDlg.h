
// DICOM2PICDlg.h : ͷ�ļ�
//

#pragma once


// CDICOM2PICDlg �Ի���
class CDICOM2PICDlg : public CDialogEx
{
// ����
public:
	CDICOM2PICDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DICOM2PIC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	CString strReadDir;
	CString strWriteDir;
	CList<CString> l_DirIN;
	bool ConvertType;
	void BrowseCurrentAllFile(CString strDir);
	bool CreateMuliteDirectory(CString P);
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnReaddir();
	afx_msg void OnBnClickedBtnWritedir();
	afx_msg void OnBnClickedBtnRun();
	bool ConvertDCMTOPIC(CString pathIN, CString pathOUT);
	afx_msg void OnBnClickedRadioBmp();
	afx_msg void OnBnClickedRadioJpeg();
};
