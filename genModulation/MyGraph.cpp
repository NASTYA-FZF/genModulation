// MyGraph.cpp: файл реализации
//

#include "pch.h"
#include "MyGraph.h"
#include <iostream>
using namespace Gdiplus;

// MyGraph

IMPLEMENT_DYNAMIC(MyGraph, CStatic)

MyGraph::MyGraph()
{
	GdiplusStartupInput input;
	Status s;
	s = GdiplusStartup(&token, &input, NULL);
	on_off = true;
	maxY = 1.5;
	minY = -1.5;
	maxX = 1000;
	minX = 0.;
	cur = 2;
	num_network = 12;
	stepX = 1. / 1000.;
	stepXchange = 0.05;
	if (s != Ok)
	{
		MessageBox(L"Не удалось нарисовать", L"Ошибка", NULL);
	}
}

MyGraph::~MyGraph()
{
	GdiplusShutdown(token);
}


BEGIN_MESSAGE_MAP(MyGraph, CStatic)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()



// Обработчики сообщений MyGraph




void MyGraph::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  Добавьте исходный код для отображения указанного элемента

	if (my_func.empty() || my_x.empty()) return;

	if (my_func.size() != my_x.size()) return;

	if (!FindMinMax()) return;

	if (Red.empty() || Green.empty() || Blue.empty()) SetColor();

	Graphics wnd(lpDrawItemStruct->hDC);
	Bitmap buffer(lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom, &wnd);
	Graphics draw_in_buffer(&buffer);
	draw_in_buffer.SetSmoothingMode(SmoothingModeAntiAlias);

	otstup = 5.;
	RectF rect_graph(
		otstup,
		otstup,
		lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left - 2. * otstup,
		lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top - 2. * otstup
	);

	double Xscale = rect_graph.Width / (maxX - minX);
	double Yscale = rect_graph.Height / (maxY - minY);

	Matrix matr;
	matr.Translate(rect_graph.X - minX * rect_graph.Width / (maxX - minX), rect_graph.Y + rect_graph.Height + minY * rect_graph.Height / (maxY - minY));
	matr.Scale(Xscale, -Yscale);

	DrawNetwork(draw_in_buffer, matr);
	
	//DrawMyText(draw_in_buffer, matr);

	DrawFunction(draw_in_buffer, matr);

	wnd.DrawImage(&buffer, 0, 0, 0, 0, lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom, UnitPixel);
}

void MyGraph::DrawNetwork(Gdiplus::Graphics& draw, Matrix& matr)
{
	draw.Clear(Color(255, 255, 255, 255)); //заливка фона
	Pen pen_ortho(Color(255, 0, 0, 0));

	pt1 = PointF(minX, minY);
	pt2 = PointF(minX, maxY);

	matr.TransformPoints(&(pt1), 1);
	matr.TransformPoints(&(pt2), 1);

	draw.DrawLine(&pen_ortho, pt1, pt2);

	pt2 = PointF(maxX, minY);

	matr.TransformPoints(&(pt2), 1);

	draw.DrawLine(&pen_ortho, pt1, pt2);

	Pen pen_network(Color(255, 220, 220, 220));

	step_ver_network = (maxX - minX) / num_network;
	step_hor_network = (maxY - minY) / num_network;

	for (int i = 0; i < num_network; i++)
	{
		pt1 = PointF(minX + (i + 1) * step_ver_network, minY);
		pt2 = PointF(minX + (i + 1) * step_ver_network, maxY);

		matr.TransformPoints(&(pt1), 1);
		matr.TransformPoints(&(pt2), 1);
		draw.DrawLine(&pen_network, pt1, pt2);

		pt1 = PointF(minX, minY + (i + 1) * step_hor_network);
		pt2 = PointF(maxX, minY + (i + 1) * step_hor_network);

		matr.TransformPoints(&(pt1), 1);
		matr.TransformPoints(&(pt2), 1);
		draw.DrawLine(&pen_network, pt1, pt2);
	}
}

void MyGraph::DrawMyText(Gdiplus::Graphics& draw, Gdiplus::Matrix& matr)
{
	FontFamily my_font_family(L"Arial");
	Gdiplus::Font my_font(&my_font_family, 12, FontStyleRegular, UnitPixel);
	SolidBrush brush_font(Color(255, 0, 0, 0));

	double x = minX;
	double y = minY;
	wchar_t podpis_ortho[10];

	step_ver_network = (maxX - minX) / num_network;
	step_hor_network = (maxY - minY) / num_network;

	UINT16 my_size = my_font_family.GetCellDescent(FontStyleRegular);

	for (int i = 0; i <= num_network; i++)
	{
		pt1 = PointF(x /*- my_size*/, minY);
		matr.TransformPoints(&pt1, 1);
		pt1.X -= my_size / 25;
		swprintf_s(podpis_ortho, L"%.2f", x);
		draw.DrawString(podpis_ortho, -1, &my_font, pt1, &brush_font);
		x += step_ver_network;

	}
	y += step_hor_network;
	for (int i = 1; i <= num_network; i++)
	{
		pt2 = PointF(minX, y);
		matr.TransformPoints(&pt2, 1);
		pt2.X -= otstup * 7 / 10;
		swprintf_s(podpis_ortho, L"%.2f", y);
		draw.DrawString(podpis_ortho, -1, &my_font, pt2, &brush_font);
		y += step_hor_network;
	}

	Gdiplus::Font my_font_main(&my_font_family, 14, FontStyleBold, UnitPixel);
	pt1 = PointF(minX, maxY);
	pt2 = PointF(maxX, minY);
	matr.TransformPoints(&pt1, 1);
	matr.TransformPoints(&pt2, 1);
	pt1.X -= otstup * 7 / 10;
	pt1.Y -= otstup * 3 / 10;
	pt2.Y -= otstup * 3 / 10;
	draw.DrawString(osY, -1, &my_font_main, pt1, &brush_font);
	draw.DrawString(osX, -1, &my_font_main, pt2, &brush_font);

	pt1 = PointF(minX, maxY);
	matr.TransformPoints(&pt1, 1);
	pt1.Y -= otstup * 5 / 10;
	draw.DrawString(nameGraph, -1, &my_font_main, pt1, &brush_font);
}

void MyGraph::DrawFunction(Gdiplus::Graphics& draw, Gdiplus::Matrix& matr)
{
	if (!fl_pt)
	{
		for (int iter = 1; iter < my_func[0].size() - 1; iter++)
		{
			for (int j = 0; j < my_func.size(); j++)
			{
				Pen pen_graph({ 255, Red[j], Green[j], Blue[j] }, 3);
				pt1 = PointF((my_x[j][iter - 1]), my_func[j][iter - 1]);
				pt2 = PointF(my_x[j][iter], my_func[j][iter]);
				matr.TransformPoints(&pt1, 1);
				matr.TransformPoints(&pt2, 1);
				draw.DrawLine(&pen_graph, pt1, pt2);
			}
		}
	}
	else
	{
		double r = 1.;
		SolidBrush b_diag({ 255, 0, 0, 255 });
		for (int iter = 1; iter < my_func[0].size(); iter++)
		{
			for (int j = 0; j < my_func.size(); j++)
			{
				pt2 = PointF(my_x[j][iter], my_func[j][iter]);
				matr.TransformPoints(&pt2, 1);
				draw.FillEllipse(&b_diag, pt2.X - (REAL)r, pt2.Y - r, 2. * r, 2. * r);
			}
		}
	}
}

void MyGraph::SetColor()
{
	int size = my_func.size();

	for (int i = 0; i < size; i++)
	{
		Red.push_back((double)rand() / RAND_MAX * 255);
		Green.push_back((double)rand() / RAND_MAX * 255);
		Blue.push_back((double)rand() / RAND_MAX * 255);
	}
}

bool MyGraph::FindMinMax()
{
	int sizeGr = my_func.size();
	int sizeX, sizeY;

	for (int i = 0; i < sizeGr; i++)
	{
		sizeX = my_x[i].size();
		sizeY = my_func[i].size();

		if (sizeX == 0 || sizeY == 0 || sizeX != sizeY) return false;

		if (i == 0)
		{
			minX = my_x[i][0];
			maxX = my_x[i][0];
			minY = my_func[i][0];
			maxY = my_func[i][0];
		}

		for (int j = 0; j < sizeX; j++)
		{
			if (my_x[i][j] > maxX) maxX = my_x[i][j];
			if (my_x[i][j] < minX) minX = my_x[i][j];
			if (my_func[i][j] > maxY) maxY = my_func[i][j];
			if (my_func[i][j] < minY) minY = my_func[i][j];
		}
	}
	if (minY == maxY)
	{
		minY -= 1.;
		maxY += 1.;
	}
	if (minX == maxX)
	{
		minX -= 1.;
		maxX += 1.;
	}
	return true;
}


BOOL MyGraph::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if (zDelta > 0 && maxX > 2 * stepXchange)
		maxX -= stepXchange;
	else
		if (zDelta < 0 && maxX < 1)
			maxX += stepXchange;
	this->Invalidate(FALSE);
	return CStatic::OnMouseWheel(nFlags, zDelta, pt);
}
