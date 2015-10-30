
// Breast_CancerTestDlg.h : 头文件
#include "cv.h"
#include "highgui.h"
#include <vector>
#include <io.h>
using namespace std;
//


#pragma once


// CBreast_CancerTestDlg 对话框
class CBreast_CancerTestDlg : public CDialogEx
{
// 构造
public:
	CBreast_CancerTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BREAST_CANCERTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	vector <char*> JPGPath;
	int IndexofShow;
	IplImage* ImgShow;
	int WIDTH, HEIGHT,DEPTH;//初期确定图像大小，保存为基准图像，并将该数值作为后期开闭运算等操作的基准。
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	void GetFiles(string path);
	void DrawIplImage2DC(IplImage *img, UINT ID);
	bool ProcessIMG(IplImage* src, IplImage** result);
	bool Select_shape_std(CvSeq* first_contour, CvSeq** Max_contour);

	bool Select_shape_row(CvSeq* In_Contour, CvSeq** Row_Contour, bool B_Small);
	bool Select_shape_col(CvSeq* In_Contour, CvSeq** Row_Contour, bool B_Small);

	bool Select_shape_row1(CvSeq* In_Contour, CvSeq** Row_Contour, int I_Small, int I_Big);
	bool Area_Center(CvSeq* In_Contour, int* Area, int* Row, int* Column);
	bool Scale_image_max(IplImage* img, IplImage** result, CvScalar scalar, IplImage* region);
	bool Auto_Threshold(IplImage* img, double* thread);
	int Reco_backdrop(IplImage* img, IplImage** result);
	bool ProcessHightLight(IplImage* src, IplImage** Resule, bool signLOR);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnRead();
	afx_msg void OnBnClickedBtnReadfile();
	bool TempTest();
};
