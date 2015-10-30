
// DICOM2PICDlg.cpp : 实现文件
//  

#include "stdafx.h"
#include "DICOM2PIC.h"
#include "DICOM2PICDlg.h"
#include "afxdialogex.h"


#define BUILD_DCM2PNM_AS_DCMJ2PNM

//DICOM .h文件


#include "dcmtk/config/osconfig.h"    /* make sure OS specific configuration is included first */

#define INCLUDE_CSTDIO
#define INCLUDE_CSTRING
#include "dcmtk/ofstd/ofstdinc.h"

#ifdef HAVE_GUSI_H
#include <GUSI.h>
#endif

#include "dcmtk/dcmdata/dctk.h"          /* for various dcmdata headers */
#include "dcmtk/dcmdata/cmdlnarg.h"      /* for prepareCmdLineArgs */
#include "dcmtk/dcmdata/dcuid.h"         /* for dcmtk version name */
#include "dcmtk/dcmdata/dcrledrg.h"      /* for DcmRLEDecoderRegistration */

#include "dcmtk/dcmimgle/dcmimage.h"     /* for DicomImage */
#include "dcmtk/dcmimgle/digsdfn.h"      /* for DiGSDFunction */
#include "dcmtk/dcmimgle/diciefn.h"      /* for DiCIELABFunction */

#include "dcmtk/ofstd/ofconapp.h"        /* for OFConsoleApplication */
#include "dcmtk/ofstd/ofcmdln.h"         /* for OFCommandLine */

#include "dcmtk/dcmimage/diregist.h"     /* include to support color images */
#include "dcmtk/ofstd/ofstd.h"           /* for OFStandard */

#ifdef BUILD_DCM2PNM_AS_DCMJ2PNM
#include "dcmtk/dcmjpeg/djdecode.h"      /* for dcmjpeg decoders */
#include "dcmtk/dcmjpeg/dipijpeg.h"      /* for dcmimage JPEG plugin */
#endif

#ifdef BUILD_DCM2PNM_AS_DCML2PNM
#include "dcmtk/dcmjpls/djdecode.h"      /* for dcmjpls decoders */
#endif

#ifdef WITH_LIBTIFF
#include "dcmtk/dcmimage/dipitiff.h"     /* for dcmimage TIFF plugin */
#endif

#ifdef WITH_LIBPNG
#include "dcmtk/dcmimage/dipipng.h"      /* for dcmimage PNG plugin */
#endif

#ifdef WITH_ZLIB
#include <zlib.h>                        /* for zlibVersion() */
#endif

#include "dcmtk/ofstd/ofstream.h"
#include "dcmtk/oflog/oflog.h"

#define OFFIS_OUTFILE_DESCRIPTION "output filename to be written (default: stdout)"

#ifdef BUILD_DCM2PNM_AS_DCML2PNM
# define OFFIS_CONSOLE_APPLICATION "dcml2pnm"
#elif defined(BUILD_DCM2PNM_AS_DCMJ2PNM)
# define OFFIS_CONSOLE_APPLICATION "dcmj2pnm"
#else
# define OFFIS_CONSOLE_APPLICATION "dcm2pnm"
#endif

static OFLogger dcm2pnmLogger = OFLog::getLogger("dcmtk.apps." OFFIS_CONSOLE_APPLICATION);

static const char *consoleDescription = "Convert DICOM images to PGM/PPM"
#ifdef WITH_LIBPNG
", PNG"
#endif
#ifdef WITH_LIBTIFF
", TIFF"
#endif
#ifdef BUILD_DCM2PNM_AS_DCMJ2PNM
", JPEG"
#endif
" or BMP";

static char rcsid[] = "$dcmtk: " OFFIS_CONSOLE_APPLICATION " v"
OFFIS_DCMTK_VERSION " " OFFIS_DCMTK_RELEASEDATE " $";

#define SHORTCOL 4
#define LONGCOL 20

/* output file types */
enum E_FileType
{
	EFT_RawPNM,
	EFT_8bitPNM,
	EFT_16bitPNM,
	EFT_NbitPNM,
	EFT_BMP,
	EFT_8bitBMP,
	EFT_24bitBMP,
	EFT_32bitBMP,
	EFT_JPEG,
	EFT_TIFF,
	EFT_PNG,
	EFT_16bitPNG
#ifdef PASTEL_COLOR_OUTPUT
	, EFT_PastelPNM
#endif
};

//

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
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


// CDICOM2PICDlg 对话框



CDICOM2PICDlg::CDICOM2PICDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DICOM2PIC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDICOM2PICDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDICOM2PICDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_READDIR, &CDICOM2PICDlg::OnBnClickedBtnReaddir)
	ON_BN_CLICKED(IDC_BTN_WRITEDIR, &CDICOM2PICDlg::OnBnClickedBtnWritedir)
	ON_BN_CLICKED(IDC_BTN_RUN, &CDICOM2PICDlg::OnBnClickedBtnRun)
	ON_BN_CLICKED(IDC_RADIO_BMP, &CDICOM2PICDlg::OnBnClickedRadioBmp)
	ON_BN_CLICKED(IDC_RADIO_JPEG, &CDICOM2PICDlg::OnBnClickedRadioJpeg)
END_MESSAGE_MAP()


// CDICOM2PICDlg 消息处理程序

BOOL CDICOM2PICDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetDlgItemText(IDC_EDIT_ReadDir, "C:\\新建文件夹\\DICOM");
	SetDlgItemText(IDC_EDIT_WriteDir, "C:\\新建文件夹");



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDICOM2PICDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDICOM2PICDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDICOM2PICDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDICOM2PICDlg::OnBnClickedBtnReaddir()
{
	// TODO: 在此添加控件通知处理程序代码
	char szPath[MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = "请选择需要转换的DICOM目录：";
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);

	if (lp && SHGetPathFromIDList(lp, szPath))
	{
		strReadDir.Format("%s", szPath);
		SetDlgItemText(IDC_EDIT_ReadDir, strReadDir);

	}
}


void CDICOM2PICDlg::OnBnClickedBtnWritedir()
{
	// TODO: 在此添加控件通知处理程序代码
	char szPath[MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = "请选择存放的目录：";
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);

	if (lp && SHGetPathFromIDList(lp, szPath))
	{
		strWriteDir.Format("%s", szPath);
		SetDlgItemText(IDC_EDIT_WriteDir, strWriteDir);

	}
}
void CDICOM2PICDlg::BrowseCurrentAllFile(CString strDir)
{
	if (strDir == _T(""))
	{
		return;
	}
	else
	{
		if (strDir.Right(1) != _T("//"))
			strDir += L"//";
		strDir = strDir + _T("*.*");
	}
	CFileFind finder;
	CString strPath;
	BOOL bWorking = finder.FindFile(strDir);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		strPath = finder.GetFilePath();
		if (finder.IsDirectory() && !finder.IsDots())
			BrowseCurrentAllFile(strPath); //递归调用
		else if (!finder.IsDirectory() && !finder.IsDots())
		{
			l_DirIN.AddTail(strPath);
		}

	}
}

void CDICOM2PICDlg::OnBnClickedBtnRun()
{
	// TODO: 在此添加控件通知处理程序代码
	l_DirIN.RemoveAll();
	GetDlgItemText(IDC_EDIT_ReadDir, strReadDir);
	GetDlgItemText(IDC_EDIT_WriteDir, strWriteDir);
	BrowseCurrentAllFile(strReadDir);

	POSITION index = l_DirIN.GetHeadPosition();
	CString strtempin,strtempout;
	while (index!=NULL)
	{
		strtempin = l_DirIN.GetAt(index);
		strtempout = strtempin;
		strtempout.Delete(0, strReadDir.GetLength());
		strtempout.Insert(0, strWriteDir+"\\DICOMPIC");
		strtempout.Insert(strtempout.GetLength(), ".jpg");
		int n = strtempout.ReverseFind('\\');
		CString strFileName = strtempout.Left(n);
		if (!PathFileExists(strFileName))
			CreateMuliteDirectory(strFileName);  //文件夹不存在
		bool result = ConvertDCMTOPIC(strtempin,strtempout);
		l_DirIN.GetNext(index);
	}

}


bool FolderExists(CString s)
{
	DWORD attr;
	attr = GetFileAttributes(s);
	return (attr != (DWORD)(-1)) &&
		(attr & FILE_ATTRIBUTE_DIRECTORY);
}

bool CDICOM2PICDlg::CreateMuliteDirectory(CString P)
{
	int len = P.GetLength();
	if (len < 2) return false;
	if ('\\' == P[len - 1])
	{
		P = P.Left(len - 1);
		len = P.GetLength();
	}
	if (len <= 0) return false;
	if (len <= 3)
	{
		if (FolderExists(P))return true;
		else return false;
	}
	if (FolderExists(P))return true;
	CString Parent;
	Parent = P.Left(P.ReverseFind('\\'));
	if (Parent.GetLength() <= 0)return false;
	BOOL Ret = CreateMuliteDirectory(Parent);
	if (Ret)
	{
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = 0;
		Ret = (CreateDirectory(P, &sa) == TRUE);
		return Ret;
	}
	else
		return FALSE;
}


bool CDICOM2PICDlg::ConvertDCMTOPIC(CString pathIN, CString pathOUT)
{
#pragma region 声明


#ifdef BUILD_DCM2PNM_AS_DCMJ2PNM
	// JPEG parameters
	OFCmdUnsignedInt    opt_quality = 90;                 /* default: 90% JPEG quality */
	E_SubSampling       opt_sampling = ESS_422;           /* default: 4:2:2 sub-sampling */
	E_DecompressionColorSpaceConversion opt_decompCSconversion = EDC_photometricInterpretation;
#endif


	OFConsoleApplication app(OFFIS_CONSOLE_APPLICATION, consoleDescription, rcsid);
	OFCommandLine cmd;

	E_FileReadMode      opt_readMode = ERM_autoDetect;    /* default: fileformat or dataset */
	E_TransferSyntax    opt_transferSyntax = EXS_Unknown; /* default: xfer syntax recognition */

	unsigned long       opt_compatibilityMode = CIF_MayDetachPixelData | CIF_TakeOverExternalDataset;
	OFBool              opt_ignoreVoiLutDepth = OFFalse;  /* default: do not ignore VOI LUT bit depth */
	OFCmdUnsignedInt    opt_frame = 1;                    /* default: first frame */
	OFCmdUnsignedInt    opt_frameCount = 1;               /* default: one frame */
	OFBool              opt_useFrameNumber = OFFalse;     /* default: use frame counter */
	OFBool              opt_multiFrame = OFFalse;         /* default: no multiframes */
	int                 opt_convertToGrayscale = 0;       /* default: color or grayscale */
	int                 opt_changePolarity = 0;           /* default: normal polarity */
	int                 opt_useAspectRatio = 1;           /* default: use aspect ratio for scaling */
	OFCmdUnsignedInt    opt_useInterpolation = 1;         /* default: use interpolation method '1' for scaling */
	int                 opt_useClip = 0;                  /* default: don't clip */
	OFCmdSignedInt      opt_left = 0, opt_top = 0;        /* clip region (origin) */
	OFCmdUnsignedInt    opt_width = 0, opt_height = 0;    /* clip region (extension) */
	int                 opt_rotateDegree = 0;             /* default: no rotation */
	int                 opt_flipType = 0;                 /* default: no flipping */
	int                 opt_scaleType = 0;                /* default: no scaling */
														  /* 1 = X-factor, 2 = Y-factor, 3=X-size, 4=Y-size */
	OFCmdFloat          opt_scale_factor = 1.0;
	OFCmdUnsignedInt    opt_scale_size = 1;
	int                 opt_windowType = 0;               /* default: no windowing */
														  /* 1=Wi, 2=Wl, 3=Wm, 4=Wh, 5=Ww, 6=Wn, 7=Wr */
	OFCmdUnsignedInt    opt_windowParameter = 0;
	OFCmdUnsignedInt    opt_roiLeft = 0, opt_roiTop = 0, opt_roiWidth = 0, opt_roiHeight = 0;
	OFCmdFloat          opt_windowCenter = 0.0, opt_windowWidth = 0.0;

	EF_VoiLutFunction   opt_voiFunction = EFV_Default;
	ES_PresentationLut  opt_presShape = ESP_Default;
	OFString            opt_displayFile;
	int                 opt_displayFunction = 0;          /* default: GSDF */
														  /* 0=GSDF, 1=CIELAB */
	OFCmdFloat          opt_ambientLight = -1;            /* default: not set */
	OFCmdFloat          opt_illumination = -1;            /* default: not set */
	OFCmdFloat          opt_minDensity = -1;              /* default: not set */
	OFCmdFloat          opt_maxDensity = -1;              /* default: not set */
	DiDisplayFunction::E_DeviceType deviceType = DiDisplayFunction::EDT_Monitor;
	int                 opt_Overlay[16];
	int                 opt_O_used = 0;                   /* flag for +O parameter */
	EM_Overlay          opt_OverlayMode = EMO_Default;    /* default: Replace or ROI */

	OFCmdFloat          opt_foregroundDensity = 1.0;
	OFCmdFloat          opt_thresholdDensity = 0.5;

	int                 opt_imageInfo = 0;                /* default: no info */
	int                 opt_suppressOutput = 0;           /* default: create output */
	E_FileType          opt_fileType = EFT_RawPNM;        /* default: 8-bit PGM/PPM */
														  /* (binary for file output and ASCII for stdout) */
	OFCmdUnsignedInt    opt_fileBits = 0;                 /* default: 0 */

	unsigned long i;
#pragma endregion



	//设置转换类型
	if (!ConvertType)
	{
		opt_fileType = EFT_RawPNM;
	}
	else
	{
		opt_fileType = EFT_JPEG;
	}

	//设置输入输出目录
	const char *        opt_ifname = pathIN.GetBuffer(sizeof(pathIN));
	const char *        opt_ofname = pathOUT.GetBuffer(sizeof(pathOUT));

/* print resource identifier */
OFLOG_DEBUG(dcm2pnmLogger, rcsid << OFendl);

/* make sure data dictionary is loaded */
if (!dcmDataDict.isDictionaryLoaded())
{
	OFLOG_WARN(dcm2pnmLogger, "no data dictionary loaded, check environment variable: "
		<< DCM_DICT_ENVIRONMENT_VARIABLE);
}

OFLOG_INFO(dcm2pnmLogger, "reading DICOM file: " << opt_ifname);

// register RLE decompression codec
DcmRLEDecoderRegistration::registerCodecs();
#ifdef BUILD_DCM2PNM_AS_DCMJ2PNM
// register JPEG decompression codecs
DJDecoderRegistration::registerCodecs(opt_decompCSconversion);
#endif
#ifdef BUILD_DCM2PNM_AS_DCML2PNM
// register JPEG-LS decompression codecs
DJLSDecoderRegistration::registerCodecs();
#endif

DcmFileFormat *dfile = new DcmFileFormat();
OFCondition cond = dfile->loadFile(opt_ifname, opt_transferSyntax, EGL_withoutGL, DCM_MaxReadLength, opt_readMode);

if (cond.bad())
{
	OFLOG_FATAL(dcm2pnmLogger, cond.text() << ": reading file: " << opt_ifname);
	return 1;
}

OFLOG_INFO(dcm2pnmLogger, "preparing pixel data");

DcmDataset *dataset = dfile->getDataset();
E_TransferSyntax xfer = dataset->getOriginalXfer();

Sint32 frameCount;
if (dataset->findAndGetSint32(DCM_NumberOfFrames, frameCount).bad())
frameCount = 1;
if ((opt_frameCount == 0) || ((opt_frame == 1) && (opt_frameCount == OFstatic_cast(Uint32, frameCount))))
{
	// since we process all frames anyway, decompress the complete pixel data (if required)
	opt_compatibilityMode |= CIF_DecompressCompletePixelData;
}
if ((frameCount > 1) && !(opt_compatibilityMode & CIF_DecompressCompletePixelData))
{
	// use partial read access to pixel data (only in case of multiple frames, but not for all frames)
	opt_compatibilityMode |= CIF_UsePartialAccessToPixelData;
}

DicomImage *di = new DicomImage(dfile, xfer, opt_compatibilityMode, opt_frame - 1, opt_frameCount);
if (di == NULL)
{
	OFLOG_FATAL(dcm2pnmLogger, "Out of memory");
	return 1;
}

if (di->getStatus() != EIS_Normal)
{
	OFLOG_FATAL(dcm2pnmLogger, DicomImage::getString(di->getStatus()));
	return 1;
}

/* create & set display function */
DiDisplayFunction *disp = NULL;

if (opt_imageInfo)
{
	/* dump image parameters */
	OFLOG_INFO(dcm2pnmLogger, "dumping image parameters");

	double minVal = 0.0;
	double maxVal = 0.0;
	const char *colorModel;
	const char *SOPClassUID = NULL;
	const char *SOPInstanceUID = NULL;
	const char *SOPClassText = NULL;
	const char *XferText = DcmXfer(xfer).getXferName();

	int minmaxValid = di->getMinMaxValues(minVal, maxVal);
	colorModel = di->getString(di->getPhotometricInterpretation());
	if (colorModel == NULL)
		colorModel = "unknown";

	dataset->findAndGetString(DCM_SOPClassUID, SOPClassUID);
	dataset->findAndGetString(DCM_SOPInstanceUID, SOPInstanceUID);

	if (SOPInstanceUID == NULL)
		SOPInstanceUID = "not present";
	if (SOPClassUID == NULL)
		SOPClassText = "not present";
	else
		SOPClassText = dcmFindNameOfUID(SOPClassUID);
	if (SOPClassText == NULL)
		SOPClassText = SOPClassUID;

	char aspectRatio[30];
	OFStandard::ftoa(aspectRatio, sizeof(aspectRatio), di->getHeightWidthRatio(), OFStandard::ftoa_format_f, 0, 2);

	/* dump some general information */
	OFLOG_INFO(dcm2pnmLogger, "  filename            : " << opt_ifname << OFendl
		<< "  transfer syntax     : " << XferText << OFendl
		<< "  SOP class           : " << SOPClassText << OFendl
		<< "  SOP instance UID    : " << SOPInstanceUID << OFendl
		<< "  columns x rows      : " << di->getWidth() << " x " << di->getHeight() << OFendl
		<< "  bits per sample     : " << di->getDepth() << OFendl
		<< "  color model         : " << colorModel << OFendl
		<< "  pixel aspect ratio  : " << aspectRatio << OFendl
		<< "  number of frames    : " << di->getNumberOfFrames() << " (" << di->getFrameCount() << " processed)");
	if (di->getFrameTime() > 0)
		OFLOG_INFO(dcm2pnmLogger, "  frame time (in ms)  : " << di->getFrameTime());

	/* dump VOI windows */
	unsigned long count;
	OFString explStr, funcStr;
	count = di->getWindowCount();
	switch (di->getVoiLutFunction())
	{
	case EFV_Default:
		funcStr = "<default>";
		break;
	case EFV_Linear:
		funcStr = "LINEAR";
		break;
	case EFV_Sigmoid:
		funcStr = "SIGMOID";
		break;
	}
	OFLOG_INFO(dcm2pnmLogger, "  VOI LUT function    : " << funcStr);
	OFLOG_INFO(dcm2pnmLogger, "  VOI windows in file : " << di->getWindowCount());
	for (i = 0; i < count; i++)
	{
		if (di->getVoiWindowExplanation(i, explStr) == NULL)
			OFLOG_INFO(dcm2pnmLogger, "  - <no explanation>");
		else
			OFLOG_INFO(dcm2pnmLogger, "  - " << explStr);
	}

	/* dump VOI LUTs */
	count = di->getVoiLutCount();
	OFLOG_INFO(dcm2pnmLogger, "  VOI LUTs in file    : " << count);
	for (i = 0; i < count; i++)
	{
		if (di->getVoiLutExplanation(i, explStr) == NULL)
			OFLOG_INFO(dcm2pnmLogger, "  - <no explanation>");
		else
			OFLOG_INFO(dcm2pnmLogger, "  - " << explStr);
	}

	/* dump presentation LUT shape */
	OFString shapeStr;
	switch (di->getPresentationLutShape())
	{
	case ESP_Default:
		shapeStr = "<default>";
		break;
	case ESP_Identity:
		shapeStr = "IDENTITY";
		break;
	case ESP_Inverse:
		shapeStr = "INVERSE";
		break;
	case ESP_LinOD:
		shapeStr = "LIN OD";
		break;
	}
	OFLOG_INFO(dcm2pnmLogger, "  presentation shape  : " << shapeStr);

	/* dump overlays */
	OFLOG_INFO(dcm2pnmLogger, "  overlays in file    : " << di->getOverlayCount());

	if (minmaxValid)
	{
		char minmaxText[30];
		OFStandard::ftoa(minmaxText, sizeof(minmaxText), maxVal, OFStandard::ftoa_format_f, 0, 0);
		OFLOG_INFO(dcm2pnmLogger, "  maximum pixel value : " << minmaxText);
		OFStandard::ftoa(minmaxText, sizeof(minmaxText), minVal, OFStandard::ftoa_format_f, 0, 0);
		OFLOG_INFO(dcm2pnmLogger, "  minimum pixel value : " << minmaxText);
	}
}

if (!opt_suppressOutput)
{
	/* try to select frame */
	if (opt_frame != di->getFirstFrame() + 1)
	{
		OFLOG_FATAL(dcm2pnmLogger, "cannot select frame " << opt_frame << ", invalid frame number");
		return 1;
	}

	/* convert to grayscale if necessary */
	if ((opt_convertToGrayscale) && (!di->isMonochrome()))
	{
		OFLOG_INFO(dcm2pnmLogger, "converting image to grayscale");

		DicomImage *newimage = di->createMonochromeImage();
		if (newimage == NULL)
		{
			OFLOG_FATAL(dcm2pnmLogger, "Out of memory or cannot convert to monochrome image");
			return 1;
		}
		else if (newimage->getStatus() != EIS_Normal)
		{
			OFLOG_FATAL(dcm2pnmLogger, DicomImage::getString(newimage->getStatus()));
			return 1;
		}
		else
		{
			delete di;
			di = newimage;
		}
	}

	/* process overlay parameters */
	di->hideAllOverlays();
	for (unsigned int k = 0; k < 16; k++)
	{
		if (opt_Overlay[k])
		{
			if ((opt_Overlay[k] == 1) || (k < di->getOverlayCount()))
			{
				OFLOG_INFO(dcm2pnmLogger, "activating overlay plane " << k + 1);
				if (opt_OverlayMode != EMO_Default)
				{
					if (!di->showOverlay(k, opt_OverlayMode, opt_foregroundDensity, opt_thresholdDensity))
						OFLOG_WARN(dcm2pnmLogger, "cannot display overlay plane " << k + 1);
				}
				else {
					if (!di->showOverlay(k)) /* use default values */
						OFLOG_WARN(dcm2pnmLogger, "cannot display overlay plane " << k + 1);
				}
			}
		}
	}

	/* process VOI parameters */
	switch (opt_windowType)
	{
	case 1: /* use the n-th VOI window from the image file */
		if ((opt_windowParameter < 1) || (opt_windowParameter > di->getWindowCount()))
		{
			OFLOG_FATAL(dcm2pnmLogger, "cannot select VOI window " << opt_windowParameter << ", only "
				<< di->getWindowCount() << " window(s) in file");
			return 1;
		}
		OFLOG_INFO(dcm2pnmLogger, "activating VOI window " << opt_windowParameter);
		if (!di->setWindow(opt_windowParameter - 1))
			OFLOG_WARN(dcm2pnmLogger, "cannot select VOI window " << opt_windowParameter);
		break;
	case 2: /* use the n-th VOI look up table from the image file */
		if ((opt_windowParameter < 1) || (opt_windowParameter > di->getVoiLutCount()))
		{
			OFLOG_FATAL(dcm2pnmLogger, "cannot select VOI LUT " << opt_windowParameter << ", only "
				<< di->getVoiLutCount() << " LUT(s) in file");
			return 1;
		}
		OFLOG_INFO(dcm2pnmLogger, "activating VOI LUT " << opt_windowParameter);
		if (!di->setVoiLut(opt_windowParameter - 1, opt_ignoreVoiLutDepth ? ELM_IgnoreValue : ELM_UseValue))
			OFLOG_WARN(dcm2pnmLogger, "cannot select VOI LUT " << opt_windowParameter);
		break;
	case 3: /* Compute VOI window using min-max algorithm */
		OFLOG_INFO(dcm2pnmLogger, "activating VOI window min-max algorithm");
		if (!di->setMinMaxWindow(0))
			OFLOG_WARN(dcm2pnmLogger, "cannot compute min/max VOI window");
		break;
	case 4: /* Compute VOI window using Histogram algorithm, ignoring n percent */
		OFLOG_INFO(dcm2pnmLogger, "activating VOI window histogram algorithm, ignoring " << opt_windowParameter << "%");
		if (!di->setHistogramWindow(OFstatic_cast(double, opt_windowParameter) / 100.0))
			OFLOG_WARN(dcm2pnmLogger, "cannot compute histogram VOI window");
		break;
	case 5: /* Compute VOI window using center and width */
		OFLOG_INFO(dcm2pnmLogger, "activating VOI window center=" << opt_windowCenter << ", width=" << opt_windowWidth);
		if (!di->setWindow(opt_windowCenter, opt_windowWidth))
			OFLOG_WARN(dcm2pnmLogger, "cannot set VOI window to specified values");
		break;
	case 6: /* Compute VOI window using min-max algorithm ignoring extremes */
		OFLOG_INFO(dcm2pnmLogger, "activating VOI window min-max algorithm, ignoring extreme values");
		if (!di->setMinMaxWindow(1))
			OFLOG_WARN(dcm2pnmLogger, "cannot compute min/max VOI window");
		break;
	case 7: /* Compute region of interest VOI window */
		OFLOG_INFO(dcm2pnmLogger, "activating region of interest VOI window");
		if (!di->setRoiWindow(opt_roiLeft, opt_roiTop, opt_roiWidth, opt_roiHeight))
			OFLOG_WARN(dcm2pnmLogger, "cannot compute region of interest VOI window");
		break;
	default: /* no VOI windowing */
		if (di->isMonochrome())
		{
			OFLOG_INFO(dcm2pnmLogger, "disabling VOI window computation");
			if (!di->setNoVoiTransformation())
				OFLOG_WARN(dcm2pnmLogger, "cannot ignore VOI window");
		}
		break;
	}
	/* VOI LUT function */
	if (opt_voiFunction != EFV_Default)
	{
		if (opt_voiFunction == EFV_Linear)
			OFLOG_INFO(dcm2pnmLogger, "setting VOI LUT function to LINEAR");
		else if (opt_voiFunction == EFV_Sigmoid)
			OFLOG_INFO(dcm2pnmLogger, "setting VOI LUT function to SIGMOID");
		if (!di->setVoiLutFunction(opt_voiFunction))
			OFLOG_WARN(dcm2pnmLogger, "cannot set VOI LUT function");
	}

	/* process presentation LUT parameters */
	if (opt_presShape != ESP_Default)
	{
		if (opt_presShape == ESP_Identity)
			OFLOG_INFO(dcm2pnmLogger, "setting presentation LUT shape to IDENTITY");
		else if (opt_presShape == ESP_Inverse)
			OFLOG_INFO(dcm2pnmLogger, "setting presentation LUT shape to INVERSE");
		else if (opt_presShape == ESP_LinOD)
			OFLOG_INFO(dcm2pnmLogger, "setting presentation LUT shape to LIN OD");
		if (!di->setPresentationLutShape(opt_presShape))
			OFLOG_WARN(dcm2pnmLogger, "cannot set presentation LUT shape");
	}

	/* change polarity */
	if (opt_changePolarity)
	{
		OFLOG_INFO(dcm2pnmLogger, "setting polarity to REVERSE");
		if (!di->setPolarity(EPP_Reverse))
			OFLOG_WARN(dcm2pnmLogger, "cannot set polarity");
	}

	/* perform clipping */
	if (opt_useClip && (opt_scaleType == 0))
	{
		OFLOG_INFO(dcm2pnmLogger, "clipping image to (" << opt_left << "," << opt_top << "," << opt_width
			<< "," << opt_height << ")");
		DicomImage *newimage = di->createClippedImage(opt_left, opt_top, opt_width, opt_height);
		if (newimage == NULL)
		{
			OFLOG_FATAL(dcm2pnmLogger, "clipping to (" << opt_left << "," << opt_top << "," << opt_width
				<< "," << opt_height << ") failed");
			return 1;
		}
		else if (newimage->getStatus() != EIS_Normal)
		{
			OFLOG_FATAL(dcm2pnmLogger, DicomImage::getString(newimage->getStatus()));
			return 1;
		}
		else
		{
			delete di;
			di = newimage;
		}
	}

	/* perform rotation */
	if (opt_rotateDegree > 0)
	{
		OFLOG_INFO(dcm2pnmLogger, "rotating image by " << opt_rotateDegree << " degrees");
		if (!di->rotateImage(opt_rotateDegree))
			OFLOG_WARN(dcm2pnmLogger, "cannot rotate image");
	}

	/* perform flipping */
	if (opt_flipType > 0)
	{
		switch (opt_flipType)
		{
		case 1:
			OFLOG_INFO(dcm2pnmLogger, "flipping image horizontally");
			if (!di->flipImage(1, 0))
				OFLOG_WARN(dcm2pnmLogger, "cannot flip image");
			break;
		case 2:
			OFLOG_INFO(dcm2pnmLogger, "flipping image vertically");
			if (!di->flipImage(0, 1))
				OFLOG_WARN(dcm2pnmLogger, "cannot flip image");
			break;
		case 3:
			OFLOG_INFO(dcm2pnmLogger, "flipping image horizontally and vertically");
			if (!di->flipImage(1, 1))
				OFLOG_WARN(dcm2pnmLogger, "cannot flip image");
			break;
		default:
			break;
		}
	}

	/* perform scaling */
	if (opt_scaleType > 0)
	{
		DicomImage *newimage;
		if (opt_useClip)
			OFLOG_INFO(dcm2pnmLogger, "clipping image to (" << opt_left << "," << opt_top << "," << opt_width << "," << opt_height << ")");
		switch (opt_scaleType)
		{
		case 1:
			OFLOG_INFO(dcm2pnmLogger, "scaling image, X factor=" << opt_scale_factor
				<< ", Interpolation=" << OFstatic_cast(int, opt_useInterpolation)
				<< ", Aspect Ratio=" << (opt_useAspectRatio ? "yes" : "no"));
			if (opt_useClip)
				newimage = di->createScaledImage(opt_left, opt_top, opt_width, opt_height, opt_scale_factor, 0.0,
					OFstatic_cast(int, opt_useInterpolation), opt_useAspectRatio);
			else
				newimage = di->createScaledImage(opt_scale_factor, 0.0, OFstatic_cast(int, opt_useInterpolation),
					opt_useAspectRatio);
			break;
		case 2:
			OFLOG_INFO(dcm2pnmLogger, "scaling image, Y factor=" << opt_scale_factor
				<< ", Interpolation=" << OFstatic_cast(int, opt_useInterpolation)
				<< ", Aspect Ratio=" << (opt_useAspectRatio ? "yes" : "no"));
			if (opt_useClip)
				newimage = di->createScaledImage(opt_left, opt_top, opt_width, opt_height, 0.0, opt_scale_factor,
					OFstatic_cast(int, opt_useInterpolation), opt_useAspectRatio);
			else
				newimage = di->createScaledImage(0.0, opt_scale_factor, OFstatic_cast(int, opt_useInterpolation),
					opt_useAspectRatio);
			break;
		case 3:
			OFLOG_INFO(dcm2pnmLogger, "scaling image, X size=" << opt_scale_size
				<< ", Interpolation=" << OFstatic_cast(int, opt_useInterpolation)
				<< ", Aspect Ratio=" << (opt_useAspectRatio ? "yes" : "no"));
			if (opt_useClip)
				newimage = di->createScaledImage(opt_left, opt_top, opt_width, opt_height, opt_scale_size, 0,
					OFstatic_cast(int, opt_useInterpolation), opt_useAspectRatio);
			else
				newimage = di->createScaledImage(opt_scale_size, 0, OFstatic_cast(int, opt_useInterpolation),
					opt_useAspectRatio);
			break;
		case 4:
			OFLOG_INFO(dcm2pnmLogger, "scaling image, Y size=" << opt_scale_size
				<< ", Interpolation=" << OFstatic_cast(int, opt_useInterpolation)
				<< ", Aspect Ratio=" << (opt_useAspectRatio ? "yes" : "no"));
			if (opt_useClip)
				newimage = di->createScaledImage(opt_left, opt_top, opt_width, opt_height, 0, opt_scale_size,
					OFstatic_cast(int, opt_useInterpolation), opt_useAspectRatio);
			else
				newimage = di->createScaledImage(0, opt_scale_size, OFstatic_cast(int, opt_useInterpolation),
					opt_useAspectRatio);
			break;
		default:
			OFLOG_INFO(dcm2pnmLogger, "internal error: unknown scaling type");
			newimage = NULL;
			break;
		}
		if (newimage == NULL)
		{
			OFLOG_FATAL(dcm2pnmLogger, "Out of memory or cannot scale image");
			return 1;
		}
		else if (newimage->getStatus() != EIS_Normal)
		{
			OFLOG_FATAL(dcm2pnmLogger, DicomImage::getString(newimage->getStatus()));
			return 1;
		}
		else
		{
			delete di;
			di = newimage;
		}
	}

	/* write selected frame(s) to file */

	int result = 0;
	FILE *ofile = NULL;
	OFString ofname;
	unsigned int fcount = OFstatic_cast(unsigned int, ((opt_frameCount > 0) && (opt_frameCount <= di->getFrameCount())) ? opt_frameCount : di->getFrameCount());
	const char *ofext = NULL;
	/* determine default file extension */
	switch (opt_fileType)
	{
	case EFT_BMP:
	case EFT_8bitBMP:
	case EFT_24bitBMP:
	case EFT_32bitBMP:
		ofext = "bmp";
		break;
	case EFT_JPEG:
		ofext = "jpg";
		break;
	case EFT_TIFF:
		ofext = "tif";
		break;
	case EFT_PNG:
	case EFT_16bitPNG:
		ofext = "png";
		break;
	default:
		if (di->isMonochrome()) ofext = "pgm"; else ofext = "ppm";
		break;
	}

	if (fcount < opt_frameCount)
	{
		OFLOG_WARN(dcm2pnmLogger, "cannot select " << opt_frameCount << " frames, limiting to "
			<< fcount << " frames");
	}

	for (unsigned int frame = 0; frame < fcount; frame++)
	{
		if (opt_ofname)
		{
			/* output to file */
			if (opt_multiFrame)
			{
				OFOStringStream stream;
				/* generate output filename */
				stream << opt_ofname << ".";
				if (opt_useFrameNumber)
					stream << "f" << (opt_frame + frame);
				else
					stream << frame;
				stream << "." << ofext << OFStringStream_ends;
				/* convert string stream into a character string */
				OFSTRINGSTREAM_GETSTR(stream, buffer_str)
					ofname.assign(buffer_str);
				OFSTRINGSTREAM_FREESTR(buffer_str)
			}
			else
				ofname.assign(opt_ofname);
			OFLOG_INFO(dcm2pnmLogger, "writing frame " << (opt_frame + frame) << " to " << ofname);
			ofile = fopen(ofname.c_str(), "wb");
			if (ofile == NULL)
			{
				OFLOG_FATAL(dcm2pnmLogger, "cannot create file " << ofname);
				return 1;
			}
		}
		else {
			/* output to stdout */
			ofile = stdout;
			OFLOG_INFO(dcm2pnmLogger, "writing frame " << (opt_frame + frame) << " to stdout");
		}

		/* finally create output image file */

		switch (opt_fileType)
		{
		case EFT_RawPNM:
			result = di->writeRawPPM(ofile, 8, frame);
			break;
		case EFT_8bitPNM:
			result = di->writePPM(ofile, 8, frame);
			break;
		case EFT_16bitPNM:
			result = di->writePPM(ofile, 16, frame);
			break;
		case EFT_NbitPNM:
			result = di->writePPM(ofile, OFstatic_cast(int, opt_fileBits), frame);
			break;
		case EFT_BMP:
			result = di->writeBMP(ofile, 0, frame);
			break;
		case EFT_8bitBMP:
			result = di->writeBMP(ofile, 8, frame);
			break;
		case EFT_24bitBMP:
			result = di->writeBMP(ofile, 24, frame);
			break;
		case EFT_32bitBMP:
			result = di->writeBMP(ofile, 32, frame);
			break;
#ifdef BUILD_DCM2PNM_AS_DCMJ2PNM
		case EFT_JPEG:
		{
			/* initialize JPEG plugin */
			DiJPEGPlugin plugin;
			plugin.setQuality(OFstatic_cast(unsigned int, opt_quality));
			plugin.setSampling(opt_sampling);
			result = di->writePluginFormat(&plugin, ofile, frame);
		}
		break;
#endif
#ifdef WITH_LIBTIFF
		case EFT_TIFF:
		{
			/* initialize TIFF plugin */
			DiTIFFPlugin tiffPlugin;
			tiffPlugin.setCompressionType(opt_tiffCompression);
			tiffPlugin.setLZWPredictor(opt_lzwPredictor);
			tiffPlugin.setRowsPerStrip(OFstatic_cast(unsigned long, opt_rowsPerStrip));
			result = di->writePluginFormat(&tiffPlugin, ofile, frame);
		}
		break;
#endif
#ifdef WITH_LIBPNG
		case EFT_PNG:
		case EFT_16bitPNG:
		{
			/* initialize PNG plugin */
			DiPNGPlugin pngPlugin;
			pngPlugin.setInterlaceType(opt_interlace);
			pngPlugin.setMetainfoType(opt_metainfo);
			if (opt_fileType == EFT_16bitPNG)
				pngPlugin.setBitsPerSample(16);
			result = di->writePluginFormat(&pngPlugin, ofile, frame);
		}
		break;
#endif
#ifdef PASTEL_COLOR_OUTPUT
		case EFT_PastelPNM:
			result = di->writePPM(ofile, MI_PastelColor, frame);
			break;
#endif
		default:
			if (opt_ofname)
				result = di->writeRawPPM(ofile, 8, frame);
			else /* stdout */
				result = di->writePPM(ofile, 8, frame);
			break;
		}

		if (opt_ofname)
			fclose(ofile);

		if (!result)
		{
			OFLOG_FATAL(dcm2pnmLogger, "cannot write frame");
			return 1;
		}
	}
}

/* done, now cleanup. */
OFLOG_INFO(dcm2pnmLogger, "cleaning up memory");
delete di;
delete disp;

// deregister RLE decompression codec
DcmRLEDecoderRegistration::cleanup();
#ifdef BUILD_DCM2PNM_AS_DCMJ2PNM
// deregister JPEG decompression codecs
DJDecoderRegistration::cleanup();
#endif
#ifdef BUILD_DCM2PNM_AS_DCML2PNM
// deregister JPEG-LS decompression codecs
DJLSDecoderRegistration::cleanup();
#endif

	return false;
}


void CDICOM2PICDlg::OnBnClickedRadioBmp()
{
	// TODO: 在此添加控件通知处理程序代码
	ConvertType = false;
}


void CDICOM2PICDlg::OnBnClickedRadioJpeg()
{
	// TODO: 在此添加控件通知处理程序代码
	ConvertType = true;
}
