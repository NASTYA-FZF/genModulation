
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
};
