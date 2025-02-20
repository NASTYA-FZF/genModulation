﻿
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
}

BEGIN_MESSAGE_MAP(CgenModulationDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CgenModulationDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTPHASE, &CgenModulationDlg::OnBnClickedButphase)
END_MESSAGE_MAP()


// Обработчики сообщений CgenModulationDlg

BOOL CgenModulationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

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
	std::vector<UINT16> x;
	createFunction(t, x, true);
	signal.my_x.push_back(t);
	signal.my_func.push_back(x);
	Invalidate(FALSE);
	createWav(x);
}


void CgenModulationDlg::OnBnClickedButphase()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (!signal.my_x.empty())
	{
		signal.my_x.clear();
		signal.my_func.clear();
	}

	std::vector<double> t;
	std::vector<UINT16> x;
	createFunction(t, x, false);
	signal.my_x.push_back(t);
	signal.my_func.push_back(x);
	Invalidate(FALSE);
}
