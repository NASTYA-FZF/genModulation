
// genModulation.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CgenModulationApp:
// Сведения о реализации этого класса: genModulation.cpp
//

class CgenModulationApp : public CWinApp
{
public:
	CgenModulationApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CgenModulationApp theApp;
