
// Breast_CancerTestDlg.cpp : ʵ���ļ�
//

#define _DEBUG "debug"//����ʱ��ʾ�����
//#define _LineSepare "_LineSepare"//б�߷ָ

#include "stdafx.h"
#include "Breast_CancerTest.h"
#include "Breast_CancerTestDlg.h"
#include "afxdialogex.h"
#include "CvvImage.h"
#include <string.h>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define _AutoSepare "_AutoSepare"


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBreast_CancerTestDlg �Ի���



CBreast_CancerTestDlg::CBreast_CancerTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BREAST_CANCERTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBreast_CancerTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBreast_CancerTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_ReadFile, &CBreast_CancerTestDlg::OnBnClickedBtnReadfile)
END_MESSAGE_MAP()


// CBreast_CancerTestDlg ��Ϣ�������

BOOL CBreast_CancerTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	IndexofShow = 0;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CBreast_CancerTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CBreast_CancerTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CBreast_CancerTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CBreast_CancerTestDlg::OnBnClickedBtnReadfile()
{
	//TCHAR szBuffer[MAX_PATH] = { 0 };
	//BROWSEINFO bi;
	//ZeroMemory(&bi, sizeof(BROWSEINFO));
	//bi.hwndOwner = NULL;
	//bi.pszDisplayName = szBuffer;
	//bi.lpszTitle = _T("������ѡ�ļ���Ŀ¼:");
	//bi.ulFlags = BIF_RETURNFSANCESTORS;
	//LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	//if (NULL == idl)
	//{
	//	return;
	//}
	//SHGetPathFromIDList(idl, szBuffer);
	//int iLen = WideCharToMultiByte(CP_ACP, 0, szBuffer, -1, NULL, 0, NULL, NULL);
	//char* chRtn = new char[iLen*sizeof(char)];
	//WideCharToMultiByte(CP_ACP, 0, szBuffer, -1, chRtn, iLen, NULL, NULL);
	//string str(chRtn);
	//GetFiles(str);
// 	char szFileName[MAX_PATH] = { 0 };
// 	OPENFILENAME openFileName = { 0 };
// 	openFileName.lStructSize = sizeof(OPENFILENAME);
// 	openFileName.nMaxFile = MAX_PATH;  //����������ã������õĻ�������ִ��ļ��Ի���  
// 	openFileName.lpstrFilter = "ͼƬ(*.txt)\0*.jpg\0�����ļ�(*.*)\0*.*\0\0";
// 	openFileName.lpstrFile = szFileName;
// 	openFileName.nFilterIndex = 1;
// 	openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
// 
// 	if (::GetOpenFileName(&openFileName))
// 	{
// 		IplImage* s = cvLoadImage(openFileName.lpstrFile, 0);
// 		ProcessIMG(s, &ImgShow);
// 		DrawIplImage2DC(ImgShow, 1002);
// 	}

	TCHAR szFilter[] = "Picture format(*.bmp *.jpg *jpeg)|*.bmp;*.jpg;*jpeg|All Files(*.*)|*.*||";
	CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);
	if (IDOK == fileDlg.DoModal())
	{
		IplImage* s = cvLoadImage(fileDlg.GetPathName(), 0);
		ProcessIMG(s, &ImgShow);
		DrawIplImage2DC(ImgShow, 1002);
	}
}

void CBreast_CancerTestDlg::GetFiles(string path)
{
	//�ļ����  
	long   hFile = 0;
	//�ļ���Ϣ  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//�����Ŀ¼,����֮  
			//�������,�����б�  
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					GetFiles(p.assign(path).append("\\").append(fileinfo.name));
			}
			else
			{
				const char* t  = p.assign(path).append("\\").append(fileinfo.name).data();
				char *buf = new char[strlen(t) + 1];
				strcpy(buf, t);
				JPGPath.push_back(buf);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
	IplImage* s = cvLoadImage(JPGPath[IndexofShow], 0);
	ProcessIMG(s, &ImgShow);
	DrawIplImage2DC(ImgShow, 1002);
}
void CBreast_CancerTestDlg::DrawIplImage2DC(IplImage *img, UINT ID)
{
	if (img != NULL)
	{
		CDC *pDC = GetDlgItem(ID)->GetDC();
		HDC hDC = pDC->GetSafeHdc();
		CRect rect;
		GetDlgItem(ID)->GetClientRect(&rect);
		CvvImage cvvimg;//��opencv��highgui.hͷ�ļ���
		cvvimg.CopyOf(img);
		cvvimg.DrawToHDC(hDC, &rect);
		UpdateData(TRUE);
		ReleaseDC(pDC);
	}
}

bool CBreast_CancerTestDlg::ProcessIMG(IplImage* src, IplImage** Result)
{
	try
	{
		WIDTH = src->width;
		HEIGHT = src->height;
		DEPTH = src->depth;
		IplImage* Threshold = cvCreateImage(CvSize(WIDTH, HEIGHT), DEPTH, 1);
		//�������ھ���
		//if (Gray->width < 1000)
		//	cvRectangle(Gray, CvPoint(0, 0), CvPoint(100, 200), CvScalar(0), CV_FILLED, 4, 0);
		//else if(Gray->width>1000)
		//	cvRectangle(Gray, CvPoint(0, 0), CvPoint(100, 200), CvScalar(0), CV_FILLED, 4, 0);
		
		//���ҵ�ͼ��������Ϊ254-255�����ֱ�ʾ���򣬲���ɾ��
		cvThreshold(src, Threshold, 254, 255, CV_THRESH_BINARY);
		IplConvKernel* kernel = cvCreateStructuringElementEx(11, 11, 5, 5, CV_SHAPE_RECT);
		cvMorphologyEx(Threshold, Threshold, NULL, kernel, CV_MOP_CLOSE,1);
 		kernel = cvCreateStructuringElementEx(21, 11, 10, 5, CV_SHAPE_RECT);
 		cvMorphologyEx(Threshold, Threshold, NULL, kernel, CV_MOP_DILATE, 1);

		//�ų���������������Ϊ254-255�ĸ��ţ�ֻ��ͼ����������Ϣ�����˳���
		CvMemStorage* store = cvCreateMemStorage();
		CvSeq* first_contour = NULL;
		IplImage* TempImg = cvCreateImage(CvSize(WIDTH, HEIGHT), DEPTH, 1);
		cvCopy(Threshold, TempImg, 0);
		if (0 < cvFindContours(TempImg, store, &first_contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, CvPoint(0, 0)))
		{
			CvSeq* Max_contour = NULL;
			Select_shape_std(first_contour, &Max_contour);
			CvSeq* tempseq = first_contour;
			while (tempseq != NULL)
			{
				if(tempseq!=Max_contour)
					cvDrawContours(Threshold, tempseq, CvScalar(0), CvScalar(0), -1, CV_FILLED, CV_AA, CvPoint(0, 0));
				tempseq = tempseq->h_next;
			}
		}
		cvSub(src, Threshold, Threshold, 0);
		Scale_image_max(Threshold, &TempImg, CvScalar(0), NULL);
		
		IplImage* TargetImg = NULL;
		int i_result = Reco_backdrop(TempImg, &TargetImg);
		if (-1 == i_result)
			return false;
		else
			*Result = cvCreateImage(CvSize(WIDTH, HEIGHT), DEPTH, 1);

		cvCopy(TargetImg, *Result, 0);
		cvReleaseImage(&Threshold);
		cvReleaseImage(&TempImg);
		cvReleaseStructuringElement(&kernel);
		return true;
	}
	catch(...)
	{
		return false;
	}
}

//ѡ������������������������ӦΪList���ӡ�
bool CBreast_CancerTestDlg::Select_shape_std(CvSeq* first_contour, CvSeq** Max_contour)
{
	try
	{
		CvSeq* tempseq = first_contour;
		double Max_Area = 0;
		while (tempseq != NULL)
		{
			double area = cvContourArea(tempseq, CV_WHOLE_SEQ, 0);
			if (Max_Area < area)
			{
				*Max_contour = tempseq;
				Max_Area = area;
			}
			tempseq = tempseq->h_next;
		}
		return true;
	}
	catch (...)
	{
		return false;
	}
}
//��ͼ����ĳ����Ҷ��Զ��Ŵ������͡�
//regionΪNULL����Ϊȫͼ�Ŵ�
bool CBreast_CancerTestDlg::Scale_image_max(IplImage *img, IplImage**result, CvScalar scalar, IplImage*region)
{
	try
	{
		*result = cvCreateImage(CvSize(WIDTH, HEIGHT), DEPTH, 1);
		IplImage* add = cvCreateImage(CvSize(WIDTH, HEIGHT), DEPTH, 1);
		cvSet(add, scalar, NULL);
		double Max_Gray = 0;
		IplImage* RegionImg = cvCreateImage(CvSize(WIDTH, HEIGHT), DEPTH, 1);
		cvCopy(img, RegionImg, region);
		cvMinMaxLoc(RegionImg, NULL, &Max_Gray, NULL, NULL, 0);
		cvScaleAdd(img, CvScalar(255.0 / Max_Gray), add, *result);

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool CBreast_CancerTestDlg::Auto_Threshold(IplImage* img, double* thread)
{
	try
	{
		return true;
	}
	catch (...)
	{
		return false;
	}
}
//�ж��Ƿ��б���ͼ���Ƿ���Ҫ���д���
//����ֵΪ<0��������ű�ǲ�ͬλ�á�
//����ֵΪ0������Ҫ��һ����������ResultΪ��λͼ��
//����ֵΪ1������Ҫ��һ����������ResultΪ��λͼ��
int CBreast_CancerTestDlg::Reco_backdrop(IplImage* img, IplImage** Result)
{
	try
	{
		*Result = cvCreateImage(CvSize(WIDTH, HEIGHT), DEPTH, 1);
		CvRect rect;
		IplImage* TempImg = cvCreateImage(CvSize(WIDTH, HEIGHT), DEPTH, 1);
		IplImage* TempImg2 = cvCreateImage(CvSize(WIDTH, HEIGHT), DEPTH, 1);
		cvThreshold(img, TempImg, 10, 255, CV_THRESH_BINARY);
		IplConvKernel* kernel = cvCreateStructuringElementEx(101, 101, 50, 50, CV_SHAPE_ELLIPSE);
		cvMorphologyEx(TempImg, TempImg, NULL, kernel, CV_MOP_CLOSE, 1);
		kernel = cvCreateStructuringElementEx(21, 11, 10, 5, CV_SHAPE_ELLIPSE);
		cvMorphologyEx(TempImg, TempImg, NULL, kernel, CV_MOP_OPEN, 1);
		cvErode(TempImg, TempImg, 0, 1);
		CvMemStorage* store = cvCreateMemStorage();
		CvSeq* contour = NULL;
		IplImage* MiddleImg = cvCreateImage(CvSize(WIDTH, HEIGHT), DEPTH, 1);
		cvCopy(img, MiddleImg, NULL);
		if (1 == cvFindContours(TempImg, store, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, CvPoint(0, 0)))
		{
			rect = cvBoundingRect(contour, 0);
			//convexHull
			cvDrawContours(MiddleImg, contour, CvScalar(128), CvScalar(128), 0, 3, 8, CvPoint(0, 0));
			cvRectangle(MiddleImg, CvPoint(rect.x, rect.y), CvPoint(rect.x+rect.width, rect.y+rect.height), CvScalar(255), 4, 4, 0);
 		}
		else
		{
			CvSeq* Max_Contour = NULL;
			Select_shape_std(contour, &Max_Contour);
			rect = cvBoundingRect(Max_Contour, 0);
			cvDrawContours(MiddleImg, Max_Contour, CvScalar(128), CvScalar(128), 0, 3, 8, CvPoint(0, 0));
			cvRectangle(MiddleImg, CvPoint(rect.x, rect.y), CvPoint(rect.x + rect.width, rect.y + rect.height), CvScalar(255), 4, 4, 0);
			Max_Contour = NULL;
		}

		//�ж�
		//����С��Ӿ������������20�����ޱ���������Ҫ��һ������
		//����С��Ӿ���������С��20�����б�������Ҫ��һ������
		if (rect.y > 20)
		{
			cvCopy(MiddleImg, *Result, 0);
		}
		else
		{
#pragma region б�߷ָ
#ifdef _LineSepare
			CvPoint Intersect_FirstPoint = CvPoint(0, 1);	//����Ѱ�ұ�����ͼƬ�ϱ�Ե�Ľ��㡣
			CvPoint Intersect_SecondPoint = CvPoint(0, 1);	//����Ѱ�ұ�����ͼƬ�ϱ�Ե�Ľ��㡣

			//���ڻ���ɸѡ����
			IplImage* Surround_Img = cvCreateImage(CvSize(WIDTH, HEIGHT), DEPTH, 1);

			int GetHeight = 0;
			int SearchAreaHigh = 30;
			int a = 0, b = 0;
			//����ɸѡ����Ϊy:0-10
			cvSet(Surround_Img, CvScalar(0), NULL);
			cvRectangle(Surround_Img, CvPoint(0, GetHeight), CvPoint(WIDTH, GetHeight + SearchAreaHigh), CvScalar(255), -1, 4, NULL);

			IplImage* Result_And = cvCreateImage(CvSize(WIDTH, HEIGHT), DEPTH, 1);
			cvAnd(Surround_Img, img, TempImg);
			kernel = cvCreateStructuringElementEx(31, 5, 15, 2, CV_SHAPE_RECT, NULL);
			cvMorphologyEx(TempImg, TempImg, NULL, kernel, CV_MOP_OPEN, 1);


 			Scale_image_max(TempImg, &Result_And, CvScalar(0), NULL);
			cvThreshold(Result_And, Result_And, 15, 255, CV_THRESH_BINARY);
			kernel = cvCreateStructuringElementEx(21, SearchAreaHigh, 10, (SearchAreaHigh - 1) / 2, CV_SHAPE_ELLIPSE);
 			cvMorphologyEx(Result_And, Result_And, NULL, kernel, CV_MOP_CLOSE, 1);
 			store = cvCreateMemStorage();
 			contour = NULL;
 			if (0 < cvFindContours(Result_And, store, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, CvPoint(0, 0)))
 			{
 				CvSeq* tempseq = contour;
 				double Max_Area = 0;
// 				while (tempseq != NULL)		//ѭ���������Ե
				Select_shape_std(contour, &tempseq);
 				{
 					CvRect boundrect = cvBoundingRect(tempseq, 0);
 					tempseq = tempseq->h_next;
 					//���ͼ
 					if (boundrect.x < MiddleWIDTH / 2)
 					{
 						if (Intersect_FirstPoint.x < boundrect.x + boundrect.width + 10)
 
 							Intersect_FirstPoint.x = boundrect.x + boundrect.width + 10;
 					}
 					else
 						//�Ҳ�ͼ
 					{
 						if (0 == Intersect_FirstPoint.x)
 						{
 							Intersect_FirstPoint.x = MiddleWIDTH;
 						}
 						if (Intersect_FirstPoint.x - 5> boundrect.x)
 							Intersect_FirstPoint.x = boundrect.x - 5;
 					}
 				}
 			}
 #ifdef _DEBUG
 			cvCopy(img, *Result, NULL);
//   			cvRectangle(*Result, CvPoint(Intersect_FirstPoint.x, Intersect_FirstPoint.y),
//   				CvPoint(Intersect_FirstPoint.x + 20, Intersect_FirstPoint.y + 20), CvScalar(128), 3, 4, NULL);
 #endif // _DEBUG
 
 			// 
 			// #pragma region ���ұ�����Y=GetHeight �Ľ���
 			GetHeight = 300;
 			Intersect_SecondPoint.y = GetHeight;
 			//����ɸѡ����Ϊy:0-10
 			cvSet(Surround_Img, CvScalar(0), NULL);
 			cvRectangle(Surround_Img, CvPoint(0, GetHeight), CvPoint(WIDTH, GetHeight + SearchAreaHigh), CvScalar(255), -1, 4, NULL);
 
 			Result_And = cvCreateImage(CvSize(WIDTH, HEIGHT), DEPTH, 1);
			cvAnd(Surround_Img, img, TempImg);
			kernel = cvCreateStructuringElementEx(31, 5, 15, 2, CV_SHAPE_RECT, NULL);
			cvMorphologyEx(TempImg, TempImg, NULL, kernel, CV_MOP_OPEN, 1);

 			Scale_image_max(TempImg, &Result_And, CvScalar(0), NULL);
 			cvThreshold(Result_And, Result_And, 25, 255, CV_THRESH_BINARY);
 			kernel = cvCreateStructuringElementEx(21, SearchAreaHigh, 10, (SearchAreaHigh - 1) / 2, CV_SHAPE_ELLIPSE);
 			cvMorphologyEx(Result_And, Result_And, NULL, kernel, CV_MOP_CLOSE, 1);
 			store = cvCreateMemStorage();
 			contour = NULL;
 			if (0 < cvFindContours(Result_And, store, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, CvPoint(0, 0)))
 			{
 				CvSeq* tempseq = contour;
				double Max_Area = 0;
// 				while (tempseq != NULL)		//ѭ���������Ե
				Select_shape_std(contour, &tempseq);
 				{
 					CvRect boundrect = cvBoundingRect(tempseq, 0);
 					tempseq = tempseq->h_next;
 					//���ͼ
 					if (boundrect.x < MiddleWIDTH / 2)
 					{
 						if (Intersect_SecondPoint.x < boundrect.x + boundrect.width + 10)
 
 							Intersect_SecondPoint.x = boundrect.x + boundrect.width + 10;
 					}
 					else
 						//�Ҳ�ͼ
 					{
 						if (0 == Intersect_SecondPoint.x)
 						{
 							Intersect_SecondPoint.x = MiddleWIDTH;
 						}
 						if (Intersect_SecondPoint.x - 5> boundrect.x)
 							Intersect_SecondPoint.x = boundrect.x - 5;
 					}
 				}
 			}
 #ifdef _DEBUG
//  			cvRectangle(*Result, CvPoint(Intersect_SecondPoint.x, Intersect_SecondPoint.y),
//  				CvPoint(Intersect_SecondPoint.x + 20, Intersect_SecondPoint.y + 20), CvScalar(128), 3, 4, NULL);
 #endif // _DEBUG
 			// 
			a = (Intersect_SecondPoint.y - Intersect_FirstPoint.y) / (Intersect_SecondPoint.x - Intersect_FirstPoint.x);
			b = Intersect_FirstPoint.y - Intersect_FirstPoint.x*(a);
  			cvReleaseImage(&Surround_Img);
#ifdef _DEBUG
			cvLine(*Result, Intersect_FirstPoint, CvPoint(0, b), CvScalar(255), 5, 8, NULL);
#endif // _DEBUG
#endif 
#pragma endregion 


#pragma region ����Ӧ��ֵ���ڣ����˱߽��������
#ifdef _AutoSepare
			IplImage* region = cvCreateImage(CvSize(WIDTH, HEIGHT), DEPTH, 1);
			cvZero(region);
			cvRectangle(region, CvPoint(0, 0), CvPoint(WIDTH / 2, HEIGHT), CvScalar(255), -1, 4, NULL);
			Scale_image_max(img, &TempImg, CvScalar(0), NULL);
			CvScalar signLOR = cvAvg(TempImg, region);
			//����ƽ��ֵ�ж�������ͼ
			IplImage* BackGround = cvCreateImage(CvSize(WIDTH, HEIGHT), DEPTH, 1);
			//��������
			CvSeq* Co_Region = NULL;
 			if (signLOR.val[0]>10)
 			{
				cvZero(region);
				cvRectangle(region, CvPoint(0, 0), CvPoint(WIDTH/4, HEIGHT/4), CvScalar(255));
				if (cvAvg(TempImg, region).val[0] > 60)
					ProcessHightLight(TempImg, &BackGround, true);
				else
					AfxMessageBox("���������ͼ��");
 			}
			else
			{
				cvZero(region);
				cvRectangle(region, CvPoint(WIDTH - WIDTH / 4, HEIGHT / 4), CvPoint(WIDTH, HEIGHT / 4), CvScalar(255));
				if (cvAvg(TempImg, region).val[0] > 60)
					ProcessHightLight(TempImg, &BackGround, false);
				else
					AfxMessageBox("�������Ҳ�ͼ��");

			}
			cvNot(BackGround, BackGround);
			cvZero(TempImg2);
			cvCopy(TempImg, TempImg2, BackGround);

			cvThreshold(TempImg2, TempImg2, 5, 255, CV_THRESH_BINARY);
			kernel = cvCreateStructuringElementEx(11,11,5,5, CV_SHAPE_ELLIPSE);
			cvMorphologyEx(TempImg2, TempImg, NULL, kernel, CV_MOP_OPEN, 1);
			CvMemStorage* store = cvCreateMemStorage();
			CvSeq* contour = NULL;
			if (0 < cvFindContours(TempImg, store, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, CvPoint(0, 0)))
			{
				CvSeq* tempseq;
				Select_shape_std(contour, &tempseq);
				if (Co_Region != NULL)
				{
					cvZero(TempImg);
					cvDrawContours(TempImg, tempseq, CvScalar(255), CvScalar(255), -1, -1, 8, NULL);
					kernel = cvCreateStructuringElementEx(51,51,25,25, CV_SHAPE_RECT);
					cvMorphologyEx(TempImg, TempImg, NULL, kernel, CV_MOP_OPEN, 1);
					if (0 == cvFindContours(TempImg, store, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, CvPoint(0, 0)))
					{
						Select_shape_std(contour, &Co_Region);
					}
				}
			}
			if (Co_Region != NULL)
			{

			}





//   			cvZero(*Result);
//   			cvCopy(BackGround, *Result, NULL);


#endif 
#pragma endregion 
 		}
		




		cvReleaseImage(&TempImg);
		cvReleaseImage(&MiddleImg);
		cvReleaseStructuringElement(&kernel);
		cvReleaseMemStorage(&store);
		return 0;
	}
	catch (cv::Exception& e)
	{
		const char * s_ERROR = e.what();
		return -1;
	}
}




bool CBreast_CancerTestDlg::ProcessHightLight(IplImage* src, IplImage** Result, bool signLOR)
{
	*Result = NULL;
//	*Result = cvCreateImage(CvSize(WIDTH, HEIGHT), DEPTH, 1);
	IplImage* Select_Region = cvCreateImage(CvSize(WIDTH, HEIGHT), DEPTH, 1);
	cvZero(Select_Region);
	cvRectangle(Select_Region, CvPoint(0, 0), CvPoint(WIDTH, 100), CvScalar(255), -1, 4, NULL);
	IplImage* TempImage = cvCreateImage(CvSize(WIDTH, HEIGHT), DEPTH, 1);
	cvZero(TempImage);
	cvCopy(src, TempImage, Select_Region);
	cvThreshold(TempImage, TempImage, 20, 255, CV_THRESH_BINARY);
	IplConvKernel* kernel = cvCreateStructuringElementEx(5, 5, 2, 2, CV_SHAPE_ELLIPSE);
	cvMorphologyEx(TempImage, TempImage, NULL, kernel, CV_MOP_OPEN, 1);

	CvMemStorage* store = cvCreateMemStorage();
	CvSeq* first_contour = NULL;
	int Max_X = 0;
	if (0 < cvFindContours(TempImage, store, &first_contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, CvPoint(0, 0)))
	{
		CvSeq* Max_contour = NULL;
		Select_shape_std(first_contour, &Max_contour);
		cvZero(Select_Region);
		cvDrawContours(Select_Region, Max_contour, CvScalar(255), CvScalar(255), -1, CV_FILLED, 8, NULL);
	}
	double max_val;
	CvPoint max_loc;
	cvMinMaxLoc(src, NULL, &max_val, NULL, &max_loc, Select_Region);
	CvScalar select_mean, select_dev;
	cvAvgSdv(src, &select_mean, &select_dev, Select_Region);
// 	if (select_dev.val[0] > 50)
// 		AfxMessageBox("������ͻ����������");
// 	else
	{
		//�Ǹ�������ͼ��
		IplImage* ThreadImg = cvCreateImage(CvSize(WIDTH, HEIGHT), DEPTH, 1);
		kernel = cvCreateStructuringElementEx(21, 21, 10, 10, CV_SHAPE_ELLIPSE);
		cvMorphologyEx(ThreadImg, ThreadImg, NULL, kernel, CV_MOP_OPEN, 1);
		cvClearMemStorage(store);
		first_contour = NULL;
		CvSeq* Contour_x = NULL;
		CvRect temp_rect;
		IplImage* DifferImg1 = cvCreateImage(CvSize(WIDTH, HEIGHT), DEPTH, 1);
		IplImage* DifferImg2 = cvCreateImage(CvSize(WIDTH, HEIGHT), DEPTH, 1);
		IplImage* DifferImg = cvCreateImage(CvSize(WIDTH, HEIGHT), DEPTH, 1);
		int M_column;
		int M_row;
		int M_Area;
		int i;
		int Under_Y;
		for (i = 0;i < 255;i++)
		{
			cvZero(DifferImg1);
			cvZero(DifferImg2);
			cvZero(DifferImg);
			cvThreshold(src, ThreadImg, i, 255, CV_THRESH_BINARY);
			kernel = cvCreateStructuringElementEx(21, 21, 10, 10, CV_SHAPE_ELLIPSE, NULL);
			cvMorphologyEx(ThreadImg, ThreadImg, NULL, kernel, CV_MOP_OPEN, 1);
 			if (0 < cvFindContours(ThreadImg, store, &first_contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, CvPoint(0, 0)))
 			{
				Select_shape_row(first_contour, &Contour_x,true);
 			}
 			temp_rect = cvBoundingRect(Contour_x, NULL);
//  			if (signLOR)
//   				cvRectangle(DifferImg1, CvPoint(temp_rect.x, temp_rect.y), CvPoint(temp_rect.x + temp_rect.width - 3, temp_rect.y + temp_rect.height-3), CvScalar(255), -1, 8, NULL);
//  			else
//  				cvRectangle(DifferImg1, CvPoint(temp_rect.x + 3, temp_rect.y), CvPoint(temp_rect.x + temp_rect.width, temp_rect.y + temp_rect.height - 3), CvScalar(255), -1, 8, NULL);
//  			cvDrawContours(DifferImg2, Contour_x, CvScalar(255), CvScalar(255), -1, -1, 8, NULL);
// 			cvSub(DifferImg2, DifferImg1, DifferImg,NULL);	
//    		if (0 < cvFindContours(DifferImg, store, &first_contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, CvPoint(0, 0)))
   			{
//    				Select_shape_row(first_contour, &Contour_x, false);
// 				Area_Center(Contour_x, &M_Area, &M_row, &M_column);
				bool b_left, b_right;
				if (i == 0)
				{
					Under_Y = temp_rect.y+temp_rect.height;
				}
// 				b_left = M_column < 150 && M_row < HEIGHT * 2 / 3 && M_Area < 50 && signLOR;
// 				b_right = M_column > WIDTH - 150 && M_row < HEIGHT * 2 / 3 && M_Area < 50 && !signLOR;
// 				if (b_left||b_right)
				if (!(temp_rect.y + temp_rect.height < Under_Y - 200&& temp_rect.y + temp_rect.height>100))
				{
					Under_Y = temp_rect.y + temp_rect.height;
				}
				else
 				{
 					if (*Result == NULL)
 					{
 						*Result = cvCreateImage(CvSize(WIDTH, HEIGHT), DEPTH, 1);
 						cvZero(*Result);
 						cvThreshold(src, TempImage, i, 255, CV_THRESH_BINARY);
 						cvMorphologyEx(TempImage, TempImage, NULL, kernel, CV_MOP_OPEN, 1);
 						cvCopy(TempImage, ThreadImg, NULL);
 						if (0 < cvFindContours(ThreadImg, store, &first_contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, CvPoint(0, 0)))
 						{
							Select_shape_row(first_contour, &Contour_x, true);
							//////////////////////////////////////////////////////////////////////////
							//����͹��Χ
							cvZero(TempImage);
							CvSeq* ConvexHull = cvConvexHull2(Contour_x, 0, CV_CLOCKWISE, 0);
							int hullcount = ConvexHull->total;
							if (signLOR)
							{
								CvPoint pt0 = CvPoint(1, 1);
								for (int n = 0;n < hullcount;n++)
								{
									CvPoint pt = **CV_GET_SEQ_ELEM(CvPoint*, ConvexHull, n);
									cvLine(TempImage, pt0, pt, CvScalar(255), 1, 8, NULL);;
									pt0 = pt;
								}
								cvLine(TempImage, **CV_GET_SEQ_ELEM(CvPoint*, ConvexHull, hullcount - 1), CvPoint(1, 1), CvScalar(255), 1, 8, NULL);
							}
							else
							{
								CvPoint pt0 = CvPoint(WIDTH, 1);
								for (int n = 0;n < hullcount;n++)
								{
									CvPoint pt = **CV_GET_SEQ_ELEM(CvPoint*, ConvexHull, n);
									cvLine(TempImage, pt0, pt, CvScalar(255), 1, 8, NULL);;
									pt0 = pt;
								}
								cvLine(TempImage, **CV_GET_SEQ_ELEM(CvPoint*, ConvexHull, hullcount - 1), CvPoint(WIDTH, 1), CvScalar(255), 1, 8, NULL);
							}
								if (0 < cvFindContours(TempImage, store, &first_contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, CvPoint(0, 0)))
								{
									CvSeq* Max_contour = NULL;
									Select_shape_std(first_contour, &Max_contour);
									cvDrawContours(*Result, Max_contour, CvScalar(255), CvScalar(255), -1, -1, 8, NULL);
								}
							//////////////////////////////////////////////////////////////////////////
								kernel = cvCreateStructuringElementEx(21, 21, 10, 10, CV_SHAPE_ELLIPSE, NULL);
								cvDilate(*Result, *Result, kernel, 1);
 						}
 					}
  					break;
    				}
   			}

		}
	}
	if (*Result == NULL)
	{
		cvZero(*Result);
		cvCopy(src, *Result, Select_Region);
	}
	return false;
}

//����Rowѡȡ��Ե��B_SmallΪtrue����ѡȡ��С��Ϊfalse����ѡȡ���
bool CBreast_CancerTestDlg::Select_shape_row(CvSeq* In_Contour, CvSeq** Row_Contour, bool B_Small)
{
	try
	{
		CvSeq* tempseq = In_Contour;
		CvRect Rect_M = CvRect(INT_MAX, INT_MAX, 0, 0);
		CvRect temp_rect;
		if (B_Small)
		{
			while (tempseq != NULL)
			{
				temp_rect = cvBoundingRect(tempseq, NULL);
				if (Rect_M.y > temp_rect.y)
				{
					*Row_Contour = tempseq;
					Rect_M = temp_rect;
				}
				tempseq = tempseq->h_next;
			}
		}
		else
		{
			Rect_M = CvRect(0, 0, 0, 0);
			while (tempseq != NULL)
			{
				temp_rect = cvBoundingRect(tempseq, NULL);
				if (Rect_M.y < temp_rect.y)
				{
					*Row_Contour = tempseq;
					Rect_M = temp_rect;
				}
				tempseq = tempseq->h_next;
			}
		}
		return true;
	}
	catch (...)
	{
		return false;
	}
	return false;
}
//����Rowѡȡ��Ե��B_SmallΪtrue����ѡȡ��С��Ϊfalse����ѡȡ���
bool CBreast_CancerTestDlg::Select_shape_col(CvSeq* In_Contour, CvSeq** Row_Contour, bool B_Small)
{
	try
	{
		CvSeq* tempseq = In_Contour;
		CvRect Rect_M = CvRect(INT_MAX, INT_MAX, 0, 0);
		CvRect temp_rect;
		if (B_Small)
		{
			while (tempseq != NULL)
			{
				temp_rect = cvBoundingRect(tempseq, NULL);
				if (Rect_M.x > temp_rect.x)
				{
					*Row_Contour = tempseq;
					Rect_M = temp_rect;
				}
				tempseq = tempseq->h_next;
			}
		}
		else
		{
			Rect_M = CvRect(0, 0, 0, 0);
			while (tempseq != NULL)
			{
				temp_rect = cvBoundingRect(tempseq, NULL);
				if (Rect_M.x < temp_rect.x)
				{
					*Row_Contour = tempseq;
					temp_rect = Rect_M;
				}
				tempseq = tempseq->h_next;
			}
		}
		return true;
	}
	catch (...)
	{
		return false;
	}
	return false;
}

bool CBreast_CancerTestDlg::Select_shape_row1(CvSeq* In_Contour, CvSeq** Row_Contour, int I_Small, int I_Big)
{
	try
	{
		CvRect temp_rect;
		CvSeq *pCurSeq = In_Contour;
		CvSeq *pOldSeq = NULL;
		while (pCurSeq)
		{
			temp_rect = cvBoundingRect(pCurSeq, NULL);
			if (temp_rect.y<I_Small||temp_rect.y>I_Big)
			{
				pOldSeq = pCurSeq;
				if (pOldSeq->h_prev)
				{
					pCurSeq = pOldSeq->h_prev;
					pCurSeq->h_next = pOldSeq->h_next;
					pOldSeq->h_next->h_prev = pCurSeq;
					pCurSeq = pCurSeq->h_next;
					cvClearSeq(pOldSeq);
				}
				else
				{
					pCurSeq = pOldSeq->h_next;
					pCurSeq->h_prev = NULL;
					cvClearSeq(pOldSeq);
				}
			}
			else
			{
				pCurSeq = pCurSeq->h_next;
			}
		}
		*Row_Contour = pOldSeq;
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool CBreast_CancerTestDlg::Area_Center(CvSeq* In_Contour, int* Area, int* Row, int* Column)
{
	CvRect rect = cvBoundingRect(In_Contour, NULL);
	if (Row!=NULL)
		*Row = (rect.height ) / 2+rect.y;
	if (Column != NULL)
		*Column = (rect.width ) / 2+rect.x;
	if (Area != NULL)
		*Area = fabs(cvContourArea(In_Contour, CV_WHOLE_SEQ));
	return true;
}

bool CBreast_CancerTestDlg::TempTest()
{
	IplImage* img = cvCreateImage(cvSize(500, 500), 8, 3);
	cvNamedWindow("hull", 1);

#if !ARRAY
	CvMemStorage* storage = cvCreateMemStorage();
#endif

	for (;;)
	{
		int i, count = rand() % 100 + 1, hullcount;
		CvPoint pt0;
#if !ARRAY
		CvSeq* ptseq = cvCreateSeq(CV_SEQ_KIND_GENERIC | CV_32SC2, sizeof(CvContour),
			sizeof(CvPoint), storage);
		CvSeq* hull;

		for (i = 0; i < count; i++)
		{
			pt0.x = rand() % (WIDTH / 2) + WIDTH / 4;
			pt0.y = rand() % (HEIGHT / 2) + HEIGHT / 4;
			cvSeqPush(ptseq, &pt0);
		}
		hull = cvConvexHull2(ptseq, 0, CV_CLOCKWISE, 0);
		hullcount = hull->total;
#else
		CvPoint* points = (CvPoint*)malloc(count * sizeof(points[0]));
		int* hull = (int*)malloc(count * sizeof(hull[0]));
		CvMat point_mat = cvMat(1, count, CV_32SC2, points);
		CvMat hull_mat = cvMat(1, count, CV_32SC1, hull);

		for (i = 0; i < count; i++)
		{
			pt0.x = rand() % (WIDTH / 2) + WIDTH / 4;
			pt0.y = rand() % (HEIGHT / 2) + HEIGHT / 4;
			points[i] = pt0;
		}
		cvConvexHull2(&point_mat, &hull_mat, CV_CLOCKWISE, 0);
		hullcount = hull_mat.cols;
#endif
		cvZero(img);
		for (i = 0; i < count; i++)
		{
#if !ARRAY
			pt0 = *CV_GET_SEQ_ELEM(CvPoint, ptseq, i);
#else
			pt0 = points[i];
#endif
			cvCircle(img, pt0, 2, CV_RGB(255, 0, 0), CV_FILLED);
		}

#if !ARRAY
		pt0 = **CV_GET_SEQ_ELEM(CvPoint*, hull, hullcount - 1);
#else
		pt0 = points[hull[hullcount - 1]];
#endif

		for (i = 0; i < hullcount; i++)
		{
#if !ARRAY
			CvPoint pt = **CV_GET_SEQ_ELEM(CvPoint*, hull, i);
#else
			CvPoint pt = points[hull[i]];
#endif
			cvLine(img, pt0, pt, CV_RGB(0, 255, 0));
			pt0 = pt;
		}

		cvShowImage("hull", img);

		int key = cvWaitKey(0);
		if (key == 27) // 'ESC'
			break;

#if !ARRAY
		cvClearMemStorage(storage);
#else
		free(points);
		free(hull);
#endif
	}
	return false;
}

