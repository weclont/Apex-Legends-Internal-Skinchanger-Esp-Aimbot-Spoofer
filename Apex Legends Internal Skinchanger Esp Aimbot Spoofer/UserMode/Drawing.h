#pragma once

#include <Windows.h>

COLORREF NotSeenColor = 0x000000FF;
#define NotSeenEnemyPen 0x000000FF
HBRUSH NotSeenEnemyBrush = CreateSolidBrush(0x000000FF);

COLORREF SeenColor = 0x0000FF00;
#define SeenEnemyPen 0x0000FF00
HBRUSH SeenEnemyBrush = CreateSolidBrush(0x0000FF00);

COLORREF KnockedColor = 0x00FF0000;
#define KnockedEnemyPen 0x00FF0000
HBRUSH KnockedEnemyBrush = CreateSolidBrush(0xFF0000);

#define PI 3.141592653589793238462643

HFONT Font;

/************ Seen  ************/

void DrawNotSeenFilledRect(HDC hdc, int x, int y, int w, int h) {
	RECT rect = { x, y, x + w, y + h };
	FillRect(hdc, &rect, NotSeenEnemyBrush);
}

void DrawNotSeenString(HDC hdc, int x, int y, const char* text) {
	SetTextAlign(hdc, TA_CENTER | TA_NOUPDATECP);
	SetBkColor(hdc, RGB(0, 0, 0));
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, NotSeenColor);
	SelectObject(hdc, Font);
	TextOutA(hdc, x, y, text, strlen(text));
	DeleteObject(Font);
}

void DrawNotSeenRectangle(HDC hdc, int x, int y, int w, int h, int thickness) {
	DrawNotSeenFilledRect(hdc, x, y, w, thickness); //Top horiz line
	DrawNotSeenFilledRect(hdc, x, y, thickness, h); //Left vertical line
	DrawNotSeenFilledRect(hdc, (x + w), y, thickness, h); //right vertical line
	DrawNotSeenFilledRect(hdc, x, y + h, w + thickness, thickness); //bottom horiz line
}

void DrawNotSeenLine(HDC hdc, float StartX, float StartY, float EndX, float EndY) {
	int a, b = 0;
	HPEN hOPen;
	HPEN hNPen = CreatePen(PS_SOLID, 2, NotSeenEnemyPen);// penstyle, width, color
	hOPen = (HPEN)SelectObject(hdc, hNPen);
	MoveToEx(hdc, StartX, StartY, NULL); //start
	a = LineTo(hdc, EndX, EndY); //end
	DeleteObject(SelectObject(hdc, hOPen));
}

/************ Seen  ************/

void DrawSeenFilledRect(HDC hdc, int x, int y, int w, int h) {
	RECT rect = { x, y, x + w, y + h };
	FillRect(hdc, &rect, SeenEnemyBrush);
}

void DrawSeenString(HDC hdc, int x, int y, const char* text) {
	SetTextAlign(hdc, TA_CENTER | TA_NOUPDATECP);
	SetBkColor(hdc, RGB(0, 0, 0));
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, SeenColor);
	SelectObject(hdc, Font);
	TextOutA(hdc, x, y, text, strlen(text));
	DeleteObject(Font);
}

void DrawSeenRectangle(HDC hdc, int x, int y, int w, int h, int thickness) {
	DrawSeenFilledRect(hdc, x, y, w, thickness); //Top horiz line
	DrawSeenFilledRect(hdc, x, y, thickness, h); //Left vertical line
	DrawSeenFilledRect(hdc, (x + w), y, thickness, h); //right vertical line
	DrawSeenFilledRect(hdc, x, y + h, w + thickness, thickness); //bottom horiz line
}

void DrawSeenLine(HDC hdc, float StartX, float StartY, float EndX, float EndY) {
	int a, b = 0;
	HPEN hOPen;
	HPEN hNPen = CreatePen(PS_SOLID, 2, SeenEnemyPen);// penstyle, width, color
	hOPen = (HPEN)SelectObject(hdc, hNPen);
	MoveToEx(hdc, StartX, StartY, NULL); //start
	a = LineTo(hdc, EndX, EndY); //end
	DeleteObject(SelectObject(hdc, hOPen));
}

/************ Knocked  ************/

void DrawKnockedFilledRect(HDC hdc, int x, int y, int w, int h) {
	RECT rect = { x, y, x + w, y + h };
	FillRect(hdc, &rect, KnockedEnemyBrush);
}

void DrawKnockedString(HDC hdc, int x, int y, const char* text) {
	SetTextAlign(hdc, TA_CENTER | TA_NOUPDATECP);
	SetBkColor(hdc, RGB(0, 0, 0));
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, KnockedColor);
	SelectObject(hdc, Font);
	TextOutA(hdc, x, y, text, strlen(text));
	DeleteObject(Font);
}

void DrawKnockedRectangle(HDC hdc, int x, int y, int w, int h, int thickness) {
	DrawKnockedFilledRect(hdc, x, y, w, thickness); //Top horiz line
	DrawKnockedFilledRect(hdc, x, y, thickness, h); //Left vertical line
	DrawKnockedFilledRect(hdc, (x + w), y, thickness, h); //right vertical line
	DrawKnockedFilledRect(hdc, x, y + h, w + thickness, thickness); //bottom horiz line
}

void DrawKnockedLine(HDC hdc, float StartX, float StartY, float EndX, float EndY) {
	int a, b = 0;
	HPEN hOPen;
	HPEN hNPen = CreatePen(PS_SOLID, 2, KnockedEnemyPen);// penstyle, width, color
	hOPen = (HPEN)SelectObject(hdc, hNPen);
	MoveToEx(hdc, StartX, StartY, NULL); //start
	a = LineTo(hdc, EndX, EndY); //end
	DeleteObject(SelectObject(hdc, hOPen));
}

/************ Universal  ************/

void DrawCrosshair(HDC hdc, int ScreenWidth, int ScreenHeight, int size) {
	DrawNotSeenLine(hdc, ScreenWidth / 2 - size / 2, ScreenHeight / 2, ScreenWidth / 2 + size / 2, ScreenHeight / 2);
	DrawNotSeenLine(hdc, ScreenWidth / 2, ScreenHeight / 2 - size / 2, ScreenWidth / 2, ScreenHeight / 2 + size / 2);
}

void DrawCircle(HDC hdc, int X, int Y, int radius, int numSides) {
	float Step = (float)(PI * 2.0 / numSides);
	int Count = 0;
	for (float a = 0; a < PI * 2.0; a += Step) {
		float X1 = (float)(radius * cos(a) + X);
		float Y1 = (float)(radius * sin(a) + Y);
		float X2 = (float)(radius * cos(a + Step) + X);
		float Y2 = (float)(radius * sin(a + Step) + Y);
		if (Count != 0) {
			DrawNotSeenLine(hdc, X1, Y1, X2, Y2);
		}
		Count += 2;
	}
}