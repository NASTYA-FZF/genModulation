
// genModulationDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "genModulation.h"
#include "genModulationDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CgenModulationDlg



CgenModulationDlg::CgenModulationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GENMODULATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CgenModulationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIC, signal);
	DDX_Control(pDX, IDC_PIC2, start_spectr);
	DDX_Control(pDX, IDC_PIC3, res_signal);
	DDX_Control(pDX, IDC_PIC4, res_spectr);
	DDX_Control(pDX, IDC_DIAG, pic_diag);
	DDX_Control(pDX, IDC_PIC5, pic_cosQ);
}

BEGIN_MESSAGE_MAP(CgenModulationDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CgenModulationDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTPHASE, &CgenModulationDlg::OnBnClickedButphase)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &CgenModulationDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDCANCEL, &CgenModulationDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO1, &CgenModulationDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO3, &CgenModulationDlg::OnBnClickedRadio3)
END_MESSAGE_MAP()


// Обработчики сообщений CgenModulationDlg

BOOL CgenModulationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	Modulation = modulator(0, 4);
	demod.SetFM(4);
	InitializeCriticalSection(&cs);
	signal.maxX = 1;
	res_signal.maxX = 1;
	start_spectr.maxX = 1000;
	res_spectr.maxX = 1000;

	pic_diag.maxX = 1.1;
	pic_diag.minX = -1.1;
	pic_diag.maxY = 1.1;
	pic_diag.minY = -1.1;
	pic_diag.fl_pt = true;
	// TODO: добавьте дополнительную инициализацию

	/*MessageBox(L"Я люблю козу.", L"Очень важная инфа.", MB_ICONERROR);
	MessageBox(L"Это написал Диман.", L"Очень важная инфа.", MB_ICONERROR);*/
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CgenModulationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CgenModulationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CgenModulationDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (!signal.my_x.empty())
	{
		signal.my_x.clear();
		signal.my_func.clear();
	}

	std::vector<double> t;
	std::vector<long long> x;
	createFunction(t, x, true);
	signal.my_x.push_back(t);
	signal.my_func.push_back(x);
	Invalidate(FALSE);
	createWav(x);
}


void CgenModulationDlg::OnBnClickedButphase()
{
	// TODO: добавьте свой код обработчика уведомлений
	stop = false;
	CreateThread(NULL, NULL, myThread, this, NULL, NULL);
	timer = SetTimer(123, 16, NULL);
}


void CgenModulationDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if (!signal.my_x.empty())
	{
		signal.my_x.clear();
		signal.my_func.clear();

		res_signal.my_x.clear();
		res_signal.my_func.clear();

		start_spectr.my_x.clear();
		start_spectr.my_func.clear();

		res_spectr.my_x.clear();
		res_spectr.my_func.clear();

		pic_cosQ.my_x.clear();
		pic_cosQ.my_func.clear();

		pic_diag.my_x.clear();
		pic_diag.my_func.clear();
	}
	EnterCriticalSection(&cs);
	signal.my_x.push_back(t);
	signal.my_func.push_back(x);

	res_signal.my_x.push_back(t);
	res_signal.my_func.push_back(resI);

	start_spectr.my_x.push_back(t);
	start_spectr.my_func.push_back(resQ);

	res_spectr.my_x.push_back(t);
	res_spectr.my_func.push_back(sinI);

	pic_cosQ.my_x.push_back(t);
	pic_cosQ.my_func.push_back(cosQ);

	pic_diag.my_x.push_back(demod.GetptX());
	pic_diag.my_func.push_back(demod.GetptY());

	x.clear();
	t.clear();
	resI.clear();
	resQ.clear();
	sinI.clear();
	cosQ.clear();
	LeaveCriticalSection(&cs);
	Invalidate(FALSE);
	CDialogEx::OnTimer(nIDEvent);
}

DWORD __stdcall CgenModulationDlg::myThread(PVOID p)
{
	CgenModulationDlg* dlg = (CgenModulationDlg*)p;
	std::vector<int> bits({
		1, 0, 0, 0, 1, 0, 0, 1, 1, 1
		});
	dlg->fHor.resize(1024);
	double step = 1000. / 1024;
	for (int i = 0; i < 1024; i++)
	{
		dlg->fHor[i] = (double)i * step;
	}
	while (!dlg->stop)
	{
		EnterCriticalSection(&dlg->cs);
		dlg->Modulation.FM(bits, dlg->x, dlg->t, 1000, dlg->sinI, dlg->cosQ);
		//dlg->fil.main(dlg->x, dlg->res, dlg->x_spectr, dlg->result_spectr);
		dlg->demod.Kostas(dlg->x, bits, dlg->resI, dlg->resQ);
		LeaveCriticalSection(&dlg->cs);
		Sleep(20);
	}
	return 0;
}


void CgenModulationDlg::OnBnClickedButton2()
{
	// TODO: добавьте свой код обработчика уведомлений
	stop = true;
	KillTimer(timer);
}


void CgenModulationDlg::OnBnClickedCancel()
{
	// TODO: добавьте свой код обработчика уведомлений
	DeleteCriticalSection(&cs);
	CDialogEx::OnCancel();
}


void CgenModulationDlg::OnBnClickedRadio1()
{
	// TODO: добавьте свой код обработчика уведомлений
	Modulation.SetFM(2);
	demod.SetFM(2);
}


void CgenModulationDlg::OnBnClickedRadio3()
{
	// TODO: добавьте свой код обработчика уведомлений
	Modulation.SetFM(4);
	demod.SetFM(4);
}
