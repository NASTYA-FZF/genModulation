
// genModulationDlg.h: файл заголовка
//

#pragma once
#include "MyGraph.h"
#include "create.h"

// Диалоговое окно CgenModulationDlg
class CgenModulationDlg : public CDialogEx
{
// Создание
public:
	CgenModulationDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GENMODULATION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	MyGraph signal;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButphase();
	modulator Modulation;
	filter fil;
	demodulator demod;
	std::vector<double> t;
	std::vector<double> fHor;
	std::vector<long long> x;
	std::vector<long long> resI;
	std::vector<long long> resQ;
	std::vector<long long> sinI;
	std::vector<long long> cosQ;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	static DWORD WINAPI myThread(PVOID p);
	bool stop = false;
	afx_msg void OnBnClickedButton2();
	UINT_PTR timer;
	CRITICAL_SECTION cs;
	afx_msg void OnBnClickedCancel();
	MyGraph start_spectr;
	MyGraph res_signal;
	MyGraph res_spectr;
	MyGraph pic_diag;
	MyGraph pic_cosQ;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio3();
};