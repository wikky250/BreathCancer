
// DICOM2PICDlg.h : 头文件
//

#pragma once


// CDICOM2PICDlg 对话框
class CDICOM2PICDlg : public CDialogEx
{
// 构造
public:
	CDICOM2PICDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DICOM2PIC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	CString strReadDir;
	CString strWriteDir;
	CList<CString> l_DirIN;
	bool ConvertType;
	void BrowseCurrentAllFile(CString strDir);
	bool CreateMuliteDirectory(CString P);
	// 生成的消息映射函数
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
